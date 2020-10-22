#include "parser.h"
#include <string>
#include <functional>
#include <sstream>
#include <cassert>

typedef unsigned long long int llu;

TokenParser::TokenParser()
{
    this->StartCallback = [](){};
    this->DigitTokenCallback = [](llu){};
    this->StringTokenCallback = [](std::string){};
    this->EndCallback = [](){};
}
void TokenParser::SetStartCallback(std::function<void()> func)
{
    this->StartCallback = func;
    return;
}
void TokenParser::SetDigitTokenCallback(std::function<void(llu)> func)
{
    this->DigitTokenCallback = func;
    return;
}
void TokenParser::SetStringTokenCallback(std::function<void(std::string)> func)
{
    this->StringTokenCallback = func;
    return;
}
void TokenParser::SetEndCallback(std::function<void()> func)
{
    this->EndCallback = func;
    return;
}
void TokenParser::Parse(std::string Str)
{
    std::string word;
    llu numb;
    std::stringstream ss(Str);
    this->StartCallback();
    while(ss >> word) {
        try {
            numb = std::stoull(word);
            this->DigitTokenCallback(numb);
        }
        catch(std::invalid_argument e) {
            this->StringTokenCallback(word);
        }
    }
    this->EndCallback();
}
TokenParser::~TokenParser(){};

void start_test() {
    TokenParser tested;
    bool check = false;
    tested.SetStartCallback([&check](){check = true;});
    tested.Parse("123 abc");
    assert(check);
}
void end_test() {
    TokenParser tested;
    bool check = false;
    tested.SetEndCallback([&check](){check = true;});
    tested.Parse("123 abc");
    assert(check);
}
void digit_test() {
    TokenParser tested;
    int check = 0;
    tested.SetDigitTokenCallback([&check](llu dig){check += dig;});
    tested.Parse("1 2 3 abc");
    assert(check == 6);
}
void string_test() {
    TokenParser tested;
    std::string check;
    tested.SetStringTokenCallback([&check](std::string str){check += str;});
    tested.Parse("abc 123 def");
    assert(check == "abcdef");
}
void all_test() {
    TokenParser tested;
    bool check_start = false, check_end = false;
    int check_digits = 0;
    std::string check_string;
    tested.SetStartCallback([&check_start](){check_start = true;});
    tested.SetDigitTokenCallback([&check_digits](llu dig){check_digits += dig;});
    tested.SetStringTokenCallback([&check_string](std::string str){check_string += str;});
    tested.SetEndCallback([&check_end](){check_end = true;});
    tested.Parse("abc 1 2 def 3 ghi");
    assert(check_start && check_digits == 6 && check_string == "abcdefghi" && check_end);
}
void new_callback_test() {
    TokenParser tested;
    bool check = false;
    tested.SetStartCallback([&check](){assert(check);});
    tested.SetStartCallback([&check](){check = true;});
    tested.Parse("123 abc");
    assert(check);
}
