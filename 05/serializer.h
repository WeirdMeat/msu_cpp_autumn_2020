#pragma once
#include <sstream>
#include <iostream>
#include <istream>
#include <ostream>
#include <string>
#include <cassert>

enum class Error
{
    NoError,
    CorruptedArchive
};

class Serializer
{
    static constexpr char Separator = ' ';
public:
    explicit Serializer(std::ostream& out)
        : out_(out)
    {
    }

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
    std::ostream& out_;
    template <typename T>
    Error process(T value) {
        return Error::CorruptedArchive;
    }
    Error process(bool& value);
    Error process(uint64_t& value);
    template <typename T, typename... Args>
    Error process(T value, Args... args) {
        if (process(value) == Error::NoError) {
            return process(args...);
        } else {
            return Error::CorruptedArchive;
        }
    }
};

class Deserializer
{
    static constexpr char Separator = ' ';
public:
    explicit Deserializer(std::istream& in)
        : in_(in)
    {
    }

    template <class T>
    Error load(T& object)
    {
        return object.serialize(*this);
    }

    template <class... ArgsT>
    Error operator()(ArgsT&... args)
    {
        return process(args...);
    }
private:
    std::istream& in_;
    template <typename T>
    Error process(T value) {
        return Error::CorruptedArchive;
    }
    Error process(bool& value);
    Error process(uint64_t& value);
    template <typename T, typename... Args>
    Error process(T& value, Args&... args) {
        if (process(value) == Error::NoError) {
            return process(args...);
        } else {
            return Error::CorruptedArchive;
        }
    }
};

struct Data;
struct WrongData;
void test_serilizer();
void test_corrupted();
