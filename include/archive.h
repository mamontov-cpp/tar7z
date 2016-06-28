/*! \file archive.h

    Defines a main archive list
 */
#pragma once
#include "entry.h"
#include <vector>

#ifndef TAR7Z_SADDY
    #include <boost/unordered/unordered_map.hpp>
#else
    #include "../../boost/boost-dist/boost/unordered/unordered_map.hpp"
#endif

namespace tar7z
{

class Archive
{
public:
    inline Archive()
    {

    }

    /*! A table of contents in archive
     */
    std::vector<char> Contents;
    /*! A list of entries
     */
    std::vector<tar7z::Entry> Entries;
    /*! A mapping between name of file and corresponding entry
     */
    boost::hash<std::string, size_t> NameToEntry;
};

}
