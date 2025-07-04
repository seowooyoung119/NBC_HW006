#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "HW_Drone.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UCapsuleComponent;

struct FInputActionValue;

UCLASS()
class MYPROJECT_API AHW_Drone : public APawn
{
	GENERATED_BODY()
protected:

	//bool bFreeLookSotop;
	//FRotator CurrentRotation;
	//FRotator DesiredRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drone|Control")
	float RotationSpeed = 5.0f;

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* DroneRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCapsuleComponent* CapsuleComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* DroneMesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UFloatingPawnMovement* FloatingPawnMovement;

public:
	AHW_Drone();

protected:
	virtual void BeginPlay() override;
	
public:	

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:

	// 드론의 이동을 처리하는 함수
	UFUNCTION()
	void MoveDrone(const FInputActionValue& Value);
	
	// 드론의 회전을 처리하는 함수	
	UFUNCTION()
	void RotateDrone(const FInputActionValue& Value);

	// 드론의 상승을 처리하는 함수 (Shift키)
	UFUNCTION()
	void MoveUp(const FInputActionValue& Value);

	// 드론의 하강을 처리하는 함수 (Spacebar)
	UFUNCTION()
	void MoveDown(const FInputActionValue& Value);
	
	//// 드론의 카메라 움직임을 시작하는 함수
	//UFUNCTION()
	//void OnFreelookStart(const FInputActionValue& Value);

	//// 드론의 카메라 움직임을 종료하는 함수
	//UFUNCTION()
	//void OnFreelookEnd(const FInputActionValue& Value);

};
