
#include <iostream>
#include <vector>

typedef unsigned char byte;

struct IInputStream {
    // Возвращает false, если поток закончился
    virtual bool Read(byte& value) = 0;
};

struct IOutputStream {
    virtual void Write(byte value) = 0;
};

class VectorInput : public IInputStream {
public:
    VectorInput(std::vector<byte>* vec) : vec_ptr(vec), pos(0) {
    }
    
    virtual bool Read(byte& value) override {
        if (pos >= vec_ptr->size()) {
            return false;
        }
        value = (*vec_ptr)[pos++];
        return true;
    }
private:
    std::vector<byte>* vec_ptr;
    int pos;
};

class VectorOutput : public IOutputStream {
public:
    VectorOutput(std::vector<byte>* vec) : vec_ptr(vec) {
    }
    
    virtual void Write(byte value) override {
        vec_ptr->push_back(value);
    }

private:
    std::vector<byte>* vec_ptr;
};


class BitInputStream {
public:
    BitInputStream(IInputStream& input) : input_stream(input), bit_pos(0) {
    }
    
    bool read_bit(byte& res) {
        if (bit_pos == 0) {
            bool res = input_stream.Read(buf);
            if (!res) {
                return false;
            }
        }
        res = (buf >> (bit_pos++)) & 1;
        if (bit_pos == 8) {
            bit_pos = 0;
        }
        return true;
    }
    
    //byte read_byte();
private:
    IInputStream& input_stream;
    
    byte buf;
    int bit_pos;
};

class BitOutputStream {
public:
    BitOutputStream(IOutputStream& output) : output_stream(output), buf(0), bit_pos(0) {
    }
    
    void write_bit(byte bit) {
        if (bit_pos == 8) {
            flush();
        }
        buf = buf | ((bit & 1) << (bit_pos++));
    }
    
    void flush() {
        output_stream.Write(buf);
        buf = 0;
        bit_pos = 0;
    }
private:
    IOutputStream& output_stream;
    byte buf;
    int bit_pos;
};





// Метод архивирует данные из потока original
//void Encode(IInputStream& original, IOutputStream& compressed);
// Метод восстанавливает оригинальные данные
//void Decode(IInputStream& compressed, IOutputStream& original);



int main(int argc, const char * argv[]) {
    std::vector<byte> vec;
    
    VectorOutput vec_output(&vec);
    BitOutputStream bit_output(vec_output);
    
    bit_output.write_bit(1);
    bit_output.write_bit(0);
    bit_output.write_bit(1);
    bit_output.write_bit(0);
    bit_output.write_bit(1);
    bit_output.write_bit(0);
    bit_output.write_bit(1);
    bit_output.write_bit(0);
    
    bit_output.write_bit(1);
    bit_output.write_bit(0);
    bit_output.flush();
    
    for (int i =0; i < vec.size(); ++i) {
        std::cout << std::hex << (int)vec[i] << std::endl;
    }
    //0101 0101
    //0000 0001
    
    
    VectorInput vec_input(&vec);
    BitInputStream bit_input(vec_input);
    
    for (int i = 0; i < 10; i++) {
        byte res = 0;
        bit_input.read_bit(res);
        std::cout << (int) res << " ";
    }
    
    return 0;
}
