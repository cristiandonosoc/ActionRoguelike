#include <ARGameClient/Items/ItemSpawnerClient.h>

namespace ar
{
namespace client
{

bool ItemSpawnerClient::Interact(NotNullPtr<AARBaseItem> item, NotNullPtr<APawn> interactor)
{
	// TODO(cdc): Play some FX if warranted.
	return true;
}

} // namespace client
} // namespace ar
