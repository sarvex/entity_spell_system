#pragma once
#include <System/System.h>
#include "SerializationBuffer.h"

using namespace CxNetworking::Utils;
using namespace System;
namespace BS {
	namespace Networking {
		struct RemoveCPlayerSpellCooldownDataMsg : public virtual ValueType
		{
				int SpellId;
				void Serialize(SerializationBuffer* buf);
				void Deserialize(SerializationBuffer* buf);
				RemoveCPlayerSpellCooldownDataMsg();
				String* ToString();
		};
	}
}
