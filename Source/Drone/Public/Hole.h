#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HOLE_Interface.h"
#include "Hole.generated.h"

class UBoxComponent;
class UStaticMeshComponent;

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

	virtual void OnOverlap(UPrimitiveComponent* OverlapComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	virtual void DestroyHole() override;

	// == 컴포넌트

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hole|Components")
	TObjectPtr<UBoxComponent> HoleCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hole|Components")
	TObjectPtr<UStaticMeshComponent> HoleMesh1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hole|Components")
	TObjectPtr<UStaticMeshComponent> HoleMesh2;
	

	// == 변수 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hole|Settings")
	FVector Position;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hole|Settings")
	FRotator Rotate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hole|Settings")
	float ReverseTime;


};
