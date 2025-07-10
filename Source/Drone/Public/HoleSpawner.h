#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HoleSpawner.generated.h"

class AHole;

UCLASS()
class DRONE_API AHL_CircleSpawner : public AActor
{
	GENERATED_BODY()

public:
	AHL_CircleSpawner();

protected:
	virtual void BeginPlay() override;

private:
	FVector GenerateRandomSpawnLocation();

	UPROPERTY(EditAnywhere, Category = "Spawning")
	TArray<TSubclassOf<AHole>> SpawnClasses;

	UPROPERTY(EditAnywhere, Category = "Spawning")
	int32 Count;
};
