#include "vector.h"
#include <cassert>
#include <algorithm>

void test_init() {

}
void test_brackets() {
    MyVector<int> v (5, 4);
    assert(v[4] == 4);
}
void test_change() {
    MyVector<int> v(1);
    v.push_back(2);
    assert(v[1] == 2);
    v.pop_back();
    v.push_back(3);
    assert(v[1] == 3);
    v.emplace_back(4);
}
void test_size() {
    MyVector<int> v;
    assert(v.size() == 0 && v.empty());
    v = {1, 3, 4};
    assert(v.size() == 3);
    v.clear();
    assert(v.size() == 0);
    v.resize(200, 5);
    assert(v.size() == 200 && v[2] == 5);
}
void test_its() {
    MyVector<int> v({1, 2, 3});
    int sum1 = 0, sum2 = 0;
    std::for_each(v.begin(), v.end(), [&](int &n){ sum1 += n; });
    std::for_each(v.rbegin(), v.rend(), [&](int &n){ sum2 += n; });
    assert(sum1 == 6 && sum2 == 6);
}
void test_cap() {
    MyVector<int> v;
    assert(v.capacity() == 1);
    v.reserve(10);
    assert(v.capacity() == 10);
}
