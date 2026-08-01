#ifndef PARSER_H
#define PARSER_H

#include <lexbor/html/html.h>
#include <string>
#include <vector>

class HtmlNode {
    public:
        explicit HtmlNode(lxb_dom_node_t *node);
        std::string text() const;
        std::string attr(const std::string &name) const;
        std::vector<HtmlNode> select(const std::string &selector) const;

    private:
        lxb_dom_node_t *node;
};

class Html {
    public:
        Html();
        ~Html();
        Html(const Html &) = delete;
        Html &operator=(const Html &) = delete;

        bool parse(const std::string &html);
        std::vector<HtmlNode> select(const std::string &selector) const;

    private:
        lxb_html_document_t *document;
};

#endif
