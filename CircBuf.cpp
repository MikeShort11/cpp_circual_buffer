#include "CircBuf.h"
#include <cstddef>

size_t CircBuf::capacity() { return capacity_; }

size_t CircBuf::size() {
  if (capacity_ == 0)
    return 0;
  if (write_index >= read_index) {
    return write_index - read_index;
  } else {
    return capacity_ - (read_index - write_index);
  }
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
    size_t local_size = size(); // local copy needed to reassign write index
    capacity_ += CHUNK;
    char *temp = new char[capacity_];

    for (int i = 0; i < local_size; i++) {
      temp[i] = get();
    }

    write_index = local_size;
    read_index = 0;
    delete[] internal_array;
    internal_array = temp;
    // I think temp (the pointer, not the array) will go away once out of scope?
  }
}

void CircBuf::shrink() { // todo: make sure capacity is updated correctly
  // check that there is enough space to remove some
  if (capacity_ - CHUNK <= size()) {
    return;
  }

  size_t local_size = size(); // local copy needed to reassign write index
  capacity_ -= CHUNK;
  char *temp = new char[capacity_];

  for (int i = 0; i < local_size; i++) {
    temp[i] = get();
  }

  write_index = local_size;
  read_index = 0;
  delete[] internal_array;
  internal_array = temp;
  // I think temp (the pointer, not the array) will go away once out of scope?
}

void CircBuf::insert(char c) {
  // allocate space if needed
  if (size() >= capacity_) {
    grow();
  }
  internal_array[write_index] = c;
  write_index = (write_index + 1) % capacity_;
}

void CircBuf::insert(const char *c, size_t sz) {
  while (size() + sz >= capacity_) {
    grow();
  }
  for (int i = 0; i < sz; i++) {
    internal_array[(write_index + i) % capacity_] = c[i];
  }
  write_index = (write_index + sz) % capacity_;
}

void CircBuf::insert(const string &s) {
  while (size() + s.length() >= capacity_) {
    grow();
  }
  for (int i=0; i < s.length(); i++) {
    internal_array[(write_index + i) % capacity_] = s[i];
  }
  write_index = (write_index + s.length()) % capacity_;
}

char CircBuf::get() {
  char c = internal_array[read_index];
  read_index = (read_index + 1) % capacity_;
  return c;
}

string CircBuf::get(size_t sz) {
  string s;
  for (int i = 0; i < sz; i++) {
    s += internal_array[read_index];
    read_index = (read_index + 1) % capacity_;
  }
  return s;
}

string CircBuf::examine() { //TODO: reimplement function to properly iterate
  string s = "[";
  size_t temp_read_index = 0;
  if (read_index <= write_index) {
    for (int i=0; i < capacity_; i++) {
      if (i >= read_index && i < write_index) {
        s += internal_array[i];
      } else {
        s += '-';
      }
    }
  }
  else if (write_index > read_index)
  {
    for (int i=0; i < capacity_; i++) {
      if (i >= read_index && i < write_index) {
        s += '-';
      }
      else {
        s += internal_array[i];
      }
    }
  }
  s += ']';
  return s;
}

string CircBuf::flush() {
  string s;
  size_t local_size = size();
  for (int i = 0; i < local_size; i++) {
    s += get();
  }
  // reset buffer
  delete[] internal_array;
  internal_array = nullptr;
  capacity_ = 0;
  write_index = 0;
  read_index = 0;
  return s;
}

