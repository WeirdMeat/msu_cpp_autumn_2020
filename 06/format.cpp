#include "format.h"
#include <sstream>
#include <iostream>
#include <string>
#include <cassert>

struct Error {
    std::string message_;
    const char* fileName_;
    int line_;
    Error(const std::string& message,
        const char* fileName, int line)
        : message_(message)
        , fileName_(fileName)
        , line_(line)
    {
    }
    void ShowError(std::ostream& out) const {
        out << this->fileName_ << ":" << this->line_ << ": " << "error: " << this->message_ << std::endl;
    }

    void ShowError() const {
        std::cout << this->fileName_ << ":" << this->line_ << ": " << "error: " << this->message_ << std::endl;
    }
};


template <typename T>
std::string sput(int num, int ind, T cur) {
    std::stringstream res;
    if (num != ind) {
        std::string message = "missing required argument";
        throw Error(message, __FILE__, __LINE__);
    }
    res << cur;
    return res.str();
}

template <typename T, typename... Args>
std::string sput(int num, int ind, T cur, Args... rest) {
    std::stringstream res;
    if (num == ind) {
        res << cur;
        return res.str();
    }
    return sput(num, ind + 1, rest...);
}

template <typename... Args>
std::string format(const char* s, Args... args) {
    std::string text, num;
    while (*s) {
        if (*s == '{') {
            num = "";
            s++;
            if (*s == '}') {
                throw Error("missing number in {}", __FILE__, __LINE__);
            }
            while (*s != '}') {
                if (*s < '0' || *s > '9') {
                    throw Error("unexpected symbol in {}", __FILE__, __LINE__);
                }
                num += s;
                s++;
            }
            text += sput(stoi(num), 0, args...);
            s++;
            if (!*s) {
                break;
            }
        }
        text += *s++;
    }
    try {
        std::ostringstream out;
        out << text;
    } catch(...) {
        throw Error("can't put in ostream", __FILE__, __LINE__);
    }
    return text;
}

void test_format() {
    auto text = format("{1}+{1} = {0}", 2, "one");
    assert(text == "one+one = 2");
}

void out_boundary() {
    try {
        auto text = format("{1}+{1} = {2}", 2, "one");
    } catch(const Error& e) {
        std::ostringstream out;
        e.ShowError(out);
        assert(out.str() == "format.cpp:33: error: missing required argument\n");
        return;
    }
    assert(false);
}

void no_number() {
    try {
        auto text = format("{}+{1} = {2}", 2, "one");
    } catch(const Error& e) {
        std::ostringstream out;
        e.ShowError(out);
        assert(out.str() == "format.cpp:57: error: missing number in {}\n");
        return;
    }
    assert(false);
}

void not_number() {
    try {
        auto text = format("{a}+{1} = {2}", 2, "one");
    } catch(const Error& e) {
        std::ostringstream out;
        e.ShowError(out);
        assert(out.str() == "format.cpp:61: error: unexpected symbol in {}\n");
        return;
    }
    assert(false);
}
