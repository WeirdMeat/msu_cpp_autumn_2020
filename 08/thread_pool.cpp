#include "thread_pool.h"
#include <chrono>
#include <cassert>

void test ()
{
    ThreadPool pool(8);
    struct A {

        int num1 = 5;
        int num2 = 6;
    };
    auto foo = [](const A& sum) { return sum.num1 + sum.num2;};

    auto task1 = pool.exec(foo, A());
    assert(task1.get() == 11);

    auto task2 = pool.exec([]() { return 1; });
    assert(task2.get() == 1);

}
