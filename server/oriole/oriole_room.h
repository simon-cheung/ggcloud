#pragma once
namespace oo{

    class oriole_room : public oriole_object
    {
    public:
        oriole_room(void);
        virtual ~oriole_room(void);

        virtual ZeroCopyOutputStream& serialize(ZeroCopyOutputStream& istream);
        virtual ZeroCopyInputStream& deserialize(ZeroCopyInputStream& istream);

        virtual const std::string& getId() const;
        virtual int proc_msg(Message* msg);
    };

}