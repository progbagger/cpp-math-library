#ifndef MATH_LIBRARIES_CPP_MATH_MATRIX_H_
#define MATH_LIBRARIES_CPP_MATH_MATRIX_H_

#include <istream>
#include <ostream>
#include <stdexcept>
#include <vector>

#include "math_vector.h"

namespace math {

/**
 * @brief Matrix class to work with matrices as mathematical object.
 *
 * @tparam T was designed to be numeric, e. g. double, float, int, ... -
 * defaults to double
 */
template <class T = double>
class Matrix {
 public:
  using ValueType = T;
  using Reference = ValueType &;
  using ConstReference = const ValueType &;
  using ConstPointer = const ValueType *;
  using DataType = std::vector<ValueType>;
  using SizeType = std::size_t;
  using Iterator = typename DataType::iterator;
  using ConstIterator = typename DataType::const_iterator;

  // for foreach compatibility
  using iterator = Iterator;
  using const_iterator = ConstIterator;

  // Constructs 3x3 identity matrix
  explicit Matrix(ValueType diag = 1) : Matrix(3, diag) {}

  /**
   * @brief Constructs size x size identity matrix, but the diagonal filled with
   * diag
   *
   * @param size matrix sizes
   * @param diag diagonal element, defaults to 1
   */
  explicit Matrix(SizeType size, ValueType diag = 1) : Matrix(size, size) {
    for (SizeType i = 0; i < rows_; ++i) GetElement(i, i) = diag;
  }

  // Constructs rows x columns matrix filled by default values
  Matrix(SizeType rows, SizeType columns) : rows_(rows), columns_(columns) {
    if (!rows_ || !columns_)
      throw std::invalid_argument("Matrix dimensions can not be 0");
    data_ = DataType(rows_ * columns_, ValueType());
  }

  /**
   * @brief Construct a new matrix from initializer list. Initializer list must
   * be in format {{1, 2}, {3, 4}, {4, 5}}, where nested initializer lists are
   * represented as rows and their length must be equal.
   *
   * @param items initializer list of initializer lists
   */
  Matrix(const std::initializer_list<std::initializer_list<ValueType>> &items)
      : rows_(items.size()) {
    if (!rows_) throw std::invalid_argument("Matrix sizes can not be 0");
    columns_ = (*(items.begin())).size();
    if (!columns_) throw std::invalid_argument("Matrix sizes can not be 0");
    data_.reserve(rows_ * columns_);
    for (const auto &row : items) {
      if (row.size() != columns_)
        throw std::invalid_argument(
            "Initializer list columns has different sizes");
      for (const auto &el : row) data_.push_back(el);
    }
  }

  /**
   * @brief Construct a new vector in form of matrix
   *
   * @param v vector to transform to matrix
   * @param is_column if true then column-vector is build; otherwise row-vector
   * is build
   */
  explicit Matrix(const Vector<ValueType> &v, bool is_column = false)
      : Matrix(is_column ? v.Size() : 1, is_column ? 1 : v.Size()) {
    for (SizeType i = 0; i < std::max(rows_, columns_); ++i)
      GetElement(is_column ? i : 0, is_column ? 0 : i) = v[i];
  }

  // Returns row count
  SizeType Rows() const noexcept { return rows_; }

  // Returns column count
  SizeType Columns() const noexcept { return columns_; }

  /**
   * @brief Get element by position, Throws std::out_of_bounds if row >= rows_
   * or column >= columns_
   *
   * @param row row of needed element, starts with 0
   * @param column column of needed element, starts with 0
   * @return read-write reference
   */
  Reference operator()(SizeType row, SizeType column) {
    BoundsCheck(row, column);
    return GetElement(row, column);
  }

  /**
   * @brief Get element by position, Throws std::out_of_bounds if row >= rows_
   * or column >= columns_
   *
   * @param row row of needed element, starts with 0
   * @param column column of needed element, starts with 0
   * @return read-only reference
   */
  ConstReference operator()(SizeType row, SizeType column) const {
    BoundsCheck(row, column);
    return GetElement(row, column);
  }

  // Returns read-write iterator to the beginning
  Iterator Begin() noexcept { return data_.begin(); }

  // Returns read-only iterator to the beginning
  ConstIterator Begin() const noexcept { return data_.begin(); }

  // Returns read-write iterator to the end
  Iterator End() noexcept { return data_.end(); }

  // Returns read-only iterator to the end
  ConstIterator End() const noexcept { return data_.end(); }

  // Returns read-write iterator to the beginning
  iterator begin() noexcept { return Begin(); }

  // Returns read-only iterator to the beginning
  const_iterator begin() const noexcept { return Begin(); }

  // Returns read-write iterator to the end
  iterator end() noexcept { return End(); }

  // Returns read-only iterator to the end
  const_iterator end() const noexcept { return End(); }

  /**
   * @brief Outputs matrix in format
   * {1 2 3}
   * {4 5 6}
   * {7 8 9}
   *
   */
  friend std::ostream &operator<<(std::ostream &out, const Matrix &m) {
    for (SizeType i = 0; i < m.rows_; ++i) {
      out << '{';
      for (SizeType j = 0; j < m.columns_; ++j) {
        out << m.GetElement(i, j);
        if (j != m.columns_ - 1) out << ' ';
      }
      out << '}';
      if (i != m.rows_ - 1) out << std::endl;
    }
    return out;
  }

  /**
   * @brief Inputs rows_ * columns_ elements into the matrix row by row. If
   * there are not enough elements in in - rest of the elements are unchanged
   *
   */
  friend std::istream &operator>>(std::istream &in, Matrix &m) {
    for (auto &el : m) in >> el;
    return in;
  }

  // Exact comparsion of two matrices
  bool operator==(const Matrix &other) const noexcept {
    if (rows_ != other.rows_ || columns_ != other.columns_) return false;
    auto i1 = Begin(), i2 = Begin();
    while (i1 != End())
      if (*(i1++) != *(i2++)) return false;
    return true;
  }

  // Exact comparsion of two matrices
  bool operator!=(const Matrix &other) const noexcept {
    return !(*this == other);
  }

  /**
   * @brief Sum of two matrices into this. Throws std::invalid_argument if sizes
   * are not equal
   *
   */
  Matrix &operator+=(const Matrix &other) {
    IsSizesEqual(other);
    auto i = other.Begin();
    for (auto &el : *this) el += *(i++);
    return *this;
  }

  /**
   * @brief Subtraction of two matrices into this. Throws std::invalid_argument
   * if sizes are not equal
   *
   */
  Matrix &operator-=(const Matrix &other) {
    IsSizesEqual(other);
    auto i = other.Begin();
    for (auto &el : *this) el -= *(i++);
    return *this;
  }

  /**
   * @brief Multiplication of two matrices into this. Throws
   * std::invalid_argument if inner sizes are not equal
   *
   */
  Matrix &operator*=(const Matrix &other) {
    IsInnerSizesEqual(other);
    Matrix result(rows_, other.columns_);
    for (SizeType i = 0; i < result.rows_; ++i)
      for (SizeType j = 0; j < result.columns_; ++j)
        for (SizeType k = 0; k < columns_; ++k)
          result.GetElement(i, j) += GetElement(i, k) * other.GetElement(k, j);
    *this = std::move(result);
    return *this;
  }

  /**
   * @brief Multiplies all matrix values by given value
   *
   */
  Matrix &operator*=(const ValueType &value) noexcept {
    for (auto &el : *this) el *= value;
    return *this;
  }

  /**
   * @brief Divides all matrix values by given value
   *
   */
  Matrix &operator/=(const ValueType &value) noexcept {
    return *this *= (1 / value);
  }

  /**
   * @brief Sum of two matrices into a new matrix. Throws std::invalid_argument
   * if matrix sizes are not equal
   *
   */
  Matrix operator+(const Matrix &other) const {
    Matrix result(*this);
    result += other;
    return result;
  }

  /**
   * @brief Subtraction of two matrices into a new matrix. Throws
   * std::invalid_argument if matrix sizes are not equal
   *
   */
  Matrix operator-(const Matrix &other) const {
    Matrix result(*this);
    result -= other;
    return result;
  }

  /**
   * @brief Multiplication of two matrices into a new matrix. Throws
   * std::invalid_argument if matrix inner sizes are not equal
   *
   */
  Matrix operator*(const Matrix &other) const {
    Matrix result(*this);
    result *= other;
    return result;
  }

  /**
   * @brief Multiplies all matrix values by given value and returns new matrix
   * with these values
   *
   */
  Matrix operator*(const ValueType &value) const noexcept {
    Matrix result(*this);
    result *= value;
    return result;
  }

  /**
   * @brief Multiplies all matrix values by given value and returns new matrix
   * with these values
   *
   */
  friend Matrix operator*(const ValueType &value,
                          const Matrix &matrix) noexcept {
    return Matrix(matrix) * value;
  }

  /**
   * @brief Divides all matrix values by given value and returns new matrix with
   * these values
   *
   */
  Matrix operator/(const ValueType &value) const noexcept {
    Matrix result(*this);
    result /= value;
    return result;
  }

  // Returns transposed matrix
  Matrix Transpose() const noexcept {
    Matrix result(columns_, rows_);
    for (SizeType i = 0; i < result.rows_; ++i)
      for (SizeType j = 0; j < result.columns_; ++j)
        result.GetElement(i, j) = GetElement(j, i);
    return result;
  }

  /**
   * @brief Returns minor matrix from given row and column. Throws
   * std::logic_error if matrix is of size 1. Throws std::out_of_range if row >=
   * rows_ or column >= columns_
   *
   */
  Matrix MinorMatrix(SizeType row, SizeType column) const {
    if (rows_ == 1 || columns_ == 1)
      throw std::logic_error("Minor matrix of matrix 1x1 is not exist");
    BoundsCheck(row, column);
    Matrix result(rows_ - 1, columns_ - 1);
    for (SizeType i = 0, im = 0; i < rows_; ++i) {
      if (i == row) continue;
      for (SizeType j = 0, jm = 0; j < columns_; ++j) {
        if (j == column) continue;
        result.GetElement(im, jm++) = GetElement(i, j);
      }
      ++im;
    }
    return result;
  }

  // Returns upper triangle matrix with zeroes under main diagonal
  Matrix TriangleMatrix() const {
    Matrix result(*this);
    for (SizeType j = 0; j < result.columns_ - 1; ++j) {
      SizeType non_zero_row = result.FindNotZeroRow(j, j);
      if (non_zero_row >= result.rows_) continue;
      if (non_zero_row != j) {
        for (SizeType i = 0; i < result.columns_; ++i)
          result.GetElement(j, i) += result.GetElement(non_zero_row, i);
      }
      for (SizeType i = j + 1; i < result.rows_; ++i) {
        if (result.GetElement(i, j)) {
          ValueType multiplier =
              result.GetElement(i, j) / result.GetElement(j, j);
          for (SizeType k = 0; k < result.columns_; ++k)
            result.GetElement(i, k) -= result.GetElement(j, k) * multiplier;
        }
      }
    }
    return result;
  }

  /**
   * @brief Calculates determinant of matrix. Throws std::logic_error if matrix
   * is not square
   *
   */
  ValueType Determinant() const {
    SquareCheck();
    Matrix triangle = TriangleMatrix();
    ValueType result = 1;
    for (SizeType i = 0; i < triangle.rows_; ++i) {
      result *= triangle.GetElement(i, i);
      if (!result) break;
    }
    return result;
  }

  /**
   * @brief Returns matrix of algebraic complements. Throws std::logic_error if
   * matrix is not square
   *
   */
  Matrix ComplementsMatrix() const {
    SquareCheck();
    Matrix result(rows_, columns_);
    for (SizeType i = 0; i < result.rows_; ++i)
      for (SizeType j = 0; j < result.columns_; ++j)
        result.GetElement(i, j) =
            MinorMatrix(i, j).Determinant() * ((i + j) % 2 ? -1 : 1);
    return result;
  }

  /**
   * @brief Returns inverse matrix. Throws std::logic_error if matrix is not
   * square or determinant == 0
   *
   */
  Matrix Inverse() const {
    ValueType det = Determinant();
    if (!det)
      throw std::logic_error(
          "Inverse matrix can not be calculated from matrix with det = 0");
    return ComplementsMatrix().Transpose() / det;
  }

  /**
   * @brief Returns inverse matrix. Throws std::logic_error if matrix is not
   * square or determinant == 0
   *
   */
  Matrix operator!() const { return Inverse(); }

  /**
   * @brief Returns new matrix with elements multiplied by -1
   *
   */
  Matrix operator-() const noexcept { return *this * -1; }

  /**
   * @brief Returns copy of matrix
   *
   */
  Matrix operator+() const noexcept { return *this; }

  /**
   * @brief Set new rows count. If rows == 0 then throws std::invalid_argument
   *
   */
  void SetRows(SizeType rows) {
    if (!rows) throw std::invalid_argument("Matrix sizes can not be 0");
    Matrix new_matrix(rows, columns_);
    for (SizeType i = 0; i < std::min(rows, rows_); ++i)
      for (SizeType j = 0; j < columns_; ++j)
        new_matrix.GetElement(i, j) = GetElement(i, j);
    *this = std::move(new_matrix);
  }

  /**
   * @brief Set new columns count. If columns == 0 then throws
   * std::invalid_argument
   *
   */
  void SetColumns(SizeType columns) {
    if (!columns) throw std::invalid_argument("Matrix sizes can not be 0");
    Matrix new_matrix(rows_, columns);
    for (SizeType i = 0; i < rows_; ++i)
      for (SizeType j = 0; j < std::min(columns, columns_); ++j)
        new_matrix.GetElement(i, j) = GetElement(i, j);
    *this = std::move(new_matrix);
  }

 private:
  Reference GetElement(SizeType row, SizeType column) noexcept {
    return data_[columns_ * row + column];
  }

  ConstReference GetElement(SizeType row, SizeType column) const noexcept {
    return data_[columns_ * row + column];
  }

  void BoundsCheck(SizeType row, SizeType column) const {
    if (row >= rows_ || column >= columns_)
      throw std::out_of_range("Out of range: rows_ = " + std::to_string(rows_) +
                              ", row = " + std::to_string(row) +
                              ", columns_ = " + std::to_string(columns_) +
                              ", column = " + std::to_string(column));
  }

  void IsSizesEqual(const Matrix &other) const {
    if (rows_ != other.rows_ || columns_ != other.columns_)
      throw std::invalid_argument(
          "Sizes mismatch: rows_ = " + std::to_string(rows_) +
          ", other.rows_ = " + std::to_string(other.rows_) +
          ", columns_ = " + std::to_string(columns_) +
          ", other.columns_ = " + std::to_string(other.columns_));
  }

  void IsInnerSizesEqual(const Matrix &other) const {
    if (columns_ != other.rows_)
      throw std::invalid_argument(
          "Inner sizes mismatch: columns_ = " + std::to_string(columns_) +
              ", other.rows_" = std::to_string(other.rows_));
  }

  void SquareCheck() const {
    if (rows_ != columns_) throw std::logic_error("Matrix is not square");
  }

  SizeType FindNotZeroRow(SizeType from_row, SizeType column) const noexcept {
    while (!GetElement(from_row, column) && from_row < rows_) ++from_row;
    return from_row;
  }

  SizeType rows_;
  SizeType columns_;
  DataType data_;
};

}  // namespace math

#endif  // MATH_LIBRARIES_CPP_MATH_MATRIX_H_
