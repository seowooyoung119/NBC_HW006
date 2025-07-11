#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HOLE_Interface.h"
#include "Hole.generated.h"

class UBoxComponent;
class UStaticMeshComponent;
class UParticleSystemComponent;
class UAudioComponent;

UCLASS()
class DRONE_API AHole : public AActor, public IHOLE_Interface
{
	GENERATED_BODY()
	
public:	
	AHole();
	
	virtual FVector RandomizePosition() override;
	
	virtual FRotator RandomizeRotate() override;

	virtual void ReverseRandomizePosition();

	virtual float RandomizeTime();

	virtual void Hide_Seek();

	virtual void OnOverlap(UPrimitiveComponent* OverlapComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	virtual void DestroyHole();

	virtual void AttachParticleToComponent(USceneComponent* ParentComponent, FName AttachSocketName = NAME_None);

	virtual void AttachParticleToActor(AActor* ParentActor, FName AttachSocketName = NAME_None);

	virtual void FinishDestroyHole();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Particles")
	TObjectPtr <UParticleSystemComponent> OurParticleSystemComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particles")
	TObjectPtr<UParticleSystem> ParticleSystemAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particles")
	FName SocketName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hole|Components")
	TObjectPtr<UAudioComponent> ExplosionSoundComp;
protected:

	// == 컴포넌트

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hole|Components")
	TObjectPtr<UBoxComponent> HoleCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hole|Components")
	TObjectPtr<UStaticMeshComponent> HoleMesh1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hole|Components")
	TObjectPtr<UStaticMeshComponent> HoleMesh2;
	
	// == 변수 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	TObjectPtr<USoundBase> DestructionSound;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Hole|Settings")
	FVector Position;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Hole|Settings")
	FRotator Rotate;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Hole|Settings")
	float ReverseTime;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hole|Settings")
	bool bIsHidden = false;

};
