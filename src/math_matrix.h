#ifndef MATH_LIBRARIES_CPP_MATH_MATRIX_H_
#define MATH_LIBRARIES_CPP_MATH_MATRIX_H_

#include <istream>
#include <ostream>
#include <stdexcept>
#include <vector>

namespace math {

/**
 * @brief Matrix class to work with matrices as mathematical object.
 *
 * @tparam T was designed to be numeric, e. g. double, float, int, ...
 */
template <class T = float>
class Matrix {
 public:
  using ValueType = T;
  using Reference = ValueType&;
  using ConstReference = const ValueType&;
  using ConstPointer = const ValueType*;
  using DataType = std::vector<ValueType>;
  using SizeType = std::size_t;

  // Constructs 3x3 identity matrix
  Matrix(ValueType diag = 1) : Matrix(3, diag) {}

  /**
   * @brief Constructs size x size identity matrix, but the diagonal filled with
   * diag
   *
   * @param size matrix sizes
   * @param diag diagonal element, defaults to 1
   */
  Matrix(SizeType size, ValueType diag = 1) : Matrix(size, size) {
    for (SizeType i = 0UL; i < rows_; ++i)  // ...
  }

  // Constructs rows x columns matrix filled by default values
  Matrix(SizeType rows, SizeType columns) : rows_(rows), columns_(columns) {
    if (!rows_ || !columns_)
      throw std::invalid_argument("Matrix dimensions can not be 0");
    data_ = DataType(rows_ * columns_, ValueType());
  }

 private:
  ValueType& GetElement(SizeType row, SizeType column) noexcept {
    return data_[]
  }

  SizeType rows_, columns_;
  DataType data_;
};

}  // namespace math

#endif  // MATH_LIBRARIES_CPP_MATH_MATRIX_H_
