#ifndef MATH_LIBRARIES_CPP_MATH_COMPLEX_H_
#define MATH_LIBRARIES_CPP_MATH_COMPLEX_H_

#include <cmath>
#include <istream>
#include <ostream>

namespace math {

/**
 * @brief Class for working with mathematical complex numbers.
 * Supports almost all operators and conversions.
 * Can be written in arithmetic expressions like "(5 + 2_i) - 8" - means that
 * complex number (5+2i) is subtracted with complex number (8+0i) and result
 * is complex (-3+2i).
 *
 * @tparam T was designed to be numeric, e. g. double, float, int, ...
 */
template <class T = float>
class Complex {
 public:
  using ValueType = T;

  // Constructs complex number equals (0 + 0i)
  Complex() noexcept : Complex(0.0, 0.0) {}

  // Constructs complex number equals (a + 0i)
  Complex(ValueType a) noexcept : Complex(a, 0.0) {}

  // Constructs complex number equals (a + bi)
  Complex(ValueType a, ValueType b) noexcept : a_(a), i_(b) {}

  // Constructs complex number equals (p.first + p.second * i)
  explicit Complex(const std::pair<ValueType, ValueType>& p) noexcept
      : Complex(p.first, p.second) {}

  explicit operator std::pair<ValueType, ValueType>() const noexcept {
    return {a_, i_};
  }

  explicit operator bool() const noexcept { return a_ && i_; }

  std::string ToString() const noexcept {
    std::string result;
    if ((!a_ && !i_) || (a_ && !i_)) {
      result += std::to_string(a_);
      return result;
    }
    if (i_ && !a_) {
      result += std::to_string(i_);
      result += 'i';
      return result;
    }
    result += '(';
    result += std::to_string(a_);
    if (i_ > 0) result += '+';
    result += std::to_string(i_);
    result += "i)";
    return result;
  }

  explicit operator std::string() const noexcept { return ToString(); }

  /**
   * @brief Outputs complex number. (0 + 5i) will be outputed as 5i. (5 + 0i)
   will be outputed as 5. (5 + 5i) will be outputed as (5+5i).
   *
   * @param out std::ostream or child object
   * @param c complex number to output
   * @return std::ostream&
   */
  friend std::ostream& operator<<(std::ostream& out, const Complex<T>& c) {
    out << c.ToString();
    return out;
  }

  /**
   * @brief Inputs complex number. Every two numbers will be counted as a and b
   * no matter how far they stand from each other in the stream.
   *
   * @param in std::istream or child object
   * @param c complex number to input
   * @return std::istream&
   */
  friend std::istream& operator>>(std::istream& in, Complex<T>& c) {
    in >> c.a_ >> c.i_;
    return in;
  }

  // Compares two complex numbers like a1 == a2 and b1 == b2.
  bool operator==(const Complex& other) const noexcept {
    return a_ == other.a_ && i_ == other.i_;
  }

  // Compares two complex numbers like a1 != a2 or b1 != b2.
  bool operator!=(const Complex& other) const noexcept {
    return !(*this == other);
  }

  Complex& operator+=(const Complex& other) noexcept {
    a_ += other.a_;
    i_ += other.i_;
    return *this;
  }

  Complex& operator-=(const Complex& other) noexcept {
    a_ -= other.a_;
    i_ -= other.i_;
    return *this;
  }

  Complex& operator*=(const Complex& other) noexcept {
    a_ = a_ * other.a_ - i_ * other.i_;
    i_ = a_ * other.i_ + i_ * other.a_;
    return *this;
  }

  Complex& operator/=(const Complex& other) noexcept {
    ValueType denominator = other.a_ * other.a_ + other.i_ + other.i_;
    a_ = (a_ * other.a_ + i_ * other.i_) / denominator;
    i_ = (other.a_ * i_ - a_ * other.i_) / denominator;
    return *this;
  }

  Complex operator+(const Complex& other) const noexcept {
    Complex result(*this);
    result += other;
    return result;
  }

  Complex operator-(const Complex& other) const noexcept {
    Complex result(*this);
    result -= other;
    return result;
  }

  Complex operator*(const Complex& other) const noexcept {
    Complex result(*this);
    result *= other;
    return result;
  }

  Complex operator/(const Complex& other) const noexcept {
    Complex result(*this);
    result /= other;
    return result;
  }

  friend Complex operator+(ValueType num, const Complex& c) {
    return Complex(num) + c;
  }

  friend Complex operator-(ValueType num, const Complex& c) {
    return Complex(num) - c;
  }

  friend Complex operator*(ValueType num, const Complex& c) {
    return Complex(num) * c;
  }

  friend Complex operator/(ValueType num, const Complex& c) {
    return Complex(num) / c;
  }

  /**
   * @brief Calculates conjugate complex number.
   *
   * @return Complex
   */
  Complex operator*() const noexcept { return Complex(a_, -i_); }

  /**
   * @brief Calculates absolute value of complex number.
   *
   * @return Complex
   */
  Complex Abs() const noexcept { return Complex(sqrt(a_ * a_ + i_ * i_)); }

  /**
   * @brief Calculates conjugate complex number.
   *
   * @return Complex
   */
  Complex Conjugate() const noexcept { return **this; }

  /**
   * @brief Returns real part of complex number.
   *
   * @return ValueType
   */
  ValueType Re() const noexcept { return a_; }

  /**
   * @brief Returns imaginary part of complex number.
   *
   * @return ValueType
   */
  ValueType Im() const noexcept { return i_; }

 private:
  ValueType a_;
  ValueType i_;
};

}  // namespace math

// overrides of std methods

namespace std {

template <class T>
math::Complex<T> abs(const math::Complex<T>& c) {
  return math::Complex(c.Abs());
}

template <class T>
math::Complex<T> pow(const math::Complex<T>& base, unsigned deg) {
  math::Complex result(1.0);
  for (unsigned i = 0U; i < deg; ++i) result *= base;
  return result;
}

}  // namespace std

// literals

math::Complex<long double> operator""_i(long double num) {
  return math::Complex<long double>(0.0, num);
}

math::Complex<long long> operator""_i(unsigned long long num) {
  return math::Complex<long long>(0.0, num);
}

#endif  // MATH_LIBRARIES_CPP_MATH_COMPLEX_H_
