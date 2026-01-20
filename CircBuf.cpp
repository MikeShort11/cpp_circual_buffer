#include "CircBuf.h"
#include <cstddef>

//TODO: rix resizing

size_t CircBuf::capacity() { return capacity_; }

size_t CircBuf::size() {
  return size_;
}

void CircBuf::grow() {
  if (!internal_array) {
    // allocate array if it doesnt exist
    internal_array = new char[CHUNK];
    capacity_ = CHUNK;
    read_index = 0; // they should be 0 already, but just in case
    write_index = 0;
  }

  else {
    char *temp = new char[capacity_ + CHUNK];

    for (int i = 0; i < size_; i++) {
      temp[i] = internal_array[(read_index + i) % capacity_];
    }

    write_index = size_;
    read_index = 0;
    capacity_ += CHUNK;
    delete[] internal_array;
    internal_array = temp;
    // I think temp (the pointer, not the array) will go away once out of scope?
  }
}

void CircBuf::shrink() { // todo: make sure capacity is updated correctly
  size_t new_size = ((size_ + CHUNK - 1) / CHUNK) * CHUNK;

  char *temp = new char[new_size];

  for (int i = 0; i < size_; i++) {
    temp[i] = internal_array[read_index];
    read_index = (read_index + 1) % capacity_;
  }
  capacity_ = new_size;
  write_index = size_;
  read_index = 0;
  delete[] internal_array;
  internal_array = temp;
}

void CircBuf::insert(char c) {
  // allocate space if needed
  if (size() >= capacity_) {
    grow();
  }
  internal_array[write_index] = c;
  write_index = (write_index + 1) % capacity_;
  size_++;
}

void CircBuf::insert(const char *c, size_t sz) {
  while (size_ + sz > capacity_) {
    grow();
  }
  for (int i = 0; i < sz; i++) {
    internal_array[write_index] = c[i];
    write_index = (write_index + 1) % capacity_;
  }
  size_ += sz;
}

void CircBuf::insert(const string &s) {
  while (size_ + s.length() > capacity_) {
    grow();
  }
  for (size_t i=0; i < s.length(); i++) {
    internal_array[write_index] = s[i];
    write_index = (write_index + 1) % capacity_;
  }
  size_ += s.length();
}

char CircBuf::get() {
  char c = internal_array[read_index];
  read_index = (read_index + 1) % capacity_;
  size_--;
  return c;
}

string CircBuf::get(size_t sz) {
  string s;
  // dont return empty characters if sz is too big
  if (sz > size_) {
    sz = size_;
  }
  for (size_t i = 0; i < sz; i++) {
    s += internal_array[read_index];
    read_index = (read_index + 1) % capacity_;
    size_--;
  }
  return s;
}

string CircBuf::examine() {
    string s = "[";
    for (size_t i = 0; i < capacity_; i++) {
        bool filled = false;
        if (size_ == 0) filled = false;
        else if (read_index < write_index) {
            filled = (i >= read_index && i < write_index);
        } else if (read_index > write_index) {
            filled = (i >= read_index || i < write_index);
        } else {
            filled = (size_ > 0);
        }
        s += filled ? internal_array[i] : '-';
    }
    s += "]";
    return s;
}

string CircBuf::flush() {
  string s;
  size_t local_size = size_;
  for (int i = 0; i < local_size; i++) {
    s += get();
  }
  // reset buffer
  delete[] internal_array;
  internal_array = nullptr;
  capacity_ = 0;
  write_index = 0;
  read_index = 0;
  size_ = 0;
  return s;
}

