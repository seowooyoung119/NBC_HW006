#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "HW_Drone_Controller.generated.h"

class UInputMappingContext;
class UInputAction;

UCLASS()
class MYPROJECT_API AHW_Drone_Controller : public APlayerController
{
	GENERATED_BODY()

public:

	AHW_Drone_Controller();

	/*���� ���ؽ�Ʈ�� �׼��� �����ϴ� �Լ�*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputMappingContext* DroneMappingContext;

		// WASD Ű �Է��� ���� �׼�
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
		UInputAction* MoveAction;

		// Drone�� ȸ���� ���� �׼�
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
		UInputAction* RollAction;

		// Drone�� Up�� ���� �׼� (ShiftŰ)
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
		UInputAction* MoveUpAction;

		// Drone�� Down�� ���� �׼� (Spacebar)
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
		UInputAction* MoveDownAction;

		// Drone�� Down�� ���� �׼� (Spacebar)
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
		UInputAction* CameraAction;

		virtual void BeginPlay() override;
};
