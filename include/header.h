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
/*! A mark for 7z links
 */
#define TAR7Z_LONGLINKNAME ("././@LongLink")

/*! We support only pure files in tar or 7z links
 */
enum FileType
{
    FT_File = '0',
    FT_7ZLink = 'L'
};

#pragma pack(push, 1)

/*! A main header for file
 */
struct Header
{
   char Name[TAR7Z_MAXLEN];      // file name
   char Mode[8];                 // file mode
   char Uid[8];                  // Owner's numeric user ID
   char Gid[8];                  // Group's numeric user ID
   char Size[12];                // File size in bytes (octal base)
   char Mtime[12];               // Last modification time in
                                 // numeric Unix time format (octal)
   char CheckSum[8];             // Checksum for header record
   char TypeFlag[1];             // file type
   char LinkName[100];           // Name of linked file
   char Magic[6];                // UStar indicator "ustar"
   char Version[2];              // UStar version "00"
   char Uname[32];               // Owner user name
   char Gname[32];               // Owner group name
   char DevMajor[8];             // Device major number
   char DevMinor[8];             // Device minor number
   char Prefix[155];             // Filename prefix
   char Padding[12];             // padding
};

#pragma pack(pop)

}
