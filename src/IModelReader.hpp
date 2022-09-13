#pragma once

#include <istream>
#include <memory>

#include "IModel.hpp"

namespace models {
class IModelReader {
 public:
  /// <summary>
  /// Read a model from an input stream.
  /// </summary>
  /// <param name="input">The stream in which we read the model from.</param>
  virtual std::unique_ptr<models::IModel> Read(std::istream input) const = 0;
};
}  // namespace models