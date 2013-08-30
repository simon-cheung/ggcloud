#pragma once
namespace oo{

    class oriole_object
    {
    public:
        oriole_object(void);
        virtual ~oriole_object(void);

        virtual std::istream& serialize(std::istream& istream) = 0;
        virtual std::ostream& deserialize(std::istream& istream) = 0;

        virtual const std::string& getId() const = 0;

        virtual int proc_msg(Message* msg) = 0;
    };

}