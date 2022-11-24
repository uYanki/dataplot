#ifndef BIT_H
#define BIT_H

#define BIT_GET(x, n) ((x >> n) & 1)
#define BIT_SET(x, n) (x | (1 << n))
#define BIT_SETV(x, n, v) (x | (v << n))

template <typename T = unsigned int>
inline int BIT_GETN(int value, int pos, int count) {
    int offset = sizeof(T) * 8  - (count + pos);
    return T(value << offset) >> (offset + pos);
}

#endif  // BIT_H
