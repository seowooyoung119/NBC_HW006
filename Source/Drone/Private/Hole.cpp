#include "Hole.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Math/UnrealMathUtility.h" 


AHole::AHole()
{

	HoleCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("HoleComp"));
	checkf(HoleCollision != nullptr, TEXT("BoxComponent HoleCollision is nullptr!"));
	SetRootComponent(HoleCollision);
	HoleCollision->OnComponentBeginOverlap.AddDynamic(this, &AHole::OnOverlap);

	HoleMesh1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HoleMesh1"));
	checkf(HoleMesh1 != nullptr, TEXT("StaticMeshComponent HoleMesh1 is nullptr!"));
	HoleMesh1->SetupAttachment(HoleCollision);

	HoleMesh2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HoleMesh2"));
	checkf(HoleMesh2 != nullptr, TEXT("StaticMeshComponent HoleMesh2 is nullptr!"));
	HoleMesh2->SetupAttachment(HoleCollision);

	ReverseTime = RandomizeTime();
}

float AHole::RandomizeTime()
{
	float Min = 1.0f;
	float Max = 5.0f;
	float Time = FMath::RandRange(Min, Max);
	return Time;
}


FVector AHole::RandomizePosition()
{
	float Min = 100.0f;
	float Max = 500.0f;
	float ZMin = 100.0f;
	float ZMax = 200.0f;
	return FVector(FMath::RandRange(Min, Max), FMath::RandRange(Min, Max), FMath::RandRange(ZMin, ZMax));
}

FRotator AHole::RandomizeRotate()
{
	float Min = 90.0f;
	float Max = 200.0f;
	return FRotator(FMath::RandRange(Min, Max), FMath::RandRange(Min, Max), FMath::RandRange(Min, Max));
}

void AHole::ReverseRandomizePosition()
{
	FVector CurrentPosition = GetActorLocation();

	if (CurrentPosition.Z > 1500.0f || CurrentPosition.Z < 100.0f)
	{
		Position.Z *= -1.0f;
	}
	if (CurrentPosition.X > 1500.0f || CurrentPosition.X < 0.0f)
	{
		Position.X *= -1.0f;
	}
	if (CurrentPosition.Y > 1500.0f || CurrentPosition.Y < 0.0f)
	{
		Position.Y *= -1.0f;
	}
}


void AHole::OnOverlap(UPrimitiveComponent* OverlapComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->ActorHasTag(TEXT("Player")))
	{
		DestroyHole();
	}
}

void AHole::DestroyHole()
{
	Destroy();
}
