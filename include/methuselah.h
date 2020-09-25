#pragma once

#include <assert.h>

#include <cmath>
#include <functional>
#include <memory>
#include <numeric>
#include <stdexcept>
#include <string>
#include <vector>

namespace methuselah {

// Exceptions
// ==========------------------------------------------------------------------
class NotImplementedException : public std::runtime_error {
 public:
  NotImplementedException(const std::string& arg) : std::runtime_error(arg) {}
  NotImplementedException() : NotImplementedException("") {}
};

class InvalidOperationException : public std::runtime_error {
 public:
  InvalidOperationException(const std::string& arg) : std::runtime_error(arg) {}
  InvalidOperationException() : InvalidOperationException("") {}
};

// Cell
// ====------------------------------------------------------------------------
template <typename T>
class Cell {
 public:
  Cell() : value(std::make_unique<T>()) {}

  virtual T* get() { return value.get(); }

 private:
  std::unique_ptr<T> value;
};

template <typename T>
class OutOfBoundsCell final : public Cell<T> {
 public:
  OutOfBoundsCell(T* ptr) : ptr(ptr) {}

  T* get() { return ptr; }

 private:
  T* ptr;
};

// Grid
// ====------------------------------------------------------------------------
std::vector<short> const VON_NEUMANN_2D_NEIGHBORHOOD{-3, -1, 1, 3};
std::vector<short> const MOORE_2D_NEIGHBORHOOD{-4, -3, -2, -1, 1, 2, 3, 4};
std::vector<short> const MOORE_1D_NEIGHBORHOOD{-1, 1};

enum Wrapping { BOUNDED, TOROIDAL };

namespace {  // Helper functions
template <typename T>
T multiplyAll(const std::vector<T>& vec) {
  return std::accumulate(vec.begin(), vec.end(), 1, std::multiplies<T>());
}

// Expand each dimension out one unit on each extremity,
// then subtract the size of the resulting shape from the
// original. This gives you the number of cells needed to
// pad the grid by one unit.
size_t determinePadding(const std::vector<size_t>& shape) {
  auto numDims = shape.size();
  auto expanded = std::vector<size_t>(shape);
  for (auto i = 0; i < numDims; ++i) {
    expanded[i] += 2;
  }

  auto size = multiplyAll<size_t>(shape);
  auto expandedSize = multiplyAll<size_t>(shape);
  return expandedSize - size;
}

std::vector<size_t> allZeros(size_t length) {
  auto result = std::vector<size_t>();
  result.insert(result.begin(), length, 0);
  return result;
}
}  // namespace

template <typename T>
class Grid {
 public:
  Grid(const std::vector<size_t>& shape, Wrapping wrapping,
       const std::vector<short>& neighborhood, T defaultValue,
       unsigned short int maxNeighborDistance = 1)
      // TODO: If neighborhood is const, should determine maxNeighborDistance
      // based on the neighborhood provided
      : shape(shape),
        size(multiplyAll<size_t>(shape)),
        padding(determinePadding(shape) * maxNeighborDistance),
        dimensions(shape.size()),
        wrapping(wrapping),
        neighborhood(neighborhood),
        defaultValue(defaultValue),
        deadCell(std::make_unique<T>(defaultValue)) {
    // Initialize grid, based on wrapping
    //_________________________________________________________________________
    //_________________________________________________________________________
    auto coordinate = allZeros(dimensions);
    for (auto i = 0; i < size; ++i) {
      if (isOutOfBounds(coordinate)) {
        switch (wrapping) {
          case Wrapping::BOUNDED:
            throw NotImplementedException();
            break;

          case Wrapping::TOROIDAL:
            throw NotImplementedException();
            break;
        }
      } else {
        throw NotImplementedException();
        // cells.push_back(std::make_unique<Cell<T>>(defaultValue));
      }
      incrementCoordinate(coordinate);
    }
    //_________________________________________________________________________
    //_________________________________________________________________________
  }

 private:
  std::vector<unsigned int> const shape;
  size_t const size;
  size_t const padding;
  unsigned short int const dimensions;
  std::vector<short> const neighborhood;
  Wrapping const wrapping;
  T const defaultValue;
  std::unique_ptr<Cell<T>> const deadCell;
  std::vector<std::unique_ptr<Cell<T>>> cells;

  size_t getIdx(std::vector<size_t> coordinates) {
    if (coordinates.size() != dimensions)
      throw InvalidOperationException(
          "Coordinate dimensions do not match grid's dimensions.");

    size_t result{0};
    for (auto i = 0; i < dimensions; ++i) {
      auto chunk = coordinates[i];
      for (auto j = 1; j < i; ++j) {
        chunk *= shape[j];
      }
      result += chunk;
    }
    return result;
  }

  void incrementCoordinate(std::vector<size_t>& coordinate) {
    throw NotImplementedException();
  }

  bool isOutOfBounds(const std::vector<size_t>& coordinate) {
    throw NotImplementedException();
  }
};

}  // namespace methuselah