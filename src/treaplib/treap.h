#include <cstdlib>
#include <iostream>
#include <stack>
#include <type_traits>
#include <utility>

template <typename T> struct TreapNode {
  T key;
  int priority;
  TreapNode *left;
  TreapNode *right;

  TreapNode(T k) : key(k), priority(rand()), left(nullptr), right(nullptr) {}

  // Copy constructor
  TreapNode(const TreapNode<T> &other)
      : key(other.key), priority(other.priority), left(nullptr),
        right(nullptr) {
    if (other.left)
      left = new TreapNode<T>(*other.left);
    if (other.right)
      right = new TreapNode<T>(*other.right);
  }

  // Move constructor
  TreapNode(TreapNode<T> &&other) noexcept
      : key(std::move(other.key)), priority(std::move(other.priority)),
        left(other.left), right(other.right) {
    other.left = nullptr;
    other.right = nullptr;
  }

  // Copy assignment operator
  TreapNode<T> &operator=(const TreapNode<T> &other) {
    if (this != &other) {
      TreapNode<T> temp(other); // Copy-and-swap idiom
      std::swap(key, temp.key);
      std::swap(priority, temp.priority);
      std::swap(left, temp.left);
      std::swap(right, temp.right);
    }
    return *this;
  }

  // Move assignment operator
  TreapNode<T> &operator=(TreapNode<T> &&other) noexcept {
    if (this != &other) {
      std::swap(key, other.key);
      std::swap(priority, other.priority);
      std::swap(left, other.left);
      std::swap(right, other.right);
    }
    return *this;
  }

  ~TreapNode() = default;
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

  void clear(TreapNode<T> *node) {
    if (node) {
      clear(node->left);
      clear(node->right);
      delete node;
    }
  }

public:
  Treap() : root(nullptr) {}

  // Copy constructor
  Treap(const Treap<T> &other) : root(nullptr) {
    if (other.root)
      root = new TreapNode<T>(*other.root);
  }

  // Move constructor
  Treap(Treap<T> &&other) noexcept : root(other.root) { other.root = nullptr; }

  // Copy assignment operator
  Treap<T> &operator=(const Treap<T> &other) {
    if (this != &other) {
      Treap temp(other); // Copy-and-swap idiom
      std::swap(root, temp.root);
    }
    return *this;
  }

  // Move assignment operator
  Treap<T> &operator=(Treap<T> &&other) noexcept {
    if (this != &other) {
      clear(root);
      root = other.root;
      other.root = nullptr;
    }
    return *this;
  }

  // Destructor
  ~Treap() { clear(root); }

  void insert(T key) {
    if (!root) {
      root = new TreapNode<T>(key);
      return;
    }
    if (search(key))
      return; // Ensure no duplicate keys
    TreapNode<T> *left;
    TreapNode<T> *right;
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
      temp->left = nullptr;
      temp->right = nullptr;
      delete temp;
    }
  }

  void erase(T key) { erase(key, root); }

  bool search(T key) const {
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

  void inorderTraversal(TreapNode<T> *curr) const {
    if (curr) {
      inorderTraversal(curr->left);
      std::cout << curr->key << " ";
      inorderTraversal(curr->right);
    }
  }

  void printInorder() const {
    inorderTraversal(root);
    std::cout << std::endl;
  }

  class Iterator {
  private:
    TreapNode<T> *current;
    std::stack<TreapNode<T> *> nodes;

    void pushLeft(TreapNode<T> *node) {
      while (node) {
        nodes.push(node);
        node = node->left;
      }
    }

  public:
    Iterator(TreapNode<T> *root) : current(nullptr) {
      pushLeft(root);
      ++(*this); // Move to the first element
    }

    T &operator*() { return current->key; }

    T *operator->() { return &(current->key); }

    Iterator &operator++() {
      if (nodes.empty()) {
        current = nullptr;
      } else {
        current = nodes.top();
        nodes.pop();
        pushLeft(current->right);
      }
      return *this;
    }

    bool operator==(const Iterator &other) const {
      return current == other.current;
    }

    bool operator!=(const Iterator &other) const {
      return current != other.current;
    }
  };

  Iterator begin() const { return Iterator(root); }

  Iterator end() const { return Iterator(nullptr); }
};
