#ifndef _LIST_H_
#define _LIST_H_
#include "iterator.h"
#include "allocator.h"

namespace mySTL{

template <class T>
struct __list_node
{
    typedef __list_node<T>* link_type;
    T data;
    link_type prev;
    link_type next;
};

template <class T>
struct __list_iterator:public mySTL::iterator<bidirection_iterator_tag,T>
{
    typedef __list_node<T>* link_type;
    link_type node;
    // constructor
    __list_iterator(){}
    __list_iterator(link_type x):node(x){}
    __list_iterator(cosnt ){}

};

template <class T,class Alloc = mySTL::Allocator<T>>
class list
{
private:
    /* data */
public:
    list(/* args */);
    ~list();



    
};
}
#endif