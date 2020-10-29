#include <cstddef>
#pragma once

class Allocator
{
public:
    char *start;
    char *cur;
    char *end;
    char *offset;
    Allocator();
    void makeAllocator(size_t maxSize);
    char* alloc(size_t size);
    void reset();
    ~Allocator();
};

void noinit_test();
void init_test();
void init_null_test();
void double_init_test();
void alloc_noinit_test();
void alloc_null_test();
void alloc_test();
void double_alloc_test();
void alloc_over_test();
void alloc_accurate_test();
void alloc_end_test();
void reset_test();
