#pragma once
#include <sstream>
#include <iostream>
#include <string>
#include <cassert>

struct Error;


template <typename T>
std::string sput(int num, int ind, T cur);

template <typename T, typename... Args>
std::string sput(int num, int ind, T cur, Args... rest);

template <typename... Args>
std::string format(const char* s, Args... args);

void test_format();
void out_boundary();
void no_number();
void not_number();
