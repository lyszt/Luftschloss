#include "parser.h"
#include <lexbor/css/css.h>
#include <lexbor/selectors/selectors.h>

namespace {

lxb_status_t collect_node(lxb_dom_node_t *node, lxb_css_selector_specificity_t,
                          void *ctx) {
  auto *out = static_cast<std::vector<lxb_dom_node_t *> *>(ctx);
  out->push_back(node);
  return LXB_STATUS_OK;
}

// Runs a CSS selector over the subtree rooted at root. Parser and selector
// engine are built per call so a failure in one query cannot corrupt the next.
std::vector<lxb_dom_node_t *> find_nodes(lxb_dom_node_t *root,
                                         const std::string &selector) {
  std::vector<lxb_dom_node_t *> out;
  if (root == nullptr) {
    return out;
  }

  lxb_css_parser_t *parser = lxb_css_parser_create();
  if (lxb_css_parser_init(parser, nullptr) != LXB_STATUS_OK) {
    lxb_css_parser_destroy(parser, true);
    return out;
  }

  lxb_selectors_t *selectors = lxb_selectors_create();
  if (lxb_selectors_init(selectors) != LXB_STATUS_OK) {
    lxb_selectors_destroy(selectors, true);
    lxb_css_parser_destroy(parser, true);
    return out;
  }

  lxb_css_selector_list_t *list = lxb_css_selectors_parse(
      parser, reinterpret_cast<const lxb_char_t *>(selector.data()),
      selector.size());
  if (list != nullptr) {
    lxb_selectors_find(selectors, root, list, collect_node, &out);
    lxb_css_selector_list_destroy_memory(list);
  }

  lxb_selectors_destroy(selectors, true);
  lxb_css_parser_destroy(parser, true);
  return out;
}

} // namespace

HtmlNode::HtmlNode(lxb_dom_node_t *node) : node(node) {}

std::string HtmlNode::text() const {
  if (node == nullptr) {
    return "";
  }
  size_t len = 0;
  lxb_char_t *content = lxb_dom_node_text_content(node, &len);
  if (content == nullptr) {
    return "";
  }
  std::string result(reinterpret_cast<char *>(content), len);
  lxb_dom_document_destroy_text(node->owner_document, content);
  return result;
}

std::string HtmlNode::attr(const std::string &name) const {
  if (node == nullptr || node->type != LXB_DOM_NODE_TYPE_ELEMENT) {
    return "";
  }
  lxb_dom_element_t *element = lxb_dom_interface_element(node);
  size_t len = 0;
  const lxb_char_t *value = lxb_dom_element_get_attribute(
      element, reinterpret_cast<const lxb_char_t *>(name.data()), name.size(),
      &len);
  if (value == nullptr) {
    return "";
  }
  return std::string(reinterpret_cast<const char *>(value), len);
}

std::vector<HtmlNode> HtmlNode::select(const std::string &selector) const {
  std::vector<HtmlNode> result;
  for (lxb_dom_node_t *matched : find_nodes(node, selector)) {
    result.emplace_back(matched);
  }
  return result;
}

Html::Html() : document(nullptr) {}

Html::~Html() {
  if (document != nullptr) {
    lxb_html_document_destroy(document);
  }
}

bool Html::parse(const std::string &html) {
  if (document != nullptr) {
    lxb_html_document_destroy(document);
    document = nullptr;
  }
  document = lxb_html_document_create();
  if (document == nullptr) {
    return false;
  }
  lxb_status_t status = lxb_html_document_parse(
      document, reinterpret_cast<const lxb_char_t *>(html.data()), html.size());
  return status == LXB_STATUS_OK;
}

std::vector<HtmlNode> Html::select(const std::string &selector) const {
  std::vector<HtmlNode> result;
  if (document == nullptr) {
    return result;
  }
  for (lxb_dom_node_t *matched :
       find_nodes(lxb_dom_interface_node(document), selector)) {
    result.emplace_back(matched);
  }
  return result;
}
