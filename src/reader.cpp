#include "../include/reader.h"
#include <cstring>
#include <fstream>


static std::string trailing_zeroes(2 * TAR7Z_ALIGNMENT_BLOCK, TAR7Z_FILL_CHARACTER);

tar7z::Error tar7z::Reader::read(const std::string& filename, tar7z::Archive& ar)
{
    ar.Contents.clear();

    std::ifstream file(filename.c_str(), std::ios::binary);
    // Failed to open
    if  (file.good() == false)
    {
        return tar7z::T7ZE_CANNOT_OPEN_FILE;
    }
    ar.Contents.insert(ar.Contents.end(),
       std::istreambuf_iterator<char>(file),
       std::istreambuf_iterator<char>()
    );
    // Failed to load
    if (file.fail())
    {
        return tar7z::T7ZE_CANNOT_READ_FILE;
    }
    // File cannot be less than 1024 - in that case it has no trailing zeroes
    if (ar.Contents.size() < 2 * TAR7Z_ALIGNMENT_BLOCK)
    {
        return tar7z::T7ZE_NO_TRAILING_ZEROES;
    }
    // Check for trailing zeroes
    int trailing_zeroes_pos = ar.Contents.size() - 2 * TAR7Z_ALIGNMENT_BLOCK;
    if (memcmp(&(ar.Contents[trailing_zeroes_pos]), &(trailing_zeroes[0]), 2 * TAR7Z_ALIGNMENT_BLOCK) != 0)
    {
        return tar7z::T7ZE_NO_TRAILING_ZEROES;
    }
    // And strip trailing zeroes
    ar.Contents.resize(ar.Contents.size() - 2 * TAR7Z_ALIGNMENT_BLOCK);

    // Actually parse file and fill it with entries
    size_t offset = 0;
    tar7z::Entry current_entry;
    current_entry.Parent = &ar;

    m_buffer = &(ar.Contents);
    std::string name;
    tar7z::Error error = tar7z::T7ZE_OK;
    size_t size = ar.Contents.size();
    while((offset != size) && (error == tar7z::T7ZE_OK) )
    {
        current_entry.Offset = offset + TAR7Z_TOTAL_HEADER_SIZE;
        error = this->readHeader(offset, current_entry);
        if (error != tar7z::T7ZE_OK)
        {
            return error;
        }
        // Invalid size - greater than archive
        offset += TAR7Z_TOTAL_HEADER_SIZE;
        if (size - offset < current_entry.Size)
        {
            return tar7z::T7ZE_INVALID_HEADER;
        }
        // Fetch real name for entry
        if (current_entry.HasLongNameLink)
        {
            name = std::string(&(m_buffer->at(offset)), current_entry.Size);
        }
        else
        {
            name = current_entry.Name;
        }
        // Disable files with weird name.
        if (tar7z::Archive::validateFileName(name) == false)
        {
            return tar7z::T7ZE_INNER_FILE_UNSUPPORTED;
        }

        offset += tar7z::Archive::sizeWithPadding(current_entry.Size);
        // If this is long name link, try to read next file as this should be
        // commin file
        if (current_entry.HasLongNameLink)
        {
            current_entry.Offset = offset + TAR7Z_TOTAL_HEADER_SIZE;
            error = this->readHeader(offset, current_entry);
            // Two link files at the same time should not be supported!
            if (current_entry.HasLongNameLink == true && error == tar7z::T7ZE_OK)
            {
                error = tar7z::T7ZE_INVALID_HEADER;
            }
            if (error != tar7z::T7ZE_OK)
            {
                return error;
            }
            // Link as this file is pointed already
            current_entry.HasLongNameLink = true;
            // Check and move to next file
            offset += TAR7Z_TOTAL_HEADER_SIZE;
            if (size - offset < current_entry.Size)
            {
                return tar7z::T7ZE_INVALID_HEADER;
            }
            offset += tar7z::Archive::sizeWithPadding(current_entry.Size);
        }
        ar.addEntry(name, current_entry);
    }

    return error;
}

tar7z::Reader::~Reader()
{

}

tar7z::Error tar7z::Reader::readHeader(size_t offset, tar7z::Entry& entry)
{
    // Out of header
    if (m_buffer->size() - offset < TAR7Z_TOTAL_HEADER_SIZE)
    {
        return tar7z::T7ZE_INVALID_HEADER;
    }

    char* buffer = &(m_buffer->at(offset));
    unsigned int real_checksum =  tar7z::Archive::headerChecksum(buffer);
    unsigned int header_checksum = 0;
    if (sscanf(buffer + TAR7Z_CHECKSUM_OFFSET, "%07o", &header_checksum) != 1)
    {
        return tar7z::T7ZE_INVALID_CHECKSUM;
    }
    // Checksum mismatch
    if (real_checksum != header_checksum)
    {
        return tar7z::T7ZE_INVALID_CHECKSUM;
    }

    char buf[TAR7Z_MAXLEN + 1] = {0};
    memcpy(buf, buffer, TAR7Z_MAXLEN);
    entry.Name = buf;

#ifdef TAR7Z_NEED_CREATION_TIME_AND_FILE_MODE
    if (sscanf(buffer + TAR7Z_MODE_OFFSET, "%07o", &(entry.Mode)) != 1)
    {
        return tar7z::T7ZE_INVALID_HEADER;
    }

    if (sscanf(buffer + TAR7Z_MODE_OFFSET, "%011lo", &(entry.Time)) != 1)
    {
        return tar7z::T7ZE_INVALID_HEADER;
    }
#endif

    if (sscanf(buffer + TAR7Z_SIZE_OFFSET, "%011llo", &(entry.Size)) != 1)
    {
        return tar7z::T7ZE_INVALID_HEADER;
    }

    entry.HasLongNameLink = false;
    if (buffer[TAR7Z_TYPEFLAG_OFFSET] != static_cast<char>(tar7z::FT_File))
    {
        if (buffer[TAR7Z_TYPEFLAG_OFFSET] == static_cast<char>(tar7z::FT_7ZLink))
        {
            entry.HasLongNameLink = true;
        }
        else
        {
            return tar7z::T7ZE_INNER_FILE_UNSUPPORTED;
        }
    }

    return tar7z::T7ZE_OK;
}
