/*! \file archive.h

    Defines a main archive list
 */
#pragma once
#include "error.h"
#include "entry.h"
#include "header.h"
#include <vector>
#include <utility>

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
    /*! Returns file by it's name or NULL if not found
        \param[in] name a name of file
        \return link to entry
     */
    tar7z::Entry* file(const std::string& name);
    /*! Adds new file to archive
        \param[in] name name of file
        \param[in] contents list of contents
        \param[in] default_time should we substitute creation time with 0
     */
    void add(const std::string& name, const std::vector<char>& contents, bool default_time = false );
    /*! Adds new file to archive
        \param[in] name name of file
        \param[in] contents list of contents
        \param[in] default_time should we substitute creation time with 0
     */
    void add(const std::string& name, const std::vector<unsigned char>& contents, bool default_time = false);
    /*! Removes a file with specified name from archove
        \param[in] name name of file
     */
    void remove(const std::string& name);

    /*! Appends header for the entry. The entry must have name of <100 characters
        \param[in,out] contents a changed content
        \param[in] entry an entry to be added
        \param[in] link whether it's link
        \param[in] default_time whether we should default time with zero
     */
    static void appendHeader(std::vector<char>& contents, const tar7z::Entry& entry, bool link, bool default_time);
    /*! Appends contents with padding
        \param destination a destination
        \param begin a beginning
        \param end an ending of file
     */
    template<typename InputIterator>
    static void appendAndPadContents(std::vector<char>& destination, InputIterator begin, InputIterator end) {
        size_t size = std::distance(begin, end);
        destination.insert(destination.end(), begin, end);
        // Pad content block
        size_t oddpart = size  % TAR7Z_ALIGNMENT_BLOCK;
        if (oddpart != 0)
        {
            destination.resize(destination.size() + (TAR7Z_ALIGNMENT_BLOCK - oddpart), TAR7Z_FILL_CHARACTER);
        }
    }
    /*! Compute header's checksum
        \param[in] contents content of checksum
        \return checksum
     */
    static unsigned int headerChecksum(const char* contents);
    /*! A byte contents of archive
     */
    std::vector<char> Contents;
    /*! A list of entries
     */
    std::vector<tar7z::Entry> Entries;
    /*! A mapping between name of file and corresponding entry
     */
    boost::unordered_map<std::string, size_t> NameToEntry;

};

}
