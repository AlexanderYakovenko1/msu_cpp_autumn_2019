#ifndef ALLOC_H
#define ALLOC_H

class LinearAllocator
{
    char *start, *curpos;
    size_t length;
public:
    LinearAllocator(size_t maxSize) : start(nullptr), curpos(nullptr), length(maxSize)
    {
        curpos = start = new char[length];
    }

    ~LinearAllocator()
    {
        delete[] start;
    }
    
    char *alloc(size_t size)
    {
        char *ret_val = nullptr;
        if (curpos + size - start <= length) {
            ret_val = curpos;
            curpos += size;
        }
        return ret_val;
    }
    
    void reset()
    {
        curpos = start;
    }
};

#endif