#ifndef _RB_TREE_H_
#define _RB_TREE_H_

#include "allocator.h"
#include "iterator.h"

namespace mySTL {

typedef bool __rb_tree_color_type;
const __rb_tree_color_type __rb_tree_red = false;
const __rb_tree_color_type __rb_tree_black = true;

template <class T>
struct __rb_tree_node {
    typedef __rb_tree_color_type color_type;
    typedef __rb_tree_node<T>* link_type;

    // 每个节点的数据
    color_type color;
    link_type parent;
    link_type left;
    link_type right;
    T value;

    static link_type minimum(link_type x) {
        while (x->left != nullptr) x = x->left;
        return x;
    }
    static link_type maximum(link_type x) {
        while (x->right != nullptr) x = x->right;
        return x;
    }
};

template <class T>
struct __rb_tree_iterator : public iterator<bidirection_iterator_tag, T> {
    typedef iterator<bidirection_iterator_tag, T> iterator_base;

    typedef typename iterator_base::iterator_category iterator_category;
    typedef typename iterator_base::value_type value_type;
    typedef typename iterator_base::pointer pointer;
    typedef typename iterator_base::reference reference;
    typedef typename iterator_base::size_type size_type;
    typedef typename iterator_base::difference_type difference_type;

    typedef __rb_tree_iterator<T> self;
    typedef typename __rb_tree_node<T>::link_type link_type;
    link_type node;

    // 构造函数
    __rb_tree_iterator() {}
    __rb_tree_iterator(link_type x) : node(x) {}
    __rb_tree_iterator(const __rb_tree_iterator& it) { node = it.node; }
    // 迭代器操作
    reference operator*() { return node->value; }
    pointer operator->() { return &(operator*()); }
    self& operator++() {
        increment();
        return *this;
    }
    self operator++(int) {
        self tmp = *this;
        ++this;
        return tmp;
    }
    self& operator--() {
        decrement();
        return *this;
    }
    self operator--(int) {
        self tmp = *this;
        --this;
        return tmp;
    }

    // 迭代器自增自减
    void increment() {
        // rb数按中序排列自增，自增的下一个，就是按左中右递归的往右的下一个
        // 存在右子树，递归遍历右子树，即转到右子树后的一直向左子树遍历
        if (node->right != nullptr) {
            node = node->right;
            while (node->left != nullptr) node = node->left;
        }
        // 不存在右子树，所有要回到上一次递归，即父亲节点
        // 如果是左子树上去的，即父节点为下一个节点（左中右）
        // 如果是右子树上去的，则需要一直回退到从左子树上去的情况
        else {
            link_type y = node->parent;
            while (node == y->right) {
                node = y;
                y = y->parent;
            }
            if (node->right != y) node = y;
        }
    }
    void decrement() {
        // 没懂
        if (node->color == __rb_tree_red && node->parent->parent == node)
            node = node->right;
        // 左中右 中的上一个是左的最右
        else if (node->left != nullptr) {
            link_type y = node->left;
            while (y->right != nullptr) y = y->right;
            node = y;
        }
        // 如果是右节点，上一个就是中节点
        // 如果是右子树上去的，即父节点为上一个节点（左中右）
        // 如果是左子树上去的，则需要一直回退到从右子树上去的情况
        else {
            link_type y = node->parent;
            while (node == y->left) {
                node = y;
                y = y->parent;
            }
            node = y;
        }
    }
};

template <class Key, class Value, class KeyOfValue, class Compare,
          class Alloc = mySTL::Allocator<__rb_tree_node<Value>>>
class rb_tree {
protected:
    typedef void* void_pointer;
    typedef __rb_tree_color_type color_type;

public:
    typedef Alloc node_allocator;
    typedef typename Alloc::rebind<Value>::other data_allocator;
    typedef typename data_allocator::value_type value_type;
    typedef typename data_allocator::pointer pointer;
    typedef typename data_allocator::reference reference;
    typedef typename data_allocator::size_type size_type;
    typedef typename data_allocator::difference_type difference_type;
    typedef __rb_tree_node<Value> node_type;
    typedef __rb_tree_node<Value>* link_type;
    typedef __rb_tree_iterator<Value> iterator;

protected:
    link_type create_node(const value_type& x) {
        link_type tmp = node_allocator::allocate();
        mySTL::construct(&tmp->value, x);
        return tmp;
    }
    void destroy_node(link_type p) {
        mySTL::destroy(&p->value);
        node_allocator::deallocate(p);
    }
    link_type clone_node(link_type p) {
        link_type tmp = create_node(p->value);
        tmp->color = p->color;
        tmp->left = 0;
        tmp->right = 0;
        return tmp;
    }

protected:
    size_type node_count;  // 节点数量
    link_type header;      // 头节点
    Compare key_compare;   // 比较函数

    link_type& root() { return (link_type&)header->parent; }
    link_type& leftmost() { return (link_type&)header->left; }
    link_type& rightmost() { return (link_type&)header->right; }

    static link_type& left(link_type x) { return (link_type&)(x->left); }
    static link_type& right(link_type x) { return (link_type&)(x->right); }
    static link_type& parent(link_type x) { return (link_type&)(x->parent); }
    static reference value(link_type x) { return x->value; }
    // 注意这里的key用的仿函数
    static Key& key(link_type x) { return KeyOfValue()(value(x)); }
    static color_type& color(link_type x) { return (color_type&)(x->color); }
    static link_type minimum(link_type x) { return node_type::minimum(x); }
    static link_type maximum(link_type x) { return node_type::maximum(x); }

public:
    rb_tree(const Compare comp = Compare()) : node_count(0), key_compare(comp) {
        init();
    }
    ~rb_tree() {
        clear();
        node_allocator::deallocate(header);
    }

public:
    Compare key_comp() { return key_compare; }
    iterator begin() { return leftmost(); }
    iterator end() { return header; }
    bool empty() { return node_count == 0; }
    size_type size() { return node_count; }

public:
    pair<iterator, bool> insert_unique(const value_type& x);
    iterator insert_equal(const value_type& x);
    void clear();

private:
    iterator __insert(link_type x, link_type y, const value_type& v);
    link_type __copy(link_type x, link_type p);
    void erase(link_type x);
    void init() {
        header = node_allocator::allocate();
        color(header) = __rb_tree_red;
        root() = nullptr;
        leftmost() = nullptr;
        rightmost() = nullptr;
    }
};

// private
// insert x是新值插入点，y是父节点
template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::__insert(link_type x, link_type y, const value_type& v) {
    link_type z;
    if (y == header || x != nullptr || key_compare(KeyOfValue()(v), key(y))) {
        z = create_node(v);
        left(y) = z;
        if (y == header) {
            root() = z;
            rightmost() = z;
        } else if (y == leftmost())
            leftmost() = z;
    } else {
        z = create_node(v);
        right(y) = z;
        if (y == rightmost()) rightmost() = z;
    }
    parent(z) = y;
    left(z) = 0;
    right(z) = 0;
    __rb_tree_rebalance(z, header->parent);
    ++node_count;
    return iterator(z);
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_equal(
    const value_type& v) {
    link_type y = header;
    link_type x = root();
    while (x != nullptr) {
        y = x;
        x = key_compare(KeyOfValue()(v), key(x)) ? left(x) : right(x);
    }
    return __insert(x, y, v);
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
pair<typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator, bool>
rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_unique(
    const value_type& v) {
        link_type y = header;
        link_type x = root();
        bool comp = true;
        while(x != nullptr){
            y = x;
            comp = key_compare(KeyOfValue()(v),key(x));
            x = comp?left(x):right(y);
        }
        iterator j = iterator(y);
        if(comp){
            if(j==begin())
                return pair<iterator,bool>(__insert(x,y,v),true);
            else
                --j;
        }
        if(key_compare(key(j.node),KeyOfValue()(v)))
            return pair<iterator,bool>(__insert(x,y,v),true);
        return pair<iterator,bool>(j,false);
    }

}  // namespace mySTL
#endif