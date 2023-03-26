#include "Modrinth.hpp"

#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <list>

namespace Modion::Modrinth
{
    void add_default_request_options(curlpp::Easy &request)
    {
        std::list<std::string> headers;
        headers.push_back("Content-Type: application/json");
        headers.push_back("User-Agent: aidutt13/modion (ajdutt13@gmail.com)");
        request.setOpt(new curlpp::options::HttpHeader(headers));
        request.setOpt(new curlpp::options::FollowLocation(true));

        #ifdef MODION_DEBUG
        request.setOpt(new curlpp::options::Verbose(true));
        #endif
    }

    nlohmann::json raw_search(const std::string &name, unsigned int limit)
    {
        curlpp::Cleanup cleaner;
        curlpp::Easy request;
        add_default_request_options(request);

        std::ostringstream url;

        url << "https://api.modrinth.com/v2/search?query=" << name << "&limit=" << limit;

        std::ostringstream response;

        request.setOpt(new curlpp::options::Url(url.str()));
        request.setOpt(new curlpp::options::WriteStream(&response));
        request.perform();
        

        return nlohmann::json::parse(response.str());
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
        curlpp::Cleanup cleaner;
        curlpp::Easy request;
        add_default_request_options(request);

        std::ostringstream url;

        url << "https://api.modrinth.com/v2/project/" << id;

        std::ostringstream response;

        request.setOpt(new curlpp::options::Url(url.str()));
        request.setOpt(new curlpp::options::WriteStream(&response));
        request.perform();
        

        return nlohmann::json::parse(response.str());
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
    
} // namespace Modrinth
