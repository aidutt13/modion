#pragma once
#include <compare>
#include <iostream>
#include <string>

namespace Modion
{
    enum class Loader
    {
        Forge,
        Fabric,
        Quilt
    };

    struct GameVersion
    {
        uint32_t major;
        uint32_t minor;
        uint32_t patch;
        uint32_t prerelease;
        uint32_t candidate;

        GameVersion(uint32_t major, uint32_t minor, uint32_t patch)
            : major(major), minor(minor), patch(patch) {}

        GameVersion(const std::string &raw_version);

        std::strong_ordering operator<=>(const GameVersion &other) const;

        inline void operator=(const std::string &version)
        {
            *this = GameVersion(version);
        }

        friend inline std::ostream &operator <<(std::ostream &os, const GameVersion &version)
        {
            os << version.major << "."
               << version.minor << "."
               << version.patch;
            
            if (version.prerelease != 0)
                os << "-pre" << version.prerelease;
            else if (version.candidate != 0)
                os << "-rc" << version.candidate;
            return os;
        }
    };

    struct Mod
    {
        std::string id;
        std::string title;
        std::vector<std::string> categories;
        std::vector<GameVersion> game_versions;
    };

    struct ModVersion
    {
        std::string id;
        std::string name;
        std::vector<Loader> loaders;
        std::vector<std::string> file_urls;
    };

} // namespace Modion
