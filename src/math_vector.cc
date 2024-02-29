#include "math_vector.h"

#include <algorithm>
#include <cmath>
#include <numeric>

namespace math {

vector::vector() noexcept : vector(3UL, value_type()) {}

vector::vector(size_type size, value_type value) {
  if (!size) {
    throw std::invalid_argument("Vector size can not be 0");
  }

  data_ = data_type(size, value);
}

vector::vector(const std::initializer_list<value_type>& list)
    : vector(list.begin(), list.end()) {}

vector::vector(value_type x1, value_type x2) noexcept : vector{x1, x2} {}

vector::vector(value_type x1, value_type x2, value_type x3) noexcept
    : vector{x1, x2, x3} {}

vector::size_type vector::size() const noexcept { return data_.size(); }

vector::reference vector::operator[](size_type pos) noexcept {
  return data_[pos];
}

vector::const_reference vector::operator[](size_type pos) const noexcept {
  return data_[pos];
}

vector::reference vector::at(size_type pos) {
  check_size_for_getter(pos);
  return data_[pos];
}

vector::const_reference vector::at(size_type pos) const {
  check_size_for_getter(pos);
  return data_[pos];
}

vector::reference vector::operator()(size_type pos) { return at(pos); }

vector::const_reference vector::operator()(size_type pos) const {
  return at(pos);
}

vector::iterator vector::begin() noexcept { return data_.begin(); }

vector::const_iterator vector::begin() const noexcept { return data_.begin(); }

vector::iterator vector::end() noexcept { return data_.end(); }

vector::const_iterator vector::end() const noexcept { return data_.end(); }

vector::const_iterator vector::cbegin() const noexcept {
  return data_.cbegin();
}

vector::const_iterator vector::cend() const noexcept { return data_.cend(); }

vector::reverse_iterator vector::rbegin() noexcept { return data_.rbegin(); }

vector::reverse_iterator vector::rend() noexcept { return data_.rend(); }

vector::const_reverse_iterator vector::rbegin() const noexcept {
  return data_.rbegin();
}

vector::const_reverse_iterator vector::rend() const noexcept {
  return data_.rend();
}

vector::const_reverse_iterator vector::crbegin() const noexcept {
  return data_.crbegin();
}

vector::const_reverse_iterator vector::crend() const noexcept {
  return data_.crend();
}

std::ostream& operator<<(std::ostream& out, const vector& v) {
  out << '[';
  bool comma = false;
  for (const auto& i : v) {
    if (comma) {
      out << ", ";
    }
    comma = true;
    out << i;
  }
  out << ']';
  return out;
}

std::istream& operator>>(std::istream& in, vector& v) {
  for (auto& i : v) {
    in >> i;
    if (in.fail()) break;
  }
  return in;
}

bool operator==(const vector& l, const vector& r) noexcept {
  return std::equal(l.begin(), l.end(), r.begin(), r.end());
}

bool operator!=(const vector& l, const vector& r) noexcept { return !(l == r); }

vector& vector::operator+=(const vector& other) {
  extend(other.size());
  std::transform(begin(), end(), other.begin(), begin(),
                 std::plus<value_type>());
  return *this;
}

vector& vector::operator-=(const vector& other) {
  extend(other.size());
  std::transform(begin(), end(), other.begin(), begin(),
                 std::minus<value_type>());
  return *this;
}

vector& vector::operator*=(const_reference value) noexcept {
  std::transform(begin(), end(), begin(),
                 [&value](const_reference v) { return v * value; });
  return *this;
}

// Divide vector values bu value
vector& vector::operator/=(const_reference value) noexcept {
  std::transform(begin(), end(), begin(),
                 [&value](const_reference v) { return v / value; });
  return *this;
}

vector operator+(const vector& l, const vector& r) {
  vector result(l);
  result += r;
  return result;
}

vector operator-(const vector& l, const vector& r) {
  vector result(l);
  result -= r;
  return result;
}

vector operator*(const vector& v, vector::const_reference value) {
  vector result(v);
  result *= value;
  return result;
}

vector operator/(const vector& v, vector::const_reference value) {
  vector result(v);
  result /= value;
  return result;
}

vector operator*(vector::const_reference value, const vector& v) {
  return v * value;
}

vector::value_type operator*(const vector& l, const vector& r) {
  l.check_size_for_operation(r);

  vector::value_type result = 0;
  auto i1 = l.begin(), i2 = r.begin();
  while (i1 != l.end()) {
    result += *(i1++) * *(i2++);
  }

  return result;
}

void vector::resize(size_type new_size, const_reference value) {
  if (!new_size) throw std::invalid_argument("Vector size can not be 0");
  data_.resize(new_size, value);
}

void vector::extend(size_type new_size, const value_type& value) {
  if (new_size > size()) data_.resize(new_size, value);
}

vector::value_type vector::abs() const noexcept {
  value_type result = 0;
  return sqrt(std::accumulate(
      begin(), end(), 0,
      [](const_reference l, const_reference r) { return l + r * r; }));
}

void vector::check_size_for_getter(size_type pos) const {
  if (pos >= size())
    throw std::out_of_range(std::string("pos >= size, pos = ") +
                            std::to_string(pos) +
                            ", size = " + std::to_string(size()));
}

void vector::check_size_for_operation(const vector& other) const {
  if (size() != other.size())
    throw std::invalid_argument(
        "size != other.size, size = " + std::to_string(size()) +
        ", other.size = " + std::to_string(other.size()));
}

}  // namespace math

namespace std {

math::vector::value_type abs(const math::vector& v) noexcept { return v.abs(); }

}  // namespace std
