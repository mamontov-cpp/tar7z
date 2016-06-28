/*! \file entry.h

    Describes a tar entry for an archive
 */
#pragma once
#include <string>

namespace tar7z
{

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
    /*! Constructs default entry
     */
    inline Entry()
    {

    }

    inline Entry(const std::string& name, int offset = 0) : Name(name), Mode(0644), Time(0), Type('0'), Offset(offset)
    {

    }
};

}
