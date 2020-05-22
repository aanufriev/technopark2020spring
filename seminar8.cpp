
#include <iostream>
#include <array>
#include <map>
#include <queue>
#include <vector>

static const int LINE_SIZE = 4;
static const int FIELD_SIZE = LINE_SIZE * LINE_SIZE;

using FieldArray = std::array<uint8_t, FIELD_SIZE>;

static const FieldArray EMPTY_FIELD = {
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0,
};

static const FieldArray GOAL_FIELD = {
    1, 2, 3, 4,
    5, 6, 7, 8,
    9, 10, 11, 12,
    13,14,15,0
    
};


class FieldState {
public:
    FieldState() {
        init(EMPTY_FIELD);
    }
    
    FieldState(const FieldArray& arr) {
        init(arr);
    }
    
    FieldState& operator= (const FieldState& r) = default;
    
    bool operator== (const FieldState& r) const {
        bool res = field == r.field;
        if (res) {
            assert( zero_pos == r.zero_pos);
        }
        return res;
    }
    
    bool operator< (const FieldState& r) const {
        return field < r.field;
    }
    
    FieldState(const FieldState&) = default;
    
    ~FieldState() = default;
    
    std::vector<FieldState> GetNextVertices() const {
        std::vector<FieldState> res;
        if (zero_pos >= LINE_SIZE) {
            FieldState st(*this);
            st.moveUp();
            res.push_back(st);
        }
        
        if (zero_pos < FIELD_SIZE - LINE_SIZE) {
            FieldState st(*this);
            st.moveDown();
            res.push_back(st);
        }
        
        if (zero_pos % LINE_SIZE != 0) {
            FieldState st(*this);
            st.moveLeft();
            res.push_back(st);
        }
        
        if ((zero_pos + 1) % LINE_SIZE != 0) {
            FieldState st(*this);
            st.moveRight();
            res.push_back(st);
        }
        
        return res;
    }
    
    void print() const {
        for (int i = 0; i < field.size(); ++i) {
            std::cout << (int)field[i] << " ";
            if ((i + 1) % LINE_SIZE == 0) {
                std::cout << std::endl;
            }
        }
    }
    
private:
    void moveUp() {
        uint8_t old_zero_pos = zero_pos;
        zero_pos -= LINE_SIZE;
        std::swap(field[zero_pos], field[old_zero_pos]);
    }
    
    void moveDown() {
        uint8_t old_zero_pos = zero_pos;
        zero_pos += LINE_SIZE;
        std::swap(field[zero_pos], field[old_zero_pos]);
    }
    
    void moveLeft() {
        uint8_t old_zero_pos = zero_pos;
        zero_pos -= 1;
        std::swap(field[zero_pos], field[old_zero_pos]);
    }
    
    void moveRight() {
        uint8_t old_zero_pos = zero_pos;
        zero_pos += 1;
        std::swap(field[zero_pos], field[old_zero_pos]);
    }
    
    void init(const FieldArray& arr) {
        for (int i = 0; i < arr.size(); ++i) {
            field[i] = arr[i];
            if (arr[i] == 0) {
                zero_pos = i;
            }
        }
    }
    FieldArray field;
    uint8_t zero_pos;
};

void bfs(const FieldState& start) {
    const FieldState empty_state(EMPTY_FIELD);
    const FieldState goal_state(GOAL_FIELD);
    
    std::queue<FieldState> queue;
    // key - посещенная вершина
    // value - parent
    std::map<FieldState, FieldState> used;
    bool found = false;
    FieldState cur_state = empty_state;
    
    used.insert(std::make_pair(start, empty_state));
    queue.push(start);
    while (!queue.empty()) {
        cur_state = queue.front();
        queue.pop();
        if (cur_state == goal_state) {
            found = true;
            break;
        }
        
        for (FieldState child : cur_state.GetNextVertices()) {
            if (used.find(child) == used.end()) {
                used.insert(std::make_pair(child, cur_state));
                queue.push(child);
            }
        }
    }
    
    int steps_count = 0;
    if (found) {
        while (!(cur_state == empty_state)) {
            cur_state.print();
            steps_count++;
            std::cout << "--------" << std::endl;
            cur_state = used.find(cur_state)->second;
        }
        std::cout << steps_count << std::endl;
    } else {
        std::cout << "Not found, steps count:" << used.size() << std::endl;
        
    }
    

}

int main(int argc, const char * argv[]) {
    FieldState start({
        1,  2,  3,  4,
        5,  6,  7,  8,
        9,  10, 11, 12,
        13, 14, 15, 0,

    });
    bfs(start);
    return 0;
}
