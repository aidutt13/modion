#pragma once
#include <sstream>
#include <string>
#include <vector>
#include <nlohmann/json.hpp>
#include <Common.hpp>

namespace Modion::Modrinth
{
    std::vector<Mod> search(const std::string &name, unsigned int limit = 10);

    Mod get_mod(const std::string &id);

    std::vector<VersionFile> get_versions(const std::string &mod_id, std::vector<Loader> loaders);

    void download(const VersionFile &file);

} // namespace Modion::Modrinth