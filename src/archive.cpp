#include "../include/archive.h"
#include "header.h"
#include <cstring>
#include <cassert>
#include <ctime>

void tar7z::Archive::appendHeader(std::vector<char>& contents, const tar7z::Entry& entry, bool link)
{
    assert(entry.Name.size() < TAR7Z_MAXLEN);
    contents.resize(contents.size() + TAR7Z_TOTAL_HEADER_SIZE, TAR7Z_FILL_CHARACTER);
    char* offset = &(contents[contents.size() - TAR7Z_MAXLEN]);
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
#ifdef TAR7Z_NEED_CREATION_TIME_AND_FILE_MODE
    sprintf(offset + TAR7Z_MTIME_OFFSET, "%011lo", entry.Time));
#else
    sprintf(offset + TAR7Z_MTIME_OFFSET, "%011lo", time(NULL));
#endif
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
