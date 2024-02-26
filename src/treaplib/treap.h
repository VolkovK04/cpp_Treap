#include <cstdlib>
#include <iostream>
#include <type_traits>

template <typename T> struct TreapNode {
  T key;
  int priority;
  TreapNode *left;
  TreapNode *right;

  TreapNode(T k) : key(k), priority(rand()), left(nullptr), right(nullptr) {}
};

template <typename T> class Treap {
private:
  TreapNode<T> *root;

  static_assert(std::is_same_v<T, float> || std::is_same_v<T, double> ||
                    (std::is_integral_v<T> && !std::is_unsigned_v<T>),
                "only float, double and signed integer types are supported");

  void split(TreapNode<T> *curr, T key, TreapNode<T> *&left,
             TreapNode<T> *&right) {
    if (!curr) {
      left = nullptr;
      right = nullptr;
    } else if (key < curr->key) {
      split(curr->left, key, left, curr->left);
      right = curr;
    } else {
      split(curr->right, key, curr->right, right);
      left = curr;
    }
  }

  TreapNode<T> *merge(TreapNode<T> *left, TreapNode<T> *right) {
    if (!left || !right)
      return left ? left : right;
    if (left->priority > right->priority) {
      left->right = merge(left->right, right);
      return left;
    } else {
      right->left = merge(left, right->left);
      return right;
    }
  }

public:
  Treap() : root(nullptr) {}

  void insert(T key) {
    if (!root) {
      root = new TreapNode<T>(key);
      return;
    }
    if (search(key))
      return; // Ensure no duplicate keys
    TreapNode<T> *left, *right;
    split(root, key, left, right);
    root = merge(merge(left, new TreapNode<T>(key)), right);
  }

  void erase(T key, TreapNode<T> *&node) {
    if (!node)
      return;

    if (key < node->key)
      erase(key, node->left);
    else if (key > node->key)
      erase(key, node->right);
    else {
      TreapNode<T> *temp = node;
      node = merge(node->left, node->right);
      delete temp;
    }
  }

  void erase(T key) { erase(key, root); }

  bool search(T key) {
    TreapNode<T> *curr = root;
    while (curr) {
      if (key == curr->key)
        return true;
      else if (key < curr->key)
        curr = curr->left;
      else
        curr = curr->right;
    }
    return false;
  }

  void inorderTraversal(TreapNode<T> *curr) {
    if (curr) {
      inorderTraversal(curr->left);
      std::cout << curr->key << " ";
      inorderTraversal(curr->right);
    }
  }

  void printInorder() {
    inorderTraversal(root);
    std::cout << std::endl;
  }
};