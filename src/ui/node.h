#pragma once
#include <memory>
#include <numeric>
#include <string>
#include <variant>
#include <vector>

using std::shared_ptr;
using std::vector;

class Node {
  public:
    struct Type {};
    struct None : public Type {};
    struct Label : public Type {
        std::string text;
        Label(const std::string& a_text) : text(a_text) {}
        Label(const char* a_text) : text(a_text) {}
    };
    struct Row : public Type {};
    struct Column : public Type {};
    struct Text : public Type {};

    // enum Type { None, Label, Row, Column, Text };

    Node() {}
    Node(Type a_type) : type(a_type) {}
    Node(Type a_type, const vector<shared_ptr<Node>>& a_children)
        : type(a_type), children(a_children) {}
    Node(const vector<shared_ptr<Node>>& a_children) : children(a_children) {}
    ~Node() {}

    static shared_ptr<Node> create() { return std::make_shared<Node>(); }
    static shared_ptr<Node> create(Type a_type) {
        return std::make_shared<Node>(a_type);
    }
    static shared_ptr<Node> create(Type a_type,
                                   const vector<shared_ptr<Node>>& a_children) {
        return std::make_shared<Node>(a_type, a_children);
    }
    static shared_ptr<Node> create(const vector<shared_ptr<Node>>& a_children) {
        return std::make_shared<Node>(a_children);
    }

    int get_child_count() const {
        int count = children.size();
        for (auto& child : children) {
            count += child->get_child_count();
        }
        return count;
    }

  private:
    vector<shared_ptr<Node>> children;
    Type type;
};
