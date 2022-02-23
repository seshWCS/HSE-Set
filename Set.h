#pragma once

#include <list>
#include <algorithm>

template<class ValueType> class Set {
private:
    int size_of_Set = 0;
    struct node {
        node* l = nullptr;
        node* r = nullptr;
        node* L = nullptr;
        node* R = nullptr;
        node* pr = nullptr;
        ValueType val;
        int h = 0;
        node() = default;
        node(ValueType v) {
            val = v;
            l = nullptr;
            r = nullptr;
            h = 1;
            L = this;
            R = this;
        }
        node(node& nd) = default;

        ~node() = default;
    };

    int height(node* root) {
        if (root == nullptr) {
            return 0;
        }
        return root->h;
    }

    int bfac(node* root) {
        return height(root->r) - height(root->l);
    }

    void updh(node* root) {
        root->h = std::max(height(root->l), height(root->r)) + 1;
    }

    node* getL(node* root) {
        if (root == nullptr) {
            return nullptr;
        }
        return root->L;
    }

    node* getR(node* root) {
        if (root == nullptr) {
            return nullptr;
        }
        return root->R;
    }

    void updLR(node* root) {
        if (root->l == nullptr) {
            root->L = root;
        } else {
            root->L = getL(root->l);
            root->l->pr = root;
        }
        if (root->r == nullptr) {
            root->R = root;
        } else {
            root->R = getR(root->r);
            root->r->pr = root;
        }
    }

    node* rightrotate(node* root) {
        node* ans = root->l;
        root->l = ans->r;
        ans->r = root;
        ans->pr = root->pr;
        updLR(root);
        updLR(ans);
        updh(root);
        updh(ans);
        return ans;
    }

    node* leftrotate(node* root) {
        node* ans = root->r;
        root->r = ans->l;
        ans->l = root;
        ans->pr = root->pr;
        updLR(root);
        updLR(ans);
        updh(root);
        updh(ans);
        return ans;
    }

    node* balance(node* root) {
        updh(root);
        updLR(root);
        if (bfac(root) == 2) {
            if (bfac(root->r) < 0) {
                root->r = rightrotate(root->r);
            }
            return leftrotate(root);
        }
        if (bfac(root) == -2) {
            if (bfac(root->l) > 0) {
                root->l = leftrotate(root->l);
            }
            return rightrotate(root);
        }
        return root;
    }

    node* insert(node* root, ValueType v) {
        if (root == nullptr) {
            ++size_of_Set;
            return new node(v);
        }
        if (v < root->val) {
            root->l = insert(root->l, v);
            root->l->pr = root;
        } else if (root->val < v) {
            root->r = insert(root->r, v);
            root->r->pr = root;
        }
        updLR(root);
        updh(root);
        return balance(root);
    }

    node* findmin(node* root) const {
        if (root->l != nullptr) {
            return findmin(root->l);
        } else {
            return root;
        }
    }

    node* erasemin(node* root) {
        if (root->l == nullptr) {
            return root->r;
        }
        root->l = erasemin(root->l);
        return balance(root);
    }

    node* erase(node* root, ValueType v) {
        if (root == nullptr) {
            return root;
        }
        if (v < root->val) {
            root->l = erase(root->l, v);
        } else if (root->val < v) {
            root->r = erase(root->r, v);
        } else {
            node* a = root->l;
            node* b = root->r;
            delete root;
            --size_of_Set;
            if (b == nullptr) {
                return a;
            }
            node* min = findmin(b);
            min->r = erasemin(b);
            min->l = a;
            return balance(min);
        }
        return balance(root);
    }

    node* fnd(node* root, ValueType v) const {
        if (root == nullptr) {
            return nullptr;
        }
        if (root->val < v) {
            return fnd(root->r, v);
        } else if (v < root->val) {
            return fnd(root->l, v);
        } else {
            return root;
        }
    }
    node* rt = nullptr;
    static void destruct_node(node* nd) {
        if (nd == nullptr) {
            return;
        }
        if (nd->l != nullptr) {
            destruct_node(nd->l);
        }
        if (nd->r != nullptr) {
            destruct_node(nd->r);
        }
        nd->L = nullptr;
        nd->R = nullptr;
        nd->l = nullptr;
        nd->r = nullptr;
        nd->pr = nullptr;
        delete nd;
    }
    node* copy(node* rt1) {
        if (rt1 == nullptr) {
            return nullptr;
        }
        node* ans;
        ans = new node(*rt1);
        ans->l = copy(rt1->l);
        ans->r = copy(rt1->r);
        updh(ans);
        updLR(ans);
        return ans;
    }
public:
    class iterator {
    private:
        friend Set;
        node* nd = nullptr;
        node* root = nullptr;
        explicit iterator(node* _rt) : root(_rt) {
            nd = nullptr;
        }
        iterator(node* _nd, node* _rt) : nd(_nd), root(_rt) {
        }
    public:
        iterator() = default;
        ~iterator() {
            nd = nullptr;
            root = nullptr;
        }
        iterator(const iterator& it) {
            nd = it.nd;
            root = it.root;
        }
        const ValueType& operator *() const {
            return nd->val;
        }
        const ValueType* operator ->() const {
            return &nd->val;
        }
        bool operator ==(const iterator& it2) {
            return nd == it2.nd;
        }
        bool operator !=(const iterator& it2) {
            return nd != it2.nd;
        }
        const iterator operator ++ (int) {
            iterator ans = *this;
            if (nd == nullptr) {
                return ans;
            }
            if (nd->r != nullptr) {
                nd = nd->r;
                while (nd->l != nullptr) {
                    nd = nd->l;
                }
                *this = iterator(nd, root);
                return ans;
            }
            if (nd == root->R) {
                *this = iterator(root);
                return ans;
            }
            node* lst = nd;
            while (nd->val < lst->val || (!(nd->val < lst->val) && !(lst->val < nd->val))) {
                nd = nd->pr;
            }
            *this = iterator(nd, root);
            return ans;
        }

        iterator& operator ++ () {
            if (nd == nullptr) {
                return *this;
            }
            if (nd->r != nullptr) {
                nd = nd->r;
                while (nd->l != nullptr) {
                    nd = nd->l;
                }
                *this = iterator(nd, root);
                return *this;
            }
            if (nd == root->R) {
                *this = iterator(root);
                return *this;
            }
            node* lst = nd;
            while (nd->val < lst->val || (!(nd->val < lst->val) && !(lst->val < nd->val))) {
                nd = nd->pr;
            }
            *this = iterator(nd, root);
            return *this;
        }

        const iterator operator -- (int) {
            iterator ans = *this;
            if (nd == nullptr) {
                *this = iterator(root->R, root);
                return ans;
            }
            if (nd->l != nullptr) {
                nd = nd->l;
                while (nd->r != nullptr) {
                    nd = nd->r;
                }
                *this = iterator(nd, root);
                return ans;
            }
            if (nd == root->L) {
                *this = iterator(root);
                return ans;
            }
            node* lst = nd;
            while ((!(nd->val < lst->val) && !(lst->val < nd->val)) || lst->val < nd->val) {
                nd = nd->pr;
            }
            *this = iterator(nd, root);
            return ans;
        }

        iterator& operator -- () {
            if (nd == nullptr) {
                *this = iterator(root->R, root);
                return *this;
            }
            if (nd->l != nullptr) {
                nd = nd->l;
                while (nd->r != nullptr) {
                    nd = nd->r;
                }
                *this = iterator(nd, root);
                return *this;
            }
            if (nd == root->L) {
                *this = iterator(root);
                return *this;
            }
            node* lst = nd;
            while ((!(nd->val < lst->val) && !(lst->val < nd->val)) || lst->val < nd->val) {
                nd = nd->pr;
            }
            *this = iterator(nd, root);
            return *this;
        }
    };

    Set() = default;
    template<class ValueTypeIterator> Set(ValueTypeIterator begin, ValueTypeIterator end) {
        for (auto i = begin; i != end; ++i) {
            rt = insert(rt, *i);
        }
    }
    Set(std::initializer_list<ValueType> lst) {
        for (auto i : lst) {
            rt = insert(rt, i);
        }
    }

    Set(const Set& st) {
        size_of_Set = st.size_of_Set;
        rt = copy(st.rt);
    }

    Set& operator = (const Set& st) {
        if (this == &st) {
            return *this;
        }
        destruct_node(rt);
        size_of_Set = st.size_of_Set;
        rt = copy(st.rt);
        return *this;
    }

    size_t size() const {
        return size_of_Set;
    }

    bool empty() const {
        return size_of_Set == 0;
    }

    void insert(ValueType v) {
        rt = insert(rt, v);
    }

    void erase(ValueType v) {
        rt = erase(rt, v);
    }

    iterator end() const {
        return iterator(rt);
    }

    iterator begin() const {
        if (size_of_Set == 0) {
            return end();
        }
        return iterator(rt->L, rt);
    }

    iterator find(ValueType v) const {
        return iterator(fnd(rt, v), rt);
    }

private:
    iterator lbound(node* root, node* pred, ValueType v) const {
        if (root == nullptr) {
            iterator ans(pred, rt);
            if (*ans < v) {
                ++ans;
            }
            return ans;
        }
        if (root->val < v) {
            return lbound(root->r, root, v);
        } else if (v < root->val) {
            return lbound(root->l, root, v);
        } else {
            return iterator(root, rt);
        }
    }
public:
    iterator lower_bound(ValueType v) const {
        if (size_of_Set == 0) {
            return end();
        }
        return lbound(rt, nullptr, v);
    }

    ~Set() {
        destruct_node(rt);
    }
};
