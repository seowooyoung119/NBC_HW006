#include "HL_Box.h"

AHL_Box::AHL_Box()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AHL_Box::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(TimerHandle_BoxTimer, this, &AHL_Box::ReverseRandomizePosition, ReverseTime, true);

	Position = RandomizePosition();
	Rotate = RandomizeRotate();
}

void AHL_Box::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AddActorLocalRotation(Rotate * DeltaTime);
	AddActorLocalOffset(Position * DeltaTime);
}

void AHL_Box::DestroyHole()
{
	Super::DestroyHole();

	GetWorldTimerManager().ClearTimer(TimerHandle_BoxTimer);
}

void AHL_Box::ReverseRandomizePosition()
{
	Super::ReverseRandomizePosition();
}
