#include "osutils.h"
#include "idutil.h"

namespace oo{
    using namespace std;

    std::string idutil::make_id(std::string body, const std::string* child, const uint64* pind){
        std::string id;
        if(child){
            id = *child;
            id += '@';
        }
        id += body;
        if(pind){
            std::stringstream ss;
            ss << *pind;
            id += '?';
            id += ss.str();
        }
        return id;
    }
    string idutil::make_child_id(string body, std::string child){
        return child + '@' + body;
    }

    string idutil::make_trans_id(string id, uint64 ind){
        std::stringstream ss;
        ss << ind;
        return id + "?" + ss.str();
    }

    void idutil::split_id(std::string id, std::string* body, std::string* child, uint64* ind){
        std::string::size_type posc = id.find_first_of('@');
        std::string::size_type post = id.find_first_of('?');
        if(posc != std::string::npos){
            if(child)
                *child = id.substr(0, posc);
        }else
            posc = 0;
        if(post != std::string::npos && ind){
            std::stringstream ss(id.substr(post + 1));
            ss >> *ind;
        }
        if(body)
            *body = id.substr(posc, post);
    }

    std::string idutil::child_from_id(std::string id){
        std::string child;
        split_id(id, NULL, &child, NULL);
        return child;
    }

    string idutil::body_from_id(string id){
        std::string body;
        split_id(id, &body, NULL, NULL);
        return body;
    }

    uint64 idutil::trans_from_id(string id){
        uint64 ind;
        split_id(id, NULL, NULL, &ind);
        return ind;
    }
}

