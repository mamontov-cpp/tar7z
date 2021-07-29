/*! \file header.h

    Describes tar file header
 */
#pragma once
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <ctime>

namespace tar7z
{
/*! A character for filling empty fields
 */
#define TAR7Z_FILL_CHARACTER  ('\0')
/*! Maximal file name length for header
 */
#define TAR7Z_MAXLEN (100)
/*! A total header size for file
 */
#define TAR7Z_TOTAL_HEADER_SIZE (512)
/*! An alignment block for file
 */
#define TAR7Z_ALIGNMENT_BLOCK (512)
/*! Default file mode for tar
 */
#define TAR7Z_DEFAULT_MODE "100777 "
/*! A mark for 7z links
 */
#define TAR7Z_LONGLINKNAME ("././@LongLink")
/*! Default UID and GID (like 7z stores tjhem)
 */
#define TAR7Z_DEFAULT_UID_GID "     0 "

/*! A mode offset in header
 */
#define TAR7Z_MODE_OFFSET (100)
/*! A uid offset in header
 */
#define TAR7Z_UID_OFFSET (108)
/*! A uid offset in header
 */
#define TAR7Z_UID_OFFSET (108)
/*! A gid offset in header
 */
#define TAR7Z_GID_OFFSET (116)
/*! A size offset in header
 */
#define TAR7Z_SIZE_OFFSET (124)
/*! A mtime offset in header
 */
#define TAR7Z_MTIME_OFFSET (136)
/*! A checksum offset
 */
#define TAR7Z_CHECKSUM_OFFSET (148)
/*! A type flag offset for checksum
 */
#define TAR7Z_TYPEFLAG_OFFSET (156)

/*! We support only pure files in tar or 7-zip (or libtar) links, which always followed by the file
 */
enum class FileType: int
{
    FT_File = '0',
    FT_7ZLink = 'L'
};

#pragma pack(push, 1)

/*! A main header for file
 */
struct Header
{
   char Name[TAR7Z_MAXLEN];      //!< File name
   char Mode[8];                 //!< File mode
   char Uid[8];                  //!< Owner's numeric user ID
   char Gid[8];                  //!< Group's numeric user ID
   char Size[12];                //!< File size in bytes (octal base)
   char Mtime[12];               //!< Last modification time in
                                 //!< numeric Unix time format (octal)
   char CheckSum[8];             //!< Checksum for header record
   char TypeFlag[1];             //!< File type
   char LinkName[100];           //!< Name of linked file
   char Magic[6];                //!< UStar indicator "ustar"
   char Version[2];              //!< UStar version "00"
   char Uname[32];               //!< Owner user name
   char Gname[32];               //!< Owner group name
   char DevMajor[8];             //!< Device major number
   char DevMinor[8];             //!< Device minor number
   char Prefix[155];             //!< Filename prefix
   char Padding[12];             //!< padding
};

#pragma pack(pop)

}
