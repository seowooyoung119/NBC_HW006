#include "HoleSpawner.h"
#include "Hole.h"
#include "HL_Circle.h"
#include "HL_Box.h"
#include "Kismet/GameplayStatics.h"

AHL_CircleSpawner::AHL_CircleSpawner()
{
	PrimaryActorTick.bCanEverTick = false;

    Count = 10; // �⺻ ���� ����
}

void AHL_CircleSpawner::BeginPlay()
{
	Super::BeginPlay();

	if (0 == SpawnClasses.Num())
	{
		UE_LOG(LogTemp, Warning, TEXT("SpawnClasses array is empty!")); return;
	}

    // ���� �õ� 
    FMath::SRandInit(FPlatformTime::Cycles());

    for (int i = 0; i < Count; ++i)
    {
        int32 Index = FMath::RandRange(0, SpawnClasses.Num() - 1);
        TSubclassOf<AHole> SpawnClass = SpawnClasses[Index];

        if (SpawnClass)
        {
            FVector SpawnLocation = GenerateRandomSpawnLocation();
            FRotator SpawnRotation = FRotator::ZeroRotator;

            FActorSpawnParameters SpawnParams;
            SpawnParams.Owner = this;

            GetWorld()->SpawnActor<AHole>(SpawnClass, SpawnLocation, SpawnRotation, SpawnParams);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("SpawnClass is null at index %d."), Index);
        }
    }

}


FVector AHL_CircleSpawner::GenerateRandomSpawnLocation()
{
	float Min = 5000.0f;
	float Max = 7000.0f;

	return FVector(
		FMath::RandRange(Min, Max),
		FMath::RandRange(Min, Max),
		FMath::RandRange(Min, Max)
	);
}
