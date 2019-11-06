#ifndef BIGINT_H
#define BIGINT_H

#include <cstdint>
#include <algorithm>

class BigInt
{
    bool sign;
    uint64_t *num;
    size_t size;
    size_t capacity;

public:
    BigInt(int base=0) : num(new uint64_t[1]), size(1), capacity(1)
    {
        if (base >= 0) {
            sign = false;
            num[0] = base;
        } else {
            sign = true;
            num[0] = -base;
        }
    }
    BigInt(int base=0, size_t size) : size(size), capacity(size)
    {
        sign = false;
        num = new uint64_t[size];
    }

    ~BigInt()
    {
        delete[] num;
    }

    BigInt operator +(const BigInt &add) const
    {
        size_t max_size = std::max(size, add.size);
        size_t min_size = std::min(size, add.size);

        res = BigInt(0, max_size + 1);
        uint64_t carryover = 0;
        
        if (sign == add.sign) {
            res.sign = sign;
            for (size_t i = 0; i < min_size; ++i) {
                res.num[i] = num[i] + add.num[i] + carryover;
                carryover = res.num[i] >> 4;
                res.num[i] &= 0xFFFFFFFF;
            }

            if (max_size == size) {
                for (size_t i = min_size; i < max_size; ++i) {
                    res.num[i] = num[i] + carryover;
                    carryover = res.num[i] >> 4;
                    res.num[i] &= 0xFFFFFFFF;
                }
            } else {
                for (size_t i = min_size; i < max_size; ++i) {
                    res.num[i] = add.num[i] + carryover;
                    carryover = res.num[i] >> 4;
                    res.num[i] &= 0xFFFFFFFF;
                }
            }
        } else {
            
        }
    }
    
};

#endif