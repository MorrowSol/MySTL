#ifndef _LIST_H_
#define _LIST_H_
#include "allocator.h"
#include "iterator.h"

namespace mySTL {

template <class T>
struct __list_node {
    typedef __list_node<T>* link_type;
    T data;
    link_type prev;
    link_type next;

    __list_node() {}
    __list_node(const T& x) : data(x) {}
    __list_node(T&& x) : data(mySTL::move(x)) {}
};

template <class T>
struct __list_iterator : public iterator<bidirection_iterator_tag, T> {
    typedef iterator<bidirection_iterator_tag, T> iterator_base;
    typedef typename iterator_base::iterator_category iterator_category;
    typedef typename iterator_base::value_type value_type;
    typedef typename iterator_base::difference_type difference_type;
    typedef typename iterator_base::pointer pointer;
    typedef typename iterator_base::reference reference;

    typedef __list_node<T>* link_type;
    typedef __list_iterator<T> self;

    link_type node;
    // constructor
    __list_iterator() {}
    __list_iterator(link_type x) : node(x) {}
    __list_iterator(const self& x) : node(x.node) {}

    // operator重载
    bool operator==(const self& x) { return node == x.node; }
    bool operator!=(const self& x) { return node != x.node; }
    reference operator*() { return node->data; }
    pointer operator->() { return &(operator*()); }
    self& operator++() {
        node = node->next;
        return *this;
    }
    self operator++(int) {
        self tmp = *this;
        ++*this;
        return tmp;
    }
    self& operator--() {
        node = node->prev;
        return *this;
    }
    self operator--(int) {
        self tmp = *this;
        --*this;
        return tmp;
    }
    self operator+(int n) {
        self tmp = *this;
        mySTL::advance<self>(tmp, n, iterator_category{});
        return tmp;
    }
    self operator-(int n) {
        self tmp = *this;
        mySTL::advance<self>(tmp, -n, iterator_category{});
        return tmp;
    }
    // 迭代器 因为只有一个变量，用默认的就行
};

template <class T, class Alloc = mySTL::Allocator<__list_node<T>>>
class list {
public:
    typedef Alloc node_allocator;
    typedef typename Alloc::rebind<T>::other data_allocator;

    typedef typename data_allocator::value_type value_type;
    typedef typename data_allocator::pointer pointer;
    typedef typename data_allocator::reference reference;
    typedef typename data_allocator::size_type size_type;
    typedef typename data_allocator::difference_type difference_type;

    typedef __list_node<T> list_node;
    typedef __list_node<T>* link_type;
    typedef __list_iterator<T> iterator;

protected:
    link_type node;
    link_type create_node(const T& x) {
        link_type p = node_allocator::allocate();
        mySTL::construct(&p->data, x);
        return p;
    }
    void destory_node(link_type p) {
        mySTL::destroy(&p->data);
        node_allocator::deallocate(p);
    }

public:
    list() {
        // 初始化创建空节点
        node = node_allocator::allocate();
        node->next = node;
        node->prev = node;
    };
    ~list() {
        clear();
        node_allocator::deallocate(node);
    }

    iterator begin() { return static_cast<iterator>(node->next); }
    iterator end() { return static_cast<iterator>(node); }
    reference front() { return static_cast<iterator>(node->next).operator*(); }
    reference back() { return *(--end()); }
    bool empty() { return node->next == node; }
    size_type size() {
        return static_cast<size_type>(mySTL::distance(begin(), end()));
    }
    void clear() {
        link_type cur = node->next;
        while (cur != node) {
            link_type tmp = cur;
            cur = cur->next;
            destory_node(tmp);
        }
        node->next = node;
        node->prev = node;
    }
    iterator insert(iterator position, const T& x) {
        link_type tmp = create_node(x);
        tmp->next = position.node;
        tmp->prev = position.node->prev;
        tmp->prev->next = tmp;
        position.node->prev = tmp;
        return static_cast<iterator>(tmp);
    }
    iterator erase(iterator position) {
        link_type next_node = position.node->next;
        link_type prev_node = position.node->prev;
        prev_node->next = next_node;
        next_node->prev = prev_node;
        destory_node(position.node);
        return static_cast<iterator>(next_node);
    }
    void push_front(const T& x) { insert(begin(), x); }
    void push_back(const T& x) { insert(end(), x); }
    void pop_front() { erase(begin()); }
    void pop_back() { erase(--end()); }

    void remove(const T& value) {
        iterator first = begin();
        iterator last = end();
        while (first != last) {
            iterator tmp = first;
            ++first;
            if (*tmp == value) erase(tmp);
        }
    }

protected:
    void transfer(iterator position, iterator first, iterator last) {
        if (position == last) return;
        link_type prev_node = position.node->prev;
        link_type new_last = last.node->prev;
        // 接上原来的
        first.node->prev->next = last.node;
        last.node->prev = first.node->prev;

        new_last->next = position.node;
        position.node->prev = new_last;
        prev_node->next = first.node;
        first.node->prev = prev_node;
    }

public:
    void splice(iterator position, list& x) {
        if (!x.empty())
            transfer(position, x.begin(), x.end());
    }
    void splice(iterator position, iterator i) {
        if (position == i || position == i + 1) return;
        transfer(position, i, i + 1);
    }
    void splice(iterator position, iterator first, iterator last) {
        if (first != last) transfer(position, first, last);
    }
    void swap(list & other){
        link_type tmp = other.node;
        other.node = node;
        node = tmp;
    }
    void reverse(){}
    // 将两个递增序链表合并，按递增序排列
    void merge(list &x){
        iterator first1 = begin();
        iterator last1 = end();
        iterator first2 = x.begin();
        iterator last2 = x.end();

        while(first1!= last1 && first2 !=last2){
            if(*first2< *first1){
                iterator next = first2;
                transfer(first1,first2,++next);
                first2 = next;
            }
            else
                ++first1;
        }
        if(first2!=last2)transfer(last1,first2,last2);
    }
    // 归并排序
    void sort(){
        if(node->next==node || node->next->next == node)
            return;
        list carry;
        list counter[64];
        int fill=0;
        while(!empty()){
            carry.splice(carry.begin(),begin());
            int i=0;
            while (i<fill&& !counter[i].empty())
            {
                counter[i].merge(carry);
                carry.swap(counter[i++]);
            }
            carry.swap(counter[i]);
            if(i==fill)++fill;
            
        }
        for(int i=1;i<fill;i++)
            counter[i].merge(counter[i-1]);
        swap(counter[fill-1]);

    }
    // 将递增序链表去重
    void unique(){
        iterator first =begin();
        iterator last =end();
        if(first==last)return;
        while(first!=last){
            iterator next = first.node->next;
            if(*first==*next)
                erase(next);
            else
                ++first;
        }
    }
    
};

}  // namespace mySTL
#endif