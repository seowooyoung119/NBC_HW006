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

private:
	FTimerHandle TimerHandle_BoxTimer;

};
