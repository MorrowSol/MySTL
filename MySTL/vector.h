#ifndef _VECTOR_H_
#define _VECTOR_H_

#include "allocator.h"
namespace mySTL {

template <class T, class Alloc = mySTL::Allocator<T> >
class vector {
public:
    typedef T value_type;
    typedef T* pointer;
    typedef T* iterator;
    typedef T& reference;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;

protected:
    typedef Alloc data_allocator;
    iterator start;
    iterator finish;
    iterator end_of_storage;

    void insert_aux(iterator position, const T& x);

    iterator allocate_and_fill(size_type n, const T& x) {
        iterator result = data_allocator::allocate(n);
        // 这里先调stl的
        std::uninitialized_fill_n(result, n, x);
        return result;
    }
    void fill_initialize(size_type n, const T& value) {
        start = allocate_and_fill(n, value);
        finish = start + n;
        end_of_storage = finish;
    }
    void __destroy_and_update(iterator new_start, iterator new_finish,
                              iterator new_end_of_storage) {
        mySTL::destroy(start, finish);
        start = new_start;
        finish = new_finish;
        end_of_storage = new_end_of_storage;
    }
    void expansion(iterator first, iterator last) {
        // 分配新空间
        size_type old_size = size();
        // 新空间容量
        size_type new_size = old_size != 0 ? 2 * old_size : 1;
        // 分配新空间
        iterator new_start = data_allocator::allocate(new_size);
        iterator new_finish = nullptr;
        iterator next_construct;
        // 转移数据,这里要满足，要么全成功，要么全失败的要求
        try {
            // 插入方式增长,分两段拷贝
            next_construct = std::uninitialized_copy(start, first, new_start);
            new_finish = std::uninitialized_copy(
                last, finish, next_construct + (last - first));
        } catch (const std::exception& e) {
            // 失败
            std::cout << "expansion fail" << std::endl;
        }
        // 释放原空间
        __destroy_and_update(new_start, new_finish, new_start + new_size);
#ifdef MYTESTLOG
        std::cout << "expansion success: " << old_size << "->" << new_size
                  << std::endl;
#endif
    }
    void expansion() { expansion(finish, finish); }

public:
    vector() : start(nullptr), finish(nullptr), end_of_storage(nullptr) {}
    // vector() {
    //     start = data_allocator::allocate(16);
    //     finish=start;
    //     end_of_storage=start+16;
    // }
    vector(size_type n) { fill_initialize(n, T()); }
    vector(size_type n, const T& value) { fill_initialize(n, value); }
    ~vector() {
        mySTL::destroy(start, finish);
        if (start != nullptr) {
            data_allocator::deallocate(start);
        }
    }

    iterator begin() { return start; }
    iterator end() { return finish; }
    size_type size() { return size_type(finish - start); }
    size_type capacity() { return size_type(end_of_storage - start); }
    bool empty() { return finish == start; }
    reference operator[](size_type n) { return *(start + n); }
    reference front() { return *start; }
    reference back() { return *(finish - 1); }

public:
    // 删除区间内的对象，后面的前移
    iterator erase(iterator first, iterator last) {
        iterator newfinish = std::copy(last, finish, first);
        destroy(newfinish, finish);
        finish = newfinish;
        return first;
    }
    iterator erase(iterator position) { return erase(position, position + 1); }
    void clear() { erase(begin(), end()); }
    void push_back(const T& x) {
        // 有空间
        if (finish != end_of_storage) {
            mySTL::construct(finish, x);
            ++finish;
        }
        // 没空间
        else {
            // 扩容
            expansion();
            // 再次push
            this->push_back(x);
        }
    }
    void push_back(T&& x) {
        // 有空间
        if (finish != end_of_storage) {
            mySTL::construct(finish, mySTL::move(x));
            ++finish;
        }
        // 没空间
        else {
            // 扩容
            expansion();
            // 再次push
            this->push_back(mySTL::move(x));
        }
    }

    void pop_back() {
        // 因为指的是后一个，所以先--在析构
        --finish;
        mySTL::destroy(finish);
    }

    iterator insert(iterator positon, const T& x) {
        if (finish != end_of_storage) {
        }
    }
    void resize(size_type new_size) {
        // 分为比size小  比size大但比capacity小   比capacity大
        if (new_size < size()) {
            erase(start + new_size, finish);
        } else if (new_size < capacity()) {
        }
    }
};

}  // namespace mySTL
#endif