#ifndef MATRIX_H
#define MATRIX_H

#include <stdexcept>

class _matrix_line
{
    int *_data;
    size_t len;
public:
    _matrix_line(int *_line, size_t cols) : _data(_line), len(cols) {}
    int &operator [](size_t col)
    {
        if (col >= len) {
            throw std::out_of_range("Out of range");
        }
        return _data[col];
    }

    const int &operator [](size_t col) const
    {
        if (col >= len) {
            throw std::out_of_range("Out of range");
        }
        return _data[col];
    }
};

class Matrix
{
    size_t rows, cols;
    int **_data;
public:
    Matrix(size_t rows, size_t cols) : rows(rows), cols(cols)
    {
        _data = new int*[rows];
        for (size_t i = 0; i < rows; ++i) {
            _data[i] = new int[cols];
        }
    }

    ~Matrix()
    {
        for (size_t i = 0; i < rows; ++i) {
            delete[] _data[i];
        }
        delete[] _data;
    }

    size_t getRows() const { return rows; }

    size_t getColumns() const { return cols; }

    Matrix& operator *=(int mul)
    {
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                _data[i][j] *= mul;
            }
        }
        return *this;
    }

    bool operator ==(const Matrix &cmp) const
    {
        if (rows != cmp.rows || cols != cmp.cols) {
            return false;
        }

        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                if (_data[i][j] != cmp._data[i][j]) {
                    return false;
                }
            }
        }

        return true;
    }

    bool operator !=(const Matrix &cmp) const
    {
        return !(*this == cmp);
    }

    _matrix_line operator [](size_t row)
    {
        if (row >= rows) {
            throw std::out_of_range("Out of range");
        }
        return _matrix_line(_data[row], cols);
    }

    const _matrix_line operator [](size_t row) const
    {
        if (row >= rows) {
            throw std::out_of_range("Out of range");
        }
        return _matrix_line(_data[row], cols);
    }

};

#endif