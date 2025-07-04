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
	// Tick 설정
	PrimaryActorTick.bCanEverTick = true; // 매 프레임 Tick 함수 호출 가능


	// 루트 컴포넌트 생성
	DroneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	check(DroneRoot);
	SetRootComponent(DroneRoot);

	// 플로팅 폰 무브먼트 컴포넌트 생성
	FloatingPawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("FloatingPawnMovement"));
	check(FloatingPawnMovement);
	FloatingPawnMovement->SetUpdatedComponent(DroneRoot); // 루트 컴포넌트를 업데이트 대상으로 설정

		// 최대 이동 속도 설정
		FloatingPawnMovement->MaxSpeed = 1000.0f; // 드론의 최대 이동 속도 설정

	// 캡슐 컴포넌트 생성
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	check(CapsuleComponent);
	CapsuleComponent->SetupAttachment(DroneRoot);

		// 캡슐 기본 설정
		CapsuleComponent->InitCapsuleSize(50.0f, 50.0f); // 캡슐 반지름과 반높이 설정

	// 메쉬	컴포넌트 생성
	DroneMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DroneMesh"));
	check(DroneMesh);
	DroneMesh->SetupAttachment(CapsuleComponent);
	
		// 메쉬 기본 설정
		static ConstructorHelpers::FObjectFinder<UStaticMesh> 
		DroneMeshAsset(TEXT("/Game/Asset/Meshs/spy_dron.spy_dron"));
		
		check(DroneMeshAsset.Succeeded());
		DroneMesh->SetStaticMesh(DroneMeshAsset.Object); 
		
		// 머티리얼 0번 기본 설정
		static ConstructorHelpers::FObjectFinder<UMaterialInterface> 
		DroneMaterialAsset0(TEXT("/Game/Asset/Materials/Material_011.Material_011"));
		
		check(DroneMaterialAsset0.Succeeded());
		DroneMesh->SetMaterial(0, DroneMaterialAsset0.Object);

		// 머티리얼 1번 기본 설정
		static ConstructorHelpers::FObjectFinder<UMaterialInterface> 
		DroneMaterialAsset1(TEXT("/Game/Asset/Materials/Material_010.Material_010"));
		
        check(DroneMaterialAsset1.Succeeded());
		DroneMesh->SetMaterial(1, DroneMaterialAsset1.Object);

		// 머티리얼 2번 기본 설정
		static ConstructorHelpers::FObjectFinder<UMaterialInterface> 
		DroneMaterialAsset2(TEXT("/Game/Asset/Materials/Material_012.Material_012"));
		
		check(DroneMaterialAsset2.Succeeded());
		DroneMesh->SetMaterial(2, DroneMaterialAsset2.Object);
		 
		// 머티리얼 3번 기본 설정
		static ConstructorHelpers::FObjectFinder<UMaterialInterface> 
		DroneMaterialAsset3(TEXT("/Game/Asset/Materials/Material_013.Material_013"));
		
		check(DroneMaterialAsset3.Succeeded());
		DroneMesh->SetMaterial(3, DroneMaterialAsset3.Object);

	/*스프링 암 컴포넌트 기본 설정*/
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	check(SpringArmComponent);
	SpringArmComponent->SetupAttachment(CapsuleComponent);

		// 스프링 암 기본 설정
		SpringArmComponent->TargetArmLength = 300.0f; // 소켓 길이 설정
		SpringArmComponent->bUsePawnControlRotation = true; // 컨트롤러 회전에 따라 회전하도록 설정
		
	/*카메라 컴포넌트 생성*/
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	check(CameraComponent);
	CameraComponent->SetupAttachment(SpringArmComponent);

		// 카메라 기본 설정
		CameraComponent->bUsePawnControlRotation = false; // 카메라는 컨트롤러의 회전에 따라 회전하지 않도록 설정

		bUseControllerRotationYaw = true; // 카메라의 좌우 회전을 드론이 따라가게 설정
		bUseControllerRotationPitch = true; // 카메라의 상하 움직임을 드론이 따라가게 설정
		bUseControllerRotationRoll = true; // 카메라의 롤 회전을 드론이 따라가게 설정
}

void AHW_Drone::BeginPlay()
{
	Super::BeginPlay();
}

void AHW_Drone::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// 플레이어의 입력을 받아서 향상된 입력 시스템 컴포넌트로 캐스팅.
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	check(EnhancedInputComponent);

	// IA를 가져오기 위해 AHW_Drone_Controller를 캐스팅
	AHW_Drone_Controller* DroneController = Cast<AHW_Drone_Controller>(GetController());
	check(DroneController);

	// WASD 키와 같은 입력 액션을 바인딩합니다.
	check(DroneController->MoveAction)
	EnhancedInputComponent->BindAction(DroneController->MoveAction, 
    ETriggerEvent::Triggered, this, &AHW_Drone::MoveDrone);

	// 드론의 회전을 위한 액션을 바인딩합니다.
	check(DroneController->RollAction)
	EnhancedInputComponent->BindAction(DroneController->RollAction, 
	ETriggerEvent::Triggered, this, &AHW_Drone::RotateDrone);

	// 드론의 상승을 위한 액션을 바인딩합니다. (Shift키)
	check(DroneController->MoveUpAction)
	EnhancedInputComponent->BindAction(DroneController->MoveUpAction, 
	ETriggerEvent::Triggered, this, &AHW_Drone::MoveUp);

	// 드론의 하강을 위한 액션을 바인딩합니다. (Spacebar)
	check(DroneController->MoveDownAction)
	EnhancedInputComponent->BindAction(DroneController->MoveDownAction, 
	ETriggerEvent::Triggered, this, &AHW_Drone::MoveDown);

	//// 드론의 카메라 움직임을 위한 액션을 바인딩합니다. (시작)
	//check(DroneController->CameraAction)
	//EnhancedInputComponent->BindAction(DroneController->CameraAction, 
	//ETriggerEvent::Started, this, &AHW_Drone::OnFreelookStart);

	//// 드론의 카메라 움직임을 위한 액션을 바인딩합니다. (종료)
	//check(DroneController->CameraAction)
	//EnhancedInputComponent->BindAction(DroneController->CameraAction, 
	//ETriggerEvent::Completed, this, &AHW_Drone::OnFreelookEnd);
}

void AHW_Drone::MoveDrone(const FInputActionValue& Value)
{
	// 컨트롤러 유효성 체크
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
	// 컨트롤러 유효성 체크
	check(Controller);

	FVector2D RotationInput = Value.Get<FVector2D>();

	UE_LOG(LogTemp, Log, TEXT("Drone is rotating: X=%f, Y=%f"), RotationInput.X, RotationInput.Y);
	AddControllerYawInput(RotationInput.X * 0.1); // 좌우 회전
	AddControllerPitchInput(RotationInput.Y * 0.1f); // 상하 회전
}

void AHW_Drone::MoveUp(const FInputActionValue& Value)
{
	// 컨트롤러 유효성 체크
	check(Controller);

	// Shift키가 눌려있을 때 상승
	UE_LOG(LogTemp, Log, TEXT("Drone is moving up"));
	AddMovementInput(GetActorUpVector(), 100.0f);
}

void AHW_Drone::MoveDown(const FInputActionValue& Value)
{
	// 컨트롤러 유효성 체크
	check(Controller);

	UE_LOG(LogTemp, Log, TEXT("Drone is moving down"));
	// Spacebar가 눌려있을 때 하강
	AddMovementInput(GetActorUpVector(), -100.0f);
}

//void AHW_Drone::OnFreelookStart(const FInputActionValue& Value)
//{
//	// 컨트롤러 유효성 체크
//	check(Controller);
//	// TODO : 자유로운 카메라 움직임 시작
//}
//
//void AHW_Drone::OnFreelookEnd(const FInputActionValue& Value)
//{
//	// 컨트롤러 유효성 체크
//	check(Controller);
//	// TODO : 자유로운 카메라 움직임 종료
//}