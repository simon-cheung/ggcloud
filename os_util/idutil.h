#pragma once

namespace oo{
    // id format
    // {msg body} -> x.y.z/a/b/c[/]
    // {msg body, child} -> /d/e/f@x.y.z/a/b/c
    // {msg body, child, transaction} -> /d/e/f@x.y.z/a/b/c?1234

    class idutil{
    public:
        static std::string make_id(std::string body, const std::string* child = NULL, const uint64* pind = NULL);
        static std::string make_child_id(std::string body, std::string child);
        static std::string make_trans_id(std::string id, uint64 ind);

        static void split_id(std::string id, std::string* body=NULL, std::string* child=NULL, uint64* ind=NULL);
        static std::string body_from_id(std::string id);
        static std::string child_from_id(std::string id);
        static uint64 trans_from_id(std::string id);
    };
}