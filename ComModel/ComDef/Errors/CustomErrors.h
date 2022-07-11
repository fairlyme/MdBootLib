#pragma once
#include <stdexcept>

namespace MdLib {

#define NameOfVar(Variable) (void(Variable),#Variable)

#define NameOfFunc(Function) (void(&Function),#Function)

#define NameOfMethod(ClassName,Method) (void(&ClassName::Method),#Method)

#define NameofType(Type) (void(sizeof(Type)),#Type)

    class NotImplErr : public std::logic_error
    {
    private:

        std::string _text;

        NotImplErr(const char* message, const char* function)
            :
            std::logic_error("Not Implemented")
        {
            _text = message;
            _text += " : ";
            _text += function;
        };

    public:

        NotImplErr()
            :
            NotImplErr("Not Implememented", __FUNCTION__)
        {
        }

        NotImplErr(const char* message)
            :
            NotImplErr(message, __FUNCTION__)
        {
        }

        virtual const char* what() const throw()
        {
            return _text.c_str();
        }
    };



    class InvalidParamErr : public std::runtime_error
    {
    private:

        std::string _text;

        InvalidParamErr(const char* message, const char* function)
            :
            std::runtime_error("Invalid Parameter")
        {
            _text = message;
            _text += " : ";
            _text += function;
        };

    public:

        InvalidParamErr()
            :
            InvalidParamErr("Invalid Parameter", __FUNCTION__)
        {
        }

        InvalidParamErr(const char* message)
            :
            InvalidParamErr(message, __FUNCTION__)
        {
        }

        virtual const char* what() const throw()
        {
            return _text.c_str();
        }
    };
}