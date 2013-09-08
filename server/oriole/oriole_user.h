#pragma once
namespace oo{

    class oriole_user : public oriole_object
    {
    public:
        oriole_user(void);
        virtual ~oriole_user(void);

        virtual ZeroCopyOutputStream& serialize(ZeroCopyOutputStream& istream);
        virtual ZeroCopyInputStream& deserialize(ZeroCopyInputStream& ostream);

        virtual const std::string& getId() const;

        virtual int proc_msg(const std::string& from, Message* msg);
    };

}