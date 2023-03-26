#include "Common.hpp"
#include <sstream>

namespace Modion
{

    GameVersion::GameVersion(const std::string &raw_version)
    {
        std::string major, minor, patch, end;
        std::istringstream iss(raw_version);
        std::getline(iss, major, '.');
        std::getline(iss, minor, '.');
        std::getline(iss, patch, '-');
        std::getline(iss, end);

        #if MODION_DEBUG
        std::cout << major << " " << minor << " " << patch << " " << end << std::endl;
        #endif

        this->major = major.empty() ? 0 : std::stoul(major);
        this->minor = minor.empty() ? 0 : std::stoul(minor);
        this->patch = patch.empty() ? 0 : std::stoul(patch);
        this->prerelease = 0;
        this->candidate = 0;

        if (end.starts_with("pre"))
            this->prerelease = std::stoul(end.substr(3));
        else if (end.starts_with("rc"))
            this->candidate = std::stoul(end.substr(2));
    }

    std::strong_ordering GameVersion::operator<=>(const GameVersion &other) const
    {
        if (major < other.major)
            return std::strong_ordering::less;
        else if (major > other.major)
            return std::strong_ordering::greater;

        if (minor < other.minor)
            return std::strong_ordering::less;
        else if (minor > other.minor)
            return std::strong_ordering::greater;

        if (patch < other.patch)
            return std::strong_ordering::less;
        else if (patch > other.patch)
            return std::strong_ordering::greater;

        if (prerelease < other.prerelease)
            return std::strong_ordering::less;
        else if (prerelease > other.prerelease)
            return std::strong_ordering::greater;

        if (candidate < other.candidate)
            return std::strong_ordering::less;
        else if (candidate > other.candidate)
            return std::strong_ordering::greater;

        return std::strong_ordering::equal;
    }

} // namespace Modion