#include <iostream>

template <class T>
struct DefaultComparator {
    int operator() (const T& l, const T& r) const {
        if (l < r) return -1;
        if (l > r) return 1;
        return 0;
    }
};

template<class Key, class Value, class Comparator=DefaultComparator<Key>>
class AVLTree {
    struct Node {
        Node* left;
        Node* right;
        
        Key key;
        Value value;
        
        int height;
        
        Node(const Key& key, const Value& value)
          : left(nullptr), right(nullptr)
          , key(key), value(value)
          , height(1)
        {
        }
    };
public:
    AVLTree(Comparator comp = Comparator()) : root(nullptr), comp(comp), nodes_count(0) {
    }
    
    ~AVLTree();
    
    size_t size() const { return nodes_count; }
    
    Value* find(const Key& key) {
        return find_aux(root, key);
    }
    
    void insert(const Key& key, const Value& value) {
        root = insert_aux(root, key, value);
    }
    
    void erase(const Key& key) {
        root = erase_aux(root, key);
    }
private:
    Node* balance(Node* node) {
        fix_height(node);
        
        int bf = balance_factor(node);
        
        if (bf == 2) {
            if (balance_factor(node->right) < 0) {
                node->right = rotate_right(node->right);
            }
            return rotate_left(node);
        } else if (bf == -2) {
            if (balance_factor(node->left) > 0) {
                node->left = rotate_left(node->left);
            }
            return rotate_right(node);
        }
        return node;
    }
    
    int balance_factor(Node* node) {
        return height(node->right) - height(node->left);
    }
    
    void fix_height(Node* node) {
        node->height = std::max(height(node->left), height(node->right)) + 1;
    }
    
    int height(Node* node) {
        if (!node) {
            return 0;
        }
        return node->height;
    }
    
    Value* find_aux(Node* node, const Key& key) {
        if (!node) {
            return nullptr;
        }
        int comp_res = comp(key, node->key);
        if (comp_res == -1) {
            return find_aux(node->left, key);
        }
        if (comp_res == 1) {
            return find_aux(node->right, key);
        }
        return &node->value;
    }
    
    Node* insert_aux(Node* node, const Key& key, const Value& value) {
        if (!node) {
            nodes_count++;
            return new Node(key, value);
        }
        int comp_res = comp(key, node->key);
        if (comp_res == -1) {
            node->left = insert_aux(node->left, key, value);
        } else {
            node->right = insert_aux(node->right, key, value);
        }
        
        return balance(node);
    }
    
    Node* erase_aux(Node* node, const Key& key) {
        if (!node) {
            return nullptr;
        }
        
        int comp_res = comp(key, node->key);
        if (comp_res == -1) {
            node->left = erase_aux(node->left, key);
        } else if (comp_res == 1) {
            node->right = erase_aux(node->right, key);
        } else {
            nodes_count--;
            
            Node* left = node->left;
            Node* right = node->right;
            delete node;
            
            if (!right) {
                return left;
            }
            
            Node* min_node = find_min(right);
            min_node->right = remove_min(right);
            min_node->left = left;
            
            return balance(min_node);
        }
        return balance(node);
    }
    
    Node* find_min(Node* node) {
        if (!node->left) {
            return node;
        }
        return find_min(node->left);
    }
    
    Node* remove_min(Node* node) {
        if (!node->left) {
            return node->right;
        }
        node->left = remove_min(node->left);
        return balance(node);
    }
    
    Node* rotate_right(Node* node);
    Node* rotate_left(Node* node);
    
    Node* root;
    Comparator comp;
    size_t nodes_count;
};


int main(int argc, const char * argv[]) {
    AVLTree<int, int> avl;
    assert(avl.size() == 0);
    
    for (int i = 0; i < 10; ++i) {
        avl.insert(i, i);
    }
    std::cout << "size: " << avl.size() << std::endl;
    
    assert(avl.size() == 10);
    assert(avl.find(-1) == nullptr);
    assert(avl.find(10) == nullptr);
    
    for (int i = 0; i < 10; ++i) {
        assert(*avl.find(i) == i);
    }
    
    for (int i = 0; i < 5; ++i) {
        avl.erase(i);
    }
    std::cout << "size: " << avl.size() << std::endl;
    assert(avl.size() == 5);
    for (int i = 0; i < 5; ++i) {
        assert(avl.find(i) == nullptr);
    }
    for (int i = 5; i < 10; ++i) {
        assert(*avl.find(i) == i);
    }
    
    return 0;
}
