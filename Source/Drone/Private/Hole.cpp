#include "Hole.h"
#include "Math/UnrealMathUtility.h" 
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Particles/ParticleSystem.h"
#include "Components/SceneComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundBase.h"  

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

	OurParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("OurParticleSystemComponent"));
	checkf(OurParticleSystemComponent != nullptr, TEXT("ParticleSystemComponent OurParticleSystemComponent is nullptr!"));
	SocketName = TEXT("HoleFire");
	OurParticleSystemComponent->SetupAttachment(HoleCollision, SocketName);
	OurParticleSystemComponent->bAutoActivate = false; 
	OurParticleSystemComponent->bAutoDestroy = false;

	ParticleSystemAsset = nullptr;

	// === 사운드 컴포넌트 생성 및 설정 ===
	ExplosionSoundComp = CreateDefaultSubobject<UAudioComponent>(TEXT("ExplosionSoundComp"));
	checkf(ExplosionSoundComp != nullptr, TEXT("AudioComponent OurAudioComponent is nullptr!"));
	ExplosionSoundComp->SetupAttachment(HoleCollision); // 충돌 컴포넌트에 부착
	ExplosionSoundComp->bAutoActivate = false; // BeginPlay에서 재생하지 않음
	ExplosionSoundComp->bAutoDestroy = true;  // 재생이 끝나면 컴포넌트가 자동으로 파괴되도록 설정
	ExplosionSoundComp->SetVolumeMultiplier(1.0f); // 기본 볼륨 설정
	ExplosionSoundComp->SetPitchMultiplier(1.0f);  // 기본 피치 설정

	ReverseTime = RandomizeTime();
}

float AHole::RandomizeTime()
{
	float Min = 1.0f;
	float Max = 5.0f;
	float Time = FMath::RandRange(Min, Max);
	return Time;
}

void AHole::Hide_Seek()
{
	bIsHidden = !bIsHidden;
	SetActorHiddenInGame(bIsHidden);
	SetActorEnableCollision(bIsHidden);

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
	UE_LOG(LogTemp, Warning, TEXT("AHole::DestroyHole called"));

	if (ParticleSystemAsset && OurParticleSystemComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("ParticleSystemAsset is valid, attaching particle system to actor."));


		SetActorEnableCollision(true);
		HoleMesh1->SetVisibility(true);
		HoleMesh2->SetVisibility(true);

		SetActorEnableCollision(false);
		HoleMesh1->SetVisibility(false);
		HoleMesh2->SetVisibility(false);

		OurParticleSystemComponent->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		OurParticleSystemComponent->SetVisibility(true);
		OurParticleSystemComponent->Activate(true);
		OurParticleSystemComponent->bAutoDestroy = true;

		ExplosionSoundComp->SetSound(DestructionSound);
		ExplosionSoundComp->Play();

	}


	FTimerHandle DestroyTimerHandle;
	GetWorldTimerManager().SetTimer(DestroyTimerHandle, this, &AHole::FinishDestroyHole, 2.0f, false);
}

void AHole::FinishDestroyHole()
{
	Super::Destroy();
}

void AHole::AttachParticleToComponent(USceneComponent* InParentComponent, FName AttachSocketName)
{
	checkf(InParentComponent != nullptr, TEXT("InParentComponent is null!"));
    if (OurParticleSystemComponent && InParentComponent)
    {
        OurParticleSystemComponent->AttachToComponent(InParentComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale, AttachSocketName);
        UE_LOG(LogTemp, Log, TEXT("Particle system attached to component: %s on socket: %s"), *InParentComponent->GetName(), *AttachSocketName.ToString());
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to attach particle system. OurParticleSystemComponent or InParentComponent is null."));
    }
}

void AHole::AttachParticleToActor(AActor* ParentActor, FName AttachSocketName)
{
	if (OurParticleSystemComponent && ParentActor && ParentActor->GetRootComponent())
	{
		AttachParticleToComponent(ParentActor->GetRootComponent(), AttachSocketName);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to attach particle system to actor. OurParticleSystemComponent or ParentActor is null, or ParentActor has no RootComponent."));
	}
}
