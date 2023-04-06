#include <iostream>

#include "tree/Tree.hpp"

int main() {
    Scapegoat tree;

    tree.insert(1);
    tree.insert(2);
    tree.insert(3);
    tree.insert(4);
    tree.insert(5);

    // for (int i = 0; i < 100; ++i)
    //     tree.insert(i);
    // for (int i = 200; i > 100; --i)
    //     tree.insert(i);

    for (auto value : tree.values())
        std::cout << value << std::endl;

    std::cout << tree.remove(3) << std::endl;

    for (auto value : tree.values())
        std::cout << value << std::endl;
}
