#ifndef _HASHTABLE_H_
#define _HASHTABLE_H_

#include "algo.h"
#include "allocator.h"
#include "iterator.h"
#include "vector.h"

namespace mySTL {

template <class Value>
struct __hash_node {
    typedef Value value_type;
    typedef __hash_node<Value>* link_type;

    link_type next;
    Value value;
};

// 预声明
template <class Value, class Key, class HashFcn, class ExtractKey, class EqualKey,
          class Alloc = mySTL::Allocator<Value> >
class hashtable;

template <class Value, class Key, class HashFcn, class ExtractKey, class EqualKey,
          class Alloc = mySTL::Allocator<Value> >
struct __hashtable_iterator : public iterator<forward_iterator_tag, Value> {
    typedef iterator<forward_iterator_tag, Value> iterator_base;

    typedef typename iterator_base::iterator_category iterator_category;
    typedef typename iterator_base::value_type value_type;
    typedef typename iterator_base::pointer pointer;
    typedef typename iterator_base::reference reference;
    typedef typename iterator_base::size_type size_type;
    typedef typename iterator_base::difference_type difference_type;

    typedef hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>* ht_link_type;
    typedef __hash_node<Value>* link_type;
    typedef __hashtable_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc> self;

    link_type cur;
    ht_link_type ht;
    // constructor
    __hashtable_iterator() {}
    __hashtable_iterator(ht_link_type ht, link_type cur) : cur(cur), ht(ht) {}
    // 迭代器操作
    reference operator*() { return cur->value; }
    pointer operator->() { return &(operator*()); }
    self& operator++() {
        link_type old = cur;
        cur = cur->next;
        // 开链，如果非空，就是下一个，否则该链结束，找下一个位置
        if (cur == nullptr) {
            size_type bucket = ht->bkt_num(old->value);
            while (cur == nullptr && ++bucket < ht->buckets.size())
                cur = ht->buckets[bucket];
        }
        return *this;
    }
    self operator++(int) {
        self tmp = *this;
        ++this;
        return tmp;
    }
    // 前向迭代器没有--
    bool operator==(const self& rhs) const { return cur == cur.node; }
    bool operator!=(const self& rhs) const { return cur != cur.node; }
};

static const int __stl_num_primes = 28;
static const unsigned long __stl_prime_list[__stl_num_primes] = {
    53ul,        97ul,         193ul,        389ul,       769ul,       1543ul,
    3079ul,      6151ul,       12289ul,      24593ul,     49157ul,     98317ul,
    196613ul,    393241ul,     786433ul,     1572869ul,   3145739ul,   6291469ul,
    12582917ul,  25165843ul,   50331653ul,   100663319ul, 201326611ul, 402653189ul,
    805306457ul, 1610612741ul, 3221225473ul, 4294967291ul};
inline unsigned long __stl_next_prime(unsigned long n) {
    const unsigned long* first = __stl_prime_list;
    const unsigned long* last = first + __stl_num_primes;
    const unsigned long* pos = std::lower_bound(first, last, n);
    return pos == last ? *last : *pos;
}
size_t max_bucket_count() { return __stl_next_prime(__stl_num_primes - 1); }

template <class Value, class Key, class HashFcn, class ExtractKey, class EqualKey,
          class Alloc>
class hashtable {
public:
    friend struct __hashtable_iterator<Value,Key,HashFcn,ExtractKey,EqualKey>;

    typedef HashFcn hasher;
    typedef EqualKey key_equal;
    typedef ExtractKey key_extract;
    typedef __hash_node<Value> node_type;
    typedef __hash_node<Value>* link_type;
    typedef __hashtable_iterator<Value, Key, HashFcn, ExtractKey, EqualKey> iterator;

    typedef Alloc allocator;
    typedef typename Alloc::rebind<node_type>::other node_allocator;
    typedef typename allocator::value_type value_type;
    typedef typename allocator::pointer pointer;
    typedef typename allocator::reference reference;
    typedef typename allocator::size_type size_type;
    typedef typename allocator::difference_type difference_type;

private:
    hasher hash;
    key_equal equals;
    ExtractKey get_key;
    mySTL::vector<link_type> buckets;
    size_type num_elements;

private:
    link_type new_node(const value_type& value) {
        link_type node = node_allocator::allocate();
        node->next = nullptr;
        mySTL::construct(&node->value, value);
        return node;
    }
    void delete_node(link_type node) {
        mySTL::destroy(&node->value);
        node_allocator::deallocate(node);
    }
    size_type next_size(size_type n) { return __stl_next_prime(n); }
    void initialize_buckets(size_type n) {
        const size_type num_buckets = next_size(n);
        buckets.resize(num_buckets, nullptr);
        num_elements = 0;
    }

public:
    // constructor
    hashtable(size_type num_elements, const HashFcn& hf, const EqualKey& eql,
              const ExtractKey& ex, const Alloc& alloc = Alloc())
        : hash(hf), equals(eql), get_key(ex), num_elements(0) {
        initialize_buckets(num_elements);
    }
    // 迭代器
    iterator begin() {
        for (size_type i = 0; i < buckets.size(); ++i)
            if (buckets[i]!= nullptr)
                return iterator(this, buckets[i]);
        return end();    
    }
    iterator end() { return iterator(this, nullptr); }
    size_type bucket_count() { return buckets.size(); }
    size_type size() { return num_elements; }
    size_type bkt_num(const value_type& obj, size_type n) {
        return hash(get_key(obj)) % n;
    }
    size_type bkt_num(const value_type& obj) {
        return hash(get_key(obj)) % bucket_count();
    }
    // insert
    pair<iterator, bool> insert_unique(const value_type& obj) {
        resize(num_elements + 1);
        return insert_unique_noresize(obj);
    }
    iterator insert_equal(const value_type& obj) {
        resize(num_elements + 1);
        return insert_equal_noresize(obj);
    }
    void resize(size_type num_elements_hint) {
        size_type old_num_buckets = buckets.size();  // 保存旧的哈希桶数量
        if (num_elements_hint >
            old_num_buckets) {  // 如果建议调整后的哈希表大小大于旧的哈希桶数量
            size_type new_num_buckets =
                next_size(num_elements_hint);  // 计算新的哈希桶数量
            if (new_num_buckets >
                old_num_buckets) {  // 如果新的哈希桶数量大于旧的哈希桶数量
                mySTL::vector<link_type> tmp(new_num_buckets,
                                             nullptr);  // 创建临时哈希桶数组
                for (size_type i = 0; i < old_num_buckets; ++i) {  // 遍历旧的哈希桶数组
                    link_type cur = buckets[i];  // 获取当前哈希桶的头结点
                    while (cur != nullptr) {     // 遍历当前哈希桶中的元素
                        size_type new_bucket =
                            bkt_num(cur->value,
                                    new_num_buckets);  // 计算元素值在新哈希桶数组中的索引
                        buckets[i] = cur->next;  // 更新旧哈希桶的头结点
                        cur->next = tmp[new_bucket];  // 更新元素在新哈希桶数组中的位置
                        tmp[new_bucket] = cur;  // 将当前元素插入到新哈希桶数组中
                        cur = buckets[i];  // 更新当前哈希桶的头结点为当前元素的下一个元素
                    }
                }
                buckets.swap(tmp);  // 交换旧哈希桶数组和新哈希桶数组
            }
        }
    }

private:
    pair<iterator, bool> insert_unique_noresize(const value_type& obj) {
        size_type bucket_num = bkt_num(obj);
        link_type first = buckets[bucket_num];
        for (link_type cur = first; cur != nullptr; cur = cur->next) {
            if (equals(get_key(cur->value), get_key(obj)))
                return pair<iterator, bool>(iterator(this, cur), false);
        }
        link_type tmp = new_node(obj);
        tmp->next = first;
        buckets[bucket_num] = tmp;
        ++num_elements;
        return pair<iterator, bool>(iterator(this, tmp), true);
    }
    iterator insert_equal_noresize(const value_type& obj) {
        size_type bucket_num = bkt_num(obj);
        link_type first = buckets[bucket_num];
        link_type cur = first;
        for (cur != nullptr; cur != nullptr; cur = cur->next) {
            if (equals(get_key(cur->value), get_key(obj))) {
                link_type tmp = new_node(obj);
                tmp->next = first;
                cur->next = tmp;
                ++num_elements;
                return iterator(this, tmp);
            }
        }
        link_type tmp = new_node(obj);
        tmp->next = first;
        buckets[bucket_num] = tmp;
        ++num_elements;
        return iterator(this, tmp);
    }

public:
    void clear() {
        for (size_type i = 0; i < bucket_count(); ++i) {
            link_type cur = buckets[i];
            while (cur != nullptr) {
                link_type tmp = cur;
                cur = cur->next;
                delete_node(tmp);
            }
            buckets[i] = nullptr;
        }
        num_elements = 0;
    }
    void copy_from(const hashtable& ht) {
        buckets.clear();
        buckets.resize(ht.bucket_count(), nullptr);
        for (size_type i = 0; i < ht.bucket_count(); ++i) {
            link_type cur = ht.buckets[i];
            if (cur != nullptr) {
                link_type copy = new_node(cur->value);
                buckets[i] = copy;
                for (link_type next = cur->next; next != nullptr; next = next->next) {
                    copy->next = new_node(next->value);
                    copy = copy->next;
                }
            }
        }
        num_elements = ht.num_elements;
    }
};

}  // namespace mySTL
#endif