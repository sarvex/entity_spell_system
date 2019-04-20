#pragma once
#include <System/System.h>
#include "SerializationBuffer.h"

using namespace CxNetworking::Utils;
using namespace System;
namespace BS {
	namespace Networking {
		struct CSendCraftItemRequestMsg : public virtual ValueType
		{
				int CraftId;
				void Serialize(SerializationBuffer* buf);
				void Deserialize(SerializationBuffer* buf);
				CSendCraftItemRequestMsg();
				String* ToString();
		};
	}
}
