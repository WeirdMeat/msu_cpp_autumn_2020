#include "allocator.h"
#include <cstddef>
#include <cassert>
#include <cstdio>

Allocator::Allocator()
{
    this->start = nullptr;
    this->offset = nullptr;
    this->end = nullptr;
    this->cur = nullptr;
}
void Allocator::makeAllocator(size_t maxSize)
{
    if (this->start) {
        delete this->start;
        this->start = nullptr;
    }
    if (maxSize > 0) {
        this->start = new char[maxSize];
        this->end = this->start + maxSize - 1;
        this->offset = this->start;
    }
}
char *Allocator::alloc(size_t size)
{
    if (this->start == nullptr || this->offset == nullptr || this->end - this->offset + 1 < size) {
        return nullptr;
    }
    this->cur = this->offset;
    if (this->end - this->offset >= size) {
        this->offset += size;
    } else {
        this->offset = nullptr;
    }
    return this->cur;
}
void Allocator::reset()
{
    this->offset = this->start;
}
Allocator::~Allocator()
{
    delete this->start;
    this->start = nullptr;
}

void noinit_test() {
    Allocator tested;
    assert(tested.start == nullptr);
}

void init_test()
{
    size_t size = 100;
    Allocator tested;
    tested.makeAllocator(size);
    assert(tested.end - tested.start + 1 == size);
}

void init_null_test() {
    size_t size = 0;
    Allocator tested;
    tested.makeAllocator(size);
    assert(tested.start == nullptr);
}

void double_init_test() {
    size_t maxsize1 = 10, maxsize2 = 20;
    Allocator tested;
    tested.makeAllocator(maxsize1);
    tested.makeAllocator(maxsize2);
    assert(tested.end - tested.start + 1 == maxsize2);
}

void alloc_noinit_test() {
    size_t size = 100;
    char *ptr;
    Allocator tested;
    ptr = tested.alloc(size);
    assert(tested.start == nullptr && ptr == nullptr);
}

void alloc_null_test() {
    size_t maxsize = 0, size = 1;
    char *ptr;
    Allocator tested;
    tested.makeAllocator(maxsize);
    ptr = tested.alloc(size);
    assert(tested.start == nullptr && ptr == nullptr);
}

void alloc_test()
{
    size_t maxsize = 100, size = 10;
    Allocator tested;
    char *ptr;
    tested.makeAllocator(maxsize);
    ptr = tested.alloc(size);
    assert(ptr && tested.offset - ptr == size && tested.start == ptr);
}

void double_alloc_test()
{
    size_t maxsize = 100, size1 = 10, size2 = 20;
    Allocator tested;
    char *ptr1, *ptr2;
    tested.makeAllocator(maxsize);
    ptr1 = tested.alloc(size1);
    ptr2 = tested.alloc(size2);
    assert(ptr1 && tested.offset - ptr2 == size2 && tested.start == ptr1 && ptr2 && ptr2 - ptr1 == size1);
}

void alloc_over_test()
{
    size_t maxsize = 10, size = 11;
    Allocator tested;
    char *ptr;
    tested.makeAllocator(maxsize);
    ptr = tested.alloc(size);
    assert(ptr == nullptr);
}

void alloc_accurate_test()
{
    size_t maxsize = 10, size = 10;
    Allocator tested;
    char *ptr;
    tested.makeAllocator(maxsize);
    ptr = tested.alloc(size);
    assert(ptr && ptr == tested.start);
}
void alloc_end_test() {
    size_t maxsize = 10, size1 = 10, size2 = 1;
    Allocator tested;
    char *ptr1, *ptr2;
    tested.makeAllocator(maxsize);
    ptr1 = tested.alloc(size1);
    ptr2 = tested.alloc(size2);
    assert(ptr1 && ptr1 == tested.start && ptr2 == nullptr);
}
void reset_test()
{
    size_t maxsize = 100, size1 = 10, size2 = 20;
    Allocator tested;
    char *ptr1, *ptr2;
    tested.makeAllocator(maxsize);
    ptr1 = tested.alloc(size1);
    tested.reset();
    ptr2 = tested.alloc(size2);
    assert(ptr1 && ptr2 && tested.offset - ptr2 == size2 && ptr2 == tested.start);
}
