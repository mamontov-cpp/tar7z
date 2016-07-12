/*! \file writer.h

    Describes a basic writer for files
 */
#pragma once
#include "archive.h"
#include "error.h"

namespace tar7z
{

/*! A basic writer, that could write TAR archives
 */
class Writer
{
public:
    inline Writer()
    {

    }
    /*! Writes archive to specified file
        \param[in] filename a name for file
        \param[in] ar archive
        \return error if occured
     */
    virtual tar7z::Error write(const std::string& filename, const tar7z::Archive& ar);
    /*! Can be inherited
     */
    virtual ~Writer();
};

}
