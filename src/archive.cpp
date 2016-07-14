#include "../include/archive.h"
#include "header.h"
#include "entry.h"
#include <cstring>
#include <cassert>
#include <ctime>
#include <iterator>

#ifndef TAR7Z_SADDY
    #include <boost/unordered/unordered_set.hpp>
#else
    #include "../../boost/boost-dist/boost/unordered/unordered_set.hpp"
#endif

size_t tar7z::Archive::count() const
{
    return m_entries.size();
}

tar7z::Entry* tar7z::Archive::file(const std::string& name)
{
    boost::unordered_map<std::string, size_t>::iterator it = m_name_to_entry.find(name);
    if (it == m_name_to_entry.end())
    {
        return NULL;
    }
    return &(m_entries[it->second]);
}

bool tar7z::Archive::add(const std::string& name, const std::vector<char>& contents, bool default_time)
{
    if (tar7z::Archive::validateFileName(name) == false)
    {
        return false;
    }
    if (m_name_to_entry.find(name) != m_name_to_entry.end())
    {
        remove(name);
    }
    tar7z::Entry e;
    // If name length is greater than 100 - append link element
    if (name.size() < TAR7Z_MAXLEN)
    {
        e.Name = name;
    }
    else
    {
        // Set entry name as less than 100
        e.Name = name.substr(0, TAR7Z_MAXLEN - 1);

        // Append link element, which will store actual name of element
        tar7z::Entry link;
        link.Parent = this;
        link.Name = TAR7Z_LONGLINKNAME;
#ifdef TAR7Z_NEED_CREATION_TIME_AND_FILE_MODE
        link.Mode = 0777;
        link.Time = time(NULL);
#endif
        link.Size = name.size();
        appendHeader(this->Contents, link, true, default_time);
        appendAndPadContents(this->Contents, name.begin(), name.end());
    }

    // Append actual file
#ifdef TAR7Z_NEED_CREATION_TIME_AND_FILE_MODE
    e.Mode = 0777;
    e.Time = time(NULL);
#endif
    e.Size = contents.size();
    e.Offset = this->Contents.size() + TAR7Z_TOTAL_HEADER_SIZE;
    e.Parent = this;
    appendHeader(this->Contents, e, false, default_time);
    appendAndPadContents(this->Contents, contents.begin(), contents.end());

    // Synchronize entry name with actual name, passed in argument
    e.Name = name;

    size_t pos = m_entries.size();
    m_entries.push_back(e);
    m_name_to_entry.insert(std::make_pair(name, pos));
    return true;
}

bool tar7z::Archive::add(const std::string& name, const std::vector<unsigned char>& contents, bool default_time)
{
    std::vector<char> c;
    c.resize(contents.size());
    memcpy(&(c[0]), &(contents[0]), contents.size() * sizeof(char));
    return this->add(name, c, default_time);
}

void tar7z::Archive::remove(const std::string &name)
{
    boost::unordered_map<std::string, size_t>::iterator it = m_name_to_entry.find(name);
    if (it == m_name_to_entry.end())
    {
        return;
    }

    size_t position = it->second;
    tar7z::Entry& entry = m_entries[position];
    size_t offset = entry.Offset;
    size_t size = tar7z::Archive::sizeWithPadding(entry.Size);

    // Take care of header - we should remove it also
    offset -= TAR7Z_TOTAL_HEADER_SIZE;
    size +=  TAR7Z_TOTAL_HEADER_SIZE;
    if (name.size() > TAR7Z_MAXLEN)
    {
        size_t linksize = tar7z::Archive::sizeWithPadding(name.size()) + TAR7Z_TOTAL_HEADER_SIZE;
        offset -= linksize;
        size += linksize;
    }

    Contents.erase(Contents.begin() + offset, Contents.begin() + (offset + size));
    m_name_to_entry.erase(name);
    m_entries.erase(m_entries.begin() + position);

    // Rebuild NameToEntry to ensure link correctness
    for(it = m_name_to_entry.begin(); it!= m_name_to_entry.end(); ++it)
    {
        if (it->second >= position)
        {
            it->second -= 1;
        }
    }
}

void tar7z::Archive::addEntry(const std::string& name, const tar7z::Entry& e)
{
    if  (m_name_to_entry.count(name) > 1)
    {
        m_name_to_entry.erase(name);
    }
    size_t position = m_entries.size();
    m_entries.push_back(e);
    m_name_to_entry.insert(std::make_pair(name, position));
}

void tar7z::Archive::appendHeader(std::vector<char>& contents, const tar7z::Entry& entry, bool link, bool default_time)
{
    assert(entry.Name.size() < TAR7Z_MAXLEN);
    contents.resize(contents.size() + TAR7Z_TOTAL_HEADER_SIZE, TAR7Z_FILL_CHARACTER);
    char* offset = &(contents[contents.size() - TAR7Z_TOTAL_HEADER_SIZE]);
    if (!link)
    {
        strcpy(offset, entry.Name.c_str());
    }
    else
    {
        strcpy(offset, TAR7Z_LONGLINKNAME);
    }
#ifdef TAR7Z_NEED_CREATION_TIME_AND_FILE_MODE
    sprintf(offset + TAR7Z_MODE_OFFSET, "%07o", entry.Mode);
#else
    strcpy(offset + TAR7Z_MODE_OFFSET, TAR7Z_DEFAULT_MODE);
#endif
    strcpy(offset + TAR7Z_UID_OFFSET, TAR7Z_DEFAULT_UID_GID);
    strcpy(offset + TAR7Z_GID_OFFSET, TAR7Z_DEFAULT_UID_GID);
    if (default_time)
    {
        sprintf(offset + TAR7Z_MTIME_OFFSET, "%011lo", 0);
    }
    else
    {
#ifdef TAR7Z_NEED_CREATION_TIME_AND_FILE_MODE
        sprintf(offset + TAR7Z_MTIME_OFFSET, "%011lo", entry.Time);
#else
        sprintf(offset + TAR7Z_MTIME_OFFSET, "%011lo", time(NULL));
#endif
    }
    sprintf(offset + TAR7Z_SIZE_OFFSET, "%011llo", entry.Size);
    if (!link)
    {
        offset[TAR7Z_TYPEFLAG_OFFSET] = static_cast<char>(tar7z::FT_File);
    }
    else
    {
        offset[TAR7Z_TYPEFLAG_OFFSET] = static_cast<char>(tar7z::FT_7ZLink);
    }

    // Fill the checksum
    unsigned int checksum = tar7z::Archive::headerChecksum(offset);
    std::sprintf(offset + TAR7Z_CHECKSUM_OFFSET, "%07o", checksum);
}


unsigned int tar7z::Archive::headerChecksum(const char* contents)
{
    unsigned int result = 0;

    const char *cur = contents;
    const char *end = cur + TAR7Z_TOTAL_HEADER_SIZE;
    const char *checksumpos = cur + TAR7Z_CHECKSUM_OFFSET;

    // 1. Iterate over header until we are at checksum field, taking last bits.
    while (cur < checksumpos) {
        result += (*cur) & 0xff;
        ++cur;
    }

    // 2. Then add eight 'ascii spaces', like we filled it
    const int checksumfieldsize = 8;
    result += ' ' * checksumfieldsize;
    cur += checksumfieldsize;

    // 3. And then go until the end.
    while (cur < end) {
        result += (*cur) & 0xff;
        ++cur;
    }
    return result;
}


static struct InvalidFilenameSet
{
    boost::unordered_set<std::string> Names;

    InvalidFilenameSet()
    {
        const char* names[] = {
            "CON",
            "PRN",
            "AUX",
            "NUL",
            "COM1",
            "COM2",
            "COM3",
            "COM4",
            "COM5",
            "COM6",
            "COM7",
            "COM8",
            "COM9",
            "LPT1",
            "LPT2",
            "LPT3",
            "LPT4",
            "LPT5",
            "LPT6",
            "LPT7",
            "LPT8",
            "LPT9",
            NULL
        };
        const char** cur = names;
        while(*cur)
        {
            Names.insert(*cur);
            ++cur;
        }
    }

} InvalidFileNameSet;


bool tar7z::Archive::validateFileName(const std::string& filename)
{
    // If name is empty - it's invalid
    if (filename.size() == 0)
    {
        return false;
    }
    // If name is found is invalid set - it's invalid for Windows
    if (InvalidFileNameSet.Names.count(filename) != 0)
    {
        return false;
    }
    // Check filename for invalid characters
    const char* invalidset = "<>:\"/\\|?*\x1\x2\x3\x4\x5\x6\x7\x8\x9\xA\xB\xC\xD\xE\xF\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1A\x1B\x1C\x1D\x1E\x1F";
    return strpbrk(&(filename[0]), invalidset) == NULL;
}
