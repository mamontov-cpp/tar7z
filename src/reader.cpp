#include "../include/reader.h"
#include <cstring>
#include <fstream>


static std::string trailing_zeroes(2 * TAR7Z_ALIGNMENT_BLOCK, TAR7Z_FILL_CHARACTER);

tar7z::Error tar7z::Reader::read(const std::string& filename, tar7z::Archive& ar)
{
    ar.Contents.clear();

    std::ifstream file(filename.c_str());
    // Failed to open
    if  (file.bad())
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

    // TODO: actually parse file and fill it with entries


    return tar7z::T7ZE_OK;
}

tar7z::Reader::~Reader()
{

}
