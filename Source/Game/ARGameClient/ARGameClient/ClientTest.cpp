#include <ARGameClient/ClientTest.h>

#include <ARBase/DebugDraw.h>

static constexpr int32 kSomeUniqueNumber = 1231123;
AR_DECLARE_DEBUG_CATEGORY(CLIENT_TEST, kSomeUniqueNumber, true, "Client test category");

void ClientTest::PrintFromClient()
{
	ARDebugDraw::Text(kSomeUniqueNumber, TEXT("Hello, from Client!"), FColor::Green); 
}