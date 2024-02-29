#ifndef CPP_MATH_LIBRARY_MATH_VECTOR_H_
#define CPP_MATH_LIBRARY_MATH_VECTOR_H_

#include <istream>
#include <ostream>
#include <vector>

namespace math {

/**
 * @brief Class that represents mathematical vector and methods and operators to
 * work with it.
 *
 */
class vector {
 public:
  using value_type = double;
  using data_type = std::vector<value_type>;
  using reference = typename data_type::reference;
  using const_reference = typename data_type::const_reference;
  using size_type = typename data_type::size_type;
  using iterator = typename data_type::iterator;
  using const_iterator = typename data_type::const_iterator;
  using reverse_iterator = typename data_type::reverse_iterator;
  using const_reverse_iterator = typename data_type::const_reverse_iterator;

  /**
   * @brief Construct a new vector of size 3 filled by 0
   *
   */
  explicit vector() noexcept;

  /**
   * @brief Construct a new vector of size filled by value (0 by default)
   *
   * @param size size of vector
   * @param value what to fill vector with
   */
  explicit vector(size_type size, value_type value = value_type());

  /**
   * @brief Construct a new vector from initializer list
   *
   * @param list initializer list which size is not 0
   */
  explicit vector(const std::initializer_list<value_type>& list);

  /**
   * @brief Construct a new vector of size 2
   *
   * @param x1 X coordinate
   * @param x2 Y coordinate
   */
  vector(value_type x1, value_type x2) noexcept;

  /**
   * @brief Construct a new vector of size 3
   *
   * @param x1 X coordinate
   * @param x2 Y coordinate
   * @param x3 Z coordinate
   */
  vector(value_type x1, value_type x2, value_type x3) noexcept;

  /**
   * @brief Construct a new Vector from a range
   *
   * @tparam Iterator forward iterator
   * @param first beginning of the range
   * @param last end of the range
   */
  template <class Iterator>
  vector(Iterator first, Iterator last) : data_() {
    if (std::distance(first, last) == 0) {
      throw std::invalid_argument("Vector size can not be 0");
    }

    data_.assign(first, last);
  }

  size_type size() const noexcept;

  /**
   * @brief Get vector element without bounds checking
   *
   * @param pos Position of needed element
   * @return changable reference
   */
  reference operator[](size_type pos) noexcept;

  /**
   * @brief Get vector element without bounds checking
   *
   * @param pos Position of needed element
   * @return constant reference
   */
  const_reference operator[](size_type pos) const noexcept;

  /**
   * @brief Get vector element with bounds checking
   *
   * @param pos Position of needed element
   * @return changable reference
   */
  reference at(size_type pos);

  /**
   * @brief Get vector element with bounds checking
   *
   * @param pos Position of needed element
   * @return constant reference
   */
  const_reference at(size_type pos) const;

  /**
   * @brief Get vector element with bounds checking
   *
   * @param pos Position of needed element
   * @return changable reference
   */
  reference operator()(size_type pos);

  /**
   * @brief Get vector element with bounds checking
   *
   * @param pos Position of needed element
   * @return constant reference
   */
  const_reference operator()(size_type pos) const;

  // Returns writable iterator to the beginning of the vector
  iterator begin() noexcept;

  // Returns read-only iterator to the beginning of the vector
  const_iterator begin() const noexcept;

  // Returns writable iterator to the past-end of the vector
  iterator end() noexcept;

  // Returns read-only iterator to the past-end of the vector
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
   * @brief Outputs vector in format [1, 2, 3].
   *
   * @param out std::ostream or child object
   * @param v vector to output
   * @return std::ostream&
   */
  friend std::ostream& operator<<(std::ostream& out, const vector& v);

  /**
   * @brief Inputs vector. If in indicated fail - leave rest of the vector
   * unchanged
   *
   * @param in std::istream or child object
   * @param v vector to input
   * @return std::istream&
   */
  friend std::istream& operator>>(std::istream& in, vector& v);

  // Comparison of values of two vectors. Return true if vectors are equal
  friend bool operator==(const vector& l, const vector& r) noexcept;

  // Comparison of values of two vectors. Return false if vectors are equal
  friend bool operator!=(const vector& l, const vector& r) noexcept;

  // Sum other into this. If this smaller than other - this extends
  vector& operator+=(const vector& other);

  // Sub other from this. If this smaller than other - this extends
  vector& operator-=(const vector& other);

  // Multiply vector values by value
  vector& operator*=(const_reference value) noexcept;

  // Divide vector values bu value
  vector& operator/=(const_reference value) noexcept;

  // Sum of two vectors. If l is smaller than other - l extends
  friend vector operator+(const vector& l, const vector& r);

  // Sub of two vectors. If l is smaller than other - l extends
  friend vector operator-(const vector& l, const vector& r);

  // Multiply vector values by value
  friend vector operator*(const vector& v, const_reference value);

  // Divide vector values by value
  friend vector operator/(const vector& v, const_reference value);

  // Multiply vector values by value
  friend vector operator*(const_reference value, const vector& v);

  /**
   * @brief Calculates cross product of two vectors. If vectors have different
   * sizes - throws std::invalid_argument
   *
   */
  friend value_type operator*(const vector& l, const vector& r);

  /**
   * @brief Change size of vector. If new size greater than size - fill with
   * value (defaults to 0), else - discard other values
   *
   * @param new_size
   * @param value
   */
  void resize(size_type new_size, const_reference value = value_type());

  /**
   * @brief Enlarge vector. If new size is less or equal - do nothing
   *
   * @param new_size
   * @param value
   */
  void extend(size_type new_size, const value_type& value = value_type());

  // Calculates vector absolute value
  value_type abs() const noexcept;

 private:
  void check_size_for_getter(size_type pos) const;
  void check_size_for_operation(const vector& other) const;

  data_type data_;
};

}  // namespace math

// overrides of std methods

namespace std {

math::vector::value_type abs(const math::vector& v) noexcept;

}  // namespace std

#endif  // CPP_MATH_LIBRARY_MATH_VECTOR_H_
