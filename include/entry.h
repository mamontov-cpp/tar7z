/*! \file entry.h

    Describes a tar entry for an archive
 */
#pragma once
#include <string>

namespace tar7z
{

class Archive;

class Entry
{
public:
    /*! A name for entry
     */
    std::string Name;
    /*! A file mode
     */
    long Mode;
    /*! A time stamp
     */
    long Time;
    /*! A size of file
     */
    long Size;
    /*! A byte offet for entry
     */
    int Offset;
    /*! A parent archive
     */
    tar7z::Archive* Parent;
    /*! Whether entry has long name link, due it's name
     */
    bool HasLongNameLink;

    /*! Constructs default entry
     */
    inline Entry() : Parent(NULL), HasLongNameLink(false)
    {

    }
    /*! Constructs new entry, given name and offset
        \param[in] name a name
        \param[in] offset an offset
     */
    inline Entry(const std::string& name, int offset = 0) : Name(name), Mode(0644), Time(0),  Offset(offset), Parent(NULL), HasLongNameLink(false)
    {

    }
    /*! Returns contents for entry
        \return contents
     */
    char* contents() const;
};

}
