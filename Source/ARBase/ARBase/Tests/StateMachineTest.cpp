#include <ARBase/Logging.h>
#include <ARBase/NotNullPtr.h>
#include <ARBase/StateMachine.h>
#include <AutomationTestModule.h>

// ReSharper disable CppDeclaratorNeverUsed

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FStateMachineTest, "StateMachine.Basic",
								 EAutomationTestFlags::ApplicationContextMask |
									 EAutomationTestFlags::SmokeFilter);

namespace
{

struct TestStateMachine;


struct TestTrigger
{
	int Value;
};

struct ARBASE_API TestBaseState
{
	virtual ~TestBaseState() = default;

	virtual void OnEnter(NotNullPtr<TestStateMachine> fsm) = 0;
	virtual void OnExit(NotNullPtr<TestStateMachine> fsm) = 0;
	virtual void OnTrigger(NotNullPtr<TestStateMachine> fsm, const TestTrigger&) {}
};

struct ARBASE_API StateA : public TestBaseState
{
	virtual void OnEnter(NotNullPtr<TestStateMachine> fsm) override;
	virtual void OnExit(NotNullPtr<TestStateMachine> fsm) override;
	virtual void OnTrigger(NotNullPtr<TestStateMachine> fsm, const TestTrigger& trigger) override;
};

struct ARBASE_API StateB : public TestBaseState
{
	virtual void OnEnter(NotNullPtr<TestStateMachine>) override;
	virtual void OnExit(NotNullPtr<TestStateMachine>) override;
};

struct TestStateMachine : public ar::fsm::StateMachine<TestStateMachine, TestBaseState, StateA>
{
	bool AEnter = false;
	bool AExit = false;
	bool BEnter = false;
	bool BExit = false;
	int TriggerData = 0;
};

void StateA::OnEnter(NotNullPtr<TestStateMachine> fsm)
{
	AR_LOG(LogTemp, Log, TEXT("ON ENTER"));
	fsm->AEnter = true;
}

void StateA::OnExit(NotNullPtr<TestStateMachine> fsm)
{
	AR_LOG(LogTemp, Log, TEXT("ON EXIT"));
	fsm->AExit = true;
}

void StateA::OnTrigger(NotNullPtr<TestStateMachine> fsm, const TestTrigger& trigger)
{
	fsm->TriggerData = trigger.Value;
	fsm->Transit<StateB>();
}

void StateB::OnEnter(NotNullPtr<TestStateMachine> fsm)
{
	AR_LOG(LogTemp, Log, TEXT("ON ENTER"));
	fsm->BEnter = true;
}

void StateB::OnExit(NotNullPtr<TestStateMachine> fsm)
{
	AR_LOG(LogTemp, Log, TEXT("ON EXIT"));
	fsm->BExit = true;
}

} // namespace

bool FStateMachineTest::RunTest(const FString& Parameters)
{
	TestStateMachine fsm;

	TestFalse("", fsm.AEnter);
	TestFalse("", fsm.AExit);
	TestFalse("", fsm.BEnter);
	TestFalse("", fsm.BExit);

	fsm.Start();

	TestTrue("", fsm.AEnter);
	TestFalse("", fsm.AExit);
	TestFalse("", fsm.BEnter);
	TestFalse("", fsm.BExit);


	TestTrigger test_trigger{2};
	fsm.Trigger(test_trigger);

	TestTrue("", fsm.AEnter);
	TestTrue("", fsm.AExit);
	TestTrue("", fsm.BEnter);
	TestFalse("", fsm.BExit);
	TestEqual("", fsm.TriggerData, test_trigger.Value);

	fsm.Shutdown();

	TestTrue("", fsm.AEnter);
	TestTrue("", fsm.AExit);
	TestTrue("", fsm.BEnter);
	TestTrue("", fsm.BExit);
	TestEqual("", fsm.TriggerData, test_trigger.Value);

	return true;
}