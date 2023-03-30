#include <ARGameServer/ServerTest.h>

#include <ARBase/DebugDraw.h>

static constexpr int32 kSomeUniqueNumber = 54934234;
AR_DECLARE_DEBUG_CATEGORY(SERVER_TEST, kSomeUniqueNumber, true, "Server test category");

void ServerTest::PrintFromServer()
{
	ARDebugDraw::Text(kSomeUniqueNumber, TEXT("Hello, from Server!"), FColor::Blue);
}