#include "../include/archive.h"

char* tar7z::Entry::contents() const
{
    return &(Parent->Contents[Offset]);
}
