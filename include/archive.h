/*! \mainpage tar7z

    A 7-zip compatible TAR archive reader/writer with support for file names larger than 100 characters.
    See https://github.com/mamontov-cpp/tar7z for details.
 */
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

class Entry;

/*! A main class, that works that in-memory archive with all data, stored as one binary vector
 */
class Archive
{
public:
    inline Archive()
    {

    }
    /*! Returns amount of files in archive
        \return amount of files
     */
    size_t count() const;
    /*! Returns file by it's name or NULL if not found
        \param[in] name a name of file
        \return link to entry
     */
    tar7z::Entry* file(const std::string& name);
    /*! Adds new file to archive
        \param[in] name name of file
        \param[in] contents list of contents
        \param[in] default_time should we substitute creation time with 0
        \return false if name is invalid
     */
    bool add(const std::string& name, const std::vector<char>& contents, bool default_time = false );
    /*! Adds new file to archive
        \param[in] name name of file
        \param[in] contents list of contents
        \param[in] default_time should we substitute creation time with 0
        \return false if name is invalid
     */
    bool add(const std::string& name, const std::vector<unsigned char>& contents, bool default_time = false);
    /*! Removes a file with specified name from archove
        \param[in] name name of file
     */
    void remove(const std::string& name);
    /*! Adds new entry to list. Used by reader to actually populate archive with data, that was read from file.
        You should not use it, unless you really need it.
        \param[in] name a name
        \param[in] e entry
     */
    void addEntry(const std::string& name, const tar7z::Entry& e);
    /*! Validates file name, returns true, if passed string could be used as real file name. Used to check, whether specified file
        could be added into archive.
        \param[in] filename name of file
        \return true if valid
     */
    static bool validateFileName(const std::string& filename);
    /*! Compute header's checksum. Used by reader to check file's content.
        You should not use it, unless you really need it.
        \param[in] contents content of checksum
        \return checksum
     */
    static unsigned int headerChecksum(const char* contents);
    /*! Returns size, extending it to size of block. Used by reader.
        \param sz size
        \return size
     */
    inline static size_t sizeWithPadding(size_t sz)
    {
        size_t result = sz;
        size_t oddpart = result % TAR7Z_ALIGNMENT_BLOCK;
        if (oddpart)
        {
            result +=  (TAR7Z_ALIGNMENT_BLOCK - oddpart);
        }
        return result;
    }
    /*! A binary contents of archive
     */
    std::vector<char> Contents;
protected:    
    /*! Appends header for the entry. The entry must have name of less than 100 characters
        \param[in,out] contents a changed content
        \param[in] entry an entry to be added
        \param[in] link whether it's link
        \param[in] default_time whether we should default time with zero
     */
    static void appendHeader(std::vector<char>& contents, const tar7z::Entry& entry, bool link, bool default_time);
    /*! Appends contents vector with padding
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
    /*! A list of entries
     */
    std::vector<tar7z::Entry> m_entries;
    /*! A mapping between name of file and corresponding entry
     */
    boost::unordered_map<std::string, size_t> m_name_to_entry;
};

}
