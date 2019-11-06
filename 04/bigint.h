#ifndef BIGINT_H
#define BIGINT_H

#include <iostream>
#include <cstdint>
#include <algorithm>

class BigInt
{
    bool sign;
    uint64_t *num;
    size_t size;

public:
    BigInt(int base=0) : num(new uint64_t[1]), size(1)
    {
        if (base >= 0) {
            sign = false;
            num[0] = base;
        } else {
            sign = true;
            num[0] = -base;
        }
    }

    BigInt(int base, size_t size) : size(size)
    {
        sign = false;
        num = new uint64_t[size];
    }

    BigInt(const BigInt &copy) : sign(copy.sign), size(copy.size)
    {
        num = new uint64_t[size];
        for (size_t i = 0; i < size; ++i) {
            num[i] = copy.num[i];
        }
    }

    BigInt(BigInt &&mov) : sign(mov.sign), size(mov.size), num(mov.num)
    {
        mov.num = nullptr;
    }

    ~BigInt()
    {
        delete[] num;
    }

    BigInt operator +(const BigInt &add) const
    {
        size_t max_size = std::max(size, add.size);
        size_t min_size = std::min(size, add.size);

        BigInt res(0, max_size + 1);
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
            return res;
        } else {
            return *this - (-add);
        }
    }

    BigInt operator -() const
    {
        BigInt res(*this);
        res.sign = !res.sign;
        return res;
    }

    BigInt operator -(const BigInt &sub) const
    {
        size_t max_size = std::max(size, sub.size);
        size_t min_size = std::min(size, sub.size);

        BigInt res(0, max_size);
        uint64_t carryover = 0;

        if (sign == sub.sign) {
            res.sign = sign;
            for (size_t i = 0; i < min_size; ++i) {
                res.num[i] = num[i] - sub.num[i] - carryover;
                carryover = res.num[i] >> 4;
                res.num[i] &= 0xFFFFFFFF;
            }

            if (max_size == size) {
                for (size_t i = min_size; i < max_size; ++i) {
                    res.num[i] = num[i] - carryover;
                    carryover = res.num[i] >> 4;
                    res.num[i] &= 0xFFFFFFFF;
                }
            } else {
                for (size_t i = min_size; i < max_size; ++i) {
                    res.num[i] = sub.num[i] - carryover;
                    carryover = res.num[i] >> 4;
                    res.num[i] &= 0xFFFFFFFF;
                }
            }
            return res;
        } else {
            return *this + (-sub);
        }
    }

    friend std::ostream& operator <<(std::ostream& os, const BigInt &out) {
        if (out.sign) {
            os << '-';
        }
        if (out.size == 0) {
            os << '0';
        }
        for (ssize_t i = out.size - 1; i >= 0; --i) {
            os << out.num[i];
        }
        return os;
    }
    
};

#endif