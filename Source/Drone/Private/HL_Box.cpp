#include "HL_Box.h"
#include <DR_Drone.h>

AHL_Box::AHL_Box()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AHL_Box::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(TimerHandle_BoxTimer, this, &AHL_Box::ReverseRandomizePosition, ReverseTime, true);
	GetWorldTimerManager().SetTimer(TimerHandle_BoxTimer, this, &AHL_Box::Hide_Seek, ReverseTime, true);

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
	GetWorldTimerManager().ClearTimer(TimerHandle_BoxTimer);

	Super::DestroyHole();

}

void AHL_Box::Hide_Seek()
{
	Super::Hide_Seek();
}

void AHL_Box::OnOverlap(UPrimitiveComponent* OverlapComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this && OtherActor->ActorHasTag(TEXT("Player")))
	{
		UE_LOG(LogTemp, Warning, TEXT("Drone Engine Toggle Triggered!"));

		TObjectPtr<ADR_Drone> Drone = Cast<ADR_Drone>(OtherActor);

		if (Drone)
		{
			UE_LOG(LogTemp, Warning, TEXT("Drone Engine Toggle: %s"), Drone->bIsEngineOn ? TEXT("On") : TEXT("Off"));
			Drone->EngineOnOff(Drone->bIsEngineOn); // 엔진 상태를 변경하는 함수 호출
		}
	}

	Super::OnOverlap(OverlapComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

}

