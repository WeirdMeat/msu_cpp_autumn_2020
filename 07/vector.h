#include <iostream>
#include <set>
#include <vector>
#include <limits>

template<class T>
class MyAllocator
{
public:
    using value_type = T;
    using pointer = T*;
    using size_type = size_t;

    pointer allocate(size_type count) {
        pointer new_ptr = nullptr;
        try {
            new_ptr = new value_type[count * sizeof(value_type)];
        }
        catch (...) {
            throw std::bad_alloc();
        }
        return new_ptr;
    }
    void deallocate(pointer ptr, size_type count) {
        delete [] ptr;
        ptr = nullptr;
    }
    template <class U, class... Args>
	void construct(U *ptr, Args&&... args) {
		::new ((void*)ptr) U(std::forward<Args>(args)...);
	}
    template <class U>
    void destroy( U* p ) {
        p->~U();
    }
    size_type max_size() const noexcept {
        return std::numeric_limits<size_type>::max() / sizeof(value_type);
    }
};

template<typename Category_, typename T,
        typename _Distance = ptrdiff_t,
        typename _Pointer = T*,
        typename _Reference = T&>
class MyIterator
{
public:
    using value_type = T;
    using difference_type = _Distance;
    using pointer = _Pointer;
    using reference = _Reference;
    using iterator_category = Category_;

    MyIterator(): ptr(nullptr) {}
    MyIterator(pointer ptr): ptr(ptr) {}
    MyIterator& operator=(const MyIterator& iter) {
        if (this == &iter) {
            return *this;
        }

        delete [] this->ptr;

        this->ptr = iter.ptr;
        return *this;
    }
    MyIterator(const MyIterator& iter): ptr(iter.ptr) {}
    MyIterator& operator=(MyIterator&& iter) {
        this->ptr = std::move(iter.ptr);
        return *this;
    }
    MyIterator(MyIterator&& iter): ptr(std::move(iter.ptr)) {}
    friend bool operator==(const MyIterator& iter1, const MyIterator& iter2) {
		return iter1.ptr == iter2.ptr;
	}
    MyIterator& operator++() {
        this->ptr++;
        return *this;
    }
    friend bool operator!=(const MyIterator& iter1, const MyIterator& iter2) {
		return !(iter1 == iter2);
	}
    MyIterator& operator--() {
        this->ptr--;
        return *this;
    }
    reference operator*() {
        return *(this->ptr);
    }
    reference operator->() {
        return this->ptr;
    }
    MyIterator operator++(int)
    {
        MyIterator temp(*this);
        this->ptr++;
        return temp;
    }
    MyIterator operator--(int)
    {
        MyIterator temp(*this);
        this->ptr--;
        return temp;
    }
    MyIterator& operator+=(difference_type dif) {
        if (dif >= 0) while (dif--) ++(*this);
        else while (dif++) --(*this);
        return *this;
    }
    friend MyIterator operator+(const MyIterator iter, difference_type dif) {
        MyIterator temp = iter;
        return temp += dif;
    }
    friend MyIterator operator+(difference_type dif, const MyIterator iter) {
        MyIterator temp = iter;
        return temp += dif;
    }
    MyIterator& operator-=(difference_type dif) {
        return *this = (*this += -dif);
    }
    friend MyIterator operator-(const MyIterator iter, difference_type dif) {
        MyIterator temp = iter;
        return temp -= dif;
    }
    friend difference_type operator-(const MyIterator iter1,
            const MyIterator iter2) {
        return iter1.ptr - iter2.ptr;
    }
    friend bool operator<(const MyIterator& iter1, const MyIterator& iter2) {
		return iter2 - iter1 > 0;
	}
    friend bool operator>(const MyIterator& iter1, const MyIterator& iter2) {
		return iter2 < iter1;
	}
    friend bool operator>=(const MyIterator& iter1, const MyIterator& iter2) {
		return !(iter1 < iter2);
	}
    friend bool operator<=(const MyIterator& iter1, const MyIterator& iter2) {
		return !(iter1 < iter2);
	}
    ~MyIterator() {
        ptr = nullptr;
    }
private:
    pointer ptr;
};

template<class T, class Alloc = MyAllocator<T>>
class MyVector
{
public:
    using size_type = size_t;
    using iterator = MyIterator<std::random_access_iterator_tag, T>;
    using reverse_iterator =
        std::reverse_iterator<MyIterator<std::random_access_iterator_tag, T>>;
    using value_type = T;
    using reference = T&;
    using const_reference = const T&;
    using allocator_type = Alloc;

    MyVector(): capacity_(1) {
        this->begin_ = this->alloc.allocate(1);
        this->end_ = this->begin_;
    }
    MyVector(size_type count): capacity_(count) {
        this->begin_ = this->alloc.allocate(count);
        this->end_ = this->begin_ + count;
        for (iterator it = this->begin_; it < this->end_; ++it) {
            *it = 0;
        }
    }
    MyVector(size_type count, const value_type& defaultValue): capacity_(count) {
        this->begin_ = this->alloc.allocate(count);
        this->end_ = this->begin_ + count;
        for (iterator it = this->begin_; it < this->end_; ++it) {
            *it = defaultValue;
        }
    }
    MyVector(const MyVector& vec): capacity_(vec.capacity_) {
        this->begin_ = this->alloc.allocate(vec.size());
        this->end_ = this->begin_ + vec.size();
        iterator old_it = vec.begin_;
        for (iterator it = this->begin_; it < this->end_; ++old_it, ++it) {
            *it = *old_it;
        }
    }
    MyVector& operator=(const MyVector& vec) {
        this->capacity_ = vec.capacity_;
        this->begin_ = this->alloc.allocate(vec.size());
        this->end_ = this->begin_ + vec.size();
        iterator old_it = vec.begin_;
        for (iterator it = this->begin_; it < this->end_; ++old_it, ++it) {
            *it = *old_it;
        }
        return *this;
    }
    MyVector(MyVector&& vec): capacity_(std::move(vec.capacity_)),
            begin_(std::move(vec.begin_)),
            end_(std::move(vec.end_)) {}
    MyVector& operator=(MyVector&& vec) {
        this->capacity_ = std::move(vec.capacity_);
        this->begin_ = std::move(vec.begin_);
        this->end_ = std::move(vec.end_);
        return *this;
    }

    MyVector(std::initializer_list<value_type> init) {
        this->capacity_ = 1;
        this->begin_ = this->alloc.allocate(1);
        this->end_ = this->begin_;
        for (value_type x : init) this->push_back(x);
    }
    MyVector& operator=(std::initializer_list<value_type> init) {
        for (value_type x : init) this->push_back(x);
        return *this;
    }
    reference operator[](size_type pos) {
        iterator it = this->begin_ + pos;
        if (it >= this->end_ || it < this->begin_) {
            throw std::out_of_range("");
        }
        return *it;
    }
    const_reference operator[](size_type pos) const {
        iterator it = this->begin_ + pos;
        if (it >= this->end_ || it < this->begin_) {
            throw std::out_of_range("");
        }
        return *it;
    }
    void push_back(const value_type& value) {
        this->resize(this->size() + 1, value);
    }
    void push_back(value_type&& value)
    {
        this->resize(this->size() + 1, value);
    }
    void pop_back() {
        if (!this->empty()) {
            --this->end_;
        }
    }
    template< class... Args >
    void emplace_back(Args&&... args) {
        if (this->size() == this->capacity()) {
            this->reserve(2 * this->capacity());
        }
        this->alloc.construct(&(*this->end_), std::forward<Args>(args)...);
        this->end_++;
    }
    bool empty() const noexcept {
        return this->size() == 0;
    }
    size_type size() const noexcept {
        return this->end_ - this->begin_;
    }
    void clear() noexcept {
        this->end_ = this->begin_;
    }
    iterator begin() noexcept {
        return this->begin_;
    }
    reverse_iterator rbegin() noexcept {
        return reverse_iterator(this->end());
    }
    iterator end() noexcept {
        return this->end_;
    }
    reverse_iterator rend() noexcept {
        return reverse_iterator(this->begin());
    }
    void resize(size_type count) {
        if (count > this->capacity_) {
            size_type new_cap = 1;
            while (new_cap < count) {
                new_cap *= 2;
            }
            this->reserve(new_cap);
        }
        iterator it = this->end_;
        this->end_ = this->begin_ + count;
        while (it < this->end_) {
            *it = 0;
            ++it;
        }
        return;
    }
    void resize(size_type count, const value_type& value) {
        if (count <= this->capacity_) {
            iterator it = this->end_;
            this->end_ = this->begin_ + count;
            while (it < this->end_) {
                *it = value;
                ++it;
            }
            return;
        }
        MyVector temp(count);
        iterator old_it = this->begin_;
        for (iterator it = temp.begin_; it < temp.end_; ++it) {
            if (old_it < this->end_ > 0) {
                *it = *old_it;
                ++old_it;
            } else {
                *it = value;
            }
        }
        *this = temp;
    }
    void reserve(size_type count) {
        if (count > this->capacity_) {
            MyVector temp(count);
            temp.end_ = temp.begin_ + this->size();
            iterator old_it = this->begin_;
            for (iterator it = temp.begin_; it < temp.end_; ++it) {
                *it = *old_it;
                ++old_it;
            }
            *this = temp;
        }
    }
    size_type capacity() const noexcept {
        return this->capacity_;
    }
private:
    iterator begin_;
    iterator end_;
    size_type capacity_;
    allocator_type alloc;
};

void test_init();
void test_brackets();
void test_change();
void test_size();
void test_its();
void test_cap();
