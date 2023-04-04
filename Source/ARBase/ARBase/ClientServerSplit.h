#pragma once

#include <ARBase/BuildDefines.h>
#include <ARBase/NotNullPtr.h>

// *************************************************************************************************
// CLIENT SERVER SPLIT
// *************************************************************************************************

// ARClientServerGlobals provides useful functions to query on what configuration the game is
// running. The main use is to know if a particular actor is running as part of the server or
// client, even when running on the editor under RUOP (Run Under One Process).
struct ARBASE_API ARClientServerGlobals
{
	static bool RunningInClient(NotNullPtr<const AActor> actor);
	static bool RunningInClient(NotNullPtr<const UActorComponent> actor_component)
	{
		return RunningInClient(actor_component->GetOwner());
	}

	static bool RunningInServer(NotNullPtr<const AActor> actor);
	static bool RunningInServer(NotNullPtr<const UActorComponent> actor_component)
	{
		return RunningInServer(actor_component->GetOwner());
	}
};

// Base Macros
// -------------------------------------------------------------------------------------------------

#define CHECK_RUNNING_ON_CLIENT() check(ARClientServerGlobals::RunningInClient(this));
#define CHECK_RUNNING_ON_SERVER() check(ARClientServerGlobals::RunningInServer(this));

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

#define __GENERATED_LEAF_COMMON_SPLIT(base_class)                                                  \
	__GENERATED_LEAF_COMMON_SPLIT_NO_INIT(base_class);                                             \
	__GENERATED_LEAF_DEFAULT_INIT(base_class);

#define __GENERATED_LEAF_COMMON_SPLIT_NO_INIT(base_class)                                          \
private:                                                                                           \
	friend class base_class;                                                                       \
	base_class* _Base = nullptr;                                                                   \
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
	client_class _ClientSplit;                                                                     \
                                                                                                   \
public:                                                                                            \
	client_class* GetClientSplit()                                                                 \
	{                                                                                              \
		return &_ClientSplit;                                                                      \
	}                                                                                              \
	const client_class* GetClientSplit() const                                                     \
	{                                                                                              \
		return &_ClientSplit;                                                                      \
	}

#define INIT_BASE_CLIENT_SPLIT() _ClientSplit.InitFromBase(this);

#define GENERATED_LEAF_CLIENT_SPLIT(base_class) __GENERATED_LEAF_COMMON_SPLIT(base_class);

// TODO(cdc): Figure out if we can use variadic template and auto to be able to return values from
//            this forwarded calls.
#define CLIENT_ONLY_CALL(function, ...)                                                            \
	do                                                                                             \
	{                                                                                              \
		if (ARClientServerGlobals::RunningInClient(this))                                          \
		{                                                                                          \
			_ClientSplit.function(__VA_ARGS__);                                                    \
		}                                                                                          \
	}                                                                                              \
	while (false)

#else
#define GENERATED_BASE_CLIENT_SPLIT(base_class, client_class)
#define INIT_BASE_CLIENT_SPLIT()
#define GENERATED_LEAF_CLIENT_SPLIT(base_class)
#endif // AR_BUILD_CLIENT

// Server Macros
// -------------------------------------------------------------------------------------------------

#if AR_BUILD_SERVER

#define GENERATED_BASE_SERVER_SPLIT(base_class, server_class)                                      \
private:                                                                                           \
	server_class _ServerSplit;                                                                     \
                                                                                                   \
public:                                                                                            \
	server_class* GetServerSplit()                                                                 \
	{                                                                                              \
		return &_ServerSplit;                                                                      \
	}                                                                                              \
	const server_class* GetServerSplit() const                                                     \
	{                                                                                              \
		return &_ServerSplit;                                                                      \
	}

#define INIT_BASE_SERVER_SPLIT() _ServerSplit.InitFromBase(this);

#define GENERATED_LEAF_SERVER_SPLIT(base_class) __GENERATED_LEAF_COMMON_SPLIT(base_class)

// TODO(cdc): Figure out if we can use variadic template and auto to be able to return values from
//            this forwarded calls.
#define SERVER_ONLY_CALL(function, ...)                                                            \
	do                                                                                             \
	{                                                                                              \
		if (ARClientServerGlobals::RunningInServer(this))                                          \
		{                                                                                          \
			_ServerSplit.function(__VA_ARGS__);                                                    \
		}                                                                                          \
	}                                                                                              \
	while (false)


#else
#define GENERATED_BASE_SERVER_SPLIT(base_class, server_class)
#define INIT_BASE_SERVER_SPLIT()
#define GENERATED_LEAF_SERVER_SPLIT(base_class)
#endif // AR_BUILD_SERVER