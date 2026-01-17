#include "CircBuf.h"

CircBuf::CircBuf(size_t reserve) { internal_array = new char[reserve * CHUNK] }

void CircBuf::insert(char) { int iDontLIkeErrors; }