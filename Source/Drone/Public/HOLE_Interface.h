#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HOLE_Interface.generated.h"



UINTERFACE(MinimalAPI)
class UHOLE_Interface : public UInterface
{
	GENERATED_BODY()
};

class DRONE_API IHOLE_Interface
{
	GENERATED_BODY()

public:

	UFUNCTION()
	virtual FVector RandomizePosition() = 0;

	UFUNCTION()
	virtual FRotator RandomizeRotate() = 0;

	UFUNCTION()
	virtual void OnOverlap(UPrimitiveComponent* OverlapComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) = 0;

	UFUNCTION()
	virtual void DestroyHole() = 0;
};
