#ifndef MATH_LIBRARIES_CPP_MATH_VECTOR_H_
#define MATH_LIBRARIES_CPP_MATH_VECTOR_H_

#include <cmath>
#include <initializer_list>
#include <istream>
#include <ostream>
#include <stdexcept>
#include <vector>

namespace math {

/**
 * @brief Class that represents mathematical vector and methods and operators to
 * work with it.
 *
 * @tparam T was designed to be numeric, e. g. double, float, int, ... -
 * defaults to float
 */
template <class T = double>
class Vector {
 public:
  using ValueType = T;
  using DataType = std::vector<ValueType>;
  using Reference = typename DataType::reference;
  using ConstReference = typename DataType::const_reference;
  using SizeType = typename DataType::size_type;
  using Iterator = typename DataType::iterator;
  using ConstIterator = typename DataType::const_iterator;

  // for foreach compatibility
  using iterator = Iterator;
  using const_iterator = ConstIterator;

  /**
   * @brief Construct a new vector of size 3 filled by 0
   *
   */
  explicit Vector() noexcept : Vector(3UL, ValueType()) {}

  /**
   * @brief Construct a new vector of size filled by value (0 by default)
   *
   * @param size size of vector
   * @param value what to fill vector with
   */
  explicit Vector(SizeType size, ConstReference value = ValueType()) {
    if (!size) throw std::invalid_argument("Vector size can not be 0");
    data_ = DataType(size, value);
  }

  /**
   * @brief Construct a new vector from initializer list
   *
   * @param list initializer list which size is not 0
   */
  explicit Vector(const std::initializer_list<ValueType>& list) {
    if (!list.size()) throw std::invalid_argument("Vector size can not be 0");
    data_.reserve(list.size());
    for (const auto& i : list) data_.push_back(i);
  }

  /**
   * @brief Construct a new vector of size 2
   *
   * @param x1 X coordinate
   * @param x2 Y coordinate
   */
  Vector(ConstReference x1, ConstReference x2) noexcept : Vector{x1, x2} {}

  /**
   * @brief Construct a new vector of size 3
   *
   * @param x1 X coordinate
   * @param x2 Y coordinate
   * @param x3 Z coordinate
   */
  Vector(ConstReference x1, ConstReference x2, ConstReference x3) noexcept
      : Vector{x1, x2, x3} {}

  SizeType Size() const noexcept { return data_.size(); }

  /**
   * @brief Get vector element without bounds checking
   *
   * @param pos Position of needed element
   * @return changable reference
   */
  Reference operator[](SizeType pos) noexcept { return data_[pos]; }

  /**
   * @brief Get vector element without bounds checking
   *
   * @param pos Position of needed element
   * @return constant reference
   */
  ConstReference operator[](SizeType pos) const noexcept { return data_[pos]; }

  /**
   * @brief Get vector element with bounds checking
   *
   * @param pos Position of needed element
   * @return changable reference
   */
  Reference At(SizeType pos) {
    CheckSizeForGetter(pos);
    return data_[pos];
  }

  /**
   * @brief Get vector element with bounds checking
   *
   * @param pos Position of needed element
   * @return constant reference
   */
  ConstReference At(SizeType pos) const {
    CheckSizeForGetter(pos);
    return data_[pos];
  }

  /**
   * @brief Get vector element with bounds checking
   *
   * @param pos Position of needed element
   * @return changable reference
   */
  Reference operator()(SizeType pos) { return At(pos); }

  /**
   * @brief Get vector element with bounds checking
   *
   * @param pos Position of needed element
   * @return constant reference
   */
  ConstReference operator()(SizeType pos) const { return At(pos); }

  // Returns writable iterator to the beginning of the vector
  Iterator Begin() noexcept { return data_.begin(); }

  // Returns read-only iterator to the beginning of the vector
  ConstIterator Begin() const noexcept { return data_.begin(); }

  // Returns writable iterator to the past-end of the vector
  Iterator End() noexcept { return data_.end(); }

  // Returns read-only iterator to the past-end of the vector
  ConstIterator End() const noexcept { return data_.end(); }

  // Returns writable iterator to the beginning of the vector
  Iterator begin() noexcept { return Begin(); }

  // Returns read-only iterator to the beginning of the vector
  ConstIterator begin() const noexcept { return Begin(); }

  // Returns writable iterator to the past-end of the vector
  Iterator end() noexcept { return End(); }

  // Returns read-only iterator to the past-end of the vector
  ConstIterator end() const noexcept { return End(); }

  /**
   * @brief Outputs vector in format [1, 2, 3].
   *
   * @param out std::ostream or child object
   * @param v vector to output
   * @return std::ostream&
   */
  friend std::ostream& operator<<(std::ostream& out, const Vector& v) {
    out << '{';
    for (auto i = v.Begin(); i != v.End(); ++i) {
      out << *i;
      if (i != v.End() - 1) out << ", ";
    }
    out << '}';
    return out;
  }

  /**
   * @brief Inputs vector. If in indicated fail - leave rest of the vector
   * unchanged
   *
   * @param in std::istream or child object
   * @param v vector to input
   * @return std::istream&
   */
  friend std::istream& operator>>(std::istream& in, Vector& v) {
    for (auto i = v.Begin(); i != v.End(); ++i) {
      in >> *i;
      if (in.fail()) break;
    }
    v.data_.shrink_to_fit();
    return in;
  }

  // Comparison of values of two vectors. Return true if vectors are equal
  bool operator==(const Vector& other) const noexcept {
    if (Size() != other.Size()) return false;
    auto i1 = Begin();
    for (const auto& i2 : other) {
      if (*(i1++) != i2) return false;
    }
    return true;
  }

  // Comparison of values of two vectors. Return false if vectors are equal
  bool operator!=(const Vector& other) const noexcept {
    return !operator==(other);
  }

  // Sum other into this. If this smaller than other - this extends
  Vector& operator+=(const Vector& other) noexcept {
    Extend(other.Size());
    auto i1 = Begin();
    for (const auto& i2 : other) *(i1++) += i2;
    return *this;
  }

  // Sub other from this. If this smaller than other - this extends
  Vector& operator-=(const Vector& other) noexcept {
    Extend(other.Size());
    auto i1 = Begin();
    for (const auto& i2 : other) *(i1++) -= i2;
    return *this;
  }

  // Multiply vector values by value
  Vector& operator*=(ConstReference value) noexcept {
    for (auto& v : *this) v *= value;
    return *this;
  }

  // Divide vector values bu value
  Vector& operator/=(ConstReference value) noexcept {
    return operator*=(1 / value);
  }

  // Sum of two vectors. If this is smaller than other - this extends
  Vector operator+(const Vector& other) const noexcept {
    Vector result(*this);
    result += other;
    return result;
  }

  // Sub of two vectors. If this is smaller than other - this extends
  Vector operator-(const Vector& other) const noexcept {
    Vector result(*this);
    result -= other;
    return result;
  }

  // Multiply vector values by value
  Vector operator*(ConstReference value) const noexcept {
    Vector result(*this);
    result *= value;
    return result;
  }

  // Divide vector values by value
  Vector operator/(ConstReference value) const noexcept {
    Vector result(*this);
    result /= value;
    return result;
  }

  // Multiply vector values by value
  friend Vector operator*(ConstReference value, const Vector& v) noexcept {
    return Vector(v) * value;
  }

  /**
   * @brief Calculates cross product of two vectors. If vectors have different
   * sizes - throws std::invalid_argument
   *
   */
  ValueType operator*(const Vector& other) const {
    CheckSizeForOperation(other);
    ValueType result = 0;
    auto i1 = Begin(), i2 = other.Begin();
    while (i1 != End()) result += *(i1++) * *(i2++);
    return result;
  }

  /**
   * @brief Change size of vector. If new size greater than size - fill with
   * value (defaults to 0), else - discard other values
   *
   * @param new_size
   * @param value
   */
  void Resize(SizeType new_size, ConstReference value = ValueType()) {
    if (!new_size) throw std::invalid_argument("Vector size can not be 0");
    data_.resize(new_size, value);
  }

  /**
   * @brief Enlarge vector. If new size is less or equal - do nothing
   *
   * @param new_size
   * @param value
   */
  void Extend(SizeType new_size,
              const ValueType& value = ValueType()) noexcept {
    if (new_size > Size()) data_.resize(new_size, value);
  }

  // Calculates vector absolute value
  ValueType Abs() const noexcept {
    ValueType result = 0;
    for (const auto& v : *this) result += v * v;
    return sqrt(result);
  }

 private:
  void CheckSizeForGetter(SizeType pos) const {
    if (pos >= Size())
      throw std::out_of_range(std::string("pos >= size, pos = ") +
                              std::to_string(pos) +
                              ", size = " + std::to_string(Size()));
  }

  void CheckSizeForOperation(const Vector& other) const {
    if (Size() != other.Size())
      throw std::invalid_argument(
          "size != other.size, size = " + std::to_string(Size()) +
          ", other.size = " + std::to_string(other.Size()));
  }

  DataType data_;
};

}  // namespace math

// overrides of std methods

namespace std {

template <class T>
T std::abs(const math::Vector<T>& v) noexcept {
  return v.Abs();
}

}  // namespace std

#endif  // MATH_LIBRARIES_CPP_MATH_VECTOR_H_
