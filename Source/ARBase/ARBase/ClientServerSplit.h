#pragma once

#include <ARBase/BuildDefines.h>
#include <ARBase/NotNullPtr.h>

// Client/Server Split - Base Part.
// -------------------------------------------------------------------------------------------------

#define GENERATED_BASE_CLIENT_SERVER_SPLIT(base_class, client_class, server_class)                 \
	GENERATED_BASE_CLIENT_SPLIT(base_class, client_class);                                         \
	GENERATED_BASE_SERVER_SPLIT(base_class, server_class);

#define INIT_BASE_CLIENT_SERVER_SPLIT()                                                            \
	INIT_BASE_CLIENT_SPLIT();                                                                      \
	INIT_BASE_SERVER_SPLIT();

#ifdef AR_BUILD_CLIENT

#define GENERATED_BASE_CLIENT_SPLIT(base_class, client_class)                                      \
private:                                                                                           \
	friend class client_class;                                                                     \
	client_class _ClientSplit;                                                                     \
                                                                                                   \
public:                                                                                            \
	client_class& GetClientSplit()                                                                 \
	{                                                                                              \
		return _ClientSplit;                                                                       \
	}                                                                                              \
	const client_class& GetClientSplit() const                                                     \
	{                                                                                              \
		return _ClientSplit;                                                                       \
	}


#define GENERATED_CLIENT_SPLIT(base_class, client_class)                                           \
private:                                                                                           \
	friend class base_class;                                                                       \
	base_class* Base;                                                                              \
	void InitFromBase(NotNullPtr<base_class> base)                                                 \
	{                                                                                              \
		Base = base.Get();                                                                         \
	}

#define INIT_BASE_CLIENT_SPLIT() _ClientSplit.InitFromBase(this);

#define CLIENT_CALL(function, ...) _ClientSplit.function(__VA_ARGS__)

#else
#define GENERATED_BASE_CLIENT_SPLIT(base_class, client_class)
#define GENERATED_CLIENT_SPLIT(base_class, client_class) #define INIT_BASE_CLIENT_SPLIT()
#define INIT_BASE_CLIENT_SPLIT()
#define CLIENT_CALL(...)
#endif // AR_BUILD_CLIENT


#ifdef AR_BUILD_SERVER

#define GENERATED_BASE_SERVER_SPLIT(base_class, server_class)                                      \
private:                                                                                           \
	friend class server_class;                                                                     \
	server_class _ServerSplit;                                                                     \
                                                                                                   \
public:                                                                                            \
	server_class& GetServerSplit()                                                                 \
	{                                                                                              \
		return _ServerSplit;                                                                       \
	}                                                                                              \
	const server_class& GetServerSplit() const                                                     \
	{                                                                                              \
		return _ServerSplit;                                                                       \
	}

#define GENERATED_SERVER_SPLIT(base_class, server_class)                                           \
private:                                                                                           \
	friend class base_class;                                                                       \
	base_class* Base;                                                                              \
	void InitFromBase(NotNullPtr<base_class> base)                                                 \
	{                                                                                              \
		Base = base.Get();                                                                         \
	}

#define INIT_BASE_SERVER_SPLIT() _ServerSplit.InitFromBase(this);

#define SERVER_CALL(function, ...) _ServerSplit.function(__VA_ARGS__);

#else
#define GENERATED_BASE_SERVER_SPLIT(base_class, server_class)
#define GENERATED_SERVER_SPLIT(base_class, server_class)
#define INIT_BASE_SERVER_SPLIT()
#define SERVER_CALL(...)
#endif // AR_BUILD_SERVER