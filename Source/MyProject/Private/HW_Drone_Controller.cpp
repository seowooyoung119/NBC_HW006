#include "HW_Drone_Controller.h"
#include "EnhancedInputSubsystems.h"

AHW_Drone_Controller::AHW_Drone_Controller()
	// �׻� nllptr�� �ʱ�ȭ�Ͽ� ���� üũ ������ �� �ְ� �� ��!
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

	/*���� �÷��̾� ��������*/
	ULocalPlayer* LocalPlayer = GetLocalPlayer(); // ���� �÷��̾ �����ɴϴ�.
	check(LocalPlayer); // ���� �÷��̾ ��ȿ���� Ȯ���մϴ�.

	/*EnhancedInputLocalPlayerSubsystem ��������*/
	UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);
	check(Subsystem);

	/*���� ���ؽ�Ʈ�� ��ȿ���� Ȯ���ϰ� �߰�*/
	check(DroneMappingContext);
	Subsystem->AddMappingContext(DroneMappingContext, 0);
}
