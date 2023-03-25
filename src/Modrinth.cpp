#include "Modrinth.hpp"

#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <list>

namespace Modrinth
{
    std::string raw_search(const std::string &name)
    {
        curlpp::Cleanup cleaner;
        curlpp::Easy request;
        std::string url = "https://api.modrinth.com/api/v2/search?query=" + name;

        std::list<std::string> headers;
        headers.push_back("Content-Type: application/json");

        request.setOpt(new curlpp::options::Url(url));
        request.setOpt(new curlpp::options::HttpHeader(headers));
        request.setOpt(new curlpp::options::Verbose(true));
        request.setOpt(new curlpp::options::FollowLocation(true));
        std::ostringstream response;
        request.setOpt(new curlpp::options::WriteStream(&response));
        request.perform();
        return response.str();
    }
    
} // namespace Modrinth
