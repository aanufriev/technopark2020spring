#include <iostream>
#include <istream>
#include <ostream>
#include <string>
#include <sstream>
#include <assert.h>

template <class T>
class Queue {
    struct Node {
        T value;
        Node* next;
        
        Node(const T& val) : value(val), next(nullptr)  {
        }
    };
public:
    Queue() : head(nullptr), tail(nullptr) {
    }
    
    Queue(const Queue&) = delete;
    Queue(Queue&&) = delete;
    
    Queue& operator= (const Queue&) = delete;
    Queue& operator= (Queue&&) = delete;
    
    ~Queue() {
        while (!is_empty()) {
            pop();
        }
    }
    
    void push(const T& value) {
        Node* node = new Node(value);
        if (is_empty()) {
            tail = head = node;
            return;
        }
        tail->next = node;
        tail = tail->next;
    }
    
    T pop() {
        T value = head->value;
        
        Node* node = head;
        head = head->next;
        delete node;
        
        if (head == nullptr) {
            tail = nullptr;
        }
        
        return value;
    }
    
    bool is_empty() const {
        return head == tail && tail == nullptr;
    }
private:
    Node* head;
    Node* tail;
};

void testQueue() {
    {
        Queue<int> q;
        assert(q.is_empty());
    }
    {
        Queue<int> q;
        for (int i = 0; i < 1000; ++i) {
            q.push(i);
        }
        for (int i = 0; i < 1000; ++i) {
            assert(q.pop() == i);
        }
        assert(q.is_empty());
    }
    {
        Queue<int> q;
        for (int i = 0; i < 1000; ++i) {
            q.push(i);
            assert(q.pop() == i);
            assert(q.is_empty());
        }
    }
    {
        Queue<std::string> q;
        q.push("test");
        assert(q.pop() == "test");
        assert(q.is_empty());
    }
}

int run(std::istream& in, std::ostream& out)
{
    Queue<int> queue;
    int commands_count = 0;
    in >> commands_count;
    for (int i = 0; i < commands_count; ++i) {
        int command = 0;
        int value = 0;
        in >> command >> value;
        int q_value = -1;
        switch (command) {
            case 2: // pop
                if (!queue.is_empty()) {
                    q_value = queue.pop();
                }
                if (q_value != value) {
                    out << "NO" << std::endl;
                    return 0;
                }
                break;
            case 3: // push
                queue.push(value);
                break;
            default:
                out << "NO" << std::endl;
                return 0;
        }
    }
    out << "YES" << std::endl;
    return 0;
}

void testLogic() {
    {
        std::stringstream ss_in;
        ss_in << "3" << std::endl;
        ss_in << "3 44" << std::endl;
        ss_in << "3 50" << std::endl;
        ss_in << "2 44" << std::endl;
        
        std::stringstream ss_out;
        assert(run(ss_in, ss_out) == 0);
        
        //for (char& c : ss_out.str()) {
        //    std::cout << std::hex << (int)c << std::endl;
        //}
        
        assert(ss_out.str() == "YES\n");
    }
    {
        std::stringstream ss_in;
        ss_in << "2" << std::endl;
        ss_in << "2 -1" << std::endl;
        ss_in << "3 10" << std::endl;
        
        std::stringstream ss_out;
        assert(run(ss_in, ss_out) == 0);
        assert(ss_out.str() == "YES\n");
    }
    {
        std::stringstream ss_in;
        ss_in << "2" << std::endl;
        ss_in << "2 44" << std::endl;
        ss_in << "3 66" << std::endl;
        
        std::stringstream ss_out;
        assert(run(ss_in, ss_out) == 0);
        assert(ss_out.str() == "NO\n");
    }
    
    {
        int count = 1000;
        std::stringstream ss_in;
        ss_in << count * 2 + 1 << std::endl;
        for (int i = 0; i < count; ++i)
            ss_in << "3 " << i << std::endl;
        for (int i = 0; i < count; ++i)
            ss_in << "2 " << i << std::endl;
        ss_in << "2 -1" << std::endl;
        
        std::stringstream ss_out;
        assert(run(ss_in, ss_out) == 0);
        assert(ss_out.str() == "YES\n");
    }
}

int main(int argc, const char * argv[]) {
    //testQueue();
    //testLogic();
    //std::cout << "tests OK" << std::endl;
    run(std::cin, std::cout);
    return 0;
}
