#pragma once

#include <vector>

class UNetConnection;
class UNetMessageChannel;

namespace ar
{

class MessageChannel
{
public:
	using IDType = FName;

private:
	struct RemoteDataAdapter
	{
		TWeakObjectPtr<UNetConnection> NetConnection;
		TWeakObjectPtr<UNetMessageChannel> NetMessageChannel;
	};
	std::vector<RemoteDataAdapter> Adapters;
};


} // namespace ar

