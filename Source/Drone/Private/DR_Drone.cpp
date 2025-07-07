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

// ��� Ŭ������ ������
// ����� �⺻ ������Ʈ���� �ʱ�ȭ�ϰ� �����ϴ� ����
ADR_Drone::ADR_Drone()
	//:
	//// ��� �������� nullptr�� �ʱ�ȭ (������ �ʱ�ȭ) - ������ �׳� �ʱ�ȭ ���༭ �� �ʿ䰡 ����.
	//Capsule(nullptr),
	//Mesh(nullptr),
	//SpringArm(nullptr),
	//Camera(nullptr)
{
	// �� �����Ӹ��� Tick �Լ��� ȣ��ǵ��� ����
	// �߷�, ���� �浹 ���� �ǽð����� ó���ϱ� ���� �ʿ�
	PrimaryActorTick.bCanEverTick = true;

	// === ĸ�� ������Ʈ ���� (�浹 ó����) ===
	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	SetRootComponent(Capsule); // ĸ���� ��Ʈ ������Ʈ�� ����
	Capsule->SetCapsuleRadius(25.0f);      // ĸ���� ������ ����
	Capsule->SetCapsuleHalfHeight(25.0f);  // ĸ���� �ݳ��� ����
	Capsule->SetSimulatePhysics(false);    // ���� �ùķ��̼� ��Ȱ��ȭ (���� ����)

	// === ȭ��ǥ ������Ʈ ���� (���� ǥ�ÿ�) ===
	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	Arrow->SetupAttachment(Capsule); // ĸ���� ����

	// === �޽� ������Ʈ ���� (����� ����) ===
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Capsule); // ĸ���� ����
	Mesh->SetSimulatePhysics(false); // ���� �ùķ��̼� ��Ȱ��ȭ

	// === �������� ������Ʈ ���� (ī�޶� ������) ===
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(Capsule); // ĸ���� ����
	SpringArm->TargetArmLength = 300.0f; // ī�޶�� ��� ������ �Ÿ�
	SpringArm->bUsePawnControlRotation = false; // �� ȸ���� ���� �ڵ� ȸ�� ��Ȱ��ȭ

	// === ī�޶� ������Ʈ ���� ===
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm); // �������Ͽ� ����
	Camera->bUsePawnControlRotation = false; // �� ȸ���� ���� �ڵ� ȸ�� ��Ȱ��ȭ

	// === ��Ʈ�ѷ� ȸ�� ���� ===
	// ��� ���� �ڵ� ȸ���� ��Ȱ��ȭ�Ͽ� �������� ȸ���� ����
	bUseControllerRotationYaw = false;   // �¿� ȸ�� ��Ȱ��ȭ
	bUseControllerRotationPitch = false; // ���� ȸ�� ��Ȱ��ȭ
	bUseControllerRotationRoll = false;  // �� ȸ�� ��Ȱ��ȭ
}

// ���� ���� �� ȣ��Ǵ� �Լ�
void ADR_Drone::BeginPlay()
{
	Super::BeginPlay();

	// ���� ���� �� ����� ���鿡 �ִ��� Ȯ��
	// �ʱ� ���¸� ��Ȯ�� �����ϱ� ���� �ʿ�
	bIsGrounded = CheckGrounded();
}

// �� �����Ӹ��� ȣ��Ǵ� �Լ� (�ǽð� ������Ʈ)
void ADR_Drone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// === �� �����Ӹ��� �����ؾ� �� �۾��� ===

	// 1. ���� ���� ������Ʈ (����� ���� ����ִ��� Ȯ��)
	bIsGrounded = CheckGrounded();

	// 2. �߷� ���� (���߿� ���� ����)
	ApplyGravity(DeltaTime);

	// 3. ���� �浹 ó�� (���� �� �ӵ� ����)
	if (bIsGrounded)
	{
		HandleGroundCollision();
	}
}

// �Է� �ý��� ���� �Լ�
// �÷��̾��� �Է�(Ű����, ���콺 ��)�� ����� ���۰� ����
void ADR_Drone::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Enhanced Input System ����� ���� ĳ����
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	check(EnhancedInputComponent); // ĳ���� ���� Ȯ��

	// �÷��̾� ��Ʈ�ѷ� ��������
	ADR_PlayerController* PlayerController = Cast<ADR_PlayerController>(GetController());
	check(PlayerController); // ĳ���� ���� Ȯ��

	// === �Է� �׼ǵ��� ����� �Լ���� ���� ===

	// ���� �̵� (WASD Ű)
	check(PlayerController->MoveAction);
	EnhancedInputComponent->BindAction(PlayerController->MoveAction, ETriggerEvent::Triggered, this, &ADR_Drone::Move);

	// ���� ȸ�� (���콺 ������)
	check(PlayerController->LookAction);
	EnhancedInputComponent->BindAction(PlayerController->LookAction, ETriggerEvent::Triggered, this, &ADR_Drone::Look);

	// ���� ��� (�����̽���)
	check(PlayerController->MoveUPAction);
	EnhancedInputComponent->BindAction(PlayerController->MoveUPAction, ETriggerEvent::Triggered, this, &ADR_Drone::MoveUP);

	// ���� �ϰ� (Shift Ű)
	check(PlayerController->MoveDOWNAction);
	EnhancedInputComponent->BindAction(PlayerController->MoveDOWNAction, ETriggerEvent::Triggered, this, &ADR_Drone::MoveDOWN);

	// ���� �� ȸ�� (Q Ű)
	check(PlayerController->RollRightAction);
	EnhancedInputComponent->BindAction(PlayerController->RollRightAction, ETriggerEvent::Triggered, this, &ADR_Drone::RollRight);

	// ���� �� ȸ�� (E Ű)
	check(PlayerController->RollLeftAction);
	EnhancedInputComponent->BindAction(PlayerController->RollLeftAction, ETriggerEvent::Triggered, this, &ADR_Drone::RollLeft);

	// ���� �۵� (p Ű)
	check(PlayerController->EngineStartAction);
	EnhancedInputComponent->BindAction(PlayerController->EngineStartAction, ETriggerEvent::Started, this, &ADR_Drone::EngineOnOff);



}

// �߷� ���� �Լ�
// ����� ���߿� ���� �� �ڿ������� ���������� �ϴ� ���� ȿ��
void ADR_Drone::ApplyGravity(float DeltaTime)
{
	// ���߿� ���� ���� �߷� ����
	if (!bIsGrounded)
	{
		// �߷� ���ӵ��� ���� �ӵ��� ����
		// �ð��� �������� �� ������ ������ (�������� �߷� ȿ��)
		VerticalVelocity += GravityForce * DeltaTime;

		// �ִ� ���� �ӵ� ���� (������ �������� �ʵ���)
		// �͹̳� ���ν�Ƽ ���� ����
		if (VerticalVelocity < MaxFallSpeed)
		{
			VerticalVelocity = MaxFallSpeed;
		}

		// ���� ���� �ӵ��� ���� ��ġ ��ȭ�� ����
		// Z��(����)���θ� �̵�
		FVector GravityOffset = FVector(0.0f, 0.0f, VerticalVelocity * DeltaTime);
		AddActorWorldOffset(GravityOffset, true); // true = �浹 ���� Ȱ��ȭ
	}
}

// ���� �浹 ���� �Լ�
// ����� ���� ����ִ��� Ȯ���ϴ� �߿��� �Լ�
bool ADR_Drone::CheckGrounded()
{
	// ���� ��ü�� ������ ���� üũ �Ұ�
	if (!GetWorld())
	{
		return false;
	}

	// === ����ĳ������ ���� �������� ���� ���� ===
	FVector StartLocation = GetActorLocation(); // ����� ���� ��ġ
	FVector EndLocation = StartLocation + FVector(0.0f, 0.0f, -GroundCheckDistance); // �Ʒ������� ���� �Ÿ�

	// �浹 ����� ������ ����
	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this); // �ڱ� �ڽ��� �浹 ��󿡼� ����

	// SweepTest�� ����� ���� �浹 ����
	// ���� �ƴ� ĸ�� ������� �浹 �˻� (�� ��Ȯ�� �浹 ����)
	bool bHit = GetWorld()->SweepSingleByChannel(
		HitResult,                    // �浹 ���
		StartLocation,                // ���� ��ġ
		EndLocation,                  // �� ��ġ
		FQuat::Identity,              // ȸ�� (ȸ�� ����)
		ECC_WorldStatic,              // �浹 ä�� (���� ������Ʈ�͸� �浹)
		FCollisionShape::MakeCapsule(Capsule->GetScaledCapsuleRadius(), Capsule->GetScaledCapsuleHalfHeight()), // ĸ�� ���
		QueryParams                   // �浹 �Ű�����
	);

	return bHit; // �浹�� �����Ǹ� true, �ƴϸ� false
}

// ���� �浹 ó�� �Լ�
// ����� ���� �������� ���� ������ ó��
void ADR_Drone::HandleGroundCollision()
{
	// �ϰ� ���̾��ٸ� (���� �ӵ��� ����)
	if (VerticalVelocity < 0.0f)
	{
		// ���� �� ���� �ӵ��� 0���� �ʱ�ȭ
		// ���� �հ� ��� �������� ���� ����
		VerticalVelocity = 0.0f;
	}
}

// ���� �̵� ó�� �Լ� (�����¿� �̵�)
// ����� ���߿����� �̵� �ӵ��� �ٸ��� ����
void ADR_Drone::ProcessHorizontalMovement(const FVector2D& MovementValue, float DeltaTime)
{
	// ����/���� ���¿� ���� �̵� �ӵ� ����
	// ���߿����� ������ �� ������� ������ (Acceleration, Deceleration�� ������ �� ���� ����)
	// ����� MaxSpeed���� ������ ��ġ���� �մϴ�.
	float CurrentMaxSpeed = bIsGrounded ? MaxSpeed : MaxSpeed * AirControlMultiplier;
	float CurrentMinSpeed = bIsGrounded ? MinSpeed : MinSpeed * AirControlMultiplier;


	// �Է°��� 3D ���ͷ� ��ȯ (X: ����, Y: �¿�, Z: 0)
	FVector DesiredDirection = FVector(MovementValue.X, MovementValue.Y, 0.0f);

	// �Է� ���� ����ȭ (�밢�� �̵� �� �ӵ� �����ϰ�)
	if (!DesiredDirection.IsNearlyZero())
	{
		DesiredDirection.Normalize();
	}

	// ===================== ���� �� ���� �� ���� ���� ===========================

	// ���� �ӵ� (CurrentVelocity)�� ���ϴ� ���� (DesiredDirection)�� ����Ͽ�
	// ����� ���� �ӵ��� ������Ʈ�մϴ�.

	// ���� ���� ��� 
	if (!DesiredDirection.IsNearlyZero() && true == bIsEngineOn)
	{
		// ��ǥ �ӵ� (DesiredDirection �������� CurrentMaxSpeed)
		FVector TargetVelocity = DesiredDirection * CurrentMaxSpeed;

		// CurrentVelocity�� TargetVelocity �������� ����
		CurrentVelocity = FMath::VInterpTo(CurrentVelocity, TargetVelocity, DeltaTime, Acceleration);

		// �ּ� �ӵ� (MinSpeed) �̻��� �����ϵ��� ���� (�ɼ�)
		// ���� �ӵ��� �ʹ� �������� MinSpeed�� ����
		if (CurrentVelocity.Size() < CurrentMinSpeed && CurrentMinSpeed > 0.0f)
		{
			// ���� �������� �ּ� �ӵ���ŭ �о���
			CurrentVelocity = CurrentVelocity.GetSafeNormal() * CurrentMinSpeed;
		}
	}
	// 2. ���� KILL ��� 
	else if(false == bIsEngineOn)
	{
		// CurrentVelocity�� ZeroVector�� ����
		CurrentVelocity = FMath::VInterpTo(CurrentVelocity, FVector::ZeroVector, DeltaTime, Deceleration);

		// CurrentVelocity�� MinSpeed���� �۾����� 0���� �����Ͽ� ������ ���߰� ��
		// �̷��� ���� ������ MinSpeed ������ �̼��� �ӵ��� ��� ������ �� ����
		if (CurrentVelocity.SizeSquared() < FMath::Square(CurrentMinSpeed * 0.5f)) // MinSpeed�� ���� �������� ������ ����
		{
			CurrentVelocity = FVector::ZeroVector;
		}
	}

	// ===================== �� �̵� ó�� ===========================
	// CurrentVelocity�� ����Ͽ� ����� �����Դϴ�.
	// �̴� �� ������ Tick �Լ����� ȣ��ǹǷ�, ���������� �̵��ϰ� �˴ϴ�.
	if (!CurrentVelocity.IsNearlyZero())
	{
		// ���� �̵� ���� ��� (CurrentVelocity * DeltaTime)
		// ����� ���� ��ǥ�� �������� �̵�
		AddActorLocalOffset(CurrentVelocity * DeltaTime, true); // true = �浹 ���� Ȱ��ȭ
	}
}

// ���� �̵� ó�� �Լ� (���/�ϰ�)
// ����� ���� �����ϴ� �Լ�
void ADR_Drone::ProcessVerticalMovement(float VerticalInput, float DeltaTime)
{
	// ����/���� ���¿� ���� �̵� �ӵ� ����
	float CurrentMoveSpeed = bIsGrounded ? MoveSpeed : MoveSpeed * AirControlMultiplier;

	// ���� �̵� ���� ���� (Z�����θ� �̵�)
	FVector LocalMoveVector = FVector(0.0f, 0.0f, VerticalInput * CurrentMoveSpeed * DeltaTime);
	AddActorLocalOffset(LocalMoveVector, true); // �浹 ���� Ȱ��ȭ

	// ���� �Է��� ���� �� �߷� �ӵ� ����
	// ���� �̵��� ���� �߷��� ������ �ٿ��� �� �ε巯�� ��� ȿ��
	if (!FMath::IsNearlyZero(VerticalInput) && VerticalInput > 0.0f)
	{
		// ���� �̵��� �� �߷� �ӵ� ���� (�ε巯�� ���)
		VerticalVelocity = FMath::Max(0.0f, VerticalVelocity);
	}
}

void ADR_Drone::EngineOnOff(const FInputActionValue& Value)
{
	bIsEngineOn = !bIsEngineOn; // ���� ���� ��� (���� ������ ����, ���� ������ Ŵ)

	if (true == bIsEngineOn) // ������ ������ ��
	{
		GravityForce = 0.0f; // �߷� ����	 ��Ȱ��ȭ
	}

	else if (false == bIsEngineOn) // ������ ������ ��
	{
		GravityForce = -980.0f;
	}
}

// === �Է� ó�� �Լ��� ===
// �� �Է¿� �����ϴ� ����� ������ ����

// ���� �̵� �Է� ó�� (WASD Ű)
void ADR_Drone::Move(const FInputActionValue& Value)
{
	check(Controller); // ��Ʈ�ѷ� ���� Ȯ��

	// �Է°��� 2D ���ͷ� �������� (X: ����, Y: �¿�)
	const FVector2D MovementValue = Value.Get<FVector2D>();

	// ���� �Է��� ���� ���� ó�� (�ſ� ���� ���� ����)
	if (!FMath::IsNearlyZero(MovementValue.X) || !FMath::IsNearlyZero(MovementValue.Y))
	{
		float DeltaTime = GetWorld()->GetDeltaSeconds(); // ������ �ð� ��������
		ProcessHorizontalMovement(MovementValue, DeltaTime); // ���� �̵� ó��
	}
}

// ���� ȸ�� �Է� ó�� (���콺 ������)
void ADR_Drone::Look(const FInputActionValue& Value)
{
	check(Controller); // ��Ʈ�ѷ� ���� Ȯ��

	// ���콺 �Է°� �������� (X: �¿�, Y: ����)
	FVector2D LookValue = Value.Get<FVector2D>();

	float DeltaTime = GetWorld()->GetDeltaSeconds(); // ������ �ð�

	// === ���ʹϾ��� ����� ȸ�� ó�� ===
	// ���ʹϾ�: 3D ȸ���� ���������� ó���ϴ� ������ ���

	// ���� ȸ���� ���ʹϾ����� ��������
	FQuat CurrentQuat = GetActorQuat();

	// ��Ÿ ȸ���� ��� (�Է� * ȸ���ӵ� * �ð�)
	float DeltaYaw = LookValue.X * LookSpeed * DeltaTime;     // �¿� ȸ��
	float DeltaPitch = LookValue.Y * LookSpeed * DeltaTime;   // ���� ȸ��

	// �� �ະ ȸ�� ���ʹϾ� ����
	FQuat YawQuat = FQuat(FVector::UpVector, FMath::DegreesToRadians(DeltaYaw));       // Y�� ȸ��
	FQuat PitchQuat = FQuat(FVector::RightVector, FMath::DegreesToRadians(DeltaPitch)); // X�� ȸ��

	// ���� ȸ���� ��Ÿ ȸ�� ����
	// ������ �߿�: Yaw * Current * Pitch
	FQuat NewQuat = YawQuat * CurrentQuat * PitchQuat;
	NewQuat.Normalize(); // ���ʹϾ� ����ȭ (������ Ȯ��)

	// ���� ���
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

// ���� �� ȸ�� �Է� ó�� (Q Ű)
void ADR_Drone::RollRight(const FInputActionValue& Value)
{
	check(Controller); // ��Ʈ�ѷ� ���� Ȯ��

	// �Է°� �������� (float Ÿ��)
	const float RollValue = Value.Get<float>();

	// ���� �Է��� ���� ���� ó��
	if (!FMath::IsNearlyZero(RollValue))
	{
		float DeltaTime = GetWorld()->GetDeltaSeconds(); // ������ �ð�
		float RollSpeed = 90.0f; // �� ȸ�� �ӵ� (��/��)

		// ���� ȸ�� ���� ��������
		FQuat CurrentQuat = GetActorQuat();

		// �� ȸ���� ��� (�������� ȸ��)
		float DeltaRoll = -RollValue * RollSpeed * DeltaTime;

		// ���� ���� ������ ȸ���ϴ� ���ʹϾ� ����
		FQuat RollQuat = FQuat(FVector::ForwardVector, FMath::DegreesToRadians(DeltaRoll));

		// ���ο� ȸ�� ����
		FQuat NewQuat = CurrentQuat * RollQuat;
		NewQuat.Normalize(); // ���ʹϾ� ����ȭ


		// ���� ���
		if (true == bIsEngineOn)
		{
			SetActorRotation(NewQuat);
		}
		else if (false == bIsEngineOn)
		{
			// �� ��
		}
	}
}

// ���� �� ȸ�� �Է� ó�� (E Ű)
void ADR_Drone::RollLeft(const FInputActionValue& Value)
{
	check(Controller); // ��Ʈ�ѷ� ���� Ȯ��

	// �Է°� ��������
	const float RollValue = Value.Get<float>();

	// ���� �Է��� ���� ���� ó��
	if (!FMath::IsNearlyZero(RollValue))
	{
		float DeltaTime = GetWorld()->GetDeltaSeconds(); // ������ �ð�
		float RollSpeed = 90.0f; // �� ȸ�� �ӵ� (��/��)

		// ���� ȸ�� ���� ��������
		FQuat CurrentQuat = GetActorQuat();

		// �� ȸ���� ��� (�������� ȸ�� = ����)
		float DeltaRoll = RollValue * RollSpeed * DeltaTime;

		// ���� ���� ������ ȸ���ϴ� ���ʹϾ� ����
		FQuat RollQuat = FQuat(FVector::ForwardVector, FMath::DegreesToRadians(DeltaRoll));

		// ���ο� ȸ�� ����
		FQuat NewQuat = CurrentQuat * RollQuat;
		NewQuat.Normalize(); // ���ʹϾ� ����ȭ


		// ���� ���
		if (true == bIsEngineOn)
		{
			SetActorRotation(NewQuat);
		}
		else if (false == bIsEngineOn)
		{
			// ����
		}
	}
}

// ���� ��� �Է� ó�� (����Ʈ Ű)
void ADR_Drone::MoveUP(const FInputActionValue& Value)
{
	check(Controller); // ��Ʈ�ѷ� ���� Ȯ��

	if (false == bIsEngineOn)
	{
		return; // ������ ���������� ��� �Է� ����
	}
	// �Է°� ��������
	const float VerticalValue = Value.Get<float>();

	// ���� �Է��� ���� ���� ó��
	if (!FMath::IsNearlyZero(VerticalValue))
	{
		float DeltaTime = GetWorld()->GetDeltaSeconds(); // ������ �ð�
		ProcessVerticalMovement(VerticalValue, DeltaTime); // ���� �̵� ó�� (��� = ���)
	}
}

// ���� �ϰ� �Է� ó�� (�����̽��� Ű)
void ADR_Drone::MoveDOWN(const FInputActionValue& Value)
{
	check(Controller); // ��Ʈ�ѷ� ���� Ȯ��

	if (false == bIsEngineOn)
	{
		return; // ������ ���������� �ϰ� �Է� ����
	}
	// �Է°� ��������
	const float VerticalValue = Value.Get<float>();

	// ���� �Է��� ���� ���� ó��
	if (!FMath::IsNearlyZero(VerticalValue))
	{
		float DeltaTime = GetWorld()->GetDeltaSeconds(); // ������ �ð�
		ProcessVerticalMovement(-VerticalValue, DeltaTime); // ���� �̵� ó�� (���� = �ϰ�)
	}
}