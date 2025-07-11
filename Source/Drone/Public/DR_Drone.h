#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputActionValue.h"
#include "DR_Drone.generated.h"

// 필요한 컴포넌트들을 전방 선언 (헤더 파일 크기 최적화)
class UCapsuleComponent;      // 충돌 처리용 캡슐 컴포넌트
class UArrowComponent;        // 드론의 방향 표시용 화살표 컴포넌트
class UStaticMeshComponent;   // 드론의 3D 모델 메시 컴포넌트
class USpringArmComponent;    // 카메라 붐 (SpringArm) 컴포넌트
class UCameraComponent;       // 카메라 컴포넌트
class UAudioComponent;       // 사운드 재생용 오디오 컴포넌트 (필요시 추가)
class USoundBase;            // 사운드 베이스 클래스 (사운드 파일을 재생하기 위한 기본 클래스)
class UParticleSystemComponent; // 파티클 시스템 컴포넌트 (필요시 추가)
/**
 * 드론 폰 클래스
 * APawn을 상속받아 플레이어가 직접 조종할 수 있는 드론을 구현
 * 6축 이동 (전후좌우상하)과 회전 기능을 제공
 */
UCLASS()
class DRONE_API ADR_Drone : public APawn
{
    GENERATED_BODY()

public:
    // 생성자: 드론 객체가 생성될 때 호출되는 함수
    ADR_Drone();

    void AttachParticleToComponent(USceneComponent* ParentComponent, FName AttachSocketName = NAME_None);

    void AttachParticleToActor(AActor* ParentActor, FName AttachSocketName = NAME_None);

protected:
    // 게임이 시작될 때 한 번 호출되는 함수 (초기화 작업)
    virtual void BeginPlay() override;

    // 매 프레임마다 호출되는 함수 (실시간 업데이트 처리)
    virtual void Tick(float DeltaTime) override;

    // 플레이어 입력을 설정하는 함수 (키보드, 마우스 등의 입력을 바인딩)
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    // ======================== 컴포넌트들 ========================

	// 파티클 시스템 컴포넌트 (드론의 효과를 위한 파티클 시스템)
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Particles")
    TObjectPtr <UParticleSystemComponent> OurParticleSystemComponent;

	// 파티클 시스템 에셋 (드론의 효과를 위한 파티클 시스템 에셋)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particles")
    TObjectPtr<UParticleSystem> ParticleSystemAsset;

    // 충돌 처리를 위한 캡슐 컴포넌트 (드론의 물리적 경계)
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UCapsuleComponent* Capsule;

    // 드론의 방향을 시각적으로 표시하는 화살표 컴포넌트
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UArrowComponent* Arrow;

    // 드론의 3D 모델을 렌더링하는 메시 컴포넌트
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* Mesh;

    // 카메라를 드론에서 일정 거리 떨어뜨려 주는 스프링 암 컴포넌트
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USpringArmComponent* SpringArm;

    // 플레이어가 드론을 보는 시점을 제공하는 카메라 컴포넌트
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UCameraComponent* Camera;

	// 오디오 컴포넌트 (필요시 추가, 예: 드론 엔진 소리)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UAudioComponent* AudioComponent;

    // ======================== 입력 처리 함수들 ========================

    // 전후좌우 이동 처리 함수 (WASD 키 또는 조이스틱)
	UFUNCTION()
    void Move(const FInputActionValue& Value);

    // 마우스 또는 우 스틱으로 시점 회전 처리 함수
    UFUNCTION()
    void Look(const FInputActionValue& Value);

    // 상승 입력 처리 함수 (스페이스바 또는 RT 트리거)
    UFUNCTION()
    void MoveUP(const FInputActionValue& Value);

    // 하강 입력 처리 함수 (Shift 키 또는 LT 트리거)
    UFUNCTION()
    void MoveDOWN(const FInputActionValue& Value);

    // 우측 롤링 입력 처리 함수 (E 키 또는 RB 버튼)
    UFUNCTION()
    void RollRight(const FInputActionValue& Value);

    // 좌측 롤링 입력 처리 함수 (Q 키 또는 LB 버튼)
    UFUNCTION()
    void RollLeft(const FInputActionValue& Value);

    // ======================== 이동 및 회전 관련 변수들 ========================

    // 드론의 이동 속도 (cm/s 단위, 언리얼 기본 단위)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float MoveSpeed = 500.0f;

    // 드론의 회전 속도 (도/초 단위)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float LookSpeed = 90.0f;

    // ======================== 중력 및 낙하 관련 변수들 ========================

    // 중력 가속도 (cm/s² 단위, 음수는 아래 방향)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gravity")
    float GravityForce = -980.0f; // 현실의 중력 (9.8m/s²)을 cm 단위로 환산

    // 최대 낙하 속도 제한 (터미널 속도)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gravity")
    float MaxFallSpeed = -2000.0f;

    // 공중에서의 조작 감도 (지상 대비 50% 효율)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float AirControlMultiplier = 0.5f;

    // 지면 감지를 위한 레이캐스트 거리 (cm 단위)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float GroundCheckDistance = 20.0f;

    // ======================== 현재 상태 변수들 ========================

    // 드론이 지면에 착지했는지 여부 (디버깅 및 로직 분기용)
    UPROPERTY(BlueprintReadOnly, Category = "Status")
    bool bIsGrounded = false;

    // 드론의 현재 3D 속도 벡터 (x, y, z 방향 속도)
    UPROPERTY(BlueprintReadOnly, Category = "Status")
    FVector Velocity = FVector::ZeroVector;

    // 드론의 수직 속도 (상하 이동 속도만 따로 관리)
    UPROPERTY(BlueprintReadOnly, Category = "Status")
    float VerticalVelocity = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Status")
	bool bIsEngineOn = false; // 드론 엔진이 켜져 있는지 여부 (키 입력에 따라 상태 변경)

    // ===================== 엔진 처리 변수들 ===========================

    UPROPERTY(BlueprintReadOnly, Category = "Movement")
	FVector CurrentVelocity= FVector::ZeroVector; // 현재 드론의 속도 벡터 (x, y, z 방향 속도)
 
    UPROPERTY(BlueprintReadOnly, Category = "Movement")
	FVector2D LastMovementInput; // 마지막으로 받은 수평 이동 입력 (2D 벡터, x: 전후, y: 좌우)

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MaxSpeed = 2000.0f; // 드론의 최대 속도 (cm/s 단위)

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MinSpeed = 500.0f; // 드론의 최소 속도 (cm/s 단위)

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float Acceleration = 1000.0f; // 드론의 가속도 (cm/s² 단위)

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float Deceleration = 1000.0f; // 드론의 감속도 (cm/s² 단위)

    // 엔진 시운드 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
    TObjectPtr<USoundBase> EngineONSound;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
    TObjectPtr<USoundBase> EngineOFFSound;

	// 파티클 시스템 소켓 이름 (드론의 파티클 효과를 부착할 소켓)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particles")
    FName SocketName;

private:
    // ======================== 내부 처리 함수들 ========================

    // 매 프레임마다 중력을 적용하는 함수
    void ApplyGravity(float DeltaTime);

    // 드론이 지면에 닿았는지 확인하는 함수 (레이캐스팅 사용)
    bool CheckGrounded();

    // 지면 충돌 시 처리하는 함수 (바운스 효과 등)
    void HandleGroundCollision();

    // 수평 이동 (전후좌우) 처리하는 함수
    void ProcessHorizontalMovement(const FVector2D& MovementValue, float DeltaTime);

    // 수직 이동 (상하) 처리하는 함수
    void ProcessVerticalMovement(float VerticalInput, float DeltaTime);

	// 엔진 시작/정지 처리 함수 (키 입력에 따라 엔진 상태 변경)
	void EngineOnOff(const FInputActionValue& Value);

};