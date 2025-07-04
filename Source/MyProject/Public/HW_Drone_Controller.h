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

	/*맵핑 컨텍스트와 액션을 설정하는 함수*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputMappingContext* DroneMappingContext;

		// WASD 키 입력을 위한 액션
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
		UInputAction* MoveAction;

		// Drone의 회전을 위한 액션
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
		UInputAction* RollAction;

		// Drone의 Up을 위한 액션 (Shift키)
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
		UInputAction* MoveUpAction;

		// Drone의 Down을 위한 액션 (Spacebar)
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
		UInputAction* MoveDownAction;

		// Drone의 Down을 위한 액션 (Spacebar)
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
		UInputAction* CameraAction;

		virtual void BeginPlay() override;
};
