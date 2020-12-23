#pragma once
#include <ostream>

class BigInt
{
private:
    int *start;
    int size;
    int cap;
    bool is_negative;
    void resize(int needs);
    void no_nulls();
    static const int BASE = 1000000000;
public:
    BigInt();
    BigInt(int int_num);
    BigInt(std::string str_num);
    BigInt(const BigInt &num);
    BigInt& operator=(const BigInt &num);
    BigInt(BigInt&& num): start(std::move(num.start)),
                size(std::move(num.size)),
                cap(std::move(num.cap)),
                is_negative(std::move(num.is_negative)) {};        
    BigInt& operator=(BigInt&& num) {
        this->start = std::move(num.start);
        this->size = std::move(num.size);
        this->cap = std::move(num.cap);
        this->is_negative = std::move(num.is_negative);
        return *this;
    }
    friend std::ostream& operator<<(std::ostream &out, const BigInt &num);
    friend BigInt operator+(const BigInt &num1, const BigInt &num2);
    friend BigInt operator+(const BigInt &num, int n);
    friend BigInt operator+(int n, const BigInt &num);
    BigInt& operator+=(int n);
    BigInt& operator+=(const BigInt &num);
    friend BigInt operator-(const BigInt &num1, const BigInt &num2);
    friend BigInt operator-(const BigInt &num, int n);
    friend BigInt operator-(int n, const BigInt &num);
    BigInt& operator-=(int n);
    BigInt& operator-=(const BigInt &num);
    friend BigInt operator*(const BigInt &num1, const BigInt &num2);
    friend BigInt operator*(const BigInt &num, int n);
    friend BigInt operator*(int n, const BigInt &num);
    BigInt& operator*=(int n);
    BigInt& operator*=(const BigInt &num);
    BigInt operator-() const;
    friend bool operator==(const BigInt &num1, const BigInt &num2);
    friend bool operator!=(const BigInt &num1, const BigInt &num2);
    friend bool operator>(const BigInt &num1, const BigInt &num2);
    friend bool operator>=(const BigInt &num1, const BigInt &num2);
    friend bool operator<(const BigInt &num1, const BigInt &num2);
    friend bool operator<=(const BigInt &num1, const BigInt &num2);
    ~BigInt();
};

void std_test();
void plus_test();
void sub_test();
void mult_test();
