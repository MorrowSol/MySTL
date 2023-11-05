#ifndef _VECTOR_H_
#define _VECTOR_H_

#include "allocator.h"
namespace mySTL {

template <class T, class Alloc = mySTL::Allocator<T> >
class vector {
public:
    typedef Alloc allocator;
    typedef typename allocator::value_type value_type;
    typedef typename allocator::pointer pointer;
    typedef typename allocator::reference reference;
    typedef typename allocator::size_type size_type;
    typedef typename allocator::difference_type difference_type;
    typedef T* iterator;

protected:
    
    iterator start;
    iterator finish;
    iterator end_of_storage;

    void fill_initialize(size_type n, const T& value) {
        start = allocator::allocate(n);
        // 这里先调stl的
        std::uninitialized_fill_n(start, n, value);
        finish = start + n;
        end_of_storage = finish;
    }
    void __destroy_and_update(iterator new_start, iterator new_finish,
                              iterator new_end_of_storage) {
        mySTL::destroy(start, finish);
        allocator::deallocate(start);
        start = new_start;
        finish = new_finish;
        end_of_storage = new_end_of_storage;
    }
    void expansion() { expansion(finish, finish, 0); }
    void expansion(iterator first, iterator last, size_type n) {
        // 分配新空间
        size_type old_size = size();
        // 新空间容量
        size_type new_size = old_size != 0 ? 2 * old_size : 1;
        new_size = std::max(new_size, old_size + n);
        // 分配新空间
        iterator new_start = allocator::allocate(new_size);
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
            allocator::deallocate(start);
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
    iterator erase(iterator position) { return erase(position, position + 1); }
    iterator erase(iterator first, iterator last) {
        iterator newfinish = std::copy(last, finish, first);
        destroy(newfinish, finish);
        finish = newfinish;
        return first;
    }
    void clear() { erase(begin(), end()); }
    // push_back
    template <typename... Args>
    void push_back(Args&&... args) {
        // 有空间
        if (finish != end_of_storage) {
            mySTL::construct(finish, mySTL::forward<Args>(args)...);
            ++finish;
        }
        // 没空间 // 扩容 // 再次push
        else {
            expansion();
            this->push_back(mySTL::forward<Args>(args)...);
        }
    }

    void pop_back() {
        // 因为指的是后一个，所以先--在析构
        --finish;
        mySTL::destroy(finish);
    }
    void insert(iterator positon, const T& x) { insert(positon, 1, x); }
    void insert(iterator positon, size_type n, const T& x) {
        difference_type cur_diff = positon - start;
        if (finish + n <= end_of_storage) {
            // 有空间 拷贝后
            std::copy(positon, finish, positon + n);
            // 构造
            std::uninitialized_fill_n(positon, n, x);
            finish += n;
        } else {
            expansion(end(), end(), n);
            this->insert(start + cur_diff, n, x);
        }
    }
    void resize(size_type new_size) { resize(new_size, T()); }
    void resize(size_type new_size, const T& x) {
        // 分为比size小  比size大但比capacity小   比capacity大
        if (new_size < size()) {
            erase(start + new_size, finish);
        } else {
            insert(finish, new_size - size(), x);
        }
    }

};

}  // namespace mySTL
#endif