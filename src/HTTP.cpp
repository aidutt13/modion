#include "HTTP.hpp"
#include <list>

namespace Modion::HTTP
{
    template <typename T>
    auto get(const std::string &url, std::initializer_list<Parameter> params = {}, std::initializer_list<Header> headers = {})
        -> std::tuple<Request, Response>
    {
        curlpp::Cleanup cleaner;
        Request request;
        add_default_request_options(request);

        std::ostringstream url_stream;
        url_stream << url;

        if (params.size() > 0)
        {
            url_stream << "?" << std::get<0>(*params.begin()) << "=" << std::get<1>(*params.begin());

            for (auto it = params.begin() + 1; it != params.end(); ++it)
            {
                url_stream << "&" << std::get<0>(*it) << "=" << std::get<1>(*it);
            }
        }

        std::list<std::string> _headers;
        for (auto &header : headers)
        {
            _headers.push_back(std::get<0>(header) + ": " + std::get<1>(header));
        }
        request.setOpt(new curlpp::options::HttpHeader(_headers));

        std::ostringstream response_stream;

        request.setOpt(new curlpp::options::Url(url_stream.str()));
        request.setOpt(new curlpp::options::WriteStream(&response_stream));
        request.perform();

        return std::make_tuple(std::move(request), Response::parse(response_stream.str()));
    }

    auto post(const std::string &url, const std::string &body, std::initializer_list<Parameter> params = {})
        -> std::tuple<Request, Response>
    {
        std::cerr << "Uninplemented function: Modion::HTTP::post\n";
        exit(1);
    }

} // namespace Modion::HTTP
