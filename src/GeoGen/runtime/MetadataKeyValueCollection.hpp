#pragma once

#include <map>

#include "MetadataValue.hpp"
#include "../utils/OwningMap.hpp"

namespace geogen 
{
	namespace runtime
	{
		class MetadataKeyValueCollection : public MetadataValue, public utils::OwningMap<MetadataValue>
		{
		private:
		public:
			typedef std::map<String, MetadataValue const*>::const_iterator const_iterator;
			typedef std::map<String, MetadataValue*>::iterator iterator;

			MetadataKeyValueCollection(CodeLocation location) : MetadataValue(location) {}

			void MoveKeyValuesFrom(MetadataKeyValueCollection& another);

			virtual MetadataType GetType() const { return METADATA_TYPE_KEYVALUE_COLLECTION; };
		};
	}
}