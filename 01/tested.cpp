#include "allocator.h"

int main(int argc, char const *argv[]) {
    noinit_test();
    init_test();
    double_init_test();
    alloc_noinit_test();
    init_null_test();
    alloc_null_test();
    alloc_test();
    double_alloc_test();
    alloc_over_test();
    alloc_accurate_test();
    alloc_end_test();
    reset_test();
    return 0;
}
