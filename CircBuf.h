//
#pragma once

#include <cstddef>
#include <string>

using std::string;

class CircBuf {
  const size_t CHUNK{8};
  // Insert your private stuff here
private:
  int write_index = 0, read_index = 0;
  char *internal_array = nullptr;
  size_t capacity_ = 0;

  void grow();

public:
  // constuctor sets capasity according to reserve size
  CircBuf(size_t reserve = 0) {
    if (reserve == 0) {
      capacity_ = 0;
    } else {
      // round capacity up to nearest multiple of chunk
      capacity_ = ((reserve + CHUNK - 1) / CHUNK) * CHUNK;
      internal_array = new char[capacity_];
    }
  }; // Number of elements you want it to be able to
     // hold to start with.
  ~CircBuf(){
    delete[] internal_array;
  };
  size_t size();
  size_t capacity();

  void insert(char);
  void insert(const char *, size_t sz);
  void insert(const string &);
  char get();
  string get(size_t);
  string flush(); // Returns a string with all the characters, AND shrinks the
                  // buffer to zero.
  string examine();
  void shrink(); // Reduces the unused space in the buffer.
};
