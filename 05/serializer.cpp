#include "serializer.h"
#include <sstream>
#include <iostream>
#include <string>
#include <cassert>

Error Serializer::process(bool& value)
{

    if (value == true)
        out_ << "true" << Separator;
    else if (value = false)
        out_ << "false" << Separator;

    return Error::NoError;
}

Error Serializer::process(uint64_t& value)
{
    out_ << std::to_string(value) << Separator;

    return Error::NoError;
}

Error Deserializer::process(bool& value)
{
    std::string text;
    in_ >> text;

    if (text == "true")
        value = true;
    else if (text == "false")
        value = false;
    else
        return Error::CorruptedArchive;

    return Error::NoError;
}

Error Deserializer::process(uint64_t& value)
{
    std::string text;
    in_ >> text;

    try {
        value = stoi(text);
    }
    catch(std::exception e) {
        return Error::CorruptedArchive;
    }

    return Error::NoError;
}

struct Data
{
    uint64_t a;
    bool b;
    uint64_t c;

    template <class T>
    Error serialize(T& serializer)
    {
        return serializer(a, b, c);
    }
};

struct WrongData
{
    char a;
    bool b;
    uint64_t c;

    template <class T>
    Error serialize(T& serializer)
    {
        return serializer(a, b, c);
    }
};


void test_serilizer(void) {

    Data x { 1, true, 2 };

    std::stringstream stream;

    Serializer serializer(stream);
    const Error err1 = serializer.save(x);

    Data y { 0, false, 0 };

    Deserializer deserializer(stream);
    const Error err2 = deserializer.load(y);

    assert(err1 == Error::NoError);
    assert(err2 == Error::NoError);
    assert(x.a == y.a);
    assert(x.b == y.b);
    assert(x.c == y.c);
}

void wrong_data() {

    Data x { 1, true, 2 };

    std::stringstream stream;

    Serializer serializer(stream);
    const Error err1 = serializer.save(x);

    Data y { 0, false, 0 };

    Deserializer deserializer(stream);
    const Error err2 = deserializer.load(y);

    assert(err1 == Error::NoError);
    assert(err2 == Error::NoError);
    assert(x.a == y.a);
    assert(x.b == y.b);
    assert(x.c == y.c);
}

void test_corrupted() {
    WrongData x { '1', true, 2 };

    std::stringstream stream;

    Serializer serializer(stream);
    const Error err1 = serializer.save(x);

    WrongData y { '0', false, 0 };

    Deserializer deserializer(stream);
    const Error err2 = deserializer.load(y);

    assert(err1 == Error::CorruptedArchive);
    assert(err2 == Error::CorruptedArchive);
}
