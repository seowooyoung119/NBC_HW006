#include "HL_Circle.h"

AHL_Circle::AHL_Circle()
{	
	PrimaryActorTick.bCanEverTick = true;
}

void AHL_Circle::BeginPlay()
{
	Super::BeginPlay();
	
	GetWorldTimerManager().SetTimer(TimerHandle_CircleTimer, this, &AHL_Circle::ReverseRandomizePosition, ReverseTime, true);

	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);

	Position = RandomizePosition();
	Rotate = RandomizeRotate();
}

void AHL_Circle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AddActorLocalRotation(Rotate * DeltaTime);
	AddActorLocalOffset(Position * DeltaTime);
}

void AHL_Circle::DestroyHole()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_CircleTimer);

	Super::DestroyHole();
	
}
