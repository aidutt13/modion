#include "Modrinth.hpp"
#include "HTTP.hpp"

#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <list>
#include <fstream>

namespace Modion::Modrinth
{

    nlohmann::json raw_search(const std::string &name, unsigned int limit)
    {
        auto [_, res] = Modion::HTTP::get("https://api.modrinth.com/v2/search", {
            {"query", name},
            {"limit", std::to_string(limit)}
        });

        return res;
    }

    std::vector<Mod> search(const std::string &name, unsigned int limit)
    {
        nlohmann::json raw = raw_search(name, limit);
        #ifdef MODION_DEBUG
        std::cout << raw.dump(4) << std::endl;
        #endif
        std::vector<Mod> mods;

        if (raw["total_hits"] == 0)
            return mods;

        for (auto &mod : raw["hits"])
        {
            Mod modrinth_mod;
            modrinth_mod.id = mod["project_id"];
            modrinth_mod.title = mod["title"];
            modrinth_mod.categories = mod["categories"];

            {
                std::vector<std::string> versions = mod["versions"];
                for (auto &version : versions)
                    modrinth_mod.game_versions.push_back(GameVersion(version));
            }

            mods.push_back(modrinth_mod);
        }

        return mods;
    }

    
    nlohmann::json raw_get_mod(const std::string &id)
    {
        auto [_, res] = Modion::HTTP::get("https://api.modrinth.com/v2/project/" + id);
        

        return res;
    }

    Mod get_mod(const std::string &id)
    {
        nlohmann::json raw = raw_get_mod(id);

        #ifdef MODION_DEBUG
        std::cout << raw.dump(4) << std::endl;
        #endif

        Mod modrinth_mod;

        modrinth_mod.id = id;
        modrinth_mod.title = raw["title"];
        modrinth_mod.categories = raw["categories"];

        {
            std::vector<std::string> versions = raw["game_versions"];
            for (auto &version : versions)
                modrinth_mod.game_versions.push_back(GameVersion(version));
        }


        return modrinth_mod;
    }

    std::vector<VersionFile> get_versions(const std::string &mod_id, std::vector<GameVersion> versions = {}, std::vector<Loader> loaders = {})
    {
        auto [_, res] = Modion::HTTP::get("https://api.modrinth.com/v2/project/" + mod_id + "/version",
        { 
            {
                "loaders",
                [&loaders]() -> std::string {
                    std::stringstream ss;
                    ss << '[';
                    for (auto &loader : loaders)
                        ss << loader << ",";
                    ss << ']';
                    return ss.str();
                }()
            },
            {
                "game_versions",
                [&versions]() -> std::string {
                    std::stringstream ss;
                    ss << '[';
                    for (auto &version : versions)
                        ss << version << ",";
                    ss << ']';
                    return ss.str();
                }()
            }
        }
        );

        #ifdef MODION_DEBUG
        std::cout << res.dump(4) << std::endl;
        #endif

        std::vector<VersionFile> version_files;

        for (auto &version : res)
        {
            VersionFile file;
            file.id = version["id"];
            file.filename = version["files"][0]["filename"];
            file.url = version["files"][0]["url"];
            file.hash = version["files"][0]["hashes"]["sha1"];
            file.mod_version = version["version_number"];
            version_files.push_back(file);
        }

        return version_files;
    }
    
    void download(const VersionFile &file)
    {
        std::ofstream out(file.filename, std::ios::binary);
    }

} // namespace Modrinth
