#include <string>
#include <functional>

typedef unsigned long long int llu;

class TokenParser
{
private:
    std::function<void()> StartCallback;
    std::function<void(llu)> DigitTokenCallback;
    std::function<void(std::string)> StringTokenCallback;
    std::function<void()> EndCallback;
public:
    TokenParser();
    void SetStartCallback(std::function<void()> func);
    void SetDigitTokenCallback(std::function<void(llu)> func);
    void SetStringTokenCallback(std::function<void(std::string)> func);
    void SetEndCallback(std::function<void()> func);
    void Parse(std::string Str);
    ~TokenParser();
};

void start_test();
void end_test();
void digit_test();
void string_test();
void all_test();
void new_callback_test();
