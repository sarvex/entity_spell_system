#pragma once
#include <System/System.h>
#include "SerializationBuffer.h"

using namespace CxNetworking::Utils;
using namespace System;
namespace BS {
	namespace Networking {
		struct PlayerAddMsg : public virtual ValueType
		{
				bool Add;
				bool ForLocalPlayer;
				uint PlayerId;
				String* PlayerName;
				void Serialize(SerializationBuffer* buf);
				void Deserialize(SerializationBuffer* buf);
				PlayerAddMsg();
				String* ToString();
		};
	}
}
