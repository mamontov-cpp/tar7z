/*! \file error.h

    Describes an error codes for both TAR reader and writer
 */
#pragma once

namespace tar7z
{

/*! An errors linked to both reading and writing of archives
 */
enum Error
{
    T7ZE_OK = 0,                     //!< No error
    T7ZE_CANNOT_OPEN_FILE  = 1,      //!< Cannot open file
    T7ZE_CANNOT_READ_FILE  = 2,      //!< Cannot read file
    T7ZE_CANNOT_WRITE_FILE = 3,      //!< Cannot write file
    T7ZE_INVALID_HEADER    = 4,      //!< Invalid header
    T7ZE_INVALID_CHECKSUM  = 5,      //!< Invalid checksum
    T7ZE_INNER_FILE_UNSUPPORTED = 6  //!< Inner file is unsupported (not a file or link)
};

}
