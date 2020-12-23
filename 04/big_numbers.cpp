#include "big_numbers.h"
#include <ostream>
#include <stdexcept>
#include <cassert>
#include <sstream>
#include <iostream>

void BigInt::resize(int needs)
{
    if (this->cap >= needs) {
        return;
    }
    while (this->cap < needs) {
        this->cap *= 2;
    }
    if (this->start != nullptr) {
        int *temp = this->start;
        this->start = new int[this->cap];
        for (int i = 0; i < this->size; i++) {
            this->start[i] = temp[i];
        }
        delete [] temp;
    } else {
        this->start = new int[this->cap];
    }
}
void BigInt::no_nulls()
{
    while (this->size > 1 && this->start[this->size - 1] == 0) {
        this->size--;
    }
    if (this->size == 1 && this->start[0] == 0) {
        this->is_negative = false;
    }
}
BigInt::BigInt(): size(1), cap(1), is_negative(false)
{
    this->start = new int[1];
    this->start[0] = 0;
}
BigInt::BigInt(int int_num): size(1), cap(1)
{
    if (int_num < 0) {
        this->is_negative = true;
        int_num = -int_num;
    } else {
        this->is_negative = false;
    }
    this->start = new int[1];
    this->start[0] = int_num;
}
BigInt::BigInt(std::string str_num)
{
    if (str_num.length() == 0) {
        this->is_negative = false;
        this->size = 1;
        this->cap = 1;
        this->start = new int[1];
        this->start[0] = 0;
    } else {
        if (str_num[0] == '-') {
            str_num = str_num.substr(1);
            this->is_negative = true;
        } else {
            this->is_negative = false;
        }
        this->size = (str_num.length() + 8) / 9;
        this->start = new int[1];
        this->cap = 1;
        this->resize(this->size);
        int ind  = 0;
        for (int i = str_num.length(); i > 0; i -= 9) {
            if (i < 9) {
                this->start[ind] = atoi(str_num.substr(0, i).c_str());
                ind++;
            } else {
                this->start[ind] = atoi(str_num.substr(i - 9, 9).c_str());
                ind++;
            }
        }
        this->no_nulls();
    }
}
BigInt::BigInt(const BigInt &num): cap(num.cap), is_negative(num.is_negative), size(num.size)
{
    this->start = new int[this->cap];
    for (size_t i = 0; i < this->size; i++) {
        this->start[i] = num.start[i];
    }
}
BigInt& BigInt::operator=(const BigInt &num)
{
    if (this == &num) {
        return *this;
    }

    delete [] this->start;
    this->start = nullptr;

    this->is_negative = num.is_negative;
    this->cap = num.cap;
    this->size = num.size;
    this->start = new int[this->cap];
    for (size_t i = 0; i < this->size; i++) {
        this->start[i] = num.start[i];
    }
    return *this;
}
std::ostream& operator<<(std::ostream &out, const BigInt &num)
{
    if (num.size == 0) {
        out << 0;
    } else {
        if (num.is_negative) {
            out << '-';
        }
        out << num.start[num.size - 1];
        std::string ss;
        for (int ind = num.size - 2; ind >= 0; ind--) {
            ss = std::to_string(num.start[ind]);
            for (size_t i = 0; i < 9 - ss.length(); i++) {
                out << "0";
            }
            out << ss;
        }
    }
    return out;
}
BigInt operator+(const BigInt &num1, const BigInt &num2)
{
    if (num1.is_negative) {
        if (num2.is_negative) {
            return -(-num1 + (-num2));
        } else {
            return num1 - (-num2);
        }
    }
    if (num2.is_negative) {
        return num1 - (-num2);
    }

    BigInt res;
    res.resize(std::max(num1.size, num2.size) + 1);

    int carry = 0;
    long long int sum;
    size_t ind;
    for (ind = 0; ind < num1.size && ind < num2.size; ind++) {
        sum = num1.start[ind] + num1.start[ind] + carry;
        carry = sum >= BigInt::BASE;
        res.start[ind] = sum % BigInt::BASE;
    }
    if (num1.size > num2.size) {
        for (; ind < num1.size; ind++) {
            sum = num1.start[ind] + carry;
            carry = sum >= BigInt::BASE;
            res.start[ind] = sum % BigInt::BASE;
        }
    }
    if (num1.size < num2.size) {
        for (; ind < num2.size; ind++) {
            sum = num2.start[ind] + carry;
            carry = sum >= BigInt::BASE;
            res.start[ind] = sum % BigInt::BASE;
        }
    }
    if (carry > 0) {
        res.start[ind] = carry;
        res.size = ind + 1;
    } else {
        res.size = ind;
    }

    return res;
}
BigInt operator+(const BigInt &num, int n)
{
    BigInt num_new(n);

    return num + num_new;
}
BigInt operator+(int n, const BigInt &num)
{
    BigInt num_new(n);

    return num + num_new;
}
BigInt& BigInt::operator+=(const BigInt &num)
{
    return *this = (*this + num);
}
BigInt& BigInt::operator+=(int n)
{
    BigInt num_new(n);
    return *this = (*this + num_new);
}
BigInt operator-(const BigInt &num1, const BigInt &num2)
{
    if (num2.is_negative) {
        return num1 + (-num2);
    } else if (num1.is_negative) {
        return -(-num1 + num2);
    } else if (num1 < num2) {
        return -(num2 - num1);
    }

    BigInt res;
    res.resize(std::max(num1.size, num2.size));

    int carry = 0;
    long long int dif;
    size_t ind;

    for (ind = 0; ind < num1.size && ind < num2.size; ind++) {
        dif = num1.start[ind] - num2.start[ind] - carry;
        carry = dif < 0;
        res.start[ind] = (BigInt::BASE + dif) % BigInt::BASE;
    }
    if (num1.size > num2.size) {
        for (; ind < num1.size; ind++) {
            dif = num1.start[ind] - carry;
            carry = dif < BigInt::BASE;
            res.start[ind] = (BigInt::BASE + dif) % BigInt::BASE;
        }
    }
    if (num1.size < num2.size) {
        for (; ind < num2.size; ind++) {
            dif = num2.start[ind] - carry;
            carry = dif < BigInt::BASE;
            res.start[ind] = (BigInt::BASE + dif) % BigInt::BASE;
        }
    }
    res.size = ind;
    res.is_negative = false;
    res.no_nulls();

    return res;
}
BigInt operator-(const BigInt &num, int n)
{
    BigInt num_new(n);

    return num - num_new;
}
BigInt operator-(int n, const BigInt &num)
{
    BigInt num_new(n);

    return num_new - num;
}
BigInt& BigInt::operator-=(const BigInt &num)
{
    return *this = (*this - num);
}
BigInt& BigInt::operator-=(int n)
{
    BigInt num_new(n);
    return *this = (*this - num_new);
}
BigInt operator*(const BigInt &num1, const BigInt &num2)
{
    BigInt res;
    res.size = num1.size + num2.size;
    res.resize(num1.size + num2.size);
    for (size_t ind = 0; ind < res.size; ind++) {
        res.start[ind] = 0;
    }
    int carry;
    long long cur;
    for (size_t ind1 = 0; ind1 < num1.size; ind1++) {
        carry = 0;
        for (size_t ind2 = 0; ind2 < num2.size || carry > 0; ind2++) {
            cur = res.start[ind1 + ind2] + num1.start[ind1] * 1LL * (ind2 < num2.size ? num2.start[ind2] : 0) + carry;
            res.start[ind1 + ind2] = static_cast<int>(cur % BigInt::BASE);
            carry = static_cast<int>(cur / BigInt::BASE);
        }
    }
    res.is_negative = num1.is_negative != num2.is_negative;
    res.no_nulls();
    return res;
}
BigInt operator*(const BigInt &num, int n)
{
    BigInt num_new(n);

    return num * num_new;
}
BigInt operator*(int n, const BigInt &num)
{
    BigInt num_new(n);

    return num * num_new;
}
BigInt& BigInt::operator*=(const BigInt &num)
{
    return *this = (*this * num);
}
BigInt& BigInt::operator*=(int n)
{
    BigInt num_new(n);

    return *this = (*this * num_new);
}
BigInt BigInt::operator-() const {
    BigInt res(*this);
    res.is_negative = !res.is_negative;
    return res;
}
bool operator==(const BigInt &num1, const BigInt &num2)
{
    if (num1.size != num2.size) {
        return false;
    }
    for (size_t ind = 0; ind < num1.size; ind++) {
        if (num1.start[num1.size - ind - 1] != num2.start[num2.size - ind - 1]) {
            return false;
        }
    }
    return true;
}
bool operator!=(const BigInt &num1, const BigInt &num2)
{
    return !(num1 == num2);
}
bool operator>(const BigInt &num1, const BigInt &num2)
{
    if (num1.size > num2.size) {
        return true;
    }
    if (num1.size < num2.size) {
        return false;
    }
    if (num1.size == num2.size) {
        for (size_t ind = 0; ind < num1.size; ind++) {
            if (num1.start[num1.size - ind - 1] < num2.start[num2.size - ind - 1]) {
                return false;
            }
            if (num1.start[num1.size - ind - 1] > num2.start[num2.size - ind - 1]) {
                return true;
            }
        }
    }
    return false;
}
bool operator>=(const BigInt &num1, const BigInt &num2)
{
    return num1 == num2 || num1 > num2;
}
bool operator<(const BigInt &num1, const BigInt &num2)
{
    return num2 > num1;
}
bool operator<=(const BigInt &num1, const BigInt &num2)
{
    return num2 >= num1;
}
BigInt::~BigInt()
{
    delete [] this->start;
    this->start = nullptr;
}

void std_test() {
    BigInt num;
    BigInt numint(5);
    BigInt emptystr("");
    BigInt numstr("54");
    BigInt longstr("10000000002156514510000000000000451316454");
    BigInt numnum(longstr);
    std::stringstream ss, ss1, ss2, ss3, ss4;
    ss << num;
    ss1 << numint;
    ss2 << emptystr;
    ss3 << numstr;
    ss4 << longstr;
    assert(ss.str() == "0" && ss1.str() == "5" && ss2.str() == "0" && ss3.str() == "54" &&
            ss4.str() == "10000000002156514510000000000000451316454");
}
void plus_test() {
    BigInt num("12345678901234567890");
    std::stringstream ss;
    ss << num + num;
    assert(ss.str() == "24691357802469135780");
};
void sub_test() {
    BigInt num1("100000000000000000000");
    BigInt num2("1");
    std::stringstream ss;
    ss << num1 - num2;
    assert(ss.str() == "99999999999999999999");
};
void mult_test() {
    BigInt num1("123456756432");
    BigInt num2("342134298409");
    std::stringstream ss;
    ss << num1 * num2;
    assert(ss.str() == "42238790745713118116688");
};
