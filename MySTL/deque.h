#ifndef _DEQUE_H_
#define _DEQUE_H_

#include "allocator.h"
#include "iterator.h"

namespace mySTL {

template <class T, size_t BufSiz>
struct __deque_iterator
    : public iterator<mySTL::random_access_iterator_tag, T> {
    typedef iterator<random_access_iterator_tag, T> iterator_base;
    typedef typename iterator_base::iterator_category iterator_category;
    typedef typename iterator_base::value_type value_type;
    typedef typename iterator_base::pointer pointer;
    typedef typename iterator_base::reference reference;
    typedef typename iterator_base::size_type size_type;
    typedef typename iterator_base::difference_type difference_type;

    typedef pointer* map_pointer;
    typedef __deque_iterator<T, BufSiz> self;
    // 决定缓存区大小，一般就是512字节了，除非这个类型对象size大于512字节，一般也不太可能
    inline static size_type __deque_buf_size(size_type n, size_type sz) {
        return n != 0 ? n : (sz < 512 ? size_type(512 / sz) : size_type(1));
    }
    static size_type buffer_size() {
        return __deque_buf_size(BufSiz, sizeof(value_type));
    }

    // 四个指针
    pointer cur;
    pointer first;
    pointer last;
    map_pointer node;

    void set_node(map_pointer new_node) {
        node = new_node;
        first = *new_node;
        last = first + difference_type(buffer_size());
    }
    reference operator*() { return *cur; }
    pointer operator->() { return &(operator*()); }
    // node间距离+node内距离
    difference_type operator-(const self& x) {
        return difference_type(buffer_size()) * (node - x.node - 1) +
               (cur - first) + (x.last - x.cur);
    }
    self& operator++() {
        ++cur;
        if (cur == last) {
            set_node(node + 1);
            cur = first;
        }
        return *this;
    }
    self& operator--() {
        if (cur == first) {
            set_node(node - 1);
            cur = last;
        }
        --cur;
        return *this;
    }
    self& operator+=(difference_type n) {
        difference_type offset = n + (cur - first);
        if (offset >= 0 && offset < difference_type(buffer_size())) {
            cur += n;
        } else {
            difference_type node_offset =
                offset > 0
                    ? offset / difference_type(buffer_size())
                    : -difference_type((-offset - 1) / buffer_size()) - 1;
            set_node(node + node_offset);
            cur =
                first + (offset - node_offset * difference_type(buffer_size()));
        }
        return *this;
    }
    self operator++(int) {
        self tmp = *this;
        ++*this;
        return tmp;
    }
    self operator--(int) {
        self tmp = *this;
        --*this;
        return tmp;
    }
    self& operator-=(difference_type n) {
        *this += -n;
        return *this;
    }
    self operator+(difference_type n) {
        self tmp = *this;
        return tmp += n;
    }
    self operator-(difference_type n) {
        self tmp = *this;
        return tmp -= n;
    }

    // reference operator[](difference_type n) { return *(*this + n); }
    bool operator==(const self& x) { return cur == x.cur; }
    bool operator!=(const self& x) { return !(*this == x); }
    bool operator<(const self& x) {
        return (node == x.node) ? (cur < x.cur) : (node < x.node);
    }
};

template <class T, class Alloc = mySTL::Allocator<T>, size_t BufSiz = 0>
class deque {
public:
    typedef Alloc data_allocator;
    typedef typename data_allocator::value_type value_type;
    typedef typename data_allocator::pointer pointer;
    typedef typename data_allocator::reference reference;
    typedef typename data_allocator::size_type size_type;
    typedef typename data_allocator::difference_type difference_type;
    typedef typename Alloc::rebind<pointer>::other map_allocator;

    typedef __deque_iterator<T, BufSiz> iterator;

protected:
    typedef pointer* map_pointer;
    // deque四个成员
    iterator start;
    iterator finish;
    map_pointer map;
    size_type map_size;

public:
    // 构造析构函数
    deque() { fill_initialize(0, value_type()); }
    deque(size_type n) { fill_initialize(n, value_type()); }
    deque(size_type n, const value_type& value) { fill_initialize(n, value); }
    ~deque() {
        clear();
        data_allocator::deallocate(*(start.node));
        map_allocator::deallocate(map);
    }

public:
    // 迭代器相关操作
    iterator begin() { return start; }
    iterator end() { return finish; }
    reference operator[](size_type n) { return *(start + difference_type(n)); }
    reference front() { return *start; }
    reference back() { return *(finish - 1); }
    size_type size() { return finish - start; }
    bool empty() { return start == finish; }

public:
    // 插入删除操作
    template <typename... Args>
    void push_back(Args&&... args);
    template <typename... Args>
    void push_front(Args&&... args);
    void pop_front();
    void pop_back();
    // clear
    void clear();
    // erase
    iterator erase(iterator position) { return erase(position, position + 1); }
    iterator erase(iterator first, iterator last);
    // insert
    iterator insert(iterator position, const value_type& value) {
        return insert(position, 1, value);
    }
    iterator insert(iterator position, size_type n, const value_type& value);

protected:
    // 分配空间
    void create_map_and_nodes(size_type n);
    // 初始化
    void fill_initialize(size_type n, const value_type& value);
    // map空间扩容
    void expansion_map(bool add_at_front, size_type n);
    // 节点空间分配
    void expansion_node(bool add_at_front, size_type n);
    void expansion_at_front(size_type n);
    void expansion_at_back(size_type n);
    // 节点空间释放
    void shrinkage_node(bool shrink_at_front);

    void expansion(bool add_at_front, size_type n);

public:
    void printinfo() {
        std::cout << "map首地址       : " << map << std::endl;
        std::cout << "node start地址  : " << start.node << std::endl;
        std::cout << "node finish地址 : " << finish.node << std::endl;
        std::cout << "map尾地址       : " << map + map_size << std::endl;
        std::cout << "map size        : " << map_size << std::endl;
        std::cout << "当前size        : " << size() << std::endl;
    }
};

// 具体实现
// protected
template <class T, class Alloc, size_t BufSiz>
void deque<T, Alloc, BufSiz>::create_map_and_nodes(size_type n) {
    size_type num_nodes = n / iterator::buffer_size() + 1;
    map_size = max(size_type(8), num_nodes + 2);
    map = map_allocator::allocate(map_size);
    map_pointer new_start = map + (map_size - num_nodes) / 2;
    map_pointer new_finish = new_start + num_nodes - 1;
    map_pointer cur;
    try {
        for (cur = new_start; cur <= new_finish; ++cur) {
            *cur = data_allocator::allocate(iterator::buffer_size());
        }
    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
    }
    start.set_node(new_start);
    finish.set_node(new_finish);
    start.cur = start.first;
    finish.cur = finish.first + (n % iterator::buffer_size());
}

template <class T, class Alloc, size_t BufSiz>
void deque<T, Alloc, BufSiz>::fill_initialize(size_type n,
                                              const value_type& value) {
    create_map_and_nodes(n);
    map_pointer cur;
    try {
        for (cur = start.node; cur < finish.node; ++cur) {
            std::uninitialized_fill(*cur, *cur + iterator::buffer_size(),
                                    value);
        }
        std::uninitialized_fill(finish.first, finish.cur, value);
    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
    }
}

template <class T, class Alloc, size_t BufSiz>
void deque<T, Alloc, BufSiz>::expansion_map(bool add_at_front, size_type n) {
    // map扩容为已使用空间的2倍
    size_type old_node_size = finish.node - start.node + 1;
    size_type new_map_size = (old_node_size + n) * 2;
    map_pointer new_map = map_allocator::allocate(new_map_size);
    map_pointer new_map_start =
        (add_at_front ? n : 0) + new_map + (new_map_size - old_node_size) / 2;
    std::copy(start.node, finish.node + 1, new_map_start);
    map_allocator::deallocate(map);
#ifdef MYTESTLOG
    std::cout << "expansion_map: " << map_size << "-> " << new_map_size
              << std::endl;
#endif
    map = new_map;
    map_size = new_map_size;
    start.set_node(new_map_start);
    finish.set_node(new_map_start + old_node_size - 1);
}

template <class T, class Alloc, size_t BufSiz>
void deque<T, Alloc, BufSiz>::expansion_node(bool add_at_front, size_type n) {
    // 先判断要分配的节点位置
    map_pointer need_to_alloc =
        add_at_front ? (start.node - 1) : (finish.node + 1);
    *need_to_alloc = data_allocator::allocate(iterator::buffer_size());
    // 移动指针
    try {
        if (add_at_front) {
            start.set_node(start.node - 1);
            start.cur = start.last - 1;
        } else {
            finish.set_node(finish.node + 1);
            finish.cur = finish.first;
        }
    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
        data_allocator::deallocate(*need_to_alloc);
    }
#ifdef MYTESTLOG
    size_type old_node_size = finish.node - start.node + 1;
    std::string msg = add_at_front ? "前扩容" : "后扩容";
    // std::cout << "扩容位置: " << need_to_alloc << std::endl;
    std::cout << "expansion_node: " << old_node_size << "-> " << msg
              << std::endl;
#endif
}

template <class T, class Alloc, size_t BufSiz>
void deque<T, Alloc, BufSiz>::expansion_at_front(size_type n) {
    expansion(true, n);
}
template <class T, class Alloc, size_t BufSiz>
void deque<T, Alloc, BufSiz>::expansion_at_back(size_type n) {
    expansion(false, n);
}
template <class T, class Alloc, size_t BufSiz>
void deque<T, Alloc, BufSiz>::shrinkage_node(bool shrink_at_front) {
    // 先判断要释放的节点位置
    map_pointer need_to_alloc = shrink_at_front ? (start.node) : (finish.node);
    data_allocator::deallocate(*need_to_alloc);
    // 移动指针
    if (shrink_at_front) {
        start.set_node(start.node + 1);
        start.cur = start.first;
    } else {
        finish.set_node(finish.node - 1);
        finish.cur = finish.last - 1;
    }
}
template <class T, class Alloc, size_t BufSiz>
void deque<T, Alloc, BufSiz>::expansion(bool add_at_front, size_type n) {
    size_type remain_size = add_at_front ? (start.cur - start.first)
                                         : (finish.last - finish.cur - 1);
    size_type need_nodes = (n - remain_size + iterator::buffer_size() - 1) /
                           iterator::buffer_size();
    bool isEnough =
        add_at_front
            ? (start.node - map >= difference_type(need_nodes))
            : (map + map_size - finish.node - 1 >= difference_type(need_nodes));
    if (isEnough) {
        // 节点空间够
        size_type num_nodes = need_nodes;
        map_pointer cur = add_at_front ? (start.node - 1) : (finish.node + 1);
        while (num_nodes--) {
            *cur = data_allocator::allocate(iterator::buffer_size());
            if (add_at_front)
                --cur;
            else
                ++cur;
        }
    } else {
        expansion_map(add_at_front, need_nodes);
        this->expansion(add_at_front, n);
    }
}
// -----------------------------------------------------------------
// public
template <class T, class Alloc, size_t BufSiz>
template <typename... Args>
void deque<T, Alloc, BufSiz>::push_back(Args&&... args) {
    mySTL::construct(finish.cur, mySTL::forward<Args>(args)...);
    // 没空间 扩容
    if (finish.cur == finish.last - 1) expansion_at_back(1);
    ++finish;
}

template <class T, class Alloc, size_t BufSiz>
template <typename... Args>
void deque<T, Alloc, BufSiz>::push_front(Args&&... args) {
    // 这里要注意，start.cur指向第一个位置，finish.cur指向最后位置的后一个位置
    // 所以先移动在构造
    if (start.cur == start.first) expansion_at_front(1);
    --start;
    mySTL::construct(start.cur, mySTL::forward<Args>(args)...);
}

template <class T, class Alloc, size_t BufSiz>
void deque<T, Alloc, BufSiz>::pop_front() {
    mySTL::destroy(start.cur);
    if (start.cur != start.last - 1)
        ++start.cur;
    else
        shrinkage_node(true);
}

template <class T, class Alloc, size_t BufSiz>
void deque<T, Alloc, BufSiz>::pop_back() {
    // stl容器都没有缩小大小的说法，只有扩容
    // 但deque由于迭代器的原因，需要在到达buffer的最后位置时，创建新buffer
    // 那么在删除时,也要在迭代器移到前一个node是,释放该node buffer
    // 否则,在到了最后位置时,会重复分配空间
    if (finish.cur != finish.first)
        --finish.cur;
    else
        shrinkage_node(false);
    mySTL::destroy(finish.cur);
}

template <class T, class Alloc, size_t BufSiz>
void deque<T, Alloc, BufSiz>::clear() {
    // 中间的满buffer
    for (map_pointer node = start.node + 1; node < finish.node; node++) {
        mySTL::destroy(*node, *node + iterator::buffer_size());
        data_allocator::deallocate(*node);
    }
    if (start.node != finish.node) {
        mySTL::destroy(start.cur, start.last);
        mySTL::destroy(finish.first, finish.cur);
        data_allocator::deallocate(*finish.node);
    } else {
        mySTL::destroy(start.cur, finish.cur);
    }
    finish = start;
}

template <class T, class Alloc, size_t BufSiz>
typename deque<T, Alloc, BufSiz>::iterator deque<T, Alloc, BufSiz>::erase(
    iterator first, iterator last) {
    // erase删除后返回下一个位置
    if (first == start && last == finish) {
        clear();
        return finish;
    }
    difference_type n = last - first;
    difference_type elems_before = first - start;
    // 看那边元素少
    if (elems_before < (difference_type(size()) - n) / 2) {
        std::copy_backward(start, first, last);
        iterator new_start = start + n;
        mySTL::destroy(start, new_start);
        for (map_pointer cur = start.node; cur < new_start.node; ++cur)
            data_allocator::deallocate(*cur);
        start = new_start;
    } else {
        std::copy(last, finish, first);
        iterator new_finish = finish - n;
        mySTL::destroy(new_finish, finish);
        for (map_pointer cur = new_finish.node + 1; cur <= finish.node; ++cur)
            data_allocator::deallocate(*cur);
        finish = new_finish;
    }
    return start + elems_before;
}

template <class T, class Alloc, size_t BufSiz>
typename deque<T, Alloc, BufSiz>::iterator deque<T, Alloc, BufSiz>::insert(
    iterator position, size_type n, const value_type& value) {
    difference_type elems_before = position - start;
    // 看那边元素少
    if (elems_before < difference_type((size() - n) / 2)) {
        // 左面前移
        if (start.cur - start.first < difference_type(n)) {
            // 空间不够，扩容
            expansion_at_front(n);
            position = start + elems_before;
        }
        std::copy(start, position, position - n);
        // 构造
        for (iterator cur = position; cur != (position + n); ++cur)
            mySTL::construct(&(*(cur.cur)), value);
        start -= n;
    } else {
        if (finish.last - finish.cur - 1 < difference_type(n)) {
            // 空间不够，扩容
            expansion_at_back(n);
            position = start + elems_before;
        }
        iterator new_start = position + n;
        iterator new_end = new_start + (finish - position);
        std::copy_backward(position, finish, new_end);
        // 构造
        for (iterator cur = position; cur != new_start; ++cur)
            mySTL::construct(&(*(cur.cur)), value);
        finish += n;
    }
    return position + n;
}

}  // namespace mySTL
#endif