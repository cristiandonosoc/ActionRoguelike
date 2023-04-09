#pragma once

namespace ar
{
namespace fsm
{

template <typename TState>
struct ARBASE_API _StateInstance
{
	using ValueType = TState;
	using Type = _StateInstance<TState>;
	static TState* StaticInstance()
	{
		static TState state;
		return &state;
	}
};

template <typename TFSM, typename TState, typename TInitialState = TState>
class ARBASE_API StateMachine
{
public:
	StateMachine() = default;

public:
	const TState& GetCurrentState() const { return *CurrentState; }

	template <typename S>
	bool IsInState() const
	{
		return CurrentState == _StateInstance<TInitialState>::StaticInstance();
	}

public:
	void Start()
	{
		// Set the initial state.
		// CurrentState = &_StateInstance<TInitialState>::Value;
		CurrentState = _StateInstance<TInitialState>::StaticInstance();
		CurrentState->OnEnter(static_cast<TFSM*>(this));
	}

	void Shutdown()
	{
		check(CurrentState);
		CurrentState->OnExit(static_cast<TFSM*>(this));
	}

	template <typename TTrigger>
	void Trigger(const TTrigger& trigger)
	{
		CurrentState->OnTrigger(static_cast<TFSM*>(this), trigger);
	}

	template <typename TNewState>
	void Transit()
	{
		static_assert(std::is_base_of_v<TState, TNewState>, "TOtherState must inherit from TState");
		static_assert(sizeof(TNewState) == sizeof(TState));
		CurrentState->OnExit(static_cast<TFSM*>(this));
		// CurrentState = &_StateInstance<TNewState>::Value;
		CurrentState = _StateInstance<TNewState>::StaticInstance();
		CurrentState->OnEnter(static_cast<TFSM*>(this));
	}

	template <typename TNewState, typename TActionFunction>
	void Transit(TActionFunction&& action_function)
	{
		static_assert(std::is_base_of_v<TState, TNewState>, "TOtherState must inherit from TState");
		static_assert(sizeof(TNewState) == sizeof(TState));
		CurrentState->OnExit(static_cast<TFSM*>(this));
		action_function();
		// CurrentState = &_StateInstance<TNewState>::Value;
		CurrentState = _StateInstance<TNewState>::StaticInstance();
		CurrentState->OnEnter(static_cast<TFSM*>(this));
	}

	template <typename TNewState, typename TActionFunction, typename TConditionFunction>
	void Transit(TActionFunction&& action_function, TConditionFunction&& condition_function)
	{
		if (condition_function())
		{
			Transit(std::move(action_function));
			return;
		}
	}

private:
	TState* CurrentState = nullptr;
};

} // namespace fsm
} // namespace ar