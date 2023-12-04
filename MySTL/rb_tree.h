#ifndef _RB_TREE_H_
#define _RB_TREE_H_

#include <vector>

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
    bool operator==(const self& rhs) const { return node == rhs.node; }

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
        if (node->color == __rb_tree_red && node->parent->parent == node) node = node->right;
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

    static link_type& left(link_type x, bool symmetry = false) {
        if (symmetry) return (link_type&)(x->right);
        return (link_type&)(x->left);
    }
    static link_type& right(link_type x, bool symmetry = false) {
        if (symmetry) return (link_type&)(x->left);
        return (link_type&)(x->right);
    }
    static link_type& parent(link_type x) { return (link_type&)(x->parent); }
    static reference value(link_type x) { return x->value; }
    // 注意这里的key用的仿函数
    static Key key(link_type x) { return KeyOfValue()(value(x)); }
    static color_type& color(link_type x) { return (color_type&)(x->color); }
    static link_type minimum(link_type x) { return node_type::minimum(x); }
    static link_type maximum(link_type x) { return node_type::maximum(x); }

public:
    rb_tree(const Compare comp = Compare()) : node_count(0), key_compare(comp) { init(); }
    ~rb_tree() {
        // clear();
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
    iterator find(const Key& x);
    void clear();

private:
    iterator __insert(link_type x, link_type y, const value_type& v);
    link_type __copy(link_type x, link_type p);
    void erase(link_type x);
    void init() {
        header = node_allocator::allocate();
        color(header) = __rb_tree_red;
        root() = nullptr;
        leftmost() = header;
        rightmost() = header;
    }
    // 重新平衡二叉搜索树
    inline void __rb_tree_rebalance(link_type x, link_type& root) {
        x->color = __rb_tree_red;  // 将当前节点的颜色设置为红色
        bool symmetry = true;
        // 循环直到当前节点为根节点或者当前节点的父节点颜色为黑色
        while (x != root && x->parent->color == __rb_tree_red) {
            // 如果当前节点的父节点为根节点的左子节点
            if (x->parent == x->parent->parent->left) symmetry = false;

            link_type y = x->parent->parent->right;        // 右子节点
            if (y && y->color == __rb_tree_red) {          // 右子节点存在且颜色为红色
                x->parent->color = __rb_tree_black;        // 父节点颜色设为黑色
                y->color = __rb_tree_black;                // 右子节点颜色设为黑色
                x->parent->parent->color = __rb_tree_red;  // 祖先节点颜色设为红色
                x = x->parent->parent;                     // 更新当前节点为祖先节点
            } else {
                if (x == right(x->parent, symmetry)) {    // 当前节点为父节点的右子节点
                    x = x->parent;                        // 更新当前节点为父节点
                    __rb_tree_rotate(x, root, symmetry);  // 对父节点进行左旋转
                }
                x->parent->color = __rb_tree_black;        // 父节点颜色设为黑色
                x->parent->parent->color = __rb_tree_red;  // 祖先节点颜色设为红色
                __rb_tree_rotate(x->parent->parent, root, !symmetry);  // 对祖先节点进行右旋转
            }
        }
        root->color = __rb_tree_black;  // 根节点颜色设为黑色
    }
    inline void __rb_tree_rotate(link_type x, link_type& root, bool symmetry = false) {
        link_type y = right(x, symmetry);
        right(x, symmetry) = left(x, symmetry);
        if (left(y, symmetry) != nullptr) {
            parent(left(y, symmetry)) = x;
        }
        y->parent = x->parent;
        if (x == root)
            root = y;
        else if (x == left(y->parent, symmetry))
            left(y->parent, symmetry) = y;
        else
            right(y->parent, symmetry) = y;
        left(y, symmetry) = x;
        x->parent = y;
    }

private:
    void inorderTraversal(link_type node, std::vector<value_type>& result) {
        if (node != nullptr) {
            inorderTraversal(node->left, result);
            result.push_back(node->value);
            inorderTraversal(node->right, result);
        }
    }

public:
    std::vector<value_type> traverseInOrder() {
        std::vector<value_type> result;
        inorderTraversal(header->parent, result);
        return result;
    }
};

// private
// insert x是新值插入点，y是父节点
template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::__insert(link_type x, link_type y,
                                                          const value_type& v) {
    link_type z = create_node(v);
    // 三种情况 1.第一个节点 2.x不为空 怎么可能 3.比父节点小（假设是less）
    // 插入作为左节点
    if (y == header || x != nullptr || key_compare(KeyOfValue()(v), key(y))) {
        left(y) = z;
        if (y == header) {
            root() = z;
            rightmost() = z;
            // 没有左节点吗
        } else if (y == leftmost())
            leftmost() = z;
    } else {
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
rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_equal(const value_type& v) {
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
rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_unique(const value_type& v) {
    link_type y = header;
    link_type x = root();
    bool comp = true;
    while (x != nullptr) {
        y = x;
        comp = key_compare(KeyOfValue()(v), key(x));
        x = comp ? left(x) : right(y);
    }
    iterator j = iterator(y);
    // 如果comp为true,说明x在左子树 也就是确定小于，需要和当前节点的上一个去比较
    // 否则就是和当前节点y比较
    if (comp) {
        if (j == begin())  // j已经是最小
            return pair<iterator, bool>(__insert(x, y, v), true);
        else
            --j;  // 找上一个
    }
    // 和j比较大小，满足说明可以插入，否则就是等于
    if (key_compare(key(j.node), KeyOfValue()(v)))
        return pair<iterator, bool>(__insert(x, y, v), true);
    return pair<iterator, bool>(j, false);
}
template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::find(const Key& k) {
    link_type y = header;
    link_type x = root();
    while (x != nullptr) {
        if (!key_compare(key(x), k)) {
            y = x;
            x = left(x);
        } else
            x = right(x);
    }
    iterator j = iterator(y);
    return (j == end() || key_compare(k, key(j.node))) ? end() : j;
}
}  // namespace mySTL
#endif