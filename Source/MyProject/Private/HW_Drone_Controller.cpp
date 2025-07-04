#include "HW_Drone_Controller.h"
#include "EnhancedInputSubsystems.h"

AHW_Drone_Controller::AHW_Drone_Controller()
	// 항상 nllptr로 초기화하여 상태 체크 정의할 수 있게 할 것!
	: DroneMappingContext(nullptr),
	  MoveAction(nullptr), 
	  RollAction(nullptr),
	  MoveUpAction(nullptr),
	  MoveDownAction(nullptr),
	  CameraAction(nullptr)
{
}

void AHW_Drone_Controller::BeginPlay()
{
    Super::BeginPlay();

	/*로컬 플레이어 가져오기*/
	ULocalPlayer* LocalPlayer = GetLocalPlayer(); // 로컬 플레이어를 가져옵니다.
	check(LocalPlayer); // 로컬 플레이어가 유효한지 확인합니다.

	/*EnhancedInputLocalPlayerSubsystem 가져오기*/
	UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);
	check(Subsystem);

	/*맵핑 컨텍스트가 유효한지 확인하고 추가*/
	check(DroneMappingContext);
	Subsystem->AddMappingContext(DroneMappingContext, 0);
}
