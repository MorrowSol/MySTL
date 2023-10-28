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
    void deallocate() {
        if (start) {
            data_allocator::deallocate(start);
        }
    }
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

public:
    vector():start(0),finish(0),end_of_storage(0) {}
    // vector() {
    //     start = data_allocator::allocate(16);
    //     finish=start;
    //     end_of_storage=start+16;
    // }
    vector(size_type n) { fill_initialize(n, T()); }
    vector(size_type n, const T& value) { fill_initialize(n, value); }
    ~vector() {
        mySTL::destroy(start, finish);
        deallocate();
    }

    reference operator[](size_type n) { return *(start + n); }
    iterator begin() { return start; }
    iterator end() { return finish; }
    size_type size() { return size_type(finish - start); }
    size_type capacity() { return size_type(end_of_storage - start); }
    bool empty() { return finish == start; }
    reference front(){return *start;}
    reference back(){return *(finish-1);}
    reference push_back(const T& x){\
        // 有空间 
        if(finish!=end_of_storage){
            mySTL::construct(finish,x);
            ++finish;
        }
        // 没空间
        else{
            insert_aux(finish,x);    
        }
    }
    void pop_back(){
        // 因为指的是后一个，所以先--在析构
        --finish;
        mySTL::destroy(finish);
    }
    iterator erase(iterator position){
        // 后面的迁移，这里应该是直接拷贝
        // 这里先调stl的
        if(position+1!=end())
            std::copy(position+1,finish,position);
        --finish;
        mySTL::destroy(finish);
        return position;
    }
    void resize(size_type new_size){
        if(new_size<size())
            erase();
    }
};

}  // namespace mySTL
#endif