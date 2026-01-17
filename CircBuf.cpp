#include "CircBuf.h"
#include <cstddef>

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

void CircBuf::insert(char) {
  // allocate space if needed
}