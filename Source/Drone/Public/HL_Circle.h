#pragma once

#include "CoreMinimal.h"
#include "Hole.h"
#include "HL_Circle.generated.h"


UCLASS()
class DRONE_API AHL_Circle : public AHole
{
	GENERATED_BODY()
	
public:
	AHL_Circle();	

	void BeginPlay() override;
	void Tick(float DeltaTime) override;
	void DestroyHole() override;

private:
	FTimerHandle TimerHandle_CircleTimer;

};
