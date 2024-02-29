#include "math_matrix.h"

#include <algorithm>
#include <stdexcept>

#include "math_vector.h"

namespace math {

matrix::matrix(const_reference diag) : matrix(3, diag) {}

matrix::matrix(size_type size, const_reference diag) : matrix(size, size) {
  for (size_type i = 0; i < rows_; ++i) {
    get_element(i, i) = diag;
  }
}

matrix::matrix(size_type rows, size_type columns)
    : rows_(rows), columns_(columns) {
  if (!rows_ || !columns_) {
    throw std::invalid_argument("Matrix dimensions can not be 0");
  }

  data_ = data_type(rows_ * columns_, value_type());
}

matrix::matrix(
    const std::initializer_list<std::initializer_list<value_type>> &items)
    : rows_(items.size()) {
  if (!rows_) {
    throw std::invalid_argument("Matrix sizes can not be 0");
  }

  columns_ = (*(items.begin())).size();
  if (!columns_) {
    throw std::invalid_argument("Matrix sizes can not be 0");
  }

  data_.reserve(rows_ * columns_);
  for (const auto &row : items) {
    if (row.size() != columns_) {
      throw std::invalid_argument(
          "Initializer list columns has different sizes");
    }

    for (const auto &el : row) {
      data_.push_back(el);
    }
  }
}

matrix::matrix(const vector &v, bool is_column)
    : matrix(is_column ? v.size() : 1, is_column ? 1 : v.size()) {
  for (size_type i = 0; i < std::max(rows_, columns_); ++i) {
    get_element(is_column ? i : 0, is_column ? 0 : i) = v[i];
  }
}

matrix::size_type matrix::rows() const noexcept { return rows_; }

matrix::size_type matrix::columns() const noexcept { return columns_; }

matrix::reference matrix::operator()(size_type row, size_type column) {
  bounds_check(row, column);
  return get_element(row, column);
}

matrix::const_reference matrix::operator()(size_type row,
                                           size_type column) const {
  bounds_check(row, column);
  return get_element(row, column);
}

// Returns read-write iterator to the beginning
matrix::iterator matrix::begin() noexcept { return data_.begin(); }

// Returns read-only iterator to the beginning
matrix::const_iterator matrix::begin() const noexcept { return data_.begin(); }

// Returns read-write iterator to the end
matrix::iterator matrix::end() noexcept { return data_.end(); }

// Returns read-only iterator to the end
matrix::const_iterator matrix::end() const noexcept { return data_.end(); }

matrix::const_iterator matrix::cbegin() const noexcept {
  return data_.cbegin();
}

matrix::const_iterator matrix::cend() const noexcept { return data_.cend(); }

matrix::reverse_iterator matrix::rbegin() noexcept { return data_.rbegin(); }

matrix::reverse_iterator matrix::rend() noexcept { return data_.rend(); }

matrix::const_reverse_iterator matrix::rbegin() const noexcept {
  return data_.rbegin();
}

matrix::const_reverse_iterator matrix::rend() const noexcept {
  return data_.rend();
}

matrix::const_reverse_iterator matrix::crbegin() const noexcept {
  return data_.crbegin();
}

matrix::const_reverse_iterator matrix::crend() const noexcept {
  return data_.crend();
}

std::ostream &operator<<(std::ostream &out, const matrix &m) {
  using size_type = matrix::size_type;

  bool endline = false;
  for (size_type i = 0; i < m.rows(); ++i) {
    if (endline) {
      out << std::endl;
    }
    endline = true;

    out << '[';
    bool comma = false;
    for (size_type j = 0; j < m.columns(); ++j) {
      if (comma) {
        out << ' ';
      }
      comma = true;

      out << m.get_element(i, j);
    }

    out << ']';
  }

  return out;
}

std::istream &operator>>(std::istream &in, matrix &m) {
  for (auto &el : m) in >> el;
  return in;
}

bool operator==(const matrix &l, const matrix &r) noexcept {
  if (l.rows() != r.rows() || l.columns() != r.columns()) {
    return false;
  }

  return std::equal(l.begin(), l.end(), r.begin(), r.end());
}

bool operator!=(const matrix &l, const matrix &r) noexcept { return !(l == r); }

matrix &matrix::operator+=(const matrix &other) {
  is_sizes_equal(other);

  std::transform(begin(), end(), other.begin(), begin(),
                 std::plus<value_type>());
  return *this;
}

matrix &matrix::operator-=(const matrix &other) {
  is_sizes_equal(other);

  std::transform(begin(), end(), other.begin(), begin(),
                 std::minus<value_type>());
  return *this;
}

matrix &matrix::operator*=(const matrix &other) {
  is_inner_sizes_equal(other);

  matrix result(rows_, other.columns_);
  for (size_type i = 0; i < result.rows_; ++i)
    for (size_type j = 0; j < result.columns_; ++j)
      for (size_type k = 0; k < columns_; ++k)
        result.get_element(i, j) += get_element(i, k) * other.get_element(k, j);

  *this = std::move(result);
  return *this;
}

matrix &matrix::operator*=(const value_type &value) noexcept {
  std::transform(begin(), end(), begin(),
                 [&value](const_reference v) { return v * value; });
  return *this;
}

matrix &matrix::operator/=(const value_type &value) noexcept {
  return *this *= (1.0 / value);
}

matrix operator+(const matrix &l, const matrix &r) {
  matrix result(l);
  result += r;
  return result;
}

matrix operator-(const matrix &l, const matrix &r) {
  matrix result(l);
  result -= r;
  return result;
}

matrix operator*(const matrix &l, const matrix &r) {
  matrix result(l);
  result *= r;
  return result;
}

matrix operator*(const matrix &m, matrix::const_reference value) {
  matrix result(m);
  result *= value;
  return result;
}

matrix operator*(matrix::const_reference &value, const matrix &matrix) {
  return matrix * value;
}

matrix operator/(const matrix &m, matrix::const_reference &value) {
  matrix result(m);
  result /= value;
  return result;
}

matrix matrix::transposed() const {
  matrix result(columns_, rows_);
  for (size_type i = 0; i < result.rows_; ++i) {
    for (size_type j = 0; j < result.columns_; ++j) {
      result.get_element(i, j) = get_element(j, i);
    }
  }

  return result;
}

matrix matrix::minor_matrix(size_type row, size_type column) const {
  if (rows_ == 1 || columns_ == 1) {
    throw std::logic_error("Minor matrix of matrix 1x1 is not exist");
  }
  bounds_check(row, column);

  matrix result(rows_ - 1, columns_ - 1);
  for (size_type i = 0, im = 0; i < rows_; ++i) {
    if (i == row) {
      continue;
    }

    for (size_type j = 0, jm = 0; j < columns_; ++j) {
      if (j == column) {
        continue;
      }

      result.get_element(im, jm++) = get_element(i, j);
    }
    ++im;
  }

  return result;
}

matrix matrix::upper_triangle_matrix() const {
  matrix result(*this);

  for (size_type j = 0; j < result.columns_ - 1; ++j) {
    size_type non_zero_row = result.find_not_zero_row(j, j);
    if (non_zero_row >= result.rows_) {
      continue;
    }

    if (non_zero_row != j) {
      for (size_type i = 0; i < result.columns_; ++i) {
        result.get_element(j, i) += result.get_element(non_zero_row, i);
      }
    }

    for (size_type i = j + 1; i < result.rows_; ++i) {
      if (result.get_element(i, j)) {
        value_type multiplier =
            result.get_element(i, j) / result.get_element(j, j);
        for (size_type k = 0; k < result.columns_; ++k) {
          result.get_element(i, k) -= result.get_element(j, k) * multiplier;
        }
      }
    }
  }

  return result;
}

matrix::value_type matrix::determinant() const {
  square_check();

  matrix triangle = upper_triangle_matrix();
  value_type result = 1;

  for (size_type i = 0; i < triangle.rows_; ++i) {
    result *= triangle.get_element(i, i);
  }
  return result;
}

matrix matrix::complements_matrix() const {
  square_check();

  matrix result(rows_, columns_);
  for (size_type i = 0; i < result.rows_; ++i) {
    for (size_type j = 0; j < result.columns_; ++j) {
      result.get_element(i, j) =
          minor_matrix(i, j).determinant() * ((i + j) % 2 ? -1 : 1);
    }
  }

  return result;
}

matrix matrix::inverse() const {
  if (value_type det = determinant(); det != 0) {
    return complements_matrix().transposed() / det;
  }

  throw std::logic_error(
      "Inverse matrix can not be calculated from matrix with det = 0");
}

matrix matrix::operator!() const { return transposed(); }

matrix matrix::operator*() const { return complements_matrix(); }

matrix matrix::operator~() const { return inverse(); }

matrix matrix::operator-() const { return *this * -1; }

matrix matrix::operator+() const { return matrix(*this); }

void matrix::set_rows(size_type rows) {
  if (!rows) {
    throw std::invalid_argument("Matrix sizes can not be 0");
  }

  matrix new_matrix(rows, columns_);
  for (size_type i = 0; i < std::min(rows, rows_); ++i) {
    for (size_type j = 0; j < columns_; ++j) {
      new_matrix.get_element(i, j) = get_element(i, j);
    }
  }

  *this = std::move(new_matrix);
}

void matrix::set_columns(size_type columns) {
  if (!columns) {
    throw std::invalid_argument("Matrix sizes can not be 0");
  }

  matrix new_matrix(rows_, columns);
  for (size_type i = 0; i < rows_; ++i) {
    for (size_type j = 0; j < std::min(columns, columns_); ++j) {
      new_matrix.get_element(i, j) = get_element(i, j);
    }
  }

  *this = std::move(new_matrix);
}

matrix::reference matrix::get_element(size_type row,
                                      size_type column) noexcept {
  return data_[columns_ * row + column];
}

matrix::const_reference matrix::get_element(size_type row,
                                            size_type column) const noexcept {
  return data_[columns_ * row + column];
}

void matrix::bounds_check(size_type row, size_type column) const {
  if (row >= rows_ || column >= columns_) {
    throw std::out_of_range("Out of range: rows_ = " + std::to_string(rows_) +
                            ", row = " + std::to_string(row) +
                            ", columns_ = " + std::to_string(columns_) +
                            ", column = " + std::to_string(column));
  }
}

void matrix::is_sizes_equal(const matrix &other) const {
  if (rows_ != other.rows_ || columns_ != other.columns_) {
    throw std::invalid_argument(
        "Sizes mismatch: rows_ = " + std::to_string(rows_) +
        ", other.rows_ = " + std::to_string(other.rows_) +
        ", columns_ = " + std::to_string(columns_) +
        ", other.columns_ = " + std::to_string(other.columns_));
  }
}

void matrix::is_inner_sizes_equal(const matrix &other) const {
  if (columns_ != other.rows_) {
    throw std::invalid_argument(
        "Inner sizes mismatch: columns_ = " + std::to_string(columns_) +
            ", other.rows_" = std::to_string(other.rows_));
  }
}

void matrix::square_check() const {
  if (rows_ != columns_) {
    throw std::logic_error("Matrix is not square");
  }
}

matrix::size_type matrix::find_not_zero_row(size_type from_row,
                                            size_type column) const noexcept {
  while (!get_element(from_row, column) && from_row < rows_) {
    ++from_row;
  }

  return from_row;
}

}  // namespace math
