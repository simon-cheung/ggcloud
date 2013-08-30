#pragma once
namespace oo{

    class oriole_service        
    {
    public:
        oriole_service(void);
        ~oriole_service(void);

        std::istream& serialize(std::istream& istream);
        std::ostream& deserialize(std::istream& istream);

        void startup();
    };

}