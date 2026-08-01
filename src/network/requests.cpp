#include "requests.h"
#include <cpr/cpr.h>
#include <cpr/response.h>
#include <optional>

void Requests::request(Method method, std::string &url,
                       std::optional<json> body) {
  cpr::Url cpr_url = cpr::Url{url};
  cpr::Body cpr_body = {};
  if (body) {
    cpr::Body cpr_body = cpr::Body{body->dump()};
  }
  cpr::Header header = {{"Content-Type", "application/json"}};
  cpr::Response r;
  switch (method) {
  case Method::Get:
    r = cpr::Get(cpr_url);
    break;
  case Method::Post:
    if (!body) {
      break;
    }
    r = cpr::Post(cpr_url, header, cpr_body);
    break;
  case Method::Put:
    r = cpr::Put(cpr_url, header, cpr_body);
    break;
  case Method::Delete:
    r = cpr::Delete(cpr_url, header, cpr_body);
    break;
  case Method::Patch:
    r = cpr::Patch(cpr_url, header, cpr_body);
    break;
  default:
    break;
  };
  this->response = r;
}