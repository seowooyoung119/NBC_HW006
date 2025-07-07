#include "DR_Drone.h"
#include "Components/CapsuleComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "DR_PlayerController.h"
#include "Engine/World.h"
#include "Engine/Engine.h"

// 드론 클래스의 생성자
// 드론의 기본 컴포넌트들을 초기화하고 설정하는 역할
ADR_Drone::ADR_Drone()
	//:
	//// 멤버 변수들을 nullptr로 초기화 (안전한 초기화) - 엔진이 그냥 초기화 해줘서 할 필요가 없음.
	//Capsule(nullptr),
	//Mesh(nullptr),
	//SpringArm(nullptr),
	//Camera(nullptr)
{
	// 매 프레임마다 Tick 함수가 호출되도록 설정
	// 중력, 지면 충돌 등을 실시간으로 처리하기 위해 필요
	PrimaryActorTick.bCanEverTick = true;

	// === 캡슐 컴포넌트 설정 (충돌 처리용) ===
	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	SetRootComponent(Capsule); // 캡슐을 루트 컴포넌트로 설정
	Capsule->SetCapsuleRadius(25.0f);      // 캡슐의 반지름 설정
	Capsule->SetCapsuleHalfHeight(25.0f);  // 캡슐의 반높이 설정
	Capsule->SetSimulatePhysics(false);    // 물리 시뮬레이션 비활성화 (수동 제어)

	// === 화살표 컴포넌트 설정 (방향 표시용) ===
	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	Arrow->SetupAttachment(Capsule); // 캡슐에 부착

	// === 메시 컴포넌트 설정 (드론의 외형) ===
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Capsule); // 캡슐에 부착
	Mesh->SetSimulatePhysics(false); // 물리 시뮬레이션 비활성화

	// === 스프링암 컴포넌트 설정 (카메라 지지대) ===
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(Capsule); // 캡슐에 부착
	SpringArm->TargetArmLength = 300.0f; // 카메라와 드론 사이의 거리
	SpringArm->bUsePawnControlRotation = false; // 폰 회전에 따른 자동 회전 비활성화

	// === 카메라 컴포넌트 설정 ===
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm); // 스프링암에 부착
	Camera->bUsePawnControlRotation = false; // 폰 회전에 따른 자동 회전 비활성화

	// === 컨트롤러 회전 설정 ===
	// 모든 축의 자동 회전을 비활성화하여 수동으로 회전을 제어
	bUseControllerRotationYaw = false;   // 좌우 회전 비활성화
	bUseControllerRotationPitch = false; // 상하 회전 비활성화
	bUseControllerRotationRoll = false;  // 롤 회전 비활성화
}

// 게임 시작 시 호출되는 함수
void ADR_Drone::BeginPlay()
{
	Super::BeginPlay();

	// 게임 시작 시 드론이 지면에 있는지 확인
	// 초기 상태를 정확히 설정하기 위해 필요
	bIsGrounded = CheckGrounded();
}

// 매 프레임마다 호출되는 함수 (실시간 업데이트)
void ADR_Drone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// === 매 프레임마다 수행해야 할 작업들 ===

	// 1. 지면 상태 업데이트 (드론이 땅에 닿아있는지 확인)
	bIsGrounded = CheckGrounded();

	// 2. 중력 적용 (공중에 있을 때만)
	ApplyGravity(DeltaTime);

	// 3. 지면 충돌 처리 (착지 시 속도 조절)
	if (bIsGrounded)
	{
		HandleGroundCollision();
	}
}

// 입력 시스템 설정 함수
// 플레이어의 입력(키보드, 마우스 등)을 드론의 동작과 연결
void ADR_Drone::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Enhanced Input System 사용을 위한 캐스팅
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	check(EnhancedInputComponent); // 캐스팅 성공 확인

	// 플레이어 컨트롤러 가져오기
	ADR_PlayerController* PlayerController = Cast<ADR_PlayerController>(GetController());
	check(PlayerController); // 캐스팅 성공 확인

	// === 입력 액션들을 드론의 함수들과 연결 ===

	// 수평 이동 (WASD 키)
	check(PlayerController->MoveAction);
	EnhancedInputComponent->BindAction(PlayerController->MoveAction, ETriggerEvent::Triggered, this, &ADR_Drone::Move);

	// 시점 회전 (마우스 움직임)
	check(PlayerController->LookAction);
	EnhancedInputComponent->BindAction(PlayerController->LookAction, ETriggerEvent::Triggered, this, &ADR_Drone::Look);

	// 수직 상승 (스페이스바)
	check(PlayerController->MoveUPAction);
	EnhancedInputComponent->BindAction(PlayerController->MoveUPAction, ETriggerEvent::Triggered, this, &ADR_Drone::MoveUP);

	// 수직 하강 (Shift 키)
	check(PlayerController->MoveDOWNAction);
	EnhancedInputComponent->BindAction(PlayerController->MoveDOWNAction, ETriggerEvent::Triggered, this, &ADR_Drone::MoveDOWN);

	// 우측 롤 회전 (Q 키)
	check(PlayerController->RollRightAction);
	EnhancedInputComponent->BindAction(PlayerController->RollRightAction, ETriggerEvent::Triggered, this, &ADR_Drone::RollRight);

	// 좌측 롤 회전 (E 키)
	check(PlayerController->RollLeftAction);
	EnhancedInputComponent->BindAction(PlayerController->RollLeftAction, ETriggerEvent::Triggered, this, &ADR_Drone::RollLeft);

	// 엔진 작동 (p 키)
	check(PlayerController->EngineStartAction);
	EnhancedInputComponent->BindAction(PlayerController->EngineStartAction, ETriggerEvent::Started, this, &ADR_Drone::EngineOnOff);



}

// 중력 적용 함수
// 드론이 공중에 있을 때 자연스럽게 떨어지도록 하는 물리 효과
void ADR_Drone::ApplyGravity(float DeltaTime)
{
	// 공중에 있을 때만 중력 적용
	if (!bIsGrounded)
	{
		// 중력 가속도를 수직 속도에 누적
		// 시간이 지날수록 더 빠르게 떨어짐 (현실적인 중력 효과)
		VerticalVelocity += GravityForce * DeltaTime;

		// 최대 낙하 속도 제한 (무한정 빨라지지 않도록)
		// 터미널 벨로시티 개념 적용
		if (VerticalVelocity < MaxFallSpeed)
		{
			VerticalVelocity = MaxFallSpeed;
		}

		// 계산된 수직 속도를 실제 위치 변화에 적용
		// Z축(수직)으로만 이동
		FVector GravityOffset = FVector(0.0f, 0.0f, VerticalVelocity * DeltaTime);
		AddActorWorldOffset(GravityOffset, true); // true = 충돌 감지 활성화
	}
}

// 지면 충돌 감지 함수
// 드론이 땅에 닿아있는지 확인하는 중요한 함수
bool ADR_Drone::CheckGrounded()
{
	// 월드 객체가 없으면 지면 체크 불가
	if (!GetWorld())
	{
		return false;
	}

	// === 레이캐스팅을 위한 시작점과 끝점 설정 ===
	FVector StartLocation = GetActorLocation(); // 드론의 현재 위치
	FVector EndLocation = StartLocation + FVector(0.0f, 0.0f, -GroundCheckDistance); // 아래쪽으로 일정 거리

	// 충돌 결과를 저장할 변수
	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this); // 자기 자신은 충돌 대상에서 제외

	// SweepTest를 사용한 지면 충돌 감지
	// 점이 아닌 캡슐 모양으로 충돌 검사 (더 정확한 충돌 감지)
	bool bHit = GetWorld()->SweepSingleByChannel(
		HitResult,                    // 충돌 결과
		StartLocation,                // 시작 위치
		EndLocation,                  // 끝 위치
		FQuat::Identity,              // 회전 (회전 없음)
		ECC_WorldStatic,              // 충돌 채널 (정적 오브젝트와만 충돌)
		FCollisionShape::MakeCapsule(Capsule->GetScaledCapsuleRadius(), Capsule->GetScaledCapsuleHalfHeight()), // 캡슐 모양
		QueryParams                   // 충돌 매개변수
	);

	return bHit; // 충돌이 감지되면 true, 아니면 false
}

// 지면 충돌 처리 함수
// 드론이 땅에 착지했을 때의 동작을 처리
void ADR_Drone::HandleGroundCollision()
{
	// 하강 중이었다면 (수직 속도가 음수)
	if (VerticalVelocity < 0.0f)
	{
		// 착지 시 수직 속도를 0으로 초기화
		// 땅을 뚫고 계속 떨어지는 것을 방지
		VerticalVelocity = 0.0f;
	}
}

// 수평 이동 처리 함수 (전후좌우 이동)
// 지상과 공중에서의 이동 속도를 다르게 적용
void ADR_Drone::ProcessHorizontalMovement(const FVector2D& MovementValue, float DeltaTime)
{
	// 지상/공중 상태에 따른 이동 속도 조절
	// 공중에서는 조작이 더 어려워야 현실적 (Acceleration, Deceleration에 영향을 줄 수도 있음)
	// 현재는 MaxSpeed에만 영향을 미치도록 합니다.
	float CurrentMaxSpeed = bIsGrounded ? MaxSpeed : MaxSpeed * AirControlMultiplier;
	float CurrentMinSpeed = bIsGrounded ? MinSpeed : MinSpeed * AirControlMultiplier;


	// 입력값을 3D 벡터로 변환 (X: 전후, Y: 좌우, Z: 0)
	FVector DesiredDirection = FVector(MovementValue.X, MovementValue.Y, 0.0f);

	// 입력 방향 정규화 (대각선 이동 시 속도 균일하게)
	if (!DesiredDirection.IsNearlyZero())
	{
		DesiredDirection.Normalize();
	}

	// ===================== 관성 및 엔진 힘 구현 로직 ===========================

	// 현재 속도 (CurrentVelocity)와 원하는 방향 (DesiredDirection)을 사용하여
	// 드론의 실제 속도를 업데이트합니다.

	// 엔진 켜진 경우 
	if (!DesiredDirection.IsNearlyZero() && true == bIsEngineOn)
	{
		// 목표 속도 (DesiredDirection 방향으로 CurrentMaxSpeed)
		FVector TargetVelocity = DesiredDirection * CurrentMaxSpeed;

		// CurrentVelocity를 TargetVelocity 방향으로 가속
		CurrentVelocity = FMath::VInterpTo(CurrentVelocity, TargetVelocity, DeltaTime, Acceleration);

		// 최소 속도 (MinSpeed) 이상을 유지하도록 강제 (옵션)
		// 현재 속도가 너무 느려지면 MinSpeed를 보장
		if (CurrentVelocity.Size() < CurrentMinSpeed && CurrentMinSpeed > 0.0f)
		{
			// 현재 방향으로 최소 속도만큼 밀어줌
			CurrentVelocity = CurrentVelocity.GetSafeNormal() * CurrentMinSpeed;
		}
	}
	// 2. 엔진 KILL 경우 
	else if(false == bIsEngineOn)
	{
		// CurrentVelocity를 ZeroVector로 감속
		CurrentVelocity = FMath::VInterpTo(CurrentVelocity, FVector::ZeroVector, DeltaTime, Deceleration);

		// CurrentVelocity가 MinSpeed보다 작아지면 0으로 설정하여 완전히 멈추게 함
		// 이렇게 하지 않으면 MinSpeed 이하의 미세한 속도로 계속 움직일 수 있음
		if (CurrentVelocity.SizeSquared() < FMath::Square(CurrentMinSpeed * 0.5f)) // MinSpeed의 절반 제곱보다 작으면 멈춤
		{
			CurrentVelocity = FVector::ZeroVector;
		}
	}

	// ===================== 폰 이동 처리 ===========================
	// CurrentVelocity를 사용하여 드론을 움직입니다.
	// 이는 매 프레임 Tick 함수에서 호출되므로, 지속적으로 이동하게 됩니다.
	if (!CurrentVelocity.IsNearlyZero())
	{
		// 최종 이동 벡터 계산 (CurrentVelocity * DeltaTime)
		// 드론의 로컬 좌표계 기준으로 이동
		AddActorLocalOffset(CurrentVelocity * DeltaTime, true); // true = 충돌 감지 활성화
	}
}

// 수직 이동 처리 함수 (상승/하강)
// 드론의 고도를 조절하는 함수
void ADR_Drone::ProcessVerticalMovement(float VerticalInput, float DeltaTime)
{
	// 지상/공중 상태에 따른 이동 속도 조절
	float CurrentMoveSpeed = bIsGrounded ? MoveSpeed : MoveSpeed * AirControlMultiplier;

	// 수직 이동 벡터 생성 (Z축으로만 이동)
	FVector LocalMoveVector = FVector(0.0f, 0.0f, VerticalInput * CurrentMoveSpeed * DeltaTime);
	AddActorLocalOffset(LocalMoveVector, true); // 충돌 감지 활성화

	// 수직 입력이 있을 때 중력 속도 조절
	// 위로 이동할 때는 중력의 영향을 줄여서 더 부드러운 상승 효과
	if (!FMath::IsNearlyZero(VerticalInput) && VerticalInput > 0.0f)
	{
		// 위로 이동할 때 중력 속도 감소 (부드러운 상승)
		VerticalVelocity = FMath::Max(0.0f, VerticalVelocity);
	}
}

void ADR_Drone::EngineOnOff(const FInputActionValue& Value)
{
	bIsEngineOn = !bIsEngineOn; // 엔진 상태 토글 (켜져 있으면 끄고, 꺼져 있으면 킴)

	if (true == bIsEngineOn) // 엔진이 켜졌을 때
	{
		GravityForce = 0.0f; // 중력 적용	 비활성화
	}

	else if (false == bIsEngineOn) // 엔진이 꺼졌을 때
	{
		GravityForce = -980.0f;
	}
}

// === 입력 처리 함수들 ===
// 각 입력에 대응하는 드론의 동작을 정의

// 수평 이동 입력 처리 (WASD 키)
void ADR_Drone::Move(const FInputActionValue& Value)
{
	check(Controller); // 컨트롤러 존재 확인

	// 입력값을 2D 벡터로 가져오기 (X: 전후, Y: 좌우)
	const FVector2D MovementValue = Value.Get<FVector2D>();

	// 실제 입력이 있을 때만 처리 (매우 작은 값은 무시)
	if (!FMath::IsNearlyZero(MovementValue.X) || !FMath::IsNearlyZero(MovementValue.Y))
	{
		float DeltaTime = GetWorld()->GetDeltaSeconds(); // 프레임 시간 가져오기
		ProcessHorizontalMovement(MovementValue, DeltaTime); // 수평 이동 처리
	}
}

// 시점 회전 입력 처리 (마우스 움직임)
void ADR_Drone::Look(const FInputActionValue& Value)
{
	check(Controller); // 컨트롤러 존재 확인

	// 마우스 입력값 가져오기 (X: 좌우, Y: 상하)
	FVector2D LookValue = Value.Get<FVector2D>();

	float DeltaTime = GetWorld()->GetDeltaSeconds(); // 프레임 시간

	// === 쿼터니언을 사용한 회전 처리 ===
	// 쿼터니언: 3D 회전을 안정적으로 처리하는 수학적 방법

	// 현재 회전을 쿼터니언으로 가져오기
	FQuat CurrentQuat = GetActorQuat();

	// 델타 회전값 계산 (입력 * 회전속도 * 시간)
	float DeltaYaw = LookValue.X * LookSpeed * DeltaTime;     // 좌우 회전
	float DeltaPitch = LookValue.Y * LookSpeed * DeltaTime;   // 상하 회전

	// 각 축별 회전 쿼터니언 생성
	FQuat YawQuat = FQuat(FVector::UpVector, FMath::DegreesToRadians(DeltaYaw));       // Y축 회전
	FQuat PitchQuat = FQuat(FVector::RightVector, FMath::DegreesToRadians(DeltaPitch)); // X축 회전

	// 현재 회전에 델타 회전 적용
	// 순서가 중요: Yaw * Current * Pitch
	FQuat NewQuat = YawQuat * CurrentQuat * PitchQuat;
	NewQuat.Normalize(); // 쿼터니언 정규화 (안정성 확보)

	// 엔진 토글
	if (true == bIsEngineOn)
	{
		SetActorRotation(NewQuat);
	}
	else if (false == bIsEngineOn)
	{
		SpringArm->bUsePawnControlRotation = true;
		AddControllerYawInput(LookValue.X * LookSpeed);
		AddControllerPitchInput(LookValue.Y * LookSpeed);
	}

}

// 우측 롤 회전 입력 처리 (Q 키)
void ADR_Drone::RollRight(const FInputActionValue& Value)
{
	check(Controller); // 컨트롤러 존재 확인

	// 입력값 가져오기 (float 타입)
	const float RollValue = Value.Get<float>();

	// 실제 입력이 있을 때만 처리
	if (!FMath::IsNearlyZero(RollValue))
	{
		float DeltaTime = GetWorld()->GetDeltaSeconds(); // 프레임 시간
		float RollSpeed = 90.0f; // 롤 회전 속도 (도/초)

		// 현재 회전 상태 가져오기
		FQuat CurrentQuat = GetActorQuat();

		// 롤 회전값 계산 (우측으로 회전)
		float DeltaRoll = -RollValue * RollSpeed * DeltaTime;

		// 전진 벡터 축으로 회전하는 쿼터니언 생성
		FQuat RollQuat = FQuat(FVector::ForwardVector, FMath::DegreesToRadians(DeltaRoll));

		// 새로운 회전 적용
		FQuat NewQuat = CurrentQuat * RollQuat;
		NewQuat.Normalize(); // 쿼터니언 정규화


		// 엔진 토글
		if (true == bIsEngineOn)
		{
			SetActorRotation(NewQuat);
		}
		else if (false == bIsEngineOn)
		{
			// 안 함
		}
	}
}

// 좌측 롤 회전 입력 처리 (E 키)
void ADR_Drone::RollLeft(const FInputActionValue& Value)
{
	check(Controller); // 컨트롤러 존재 확인

	// 입력값 가져오기
	const float RollValue = Value.Get<float>();

	// 실제 입력이 있을 때만 처리
	if (!FMath::IsNearlyZero(RollValue))
	{
		float DeltaTime = GetWorld()->GetDeltaSeconds(); // 프레임 시간
		float RollSpeed = 90.0f; // 롤 회전 속도 (도/초)

		// 현재 회전 상태 가져오기
		FQuat CurrentQuat = GetActorQuat();

		// 롤 회전값 계산 (좌측으로 회전 = 음수)
		float DeltaRoll = RollValue * RollSpeed * DeltaTime;

		// 전진 벡터 축으로 회전하는 쿼터니언 생성
		FQuat RollQuat = FQuat(FVector::ForwardVector, FMath::DegreesToRadians(DeltaRoll));

		// 새로운 회전 적용
		FQuat NewQuat = CurrentQuat * RollQuat;
		NewQuat.Normalize(); // 쿼터니언 정규화


		// 엔진 토글
		if (true == bIsEngineOn)
		{
			SetActorRotation(NewQuat);
		}
		else if (false == bIsEngineOn)
		{
			// 안함
		}
	}
}

// 수직 상승 입력 처리 (쉬프트 키)
void ADR_Drone::MoveUP(const FInputActionValue& Value)
{
	check(Controller); // 컨트롤러 존재 확인

	if (false == bIsEngineOn)
	{
		return; // 엔진이 꺼져있으면 상승 입력 무시
	}
	// 입력값 가져오기
	const float VerticalValue = Value.Get<float>();

	// 실제 입력이 있을 때만 처리
	if (!FMath::IsNearlyZero(VerticalValue))
	{
		float DeltaTime = GetWorld()->GetDeltaSeconds(); // 프레임 시간
		ProcessVerticalMovement(VerticalValue, DeltaTime); // 수직 이동 처리 (양수 = 상승)
	}
}

// 수직 하강 입력 처리 (스페이스바 키)
void ADR_Drone::MoveDOWN(const FInputActionValue& Value)
{
	check(Controller); // 컨트롤러 존재 확인

	if (false == bIsEngineOn)
	{
		return; // 엔진이 꺼져있으면 하강 입력 무시
	}
	// 입력값 가져오기
	const float VerticalValue = Value.Get<float>();

	// 실제 입력이 있을 때만 처리
	if (!FMath::IsNearlyZero(VerticalValue))
	{
		float DeltaTime = GetWorld()->GetDeltaSeconds(); // 프레임 시간
		ProcessVerticalMovement(-VerticalValue, DeltaTime); // 수직 이동 처리 (음수 = 하강)
	}
}