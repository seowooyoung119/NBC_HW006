#pragma once

#include "CoreMinimal.h"
#include "Hole.h"
#include "HL_Box.generated.h"

UCLASS()
class DRONE_API AHL_Box : public AHole
{
	GENERATED_BODY()

public:
	AHL_Box();

	void BeginPlay() override;

	void Tick(float DeltaTime) override;

	void DestroyHole() override;

	void Hide_Seek() override;

	void OnOverlap(UPrimitiveComponent* OverlapComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

private:
	FTimerHandle TimerHandle_BoxTimer;

	bool Stop = false;
};
