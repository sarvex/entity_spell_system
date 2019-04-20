#pragma once
#include <System/System.h>
#include "SerializationBuffer.h"

using namespace CxNetworking::Utils;
using namespace System;
namespace BS {
	namespace Networking {
		struct SSendRankValueMsg : public virtual ValueType
		{
				uint Guid;
				int TalentId;
				int Rank;
				void Serialize(SerializationBuffer* buf);
				void Deserialize(SerializationBuffer* buf);
				SSendRankValueMsg();
				String* ToString();
		};
	}
}
