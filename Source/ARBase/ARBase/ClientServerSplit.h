#pragma once

#include <ARBase/BuildDefines.h>
// ReSharper disable once CppUnusedIncludeDirective
#include <ARBase/NotNullPtr.h>

// ReSharper disable once CppUnusedIncludeDirective
#include <memory>

// *************************************************************************************************
// CLIENT SERVER SPLIT
// *************************************************************************************************

// ARClientServerGlobals provides useful functions to query on what configuration the game is
// running. The main use is to know if a particular actor is running as part of the server or
// client, even when running on the editor under RUOP (Run Under One Process).
struct ARBASE_API ARClientServerGlobals
{
	static bool RunningInClient(const AActor* actor);
	static bool RunningInClient(const UActorComponent* actor_component)
	{
		check(actor_component);
		return RunningInClient(actor_component->GetOwner());
	}

	static bool RunningInServer(const AActor* actor);
	static bool RunningInServer(const UActorComponent* actor_component)
	{
		check(actor_component);
		return RunningInServer(actor_component->GetOwner());
	}
};

// Base Macros
// -------------------------------------------------------------------------------------------------

// CHECK_RUNNING_ON_CLIENT verifies that the AActor or UActorComponent is running on the client.
// Normally called as CHECK_RUNNING_ON_CLIENT(this).
#define CHECK_RUNNING_ON_CLIENT(actor_or_actor_component)                                          \
	check(ARClientServerGlobals::RunningInClient(actor_or_actor_component));

// CHECK_RUNNING_ON_SERVER verifies that the AActor or UActorComponent is running on the server
// Normally called as CHECK_RUNNING_ON_SERVER(this).
#define CHECK_RUNNING_ON_SERVER(actor_or_actor_component)                                          \
	check(ARClientServerGlobals::RunningInServer(actor_or_actor_component));

// GENERATED_BASE_CLIENT_SERVER_SPLIT generates code for client and server split of a AActor.
// This split depends on where (and how) the code is being compiled.
//
// In the Client/Server split, there are 3 objects:
//
// - Base: The Base is the UObject that is meant to be the "common" part, which runs on both client
//         and server, as well as the common interface both objects that are meant to replicate.
// - ClientSplit: Part that will only be available in modules that have client code.
//                Can be obtained via |GetClientSplit|.
// - ServerSplit: Similar counterpart but for the server.
//                Can be obtained via |GetServerSplit|.

#define INIT_BASE_CLIENT_SERVER_SPLIT()                                                            \
	INIT_BASE_CLIENT_SPLIT();                                                                      \
	INIT_BASE_SERVER_SPLIT();

// CLIENT_SERVER_CALL is meant to create a call the same function for both client and server.
// This is meant for functions that are meant to run at both places, but they call the same
// corresponding functionality at their splits.
//
// In the places where some of them is not available, that part simply will not be called.
//
// IMPORTANT: Returning values make it *A LOT* more confusing, so we have this macros only for void
//            or things where you don't care about the result.
#define CLIENT_SERVER_CALL(function, ...)                                                          \
	CLIENT_ONLY_CALL(function, __VA_ARGS__);                                                       \
	SERVER_ONLY_CALL(function, __VA_ARGS__);

#define __GENERATED_LEAF_COMMON_SPLIT(base_class, leaf_class)                                      \
private:                                                                                           \
	friend class base_class;                                                                       \
	base_class* _Base;                                                                             \
                                                                                                   \
public:                                                                                            \
	leaf_class(base_class* base) : _Base(base)                                                     \
	{                                                                                              \
	}                                                                                              \
                                                                                                   \
public:                                                                                            \
	base_class* GetBase()                                                                          \
	{                                                                                              \
		checkf(_Base, TEXT("did you forget to do INIT_BASE_CLIENT_SERVER_SPLIT?"));                \
		return _Base;                                                                              \
	}                                                                                              \
	const base_class* GetBase() const                                                              \
	{                                                                                              \
		checkf(_Base, TEXT("did you forget to do INIT_BASE_CLIENT_SERVER_SPLIT?"));                \
		return _Base;                                                                              \
	}                                                                                              \
	template <typename THackUsedToNotRequireTheTypeOnHeadersAsItWillBeInlined = void>              \
	TWeakObjectPtr<base_class> GetWeakBase()                                                       \
	{                                                                                              \
		return TWeakObjectPtr<base_class>(GetBase());                                              \
	}                                                                                              \
	template <typename THackUsedToNotRequireTheTypeOnHeadersAsItWillBeInlined = void>              \
	UWorld* GetWorld() const                                                                       \
	{                                                                                              \
		return GetBase()->GetWorld();                                                              \
	}                                                                                              \
	template <typename THackUsedToNotRequireTheTypeOnHeadersAsItWillBeInlined = void>              \
	AActor* GetOwner() const                                                                       \
	{                                                                                              \
		return GetBase()->GetOwner();                                                              \
	}


#define __GENERATED_LEAF_DEFAULT_INIT(base_class)                                                  \
private:                                                                                           \
	void InitFromBase(NotNullPtr<base_class> base)                                                 \
	{                                                                                              \
		_Base = base.Get();                                                                        \
	}

// Client Macros
// -------------------------------------------------------------------------------------------------

#if AR_BUILD_CLIENT

#define GENERATED_BASE_CLIENT_SPLIT(base_class, client_class)                                      \
private:                                                                                           \
	friend class client_class;                                                                     \
	using ClientSplitClass = client_class;                                                         \
	std::unique_ptr<client_class> _ClientSplit;                                                    \
                                                                                                   \
public:                                                                                            \
	NotNullPtr<client_class> GetClientSplit()                                                      \
	{                                                                                              \
		return _ClientSplit.get();                                                                 \
	}                                                                                              \
	NotNullPtr<const client_class> GetClientSplit() const                                          \
	{                                                                                              \
		return _ClientSplit.get();                                                                 \
	}

#define INIT_BASE_CLIENT_SPLIT() _ClientSplit = std::make_unique<ClientSplitClass>(this);

#define GENERATED_LEAF_CLIENT_SPLIT(base_class, client_class)                                      \
	__GENERATED_LEAF_COMMON_SPLIT(base_class, client_class);

// TODO(cdc): Figure out if we can use variadic template and auto to be able to return values from
//            this forwarded calls.
#define CLIENT_ONLY_CALL(function, ...)                                                            \
	do                                                                                             \
	{                                                                                              \
		if (ARClientServerGlobals::RunningInClient(this))                                          \
		{                                                                                          \
			check(_ClientSplit);                                                                   \
			_ClientSplit->function(__VA_ARGS__);                                                   \
		}                                                                                          \
	}                                                                                              \
	while (false)

#else
#define GENERATED_BASE_CLIENT_SPLIT(base_class, client_class)
#define INIT_BASE_CLIENT_SPLIT() std::unique_ptr<void> _PaddingNoClientSplitAvailable;
#define GENERATED_LEAF_CLIENT_SPLIT(base_class)
#endif // AR_BUILD_CLIENT

// Server Macros
// -------------------------------------------------------------------------------------------------

#if AR_BUILD_SERVER

#define GENERATED_BASE_SERVER_SPLIT(base_class, server_class)                                      \
private:                                                                                           \
	friend class server_class;                                                                     \
	using ServerSplitClass = server_class;                                                         \
	std::unique_ptr<server_class> _ServerSplit;                                                    \
                                                                                                   \
public:                                                                                            \
	NotNullPtr<server_class> GetServerSplit()                                                      \
	{                                                                                              \
		return _ServerSplit.get();                                                                 \
	}                                                                                              \
	NotNullPtr<const server_class> GetServerSplit() const                                          \
	{                                                                                              \
		return _ServerSplit.get();                                                                 \
	}

#define INIT_BASE_SERVER_SPLIT() _ServerSplit = std::make_unique<ServerSplitClass>(this);

#define GENERATED_LEAF_SERVER_SPLIT(base_class, server_class)                                      \
	__GENERATED_LEAF_COMMON_SPLIT(base_class, server_class);

// TODO(cdc): Figure out if we can use variadic template and auto to be able to return values from
//            this forwarded calls.
#define SERVER_ONLY_CALL(function, ...)                                                            \
	do                                                                                             \
	{                                                                                              \
		if (ARClientServerGlobals::RunningInServer(this))                                          \
		{                                                                                          \
			check(_ServerSplit);                                                                   \
			_ServerSplit->function(__VA_ARGS__);                                                   \
		}                                                                                          \
	}                                                                                              \
	while (false)

#else
#define GENERATED_BASE_SERVER_SPLIT(base_class, server_class)
#define INIT_BASE_SERVER_SPLIT() std::unique_ptr<void> _PaddingNoServerSplitAvailable;
#define GENERATED_LEAF_SERVER_SPLIT(base_class)
#endif // AR_BUILD_SERVER