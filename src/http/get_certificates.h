#ifndef _CASERV_HTTP_GET_CERTIFICATES_H_
#define _CASERV_HTTP_GET_CERTIFICATES_H_

#include "./../service/caservice.h"
#include "base/get_endpoint.h"

#include <httpserver.hpp>
#include <string_view>

namespace http {

using namespace nlohmann;
using namespace nlohmann::literals;


class GetCertificatesEndpoint : public ApiGetEndpoint<std::string_view> {
public:
  GetCertificatesEndpoint(serivce::CaServicePtr caService)
      : _caService(caService) {}
  virtual ~GetCertificatesEndpoint() = default;
  const char *Route() const override { return "ca/{caSerial}/certificates"; }

protected:
  std::string_view
  BuildRequestModel(const httpserver::http_request &req) override {
    auto pathPieces = req.get_path_pieces();
    auto args = req.get_arg("caSerial").get_all_values();
    if (args.empty())
      throw std::runtime_error("Invalid request");
    return args[0];
  }

  HttpResponsePtr Handle(const std::string_view &caSerial) override {
    auto certs = _caService->GetCertificates(caSerial.data());
    json response = certs;

    return HttpResponsePtr(
        new httpserver::string_response(response.dump(), 200));
  }

private:
  serivce::CaServicePtr _caService;
};

} // namespace http

#endif //_CASERV_HTTP_GET_CERTIFICATES_H_
