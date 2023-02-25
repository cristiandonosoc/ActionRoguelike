#pragma once

#include <assert.h>

// NotNullPtr is very similar to TNotNullPtr, with the difference that it will assert on assignment
// rather than on utilization time as the unreal one will do. In *most* cases, you want to detect
// a null pointer the moment it is obtained, rather than when it is used, which can be way later.
template <typename T>
class NotNullPtr
{
	template <typename OtherT>
	friend class NotNullPtr;

public:
	NotNullPtr() = delete;

	explicit NotNullPtr(std::nullptr_t) : Pointer(nullptr)
	{
		static_assert(sizeof(T) == 0, "Initializing NotNullPtr with nullptr");
	}

	// Constructing from pointer.
	template <typename OtherT, typename = std::enable_if_t<std::is_convertible_v<OtherT*, T*>>>
	NotNullPtr(OtherT* pointer) : Pointer(pointer)
	{
		assert(pointer);  // "Initializing a null pointer to NotNullPtr.
	}

	// Constructing from other NotNullPtr pointers.
	template <typename OtherT, typename = std::enable_if_t<std::is_convertible_v<OtherT*, T*>>>
	NotNullPtr(NotNullPtr<OtherT>& other) : Pointer(other.Pointer)
	{
	}

	// Assigning from nullptr.
	NotNullPtr& operator=(std::nullptr_t)
	{
		static_assert(sizeof(T) == 0, "Initializing NotNullPtr with nullptr");
		return nullptr;
	}

	// Assigning from other pointers.
	template <typename OtherT, typename = std::enable_if_t<std::is_convertible_v<OtherT*, T*>>>
	NotNullPtr& operator=(OtherT* pointer)
	{
		assert(pointer); // Assigning a null pointer to NotNullPtr.
		Pointer = pointer;
		return *this;
	}

	// Assigning from other NotNullPtr.
	template <typename OtherT, typename = std::enable_if_t<std::is_convertible_v<OtherT*, T*>>>
	NotNullPtr& operator=(NotNullPtr<OtherT>& other)
	{
		Pointer = other.Pointer;
		return *this;
	}

	operator T*() const { return Pointer; }
	T* Get() const { return Pointer; }
	T& operator*() const { return *Pointer; }
	T* operator->() const { return Pointer; }

private:
	T* Pointer;
};

template <typename T>
NotNullPtr(T*) -> NotNullPtr<T>;