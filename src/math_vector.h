#ifndef MATH_LIBRARIES_CPP_MATH_VECTOR_H_
#define MATH_LIBRARIES_CPP_MATH_VECTOR_H_

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
 * @tparam T was designed to be numeric, e. g. double, float, int, ...
 */
template <class T = float>
class Vector {
 public:
  using ValueType = T;
  using Reference = ValueType&;
  using ConstReference = const ValueType&;
  using DataType = std::vector<ValueType>;
  using SizeType = std::size_t;
  using Iterator = typename DataType::iterator;
  using ConstIterator = typename DataType::const_iterator;

  // for foreach compatibility
  using iterator = Iterator;
  using const_iterator = ConstIterator;

  /**
   * @brief Construct a new vector of size 3 filled by 0
   *
   */
  explicit Vector() noexcept : Vector(3UL, 0) {}

  /**
   * @brief Construct a new vector of size filled by value (0 by default)
   *
   * @param size size of vector
   * @param value what to fill vector with
   */
  explicit Vector(SizeType size, ValueType value = 0) {
    if (!size) throw std::invalid_argument("Vector size can not be 0");
    data_ = DataType(size, value);
  }

  /**
   * @brief Construct a new vector from initializer list
   *
   * @param list initializer list which size is not 0
   */
  explicit Vector(const std::initializer_list<ValueType>& list) noexcept {
    static_assert(list.size(), "Vector initializer list can not be empty");
    data_.reserve(list.size());
    for (const auto& i : list) data_.push_back(i);
    data_.shrink_to_fit();
  }

  /**
   * @brief Construct a new vector of size 2
   *
   * @param x1 X coordinate
   * @param x2 Y coordinate
   */
  Vector(ValueType x1, ValueType x2) noexcept : Vector{x1, x2} {}

  /**
   * @brief Construct a new vector of size 3
   *
   * @param x1 X coordinate
   * @param x2 Y coordinate
   * @param x3 Z coordinate
   */
  Vector(ValueType x1, ValueType x2, ValueType x3) noexcept
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
    return in;
  }

  Vector& operator+=(const Vector& other) {
    CheckSizeForOperation(other);
    auto i1 = Begin();
    for (const auto& i2 : other) *(i1++) += i2;
    return *this;
  }

  Vector& operator-=(const Vector& other) {
    CheckSizeForOperation(other);
    auto i1 = Begin();
    for (const auto& i2 : other) *(i1++) -= i2;
    return *this;
  }

  Vector operator+(const Vector& other) {
    CheckSizeForOperation(other);
    Vector result(*this);
    result += other;
    return result;
  }

  Vector operator-(const Vector* other) {
    CheckSizeForOperation(other);
    Vector result(*this);
    result -= other;
    return result;
  }

  void Resize(SizeType new_size) noexcept { if () }

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

#endif  // MATH_LIBRARIES_CPP_MATH_VECTOR_H_
