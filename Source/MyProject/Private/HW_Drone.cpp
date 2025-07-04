#include "HW_Drone.h"
#include "HW_Drone_Controller.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"

AHW_Drone::AHW_Drone() 
	: CapsuleComponent(nullptr), 
	  DroneMesh(nullptr), 
	  SpringArmComponent(nullptr), 
	  CameraComponent(nullptr), 
	  FloatingPawnMovement(nullptr)
	  //bFreeLookSotop(false), 
	  //CurrentRotation(FRotator::ZeroRotator), 
	  //DesiredRotation(FRotator::ZeroRotator)
{
	// Tick ����
	PrimaryActorTick.bCanEverTick = true; // �� ������ Tick �Լ� ȣ�� ����


	// ��Ʈ ������Ʈ ����
	DroneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	check(DroneRoot);
	SetRootComponent(DroneRoot);

	// �÷��� �� �����Ʈ ������Ʈ ����
	FloatingPawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("FloatingPawnMovement"));
	check(FloatingPawnMovement);
	FloatingPawnMovement->SetUpdatedComponent(DroneRoot); // ��Ʈ ������Ʈ�� ������Ʈ ������� ����

		// �ִ� �̵� �ӵ� ����
		FloatingPawnMovement->MaxSpeed = 1000.0f; // ����� �ִ� �̵� �ӵ� ����

	// ĸ�� ������Ʈ ����
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	check(CapsuleComponent);
	CapsuleComponent->SetupAttachment(DroneRoot);

		// ĸ�� �⺻ ����
		CapsuleComponent->InitCapsuleSize(50.0f, 50.0f); // ĸ�� �������� �ݳ��� ����

	// �޽�	������Ʈ ����
	DroneMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DroneMesh"));
	check(DroneMesh);
	DroneMesh->SetupAttachment(CapsuleComponent);
	
		// �޽� �⺻ ����
		static ConstructorHelpers::FObjectFinder<UStaticMesh> 
		DroneMeshAsset(TEXT("/Game/Asset/Meshs/spy_dron.spy_dron"));
		
		check(DroneMeshAsset.Succeeded());
		DroneMesh->SetStaticMesh(DroneMeshAsset.Object); 
		
		// ��Ƽ���� 0�� �⺻ ����
		static ConstructorHelpers::FObjectFinder<UMaterialInterface> 
		DroneMaterialAsset0(TEXT("/Game/Asset/Materials/Material_011.Material_011"));
		
		check(DroneMaterialAsset0.Succeeded());
		DroneMesh->SetMaterial(0, DroneMaterialAsset0.Object);

		// ��Ƽ���� 1�� �⺻ ����
		static ConstructorHelpers::FObjectFinder<UMaterialInterface> 
		DroneMaterialAsset1(TEXT("/Game/Asset/Materials/Material_010.Material_010"));
		
        check(DroneMaterialAsset1.Succeeded());
		DroneMesh->SetMaterial(1, DroneMaterialAsset1.Object);

		// ��Ƽ���� 2�� �⺻ ����
		static ConstructorHelpers::FObjectFinder<UMaterialInterface> 
		DroneMaterialAsset2(TEXT("/Game/Asset/Materials/Material_012.Material_012"));
		
		check(DroneMaterialAsset2.Succeeded());
		DroneMesh->SetMaterial(2, DroneMaterialAsset2.Object);
		 
		// ��Ƽ���� 3�� �⺻ ����
		static ConstructorHelpers::FObjectFinder<UMaterialInterface> 
		DroneMaterialAsset3(TEXT("/Game/Asset/Materials/Material_013.Material_013"));
		
		check(DroneMaterialAsset3.Succeeded());
		DroneMesh->SetMaterial(3, DroneMaterialAsset3.Object);

	/*������ �� ������Ʈ �⺻ ����*/
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	check(SpringArmComponent);
	SpringArmComponent->SetupAttachment(CapsuleComponent);

		// ������ �� �⺻ ����
		SpringArmComponent->TargetArmLength = 300.0f; // ���� ���� ����
		SpringArmComponent->bUsePawnControlRotation = true; // ��Ʈ�ѷ� ȸ���� ���� ȸ���ϵ��� ����
		
	/*ī�޶� ������Ʈ ����*/
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	check(CameraComponent);
	CameraComponent->SetupAttachment(SpringArmComponent);

		// ī�޶� �⺻ ����
		CameraComponent->bUsePawnControlRotation = false; // ī�޶�� ��Ʈ�ѷ��� ȸ���� ���� ȸ������ �ʵ��� ����

		bUseControllerRotationYaw = true; // ī�޶��� �¿� ȸ���� ����� ���󰡰� ����
		bUseControllerRotationPitch = true; // ī�޶��� ���� �������� ����� ���󰡰� ����
		bUseControllerRotationRoll = true; // ī�޶��� �� ȸ���� ����� ���󰡰� ����
}

void AHW_Drone::BeginPlay()
{
	Super::BeginPlay();
}

void AHW_Drone::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// �÷��̾��� �Է��� �޾Ƽ� ���� �Է� �ý��� ������Ʈ�� ĳ����.
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	check(EnhancedInputComponent);

	// IA�� �������� ���� AHW_Drone_Controller�� ĳ����
	AHW_Drone_Controller* DroneController = Cast<AHW_Drone_Controller>(GetController());
	check(DroneController);

	// WASD Ű�� ���� �Է� �׼��� ���ε��մϴ�.
	check(DroneController->MoveAction)
	EnhancedInputComponent->BindAction(DroneController->MoveAction, 
    ETriggerEvent::Triggered, this, &AHW_Drone::MoveDrone);

	// ����� ȸ���� ���� �׼��� ���ε��մϴ�.
	check(DroneController->RollAction)
	EnhancedInputComponent->BindAction(DroneController->RollAction, 
	ETriggerEvent::Triggered, this, &AHW_Drone::RotateDrone);

	// ����� ����� ���� �׼��� ���ε��մϴ�. (ShiftŰ)
	check(DroneController->MoveUpAction)
	EnhancedInputComponent->BindAction(DroneController->MoveUpAction, 
	ETriggerEvent::Triggered, this, &AHW_Drone::MoveUp);

	// ����� �ϰ��� ���� �׼��� ���ε��մϴ�. (Spacebar)
	check(DroneController->MoveDownAction)
	EnhancedInputComponent->BindAction(DroneController->MoveDownAction, 
	ETriggerEvent::Triggered, this, &AHW_Drone::MoveDown);

	//// ����� ī�޶� �������� ���� �׼��� ���ε��մϴ�. (����)
	//check(DroneController->CameraAction)
	//EnhancedInputComponent->BindAction(DroneController->CameraAction, 
	//ETriggerEvent::Started, this, &AHW_Drone::OnFreelookStart);

	//// ����� ī�޶� �������� ���� �׼��� ���ε��մϴ�. (����)
	//check(DroneController->CameraAction)
	//EnhancedInputComponent->BindAction(DroneController->CameraAction, 
	//ETriggerEvent::Completed, this, &AHW_Drone::OnFreelookEnd);
}

void AHW_Drone::MoveDrone(const FInputActionValue& Value)
{
	// ��Ʈ�ѷ� ��ȿ�� üũ
	check(Controller);

	const FVector2D MoveInput = Value.Get<FVector2D>();

	if (!FMath::IsNearlyZero(MoveInput.X))
	{
		UE_LOG(LogTemp, Log, TEXT("Drone is moving forward: %f"), MoveInput.X);
		AddMovementInput(GetActorForwardVector(), MoveInput.X);
	}
	
	if (!FMath::IsNearlyZero(MoveInput.Y))
	{
		UE_LOG(LogTemp, Log, TEXT("Drone is moving sideways: %f"), MoveInput.Y);
		AddMovementInput(GetActorRightVector(), MoveInput.Y);
	}
}

void AHW_Drone::RotateDrone(const FInputActionValue& Value)
{
	// ��Ʈ�ѷ� ��ȿ�� üũ
	check(Controller);

	FVector2D RotationInput = Value.Get<FVector2D>();

	UE_LOG(LogTemp, Log, TEXT("Drone is rotating: X=%f, Y=%f"), RotationInput.X, RotationInput.Y);
	AddControllerYawInput(RotationInput.X * 0.1); // �¿� ȸ��
	AddControllerPitchInput(RotationInput.Y * 0.1f); // ���� ȸ��
}

void AHW_Drone::MoveUp(const FInputActionValue& Value)
{
	// ��Ʈ�ѷ� ��ȿ�� üũ
	check(Controller);

	// ShiftŰ�� �������� �� ���
	UE_LOG(LogTemp, Log, TEXT("Drone is moving up"));
	AddMovementInput(GetActorUpVector(), 100.0f);
}

void AHW_Drone::MoveDown(const FInputActionValue& Value)
{
	// ��Ʈ�ѷ� ��ȿ�� üũ
	check(Controller);

	UE_LOG(LogTemp, Log, TEXT("Drone is moving down"));
	// Spacebar�� �������� �� �ϰ�
	AddMovementInput(GetActorUpVector(), -100.0f);
}

//void AHW_Drone::OnFreelookStart(const FInputActionValue& Value)
//{
//	// ��Ʈ�ѷ� ��ȿ�� üũ
//	check(Controller);
//	// TODO : �����ο� ī�޶� ������ ����
//}
//
//void AHW_Drone::OnFreelookEnd(const FInputActionValue& Value)
//{
//	// ��Ʈ�ѷ� ��ȿ�� üũ
//	check(Controller);
//	// TODO : �����ο� ī�޶� ������ ����
//}