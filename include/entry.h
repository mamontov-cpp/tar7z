/*! \file entry.h

    Describes a tar entry for an archive
 */
#pragma once
#include <string>

namespace tar7z
{

class Archive;

/*! A simple description of file, that is stored inside of archive.
 */
class Entry
{
public:
    /*! A name for entry. Note, that this could be not actual file, because it's read from file.
        In that case it would be cropped down to 100 character, but still you can obtain it by full name
        from archive
     */
    std::string Name;
#ifdef TAR7Z_NEED_CREATION_TIME_AND_FILE_MODE
    /*! A file mode
     */
    long Mode;
    /*! A time stamp
     */
    long Time;
#endif
    /*! A size of file in bytes
     */
    long long Size;
    /*! A byte offet for entry
     */
    int Offset;
    /*! A parent archive
     */
    tar7z::Archive* Parent;
    /*! Whether entry is preceded by link file and, due that it's real name is larger than 100 characters
     */
    bool HasLongNameLink;

    /*! Constructs default entry
     */
    inline Entry() : Parent(nullptr), HasLongNameLink(false)
    {

    }
    /*! Constructs new entry, given name and offset
        \param[in] name a name
        \param[in] offset an offset
     */
    inline Entry(const std::string& name, int offset = 0)
    : Name(name),
#ifdef TAR7Z_NEED_CREATION_TIME_AND_FILE_MODE
      Mode(0644),
      Time(0),
#endif
      Offset(offset),
      Parent(nullptr),
      HasLongNameLink(false)
    {

    }
    /*! Returns contents for entry
        \return contents
     */
    char* contents() const;
};

}
