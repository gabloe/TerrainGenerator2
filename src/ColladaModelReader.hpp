#pragma once

#include "IModelReader.hpp"

namespace models {
class ColladaModelReader : public models::IModelReader {
 public:
  virtual std::unique_ptr<models::IModel> Read(std::istream input) const;
};
}  // namespace models