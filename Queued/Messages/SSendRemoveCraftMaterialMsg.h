#pragma once
#include <System/System.h>
#include "SerializationBuffer.h"

using namespace CxNetworking::Utils;
using namespace System;
namespace BS {
	namespace Networking {
		struct SSendRemoveCraftMaterialMsg : public virtual ValueType
		{
				int ItemId;
				int Count;
				void Serialize(SerializationBuffer* buf);
				void Deserialize(SerializationBuffer* buf);
				SSendRemoveCraftMaterialMsg();
				String* ToString();
		};
	}
}
