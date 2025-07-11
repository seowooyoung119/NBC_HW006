#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputActionValue.h"
#include "DR_Drone.generated.h"

// �ʿ��� ������Ʈ���� ���� ���� (��� ���� ũ�� ����ȭ)
class UCapsuleComponent;      // �浹 ó���� ĸ�� ������Ʈ
class UArrowComponent;        // ����� ���� ǥ�ÿ� ȭ��ǥ ������Ʈ
class UStaticMeshComponent;   // ����� 3D �� �޽� ������Ʈ
class USpringArmComponent;    // ī�޶� �� (SpringArm) ������Ʈ
class UCameraComponent;       // ī�޶� ������Ʈ
class UAudioComponent;       // ���� ����� ����� ������Ʈ (�ʿ�� �߰�)
class USoundBase;            // ���� ���̽� Ŭ���� (���� ������ ����ϱ� ���� �⺻ Ŭ����)
class UParticleSystemComponent; // ��ƼŬ �ý��� ������Ʈ (�ʿ�� �߰�)
/**
 * ��� �� Ŭ����
 * APawn�� ��ӹ޾� �÷��̾ ���� ������ �� �ִ� ����� ����
 * 6�� �̵� (�����¿����)�� ȸ�� ����� ����
 */
UCLASS()
class DRONE_API ADR_Drone : public APawn
{
    GENERATED_BODY()

public:
    // ������: ��� ��ü�� ������ �� ȣ��Ǵ� �Լ�
    ADR_Drone();

    void AttachParticleToComponent(USceneComponent* ParentComponent, FName AttachSocketName = NAME_None);

    void AttachParticleToActor(AActor* ParentActor, FName AttachSocketName = NAME_None);

protected:
    // ������ ���۵� �� �� �� ȣ��Ǵ� �Լ� (�ʱ�ȭ �۾�)
    virtual void BeginPlay() override;

    // �� �����Ӹ��� ȣ��Ǵ� �Լ� (�ǽð� ������Ʈ ó��)
    virtual void Tick(float DeltaTime) override;

    // �÷��̾� �Է��� �����ϴ� �Լ� (Ű����, ���콺 ���� �Է��� ���ε�)
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    // ======================== ������Ʈ�� ========================

	// ��ƼŬ �ý��� ������Ʈ (����� ȿ���� ���� ��ƼŬ �ý���)
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Particles")
    TObjectPtr <UParticleSystemComponent> OurParticleSystemComponent;

	// ��ƼŬ �ý��� ���� (����� ȿ���� ���� ��ƼŬ �ý��� ����)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particles")
    TObjectPtr<UParticleSystem> ParticleSystemAsset;

    // �浹 ó���� ���� ĸ�� ������Ʈ (����� ������ ���)
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UCapsuleComponent* Capsule;

    // ����� ������ �ð������� ǥ���ϴ� ȭ��ǥ ������Ʈ
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UArrowComponent* Arrow;

    // ����� 3D ���� �������ϴ� �޽� ������Ʈ
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* Mesh;

    // ī�޶� ��п��� ���� �Ÿ� ����߷� �ִ� ������ �� ������Ʈ
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USpringArmComponent* SpringArm;

    // �÷��̾ ����� ���� ������ �����ϴ� ī�޶� ������Ʈ
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UCameraComponent* Camera;

	// ����� ������Ʈ (�ʿ�� �߰�, ��: ��� ���� �Ҹ�)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UAudioComponent* AudioComponent;

    // ======================== �Է� ó�� �Լ��� ========================

    // �����¿� �̵� ó�� �Լ� (WASD Ű �Ǵ� ���̽�ƽ)
	UFUNCTION()
    void Move(const FInputActionValue& Value);

    // ���콺 �Ǵ� �� ��ƽ���� ���� ȸ�� ó�� �Լ�
    UFUNCTION()
    void Look(const FInputActionValue& Value);

    // ��� �Է� ó�� �Լ� (�����̽��� �Ǵ� RT Ʈ����)
    UFUNCTION()
    void MoveUP(const FInputActionValue& Value);

    // �ϰ� �Է� ó�� �Լ� (Shift Ű �Ǵ� LT Ʈ����)
    UFUNCTION()
    void MoveDOWN(const FInputActionValue& Value);

    // ���� �Ѹ� �Է� ó�� �Լ� (E Ű �Ǵ� RB ��ư)
    UFUNCTION()
    void RollRight(const FInputActionValue& Value);

    // ���� �Ѹ� �Է� ó�� �Լ� (Q Ű �Ǵ� LB ��ư)
    UFUNCTION()
    void RollLeft(const FInputActionValue& Value);

    // ======================== �̵� �� ȸ�� ���� ������ ========================

    // ����� �̵� �ӵ� (cm/s ����, �𸮾� �⺻ ����)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float MoveSpeed = 500.0f;

    // ����� ȸ�� �ӵ� (��/�� ����)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float LookSpeed = 90.0f;

    // ======================== �߷� �� ���� ���� ������ ========================

    // �߷� ���ӵ� (cm/s�� ����, ������ �Ʒ� ����)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gravity")
    float GravityForce = -980.0f; // ������ �߷� (9.8m/s��)�� cm ������ ȯ��

    // �ִ� ���� �ӵ� ���� (�͹̳� �ӵ�)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gravity")
    float MaxFallSpeed = -2000.0f;

    // ���߿����� ���� ���� (���� ��� 50% ȿ��)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float AirControlMultiplier = 0.5f;

    // ���� ������ ���� ����ĳ��Ʈ �Ÿ� (cm ����)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float GroundCheckDistance = 20.0f;

    // ======================== ���� ���� ������ ========================

    // ����� ���鿡 �����ߴ��� ���� (����� �� ���� �б��)
    UPROPERTY(BlueprintReadOnly, Category = "Status")
    bool bIsGrounded = false;

    // ����� ���� 3D �ӵ� ���� (x, y, z ���� �ӵ�)
    UPROPERTY(BlueprintReadOnly, Category = "Status")
    FVector Velocity = FVector::ZeroVector;

    // ����� ���� �ӵ� (���� �̵� �ӵ��� ���� ����)
    UPROPERTY(BlueprintReadOnly, Category = "Status")
    float VerticalVelocity = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Status")
	bool bIsEngineOn = false; // ��� ������ ���� �ִ��� ���� (Ű �Է¿� ���� ���� ����)

    // ===================== ���� ó�� ������ ===========================

    UPROPERTY(BlueprintReadOnly, Category = "Movement")
	FVector CurrentVelocity= FVector::ZeroVector; // ���� ����� �ӵ� ���� (x, y, z ���� �ӵ�)
 
    UPROPERTY(BlueprintReadOnly, Category = "Movement")
	FVector2D LastMovementInput; // ���������� ���� ���� �̵� �Է� (2D ����, x: ����, y: �¿�)

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MaxSpeed = 2000.0f; // ����� �ִ� �ӵ� (cm/s ����)

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MinSpeed = 500.0f; // ����� �ּ� �ӵ� (cm/s ����)

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float Acceleration = 1000.0f; // ����� ���ӵ� (cm/s�� ����)

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float Deceleration = 1000.0f; // ����� ���ӵ� (cm/s�� ����)

    // ���� �ÿ�� 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
    TObjectPtr<USoundBase> EngineONSound;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
    TObjectPtr<USoundBase> EngineOFFSound;

	// ��ƼŬ �ý��� ���� �̸� (����� ��ƼŬ ȿ���� ������ ����)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particles")
    FName SocketName;

private:
    // ======================== ���� ó�� �Լ��� ========================

    // �� �����Ӹ��� �߷��� �����ϴ� �Լ�
    void ApplyGravity(float DeltaTime);

    // ����� ���鿡 ��Ҵ��� Ȯ���ϴ� �Լ� (����ĳ���� ���)
    bool CheckGrounded();

    // ���� �浹 �� ó���ϴ� �Լ� (�ٿ ȿ�� ��)
    void HandleGroundCollision();

    // ���� �̵� (�����¿�) ó���ϴ� �Լ�
    void ProcessHorizontalMovement(const FVector2D& MovementValue, float DeltaTime);

    // ���� �̵� (����) ó���ϴ� �Լ�
    void ProcessVerticalMovement(float VerticalInput, float DeltaTime);

	// ���� ����/���� ó�� �Լ� (Ű �Է¿� ���� ���� ���� ����)
	void EngineOnOff(const FInputActionValue& Value);

};