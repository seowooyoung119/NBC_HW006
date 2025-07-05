#include "DR_PlayerController.h"
#include "EnhancedInputSubsystems.h"

ADR_PlayerController::ADR_PlayerController()
	:
	DroneMappingContext(nullptr),
	MoveAction(nullptr),
	LookAction(nullptr),
	MoveUPAction(nullptr),
	MoveDOWNAction(nullptr),
	RollRightAction(nullptr),
	RollLeftAction(nullptr)
{
}

void ADR_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	ULocalPlayer* LocalPlayer = GetLocalPlayer();
	check(LocalPlayer);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(Subsystem);

	check(DroneMappingContext)
	Subsystem->AddMappingContext(DroneMappingContext, 0);
}