#pragma once
#include <ostream>

class Matrix
{
private:
    size_t m_rows;
    size_t m_cols;
    int *m_start;
    class RowProxy
    {
    private:
        int *m_pos;
        size_t m_length;
    public:
        RowProxy(int *i, int cols);
        int& operator[](int j);
        const int& operator[](int j) const;
    };
public:
    Matrix(int rows, int cols);
    Matrix(const Matrix &m);
    size_t getRows() const;
    size_t getColumns() const;
    RowProxy operator[](int i) const;
    friend Matrix operator*(const Matrix &m, int n);
    friend Matrix operator*(int n, const Matrix &m);
    Matrix operator*=(int n);
    friend std::ostream& operator<<(std::ostream &out, const Matrix &m);
    friend Matrix operator+(const Matrix &m1, const Matrix &m2);
    friend bool operator==(const Matrix &m1, const Matrix &m2);
    friend bool operator!=(const Matrix &m1, const Matrix &m2);
    ~Matrix();
};

void matrix_wrong_init();
void matrix_cols();
void matrix_rows();
void matrix_el();
void matrix_output();
void matrix_comp();
void matrix_sum();
void matrix_mult();
