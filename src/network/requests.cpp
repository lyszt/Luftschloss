#include "requests.h"
#include <cpr/cpr.h>
#include <cpr/response.h>
#include <optional>

void Requests::request(Method method, std::string &url,
                       std::optional<json> body) {
  session.SetUrl(cpr::Url{url});
  session.SetTimeout(cpr::Timeout{timeout_ms});
  session.SetHeader({{"Content-Type", "application/json"}});
  session.SetBody(body ? cpr::Body{body->dump()} : cpr::Body{});
  switch (method) {
  case Method::Get:
    response = session.Get();
    break;
  case Method::Post:
    response = session.Post();
    break;
  case Method::Put:
    response = session.Put();
    break;
  case Method::Delete:
    response = session.Delete();
    break;
  case Method::Patch:
    response = session.Patch();
    break;
  }
}