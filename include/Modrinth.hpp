#pragma once
#include <sstream>
#include <string>
#include <vector>
#include <nlohmann/json.hpp>
#include <Common.hpp>

namespace Modion::Modrinth
{

    nlohmann::json raw_search(const std::string &name, unsigned int limit = 10);
    std::vector<Mod> search(const std::string &name, unsigned int limit = 10);

    nlohmann::json raw_get_mod(const std::string &id);
    Mod get_mod(const std::string &id);

} // namespace Modion::Modrinth