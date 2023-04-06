#ifndef TREE_HPP
#define TREE_HPP

#include <cstddef>
#include <cstdint>
#include <vector>

class Scapegoat {
private:
    struct Node {
        int key;
        size_t subtree_size = 1;
        Node* parent        = nullptr;
        Node* left          = nullptr;
        Node* right         = nullptr;

        Node() = default;
        Node(int key);
        Node(int key, Node* parent);

        ~Node();
    };

    Node* root = nullptr;

    size_t tree_size = 0, max_size = 0;
    float alpha = 0.75;

    static Node* search(Node* v, int key);

    static Node* insert_key(Node* v, int value, unsigned int& depth);

    static Node* remove_key(Node* v, int value);

    static void values_rec(std::vector<int>& values, Node* v);

    unsigned int h_alpha(size_t size) const;
    static float base_log(float base, float value);

    Node* find_scapegoat(Node* v) const;
    void rebuild_tree(Node* scapegoat);
    static Node* build_balanced_tree(Node* v, size_t size);
    static Node* flatten_tree(Node* v, Node* head);

public:
    Scapegoat() = default;

    [[nodiscard]] bool contains(int value) const noexcept;
    bool insert(int value);
    bool remove(int value);

    [[nodiscard]] size_t size() const noexcept;
    [[nodiscard]] bool empty() const noexcept;

    [[nodiscard]] std::vector<int> values() const noexcept;

    ~Scapegoat();
};

#endif
