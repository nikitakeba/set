#include <bits/stdc++.h>

#define nd shared_ptr<Node>

using namespace std;

template<class ValueType>
class Set {
public:
    bool empty() const {
        return (sz == 0);
    }

    size_t size() const {
        return sz;
    }

    Set() : root(nullptr), sz(0) {}

    Set(initializer_list<ValueType> v) : root(nullptr), sz(0) {
        for (auto &u : v) {
            insert(u);
        }
    }

    Set(const Set<ValueType> &s) : root(nullptr), sz(0) {
        for (auto &u : s) {
            insert(u);
        }
    }

    template<typename It>
    Set(It begin, It end) : root(nullptr), sz(0) {
        for (; begin != end; begin++) {
            insert(*begin);
        }
    }

    Set<ValueType> &operator=(const Set<ValueType> &s) {
        if (s.root == root)
            return *this;
        root = nullptr;
        sz = 0;
        for (auto &u : s) {
            insert(u);
        }
        return *this;
    }

    void insert(const ValueType &val) {
        if (find(val) == end()) {
            root = add(root, val);
            sz++;
        }
    }
    void erase(const ValueType &val) {
        if (find(val) != end()) {
            root = del(root, val);
            sz--;
        }
    }

private:
    struct Node {
        shared_ptr<Node> left;
        shared_ptr<Node> right;
        ValueType val;
        int h;

        Node(const ValueType &_val) : left(nullptr), right(nullptr), val(_val), h(1) {}

        ~Node() = default;
    };


    int geth(nd &n) {
        if (!n)
            return 0;
        return n->h;
    }

    int bal(nd &n) {
        if (!n)
            return 0;
        return geth(n->right) - geth(n->left);
    }

    void update(nd &n) {
        if (!n)
            return;
        n->h = 1 + max(geth(n->left), geth(n->right));
    }

    nd right_s(nd &n) {
        nd c = n->left;
        n->left = c->right;
        c->right = n;
        update(n);
        update(c);
        return c;
    }


    nd left_s(nd &n) {
        nd c = n->right;
        n->right = c->left;
        c->left = n;
        update(n);
        update(c);
        return c;
    }


    nd balanceNode(nd &n) {
        update(n);
        if (abs(bal(n)) < 2)
            return n;
        if (bal(n) == 2) {
            if (bal(n->right) < 0)
                n->right = right_s(n->right);
            return left_s(n);
        }
        if (bal(n) == -2) {
            if (bal(n->left) > 0)
                n->left = left_s(n->left);
            return right_s(n);
        }
        assert(0);
    }


    nd add(nd &n, const ValueType &x) {
        if (!n) {
            n = std::make_shared<Node>(x);
            return n;
        }
        if (x < n->val)
            n->left = add(n->left, x);
        else if (n->val < x)
            n->right = add(n->right, x);
        else {
            return n;
        }
        n = balanceNode(n);
        return n;
    }


    nd delete_min(nd &n) {
        if (!n->left)
            return n->right;
        n->left = delete_min(n->left);
        n = balanceNode(n);
        return n;
    }

    nd find_(nd &n) {
        if(!n->left)
            return n;
        return find_(n->left);
    }

    nd del(nd &n, const ValueType &x) {
        if (!n)
            return n;
        if (x < n->val) {
            n->left = del(n->left, x);
            n = balanceNode(n);
            return n;
        } else if (n->val < x) {
            n->right = del(n->right, x);
            n = balanceNode(n);
            return n;
        }
        nd l = n->left;
        nd r = n->right;

        n = nullptr;

        if (!r)
            return l;
        if (!l)
            return r;
        nd m = find_(r);
        r = delete_min(r);
        m->right = r;
        m->left = l;
        m = balanceNode(m);
        return m;
    }



    nd root;
    size_t sz;

public:
    class iterator {
    public:

        iterator(){};

        iterator(nd _root) : st(stack<nd>()), root(_root){
        }

        ~iterator() {}

        iterator(const iterator &it) {
            st = it.st;
            root = it.root;
        }

        iterator &operator=(const iterator &it) {
            st = it.st;
            root = it.root;
            return *this;
        }

        void getmin(nd n) {
            if (!n) {
                return;
            }
            st.push(n);
            getmin(n->left);
        }

        void getmax(nd n) {
            if (!n)
                return;
            st.push(n);
            getmax(n->right);
        }

        iterator &operator++() {
            if (!st.top()->right) {
                nd x = st.top();
                st.pop();
                while (!st.empty() && st.top()->right == x) {
                    x = st.top();
                    st.pop();
                }
            } else {
                getmin(st.top()->right);
            }
            return *this;
        }

        iterator operator++(int) {
            iterator it = *this;
            if (!st.top()->right) {
                nd x = st.top();
                st.pop();
                while (!st.empty() && st.top()->right == x) {
                    x = st.top();
                    st.pop();
                }
            } else {
                getmin(st.top()->right);
            }
            return it;
        }


        iterator &operator--() {
            if (st.empty()) {
                getmax(root);
            } else {
                if (!st.top()->left) {
                    nd x = st.top();
                    st.pop();
                    while (!st.empty() && st.top()->left == x) {
                        x = st.top();
                        st.pop();
                    }
                } else {
                    getmax(st.top()->left);
                }
            }
            return *this;
        }

        iterator operator--(int) {
            iterator it = *this;
            if (st.empty()) {
                getmax(root);
            } else {
                if (!st.top()->left) {
                    nd x = st.top();
                    st.pop();
                    while (!st.empty() && st.top()->left == x) {
                        x = st.top();
                        st.pop();
                    }
                } else {
                    getmax(st.top()->left);
                }
            }
            return it;
        }

        const ValueType &operator*() const {
            return st.top()->val;
        }

        ValueType const *operator->() const {
            return &st.top()->val;
        }

        friend bool operator==(const iterator &it1, const iterator &it2) {
            return (((it1.st.empty() && it2.st.empty()) || (it1.st.size() == it2.st.size() && it1.st.top() == it2.st.top())));
        }

        friend bool operator!=(const iterator &it1, const iterator &it2) {
            return !(((it1.st.empty() && it2.st.empty()) || (it1.st.size() == it2.st.size() && it1.st.top() == it2.st.top())));
        }

        iterator(nd _root, const ValueType &val) : st(stack<nd>()), root(_root){
            while (root) {
                st.push(root);
                if (root->val < val) {
                    root = root->right;
                } else {
                    root = root->left;
                }
            }
            while (!st.empty() && st.top()->val < val) {
                st.pop();
            }
        }

    private:
        stack<nd > st;
        nd root;
    };

public:
    iterator begin() const {
        iterator it(root);
        it.getmin(root);
        return it;
    }

    iterator end() const {
        iterator it(root);
        return it;
    }

    iterator lower_bound(const ValueType &val) const {
        iterator it(root, val);
        return it;
    }

    iterator find(const ValueType &val) const {
        iterator it(root, val);
        if (it == end())
            return it;
        if (*it < val || val < *it)
            return end();
        return it;
    }

};
