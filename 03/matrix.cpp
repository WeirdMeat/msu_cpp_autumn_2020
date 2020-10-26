#include "matrix.h"
#include <ostream>
#include <stdexcept>
#include <cassert>
#include <sstream>

Matrix::Matrix(int rows, int cols)
{
    if (rows <= 0 || cols <= 0) {
        throw std::out_of_range("");
    }
    this->m_rows = rows;
    this->m_cols = cols;
    this->m_start = new int[rows * cols];
}
Matrix::Matrix(const Matrix &m): m_rows(m.m_rows), m_cols(m.m_cols)
{
    this->m_start = new int[m.m_rows * m.m_cols];
    for (size_t i = 0; i < m.m_rows; i++) {
        for (size_t j = 0; j < m.m_cols; j++) {
            (*this)[i][j] = m[i][j];
        }
    }
}
size_t Matrix::getRows() const
{
    return this->m_rows;
}
size_t Matrix::getColumns() const
{
    return this->m_cols;
}
Matrix::RowProxy Matrix::operator[](int i) const
{
    if (i < 0 || i >= this->m_rows) {
        throw std::out_of_range("");
    }
    return RowProxy(this->m_start + i * this->m_rows, this->m_cols);
}
Matrix::RowProxy::RowProxy(int *i, int cols)
{
    this->m_pos = i;
    this->m_length = cols;
}
int& Matrix::RowProxy::operator[](int j)
{
    if (j < 0 || j >= this->m_length) {
        throw std::out_of_range("");
    }
    return *(this->m_pos + j);
}
const int& Matrix::RowProxy::operator[](int j) const
{
    if (j <= 0 || j >= this->m_length) {
        throw std::out_of_range("");
    }
    return *(this->m_pos + j);
}
Matrix operator*(const Matrix &m, int n)
{
    Matrix m_new(m);
    for (size_t i = 0; i < m.m_rows; i++) {
        for (size_t j = 0; j < m.m_cols; j++) {
            m_new[i][j] = m_new[i][j] * n;
        }
    }
    return m_new;
}
Matrix operator*(int n, const Matrix &m)
{
    Matrix m_new(m);
    for (size_t i = 0; i < m.m_rows; i++) {
        for (size_t j = 0; j < m.m_cols; j++) {
            m_new[i][j] = m_new[i][j] * n;
        }
    }
    return m_new;
}
Matrix Matrix::operator*=(int n)
{
    for (size_t i = 0; i < this->m_rows; i++) {
        for (size_t j = 0; j < this->m_cols; j++) {
            (*this)[i][j] = (*this)[i][j] * n;
        }
    }
    return *this;
}
std::ostream& operator<<(std::ostream &out, const Matrix &m)
{
    for (size_t i = 0; i < m.m_rows; i++) {
        for (size_t j = 0; j < m.m_cols; j++) {
            out << m[i][j] << " ";
        }
        out << std::endl;
    }
    return out;
}
Matrix operator+(const Matrix &m1, const Matrix &m2)
{
    if ((m1.m_rows != m2.m_rows) && (m1.m_cols != m2.m_cols)) {
        throw std::out_of_range("");
    }
    Matrix m(m1.m_rows, m1.m_cols);
    for (size_t i = 0; i < m1.m_rows; i++) {
        for (size_t j = 0; j < m1.m_cols; j++) {
            m[i][j] = m1[i][j] + m2[i][j];
        }
    }
    return m;
}
bool operator==(const Matrix &m1, const Matrix &m2)
{
    if ((m1.m_rows != m2.m_rows) && (m1.m_cols != m2.m_cols)) {
        return false;
    }
    Matrix m(m1.m_rows, m1.m_cols);
    for (size_t i = 0; i < m1.m_rows; i++) {
        for (size_t j = 0; j < m1.m_cols; j++) {
            if (m1[i][j] != m2[i][j]) {
                return false;
            }
        }
    }
    return true;
}
bool operator!=(const Matrix &m1, const Matrix &m2)
{
    return !(m1 == m2);
}
Matrix::~Matrix()
{
    delete this->m_start;
    this->m_start = nullptr;
}

void matrix_wrong_init()
{
    bool check = false;
    try {
        Matrix tested(-1, 6);
    }
    catch(std::out_of_range) {
        check = true;
    }
    assert(check);
}
void matrix_cols()
{
    Matrix tested(2, 3);
    assert(tested.getColumns() == 3);
}
void matrix_rows()
{
    Matrix tested(2, 3);
    assert(tested.getRows() == 2);
}
void matrix_el()
{
    Matrix tested(2, 3);
    bool check = false;
    tested[1][1] = 1;
    try {
        tested[-2][1] = 1;
    }
    catch(std::out_of_range) {
        check = true;
    }
    assert(check && tested[1][1] == 1);
}
void matrix_output()
{
    std::stringstream ss, ss_check;
    std::string str, str_check;
    Matrix m(2, 2);
    m[0][0] = 1;
    m[0][1] = 2;
    m[1][0] = 3;
    m[1][1] = 4;
    ss << m;
    ss >> str;
    ss_check << 1 << " " << 2 << " " << std::endl << 3 << 4 << std::endl;
    ss_check >> str_check;
    assert(str == str_check);
}
void matrix_comp()
{
    Matrix m1(2, 2), m2(2, 2), m3(2, 2), m4(3, 2);
    m1[0][0] = 1;
    m1[0][1] = 2;
    m1[1][0] = 3;
    m1[1][1] = 4;
    m2[0][0] = 1;
    m2[0][1] = 3;
    m2[1][0] = 4;
    m2[1][1] = 1;
    m3[0][0] = 1;
    m3[0][1] = 2;
    m3[1][0] = 3;
    m3[1][1] = 4;
    assert(m1 != m2 && m1 == m3 && m1 != m4);
}
void matrix_sum()
{
    Matrix m1(2, 2), m2(2, 2), m_check(2, 2);
    m_check[0][0] = 2;
    m_check[0][1] = 5;
    m_check[1][0] = 7;
    m_check[1][1] = 5;
    m1[0][0] = 1;
    m1[0][1] = 2;
    m1[1][0] = 3;
    m1[1][1] = 4;
    m2[0][0] = 1;
    m2[0][1] = 3;
    m2[1][0] = 4;
    m2[1][1] = 1;
    Matrix m = m1 + m2;
    assert(m == m_check);
}
void matrix_mult()
{
    Matrix m(2, 2), m_check(2, 2), m1_check(2, 2), m2_check(2, 2);
    m_check[0][0] = 2;
    m_check[0][1] = 4;
    m_check[1][0] = 6;
    m_check[1][1] = 8;
    m1_check[0][0] = 4;
    m1_check[0][1] = 8;
    m1_check[1][0] = 12;
    m1_check[1][1] = 16;
    m2_check[0][0] = 8;
    m2_check[0][1] = 16;
    m2_check[1][0] = 24;
    m2_check[1][1] = 32;
    m[0][0] = 1;
    m[0][1] = 2;
    m[1][0] = 3;
    m[1][1] = 4;
    m *= 2;
    Matrix m1 = m * 2;
    Matrix m2 = 2 * m1;
    assert(m == m_check && m1 == m1_check && m2 == m2_check);
}
