#include "ColladaModelReader.hpp"

namespace models
{
	std::unique_ptr<models::IModel> ColladaModelReader::Read(std::istream inputStream) const
	{
		return std::unique_ptr<models::IModel>{nullptr};
	}
}