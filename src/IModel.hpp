#pragma once

#include <vector>

#include <glm/vec3.hpp>

namespace models {

	/// <summary>
/// The interface for a model.
/// </summary>
class IModel {
  /// <summary>
  /// Get the list of vertices for the model.
  /// </summary>
  /// <returns>A constant list of vertices</returns>
  virtual const std::vector<glm::vec3> GetVertices() = 0;

  /// <summary>
  /// Get the list of indices.
  /// </summary>
  /// <returns>A constant list of indixes</returns>
  virtual const std::vector<uint32_t> GetIndixes() const = 0;

  /// <summary>
  /// Get the texture for the model.
  /// </summary>
  /// <returns>The model texture</returns>
  virtual const std::vector<uint8_t> GetTexture() const = 0;
};
}  // namespace model
