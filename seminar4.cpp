#include <iostream>
#include <vector>
#include <string>
#include <assert.h>


size_t str_hash(const std::string& data);


template <class T> struct HashFunc;

template<> struct HashFunc<int> {
    size_t operator() (const int& key) { return key; }
};

template<> struct HashFunc<std::string> {
    size_t operator() (const std::string& key) { return str_hash(key); }
};

struct NewHash {
    size_t operator() (const std::string& key) { return str_hash(key); }
};

template <class T> struct EqualsFunc {
    bool operator() (const T& l, const T& r) const { return l == r; }
};


static constexpr size_t BUCKETS_COUNT[] = {7, 17, 37, 73, 149, 251, 509, 1021, 2027, 5003, 10837};

template <class Key, class Value, class Hash = HashFunc<Key>, class Equals = EqualsFunc<Key>>
class HashTable {
    static constexpr size_t max_load_factor = 7;
    
    struct Node {
        Key key;
        Value value;
        Node* next;
        
        Node(const Key& key, const Value& value, Node* next)
            : key(key)
            , value(value)
            , next(next) {
        }
    };
public:
    HashTable(Hash hash = Hash(), Equals equals = Equals())
        : buckets(nullptr)
        , buckets_count(0)
        , items_count(0)
        , size_idx(0)
        , hash(hash)
        , equals(equals) {
    }
    
    ~HashTable() {
        for (int i = 0; i < buckets_count; ++i) {
            Node* cur = buckets[i];
            while (cur) {
                Node* tmp = cur;
                cur = cur->next;
                delete tmp;
            }
        }
        delete[] buckets;
    }
    
    Value* find(const Key& key) {
        if (!buckets) {
            return nullptr;
        }
        
        size_t bucket_idx = hash(key) % buckets_count;
        Node* cur = buckets[bucket_idx];
        while (cur) {
            if (equals(cur->key, key)) {
                return &cur->value;
            }
            cur = cur->next;
        }
        
        return nullptr;
    }
    
    bool insert(const Key& key, const Value& value) {
        if (!buckets || items_count > max_load_factor * buckets_count) {
            grow();
        }
        
        if (find(key)) {
            return false;
        }
        
        size_t bucket_idx = hash(key) % buckets_count;
        buckets[bucket_idx] = new Node(key, value, buckets[bucket_idx]);
        items_count++;

        return true;
    }
    
    bool erase(const Key& key) {
        if (!buckets) {
            return false;
        }
        
        size_t bucket_idx = hash(key) % buckets_count;
        Node* cur = buckets[bucket_idx];
        Node* prev = nullptr;
        while (cur) {
            if (equals(cur->key , key)) {
                if (prev) {
                    prev->next = cur->next;
                } else {
                    buckets[bucket_idx] = cur->next;
                }
                delete cur;
                return true;
            }
            prev = cur;
            cur = cur->next;
        }
        return false;
        
    }
    
private:
    void grow() {
        Node** old_bucket = buckets;
        size_t old_buckets_count = buckets_count;
        
        buckets_count = BUCKETS_COUNT[size_idx++];
        buckets = new Node*[buckets_count];
        for (int i = 0; i < buckets_count; ++i) {
            buckets[i] = nullptr;
        }
        
        for (int old_bucket_idx = 0; old_bucket_idx < old_buckets_count; ++old_bucket_idx) {
            Node* cur = old_bucket[old_bucket_idx];
            while (cur) {
                Node* tmp = cur;
                cur = cur->next;
                
                size_t bucket_idx = hash(tmp->key) % buckets_count;
                tmp->next = buckets[bucket_idx];
                buckets[bucket_idx] = tmp;
            }
        }
        
        delete[] old_bucket;
        
    }
    
    Node** buckets;
    size_t buckets_count;
    size_t items_count;
    
    size_t size_idx;
    
    Hash hash;
    Equals equals;
};

int main(int argc, const char * argv[]) {
    char operation = '\0';
    std::string key = "";
    
    HashTable<std::string, std::string> hash;
    while (std::cin >> operation >> key) {
        bool res = false;
        switch (operation) {
            case '+':
                res = hash.insert(key, key);
                break;
            case '-':
                res = hash.erase(key);
                break;
            case '?':
                res = (hash.find(key) != nullptr);
                break;
            default:
                break;
        }
        if (res) {
            std::cout << "OK" << std::endl;
        } else {
            std::cout << "FAIL" << std::endl;
        }
    }
    
    return 0;
}
