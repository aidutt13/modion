#pragma once
#include <nlohmann/json.hpp>
#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>

#include <initializer_list>
#include <functional>
#include <tuple>

namespace Modion::HTTP
{
    using Response = nlohmann::json;
    using Request = curlpp::Easy;
    using Parameter = std::tuple<std::string, std::string>;
    using Header = std::tuple<std::string, std::string>;

    inline void add_default_request_options(Request &request)
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

    template <std::function<std::ostream()> CREATE_STREAM>
    auto get(const std::string &url, std::initializer_list<Parameter> params = {}, std::initializer_list<Header> headers = {})
        -> std::tuple<Request, Response>;

    auto post(const std::string &url, const std::string &body, std::initializer_list<Header> headers = {})
        -> std::tuple<Request, Response>;
    
} // namespace Modion
