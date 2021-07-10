#include "Serialization/Archive.h"

namespace ScarletEngine
{
    Archive::Archive(ArchiveMode InMode)
        : Filename()
        , Mode(InMode)
    {
    }

    Archive::Archive(const String& InFilename, ArchiveMode InMode)
        : Filename(InFilename)
        , Mode(InMode)
    {
    }

    bool Archive::SaveToFile(const char*) const
    {
        return false;
    }
}
