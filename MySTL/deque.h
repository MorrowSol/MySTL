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
        return __deque_buf_size(BufSiz, sizeof(T));
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
    iterator start;
    iterator finish;

    map_pointer map;
    size_type map_size;

    void create_map_and_nodes(size_type n) {
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
    void fill_initialize(size_type n, const T& value) {
        create_map_and_nodes(n);
        map_pointer cur;
        try {
            for (cur = start.node; cur < finish.node; ++cur) {
                std::uninitialized_fill(*cur, *cur + iterator::buffer_size(),value);
            }
            std::uninitialized_fill(finish.first, finish.cur, value);
        } catch (const std::exception& e) {
            std::cerr << e.what() << '\n';
        }
    }

public:
    deque() { fill_initialize(0, T()); }
    deque(size_type n) { fill_initialize(n, T()); }
    deque(size_type n, const T& value) { fill_initialize(n, value); }
    ~deque() {
        clear();
        data_allocator::deallocate(*(start.node));
        map_allocator::deallocate(map);
    }
    iterator begin() { return start; }
    iterator end() { return finish; }
    reference operator[](size_type n) { return *(start + difference_type(n)); }
    reference front() { return *start; }
    reference back() { return *(finish - 1); }
    size_type size() { return finish - start; }
    bool empty() { return start == finish; }
    void printinfo() {
        std::cout << "map首地址       : " << map << std::endl;
        std::cout << "node start地址  : " << start.node << std::endl;
        std::cout << "node finish地址 : " << finish.node << std::endl;
        std::cout << "map尾地址       : " << map + map_size << std::endl;
        std::cout << "map size        : " << map_size << std::endl;
        std::cout << "当前size        : " << size() << std::endl;
    }

protected:
    void expansion_map() {
        // map扩容为已使用空间的2倍
        size_type old_node_size = finish.node - start.node + 1;
        size_type new_map_size = old_node_size * 2;
        map_pointer new_map = map_allocator::allocate(new_map_size);
        map_pointer new_map_start =
            new_map + (new_map_size - old_node_size) / 2;
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
    void expansion_node(bool add_at_front) {
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
    void expansion_at_front() {
        // 有空间构造，没空间扩容
        if ((start.node - map) > 0) {
            expansion_node(true);
        } else {
            expansion_map();
            this->expansion_at_front();
        }
    }
    void expansion_at_back() {
        // 有空间构造，没空间扩容
        if (map_size - (finish.node - map + 1) > 0) {
            expansion_node(false);
        } else {
            expansion_map();
            this->expansion_at_back();
        }
    }
    void shrinkage_node(bool shrink_at_front) {
        // 先判断要释放的节点位置
        map_pointer need_to_alloc =
            shrink_at_front ? (start.node) : (finish.node);
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

public:
    template <typename... Args>
    void push_back(Args&&... args) {
        mySTL::construct(finish.cur, mySTL::forward<Args>(args)...);
        // 没空间 扩容
        if (finish.cur != finish.last - 1)
            ++finish.cur;
        else
            expansion_at_back();
    }
    template <typename... Args>
    void push_front(Args&&... args) {
        // 这里要注意，start.cur指向第一个位置，finish.cur指向最后位置的后一个位置
        // 所以先移动在构造
        if (start.cur != start.first)
            --start.cur;
        else
            expansion_at_front();
        mySTL::construct(start.cur, mySTL::forward<Args>(args)...);
    }
    void pop_front() {
        mySTL::destroy(start.cur);
        if (start.cur != start.last - 1)
            ++start.cur;
        else
            shrinkage_node(true);
    }
    void pop_back() {
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
    void clear() {
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
    iterator erase(iterator position) { return erase(position, position + 1); }
    iterator erase(iterator first, iterator last) {
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
            for (map_pointer cur = new_finish.node + 1; cur <= finish.node;
                 ++cur)
                data_allocator::deallocate(*cur);
            finish = new_finish;
        }
        return start + elems_before;
    }

};

}  // namespace mySTL
#endif