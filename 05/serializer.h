#ifndef SERIALIZER_H
#define SERIALIZER_H

#include <iostream>
#include <utility>

enum class Error
{
    NoError,
    CorruptedArchive
};

class Serializer
{
    std::ostream &out_;
    static constexpr char Separator = ' ';

public:
    explicit Serializer(std::ostream &out) : out_(out) {}

    template <class T>
    Error save(T& object)
    {
        return object.serialize(*this);
    }

    template <class... ArgsT>
    Error operator()(ArgsT... args)
    {
        return process(args...);
    }
    
private:
    template <class T, class ...ArgsT>
    Error process(T &&value, ArgsT &&...args)
    {
        if (process(value) == Error::NoError) {
            return process(std::forward<ArgsT>(args)...);
        } else {
            return Error::CorruptedArchive;
        }
    }

    Error process(uint64_t &value)
    {
        out_ << value << Separator;
        return Error::NoError;
    }

    Error process(bool &value)
    {
        out_ << (value ? "true" : "false") << Separator;
        return Error::NoError;
    }
};

class Deserializer
{
    std::istream &in_;
    static constexpr char Separator = ' ';

public:
    explicit Deserializer(std::istream &in) : in_(in) {}

    template <class T>
    Error load(T& object)
    {
        return object.serialize(*this);
    }

    template <class... ArgsT>
    Error operator()(ArgsT &&...args)
    {
        return process(std::forward<ArgsT>(args)...);
    }
    
private:
    template <class T, class ...ArgsT>
    Error process(T &&value, ArgsT &&...args)
    {
        if (process(value) == Error::NoError) {
            return process(std::forward<ArgsT>(args)...);
        } else {
            return Error::CorruptedArchive;
        }
    }

    Error process(uint64_t &value)
    {
        std::string buf;
        in_ >> buf;
        try {
            value = std::stoull(buf);
        } catch (...) {
            return Error::CorruptedArchive;
        }
        return Error::NoError;
    }

    Error process(bool &value)
    {
        std::string buf;
        in_ >> buf;
        if (buf == "true") {
            value = true;
        } else if (buf == "false") {
            value = false;
        } else {
            return Error::CorruptedArchive;
        }
        return Error::NoError;
    }
};

#endif