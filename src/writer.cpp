#include "../include/writer.h"
#include <cstdio>

tar7z::Error tar7z::Writer::write(const std::string& filename, const tar7z::Archive& ar)
{
    FILE* file = fopen(filename.c_str(), "wb");
    if (!file)
    {
        return tar7z::T7ZE_CANNOT_OPEN_FILE;
    }
    if (ar.Contents.size() != 0)
    {
        int result = fwrite(&(ar.Contents[0]), ar.Contents.size(), 1, file);
        if (result != 1)
        {
            fclose(file);
            return tar7z::T7ZE_CANNOT_WRITE_FILE;
        }
    }
    std::vector<char> pad_bytes;
    pad_bytes.resize(TAR7Z_ALIGNMENT_BLOCK * 2, TAR7Z_FILL_CHARACTER); // Two header sizes for file
    int result = fwrite(&(pad_bytes[0]), pad_bytes.size(), 1, file);
    if (result != 1)
    {
        fclose(file);
        return tar7z::T7ZE_CANNOT_WRITE_FILE;
    }
    fclose(file);
    return tar7z::T7ZE_OK;
}

tar7z::Writer::~Writer()
{

}
