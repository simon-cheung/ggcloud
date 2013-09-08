#pragma once
namespace oo{
    using google::protobuf::io::ZeroCopyInputStream;
    using google::protobuf::io::ZeroCopyOutputStream;

    class oriole_object
    {
    public:
        oriole_object(void);
        virtual ~oriole_object(void);

        virtual ZeroCopyOutputStream& serialize(ZeroCopyOutputStream& istream) = 0;
        virtual ZeroCopyInputStream& deserialize(ZeroCopyInputStream& istream) = 0;

        virtual const std::string& getId() const = 0;

        virtual int proc_msg(Message* msg) = 0;
    };

}