#ifndef CPP_MATH_LIBRARY_MATH_MATRIX_H_
#define CPP_MATH_LIBRARY_MATH_MATRIX_H_

#include <istream>
#include <ostream>
#include <vector>

#include "math_vector.h"

namespace math {

/**
 * @brief Matrix class to work with matrices as mathematical object.
 *
 */
class matrix {
 public:
  using value_type = double;
  using data_type = std::vector<value_type>;
  using reference = typename data_type::reference;
  using const_reference = typename data_type::const_reference;
  using const_pointer = typename data_type::const_pointer;
  using size_type = typename data_type::size_type;
  using iterator = typename data_type::iterator;
  using const_iterator = typename data_type::const_iterator;
  using reverse_iterator = typename data_type::reverse_iterator;
  using const_reverse_iterator = typename data_type::const_reverse_iterator;

  // Constructs 3x3 identity matrix
  explicit matrix(const_reference diag = 1);

  /**
   * @brief Constructs size x size identity matrix, but the diagonal filled with
   * diag
   *
   * @param size matrix sizes
   * @param diag diagonal element, defaults to 1
   */
  explicit matrix(size_type size, const_reference diag = 1);

  // Constructs rows x columns matrix filled by default values
  matrix(size_type rows, size_type columns);

  /**
   * @brief Construct a new matrix from initializer list. Initializer list must
   * be in format {{1, 2}, {3, 4}, {4, 5}}, where nested initializer lists are
   * represented as rows and their length must be equal.
   *
   * @param items initializer list of initializer lists
   */
  explicit matrix(
      const std::initializer_list<std::initializer_list<value_type>> &items);

  /**
   * @brief Construct a new vector in form of matrix
   *
   * @param v vector to transform to matrix
   * @param is_column if true then column-vector is build; otherwise row-vector
   * is build
   */
  explicit matrix(const vector &v, bool is_column = false);

  // Returns row count
  size_type rows() const noexcept;

  // Returns column count
  size_type columns() const noexcept;

  /**
   * @brief Get element by position, Throws std::out_of_bounds if row >= rows_
   * or column >= columns_
   *
   * @param row row of needed element, starts with 0
   * @param column column of needed element, starts with 0
   * @return read-write reference
   */
  reference operator()(size_type row, size_type column);

  /**
   * @brief Get element by position, Throws std::out_of_bounds if row >= rows_
   * or column >= columns_
   *
   * @param row row of needed element, starts with 0
   * @param column column of needed element, starts with 0
   * @return read-only reference
   */
  const_reference operator()(size_type row, size_type column) const;

  // Returns read-write iterator to the beginning
  iterator begin() noexcept;

  // Returns read-only iterator to the beginning
  const_iterator begin() const noexcept;

  // Returns read-write iterator to the end
  iterator end() noexcept;

  // Returns read-only iterator to the end
  const_iterator end() const noexcept;

  const_iterator cbegin() const noexcept;

  const_iterator cend() const noexcept;

  reverse_iterator rbegin() noexcept;

  reverse_iterator rend() noexcept;

  const_reverse_iterator rbegin() const noexcept;

  const_reverse_iterator rend() const noexcept;

  const_reverse_iterator crbegin() const noexcept;

  const_reverse_iterator crend() const noexcept;

  /**
   * @brief Outputs matrix in format
   * [1 2 3]
   * [4 5 6]
   * [7 8 9]
   *
   */
  friend std::ostream &operator<<(std::ostream &out, const matrix &m);

  /**
   * @brief Inputs rows_ * columns_ elements into the matrix row by row.
   *
   */
  friend std::istream &operator>>(std::istream &in, matrix &m);

  // Exact comparsion of two matrices
  friend bool operator==(const matrix &l, const matrix &r) noexcept;

  // Exact comparsion of two matrices
  friend bool operator!=(const matrix &l, const matrix &r) noexcept;

  /**
   * @brief Sum of two matrices into this. Throws std::invalid_argument if sizes
   * are not equal
   *
   */
  matrix &operator+=(const matrix &other);

  /**
   * @brief Subtraction of two matrices into this. Throws std::invalid_argument
   * if sizes are not equal
   *
   */
  matrix &operator-=(const matrix &other);

  /**
   * @brief Multiplication of two matrices into this. Throws
   * std::invalid_argument if inner sizes are not equal
   *
   */
  matrix &operator*=(const matrix &other);

  /**
   * @brief Multiplies all matrix values by given value
   *
   */
  matrix &operator*=(const value_type &value) noexcept;

  /**
   * @brief Divides all matrix values by given value
   *
   */
  matrix &operator/=(const value_type &value) noexcept;

  /**
   * @brief Sum of two matrices into a new matrix. Throws std::invalid_argument
   * if matrix sizes are not equal
   *
   */
  friend matrix operator+(const matrix &l, const matrix &r);

  /**
   * @brief Subtraction of two matrices into a new matrix. Throws
   * std::invalid_argument if matrix sizes are not equal
   *
   */
  friend matrix operator-(const matrix &l, const matrix &r);

  /**
   * @brief Multiplication of two matrices into a new matrix. Throws
   * std::invalid_argument if matrix inner sizes are not equal
   *
   */
  friend matrix operator*(const matrix &l, const matrix &r);

  /**
   * @brief Multiplies all matrix values by given value and returns new matrix
   * with these values
   *
   */
  friend matrix operator*(const matrix &m, const_reference value);

  /**
   * @brief Multiplies all matrix values by given value and returns new matrix
   * with these values
   *
   */
  friend matrix operator*(const_reference &value, const matrix &matrix);

  /**
   * @brief Divides all matrix values by given value and returns new matrix
   * with these values
   *
   */
  friend matrix operator/(const matrix &m, const_reference &value);

  // Returns transposed matrix
  matrix transposed() const;

  /**
   * @brief Returns minor matrix from given row and column. Throws
   * std::logic_error if matrix is of size 1. Throws std::out_of_range if row
   * >= rows_ or column >= columns_
   *
   */
  matrix minor_matrix(size_type row, size_type column) const;

  // Returns upper triangle matrix with zeroes under main diagonal
  matrix upper_triangle_matrix() const;

  /**
   * @brief Calculates determinant of matrix. Throws std::logic_error if
   * matrix is not square
   *
   */
  value_type determinant() const;

  /**
   * @brief Returns matrix of algebraic complements. Throws std::logic_error
   * if matrix is not square
   *
   */
  matrix complements_matrix() const;

  /**
   * @brief Returns inverse matrix. Throws std::logic_error if matrix is not
   * square or determinant == 0
   *
   */
  matrix inverse() const;

  /**
   * @brief Returns transposed matrix.
   *
   */
  matrix operator!() const;

  /**
   * @brief Returns matrix of algebraic complements. Throws std::logic_error if
   * matrix is not square or determinant == 0
   *
   * @return matrix
   */
  matrix operator*() const;

  /**
   * @brief Returns inverse matrix. Throws std::logic_error if matrix is not
   * square or determinant == 0
   *
   */
  matrix operator~() const;

  /**
   * @brief Returns new matrix with elements multiplied by -1
   *
   */
  matrix operator-() const;

  /**
   * @brief Returns copy of matrix
   *
   */
  matrix operator+() const;

  /**
   * @brief Set new rows count. If rows == 0 then throws std::invalid_argument
   *
   */
  void set_rows(size_type rows);

  /**
   * @brief Set new columns count. If columns == 0 then throws
   * std::invalid_argument
   *
   */
  void set_columns(size_type columns);

 private:
  reference get_element(size_type row, size_type column) noexcept;
  const_reference get_element(size_type row, size_type column) const noexcept;

  void bounds_check(size_type row, size_type column) const;
  void is_sizes_equal(const matrix &other) const;
  void is_inner_sizes_equal(const matrix &other) const;
  void square_check() const;

  size_type find_not_zero_row(size_type from_row,
                              size_type column) const noexcept;

  size_type rows_;
  size_type columns_;
  data_type data_;
};

}  // namespace math

#endif  // CPP_MATH_LIBRARY_MATH_MATRIX_H_
