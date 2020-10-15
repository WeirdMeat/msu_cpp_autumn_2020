#include <cstddef>

class Allocator
{
public:
    char *start;
    char *cur;
    char *end;
    char *offset;
    void makeAllocator(size_t maxSize);
    char* alloc(size_t size);
    void reset();
};

void init_test();
void init_null_test();
void alloc_null_test();
void alloc_test();
void double_alloc_test();
void alloc_over_test();
void alloc_accurate_test();
void alloc_end_test();
void reset_test();
