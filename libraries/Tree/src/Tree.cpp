#include "tree/Tree.hpp"

#include <cmath>

Scapegoat::Node::Node(int key) : key(key) {}
Scapegoat::Node::Node(int key, Node* parent) : key(key), parent(parent) {}
Scapegoat::Node::~Node() {
    delete left;
    delete right;
}

// ------------------------------------------------- General Algorithm -------------------------------------------------

float Scapegoat::base_log(float base, float x)  // To extract common code
{
    return std::log(x) / std::log(base);
}

unsigned int Scapegoat::h_alpha(size_t size) const {
    return -base_log(alpha, size);
}

Scapegoat::Node* Scapegoat::flatten_tree(Node* cur, Node* start) {
    if (cur == nullptr) {
        return start;
    }
    cur->right = flatten_tree(cur->right, start);
    return flatten_tree(cur->left, cur);
}

Scapegoat::Node* Scapegoat::build_balanced_tree(Node* cur, size_t size) {
    if (size == 0) {
        cur->left         = nullptr;
        cur->subtree_size = 1;
        return cur;
    }
    double tmp      = (size - 1) / 2.0;
    Node* new_root  = build_balanced_tree(cur, std::ceil(tmp));
    Node* new_last  = build_balanced_tree(new_root->right, std::floor(tmp));
    new_root->right = new_last->left;
    if (new_last->left != nullptr) {
        new_last->subtree_size -= new_last->left->subtree_size;
        new_last->left->parent = new_root;
        new_root->subtree_size += new_last->left->subtree_size;
    }
    new_last->left   = new_root;
    new_root->parent = new_last;
    new_last->subtree_size += new_root->subtree_size;
    return new_last;
}

void Scapegoat::rebuild_tree(Node* scapegoat) {
    if (scapegoat == nullptr) {
        return;
    }

    if (scapegoat == root) {
        max_size = tree_size;
    }
    Node tmp;
    Node* parent = scapegoat->parent;
    build_balanced_tree(flatten_tree(scapegoat, &tmp), scapegoat->subtree_size);
    if (parent == nullptr) {
        root         = tmp.left;
        root->parent = nullptr;
    } else {
        if (parent->left != nullptr && parent->left->key == scapegoat->key) {
            parent->left = tmp.left;
        } else {
            parent->right = tmp.left;
        }
        tmp.left->parent = parent;
    }
    tmp.left = nullptr;
}

// ------------------------------------------------------- Size --------------------------------------------------------

size_t Scapegoat::size() const noexcept {
    return tree_size;
}

bool Scapegoat::empty() const noexcept {
    return size() == 0;  // To extract common code
}

// ----------------------------------------------------- Contains ------------------------------------------------------

Scapegoat::Node* Scapegoat::search(Node* cur, int key) {
    while (cur != nullptr && cur->key != key) {
        cur = key < cur->key ? cur->left : cur->right;
    }
    return cur;
}

bool Scapegoat::contains(int value) const noexcept {
    return search(root, value) != nullptr;
}

// ------------------------------------------------------ Insert -------------------------------------------------------

Scapegoat::Node* Scapegoat::find_scapegoat(Node* cur) const {
    unsigned int height = 0;
    while (cur != nullptr && height++ <= h_alpha(cur->subtree_size)) {
        cur = cur->parent;
    }
    return cur;
}

Scapegoat::Node* Scapegoat::insert_key(Node* cur, int key, unsigned int& insertion_depth) {
    Node* parent = cur;
    while (cur != nullptr) {
        ++cur->subtree_size;
        ++insertion_depth;
        parent = cur;
        cur    = key < cur->key ? cur->left : cur->right;
    }
    cur = new Node(key, parent);
    if (key < parent->key) {
        parent->left = cur;
    } else {
        parent->right = cur;
    }
    return cur;
}

bool Scapegoat::insert(int value) {
    if (contains(value)) {
        return false;
    }

    max_size = std::max(++tree_size, max_size);

    if (root == nullptr) {
        root = new Node(value);
    } else {
        unsigned int insertion_depth = 0;
        Node* cur                    = insert_key(root, value, insertion_depth);
        if (insertion_depth > h_alpha(tree_size)) {
            rebuild_tree(find_scapegoat(cur));
        }
    }
    return true;
}

// ------------------------------------------------------ Remove -------------------------------------------------------

Scapegoat::Node* Scapegoat::remove_key(Node* cur, int key) {
    if (cur != nullptr) {
        --(cur->subtree_size);
    }

    if (key < cur->key) {
        cur->left = remove_key(cur->left, key);
    } else if (key > cur->key) {
        cur->right = remove_key(cur->right, key);
    } else {
        if (cur->left == nullptr && cur->right == nullptr) {
            delete cur;
            return nullptr;
        }
        if (cur->left == nullptr || cur->right == nullptr) {
            Node* tmp  = cur->left == nullptr ? cur->right : cur->left;
            cur->left  = nullptr;
            cur->right = nullptr;
            delete cur;
            tmp->parent = nullptr;
            return tmp;
        }
        Node* tmp = cur->right;
        while (tmp->left != nullptr) {
            tmp = tmp->left;
        }
        cur->key   = tmp->key;
        cur->right = remove_key(cur->right, tmp->key);
    }
    return cur;
}

bool Scapegoat::remove(int value) {
    if (!contains(value)) {
        return false;
    }
    root = remove_key(root, value);
    if (--tree_size < alpha * max_size) {
        rebuild_tree(root);
    }
    return true;
}

// ------------------------------------------------------ Values -------------------------------------------------------

void Scapegoat::values_rec(std::vector<int>& values, Node* cur) {
    if (cur == nullptr) {
        return;
    }
    values_rec(values, cur->left);
    values.emplace_back(cur->key);
    values_rec(values, cur->right);
}

std::vector<int> Scapegoat::values() const noexcept {
    std::vector<int> result;
    result.reserve(tree_size);
    values_rec(result, root);
    return result;
}

// ---------------------------------------------------------------------------------------------------------------------

Scapegoat::~Scapegoat() {
    delete root;
}
