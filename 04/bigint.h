#ifndef BIGINT_H
#define BIGINT_H

#include <iostream>
#include <iomanip>
#include <cstdint>
#include <algorithm>

enum { BASE = 1000000000 };

class BigInt
{
    bool sign;
    int64_t *num;
    size_t size;

    void shrink_to_fit(BigInt &shr) const
    {
        size_t nulls = 0;
        while (shr.size - 1 - nulls > 0 && !shr.num[shr.size - 1 - nulls]) {
            ++nulls;
        }
        
        shr.size -= nulls;
        int64_t *buf = new int64_t[shr.size];
        for (size_t i = 0; i < shr.size; ++i) {
            buf[i] = shr.num[i];
        }

        delete[] shr.num;
        shr.num = buf;

        // fix -0
        if (shr.size == 1 && shr.num[0] == 0) {
            shr.sign = false;
        }
    }

public:
    BigInt(int base=0) : num(new int64_t[2]), size(2)
    {
        if (base >= 0) {
            sign = false;
            num[0] = base % BASE;
            num[1] = base / BASE;
        } else {
            sign = true;
            num[0] = -(base % BASE);
            num[1] = -(base / BASE);
        }
        shrink_to_fit(*this);
    }

    BigInt(int base, size_t size) : size(size)
    {
        sign = false;
        num = new int64_t[size];
        for (size_t i = 0; i < size; ++i) {
            num[i] = 0;
        }
    }

    BigInt(const BigInt &copy) : sign(copy.sign), size(copy.size)
    {
        num = new int64_t[size];
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

    bool operator ==(const BigInt &cmp) const
    {
        if (sign == cmp.sign && size == cmp.size) {
            for (size_t i = 0; i < size; ++i) {
                if (num[i] != cmp.num[i]) {
                    return false;
                }
            }
            return true;
        } else {
            return false;
        }
    }

    bool operator !=(const BigInt &cmp) const
    {
        return !(*this == cmp);
    }

    bool operator <(const BigInt &cmp) const
    {
        if (sign && !cmp.sign) {
            return true;
        } else if (!sign && cmp.sign) {
            return false;
        }
        if (size < cmp.size) {
            return !sign;
        } else if (size > cmp.size) {
            return sign;
        }
        for (size_t i = 0; i < size; ++i) {
            int64_t dif = num[size - i - 1] - cmp.num[size - i - 1];
            if (dif < 0) {
                return !sign;
            } else if (dif > 0) {
                return sign;
            }
        }
        return false;
    }

    bool operator >(const BigInt &cmp) const
    {
        return !(*this == cmp || *this < cmp);
    }

    bool operator >=(const BigInt &cmp) const
    {
        return (*this == cmp || *this > cmp);
    }

    bool operator <=(const BigInt &cmp) const
    {
        return (*this == cmp || *this < cmp);
    }

    BigInt abs() const
    {
        BigInt res(*this);
        res.sign = false;
        return res;
    }

    BigInt operator +(const BigInt &add) const
    {
        size_t max_size = std::max(size, add.size);
        size_t min_size = std::min(size, add.size);

        BigInt res(0, max_size + 1);
        int64_t carryover = 0;
        
        if (sign == add.sign) {
            res.sign = sign;
            for (size_t i = 0; i < min_size; ++i) {
                res.num[i] = num[i] + add.num[i] + carryover;
                carryover = res.num[i] / BASE;
                res.num[i] %= BASE;
            }

            if (max_size == size) {
                for (size_t i = min_size; i < max_size; ++i) {
                    res.num[i] = num[i] + carryover;
                    carryover = res.num[i] / BASE;
                    res.num[i] %= BASE;
                }
            } else {
                for (size_t i = min_size; i < max_size; ++i) {
                    res.num[i] = add.num[i] + carryover;
                    carryover = res.num[i] / BASE;
                    res.num[i] %= BASE;
                }
            }
            shrink_to_fit(res);
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

        if (*this < sub && !(sign && sub.sign)) {
            return -(sub - *this);
        }

        BigInt res(0, max_size);
        int64_t carryover = 0;

        if (sign == sub.sign) {
            res.sign = sign;
            for (size_t i = 0; i < min_size; ++i) {
                res.num[i] = num[i] - sub.num[i] - carryover;
                carryover = res.num[i] < 0;
                res.num[i] += BASE;
                res.num[i] %= BASE;
            }

            if (max_size == size) {
                for (size_t i = min_size; i < max_size; ++i) {
                    res.num[i] = num[i] - carryover;
                    carryover = res.num[i] < 0;
                    res.num[i] += BASE;
                    res.num[i] %= BASE;
                }
            } else {
                for (size_t i = min_size; i < max_size; ++i) {
                    res.num[i] = sub.num[i] - carryover;
                    carryover = res.num[i] < 0;
                    res.num[i] += BASE;
                    res.num[i] %= BASE;
                }
            }
            shrink_to_fit(res);
            return res;
        } else {
            return *this + (-sub);
        }
    }

    friend std::ostream& operator <<(std::ostream& os, const BigInt &out) {
        if (out.sign) {
            os << '-';
        }
        os << out.num[out.size - 1];
        for (ssize_t i = out.size - 2; i >= 0; --i) {
            os << std::setw(9) << std::setfill('0') << out.num[i];
        }
        return os;
    }
    
};

#endif