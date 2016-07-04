# tar7z
A 7-zip compatible TAR archive reader/writer with support for file names larger than 100 characters.

## Motivation

This project is related to [Saddy](https://github.com/mamontov-cpp/saddy-graphics-engine-2d) . The goal is to create TAR archive reader, that could read archive file to memory fast and provide simple interface for fetching file content, adding file to archive and removing fie from archive.

This reader could be used for asset loading in  your  applications

Also, a writer is provided to make sure, that application could write such archives.

## Limitations

Due the goal of fast reading, this library will support only:

 * Files
 * Links, that are used in 7-Zip to store long file names (not standard soft/hard links).

On any other cases, reader shoud return tar7z::Error::T7ZE_INNER_FILE_UNSUPPORTED error code. 

Note, that reader will ignore file mode and creation time, unless TAR7Z_NEED_CREATION_TIME_AND_FILE_MODE specified. 

## Status

At the moment, this project is actively developed. It will be released as soon as possible.
