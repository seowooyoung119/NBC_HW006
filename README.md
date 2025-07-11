# Drone Project

## 1. 프로젝트 소개

이 프로젝트는 Unreal Engine을 사용하여 제작된 드론 시뮬레이션입니다. 사용자는 드론을 자유롭게 조종하며 3D 공간을 탐험할 수 있습니다.

## 2. 주요 기능

*   **드론 조작**: 전후좌우 이동, 상승 및 하강, 좌우 회전 및 롤(Roll)이 가능합니다.
*   **카메라**: 드론에 부착된 카메라를 통해 1인칭 또는 3인칭 시점으로 비행을 경험할 수 있습니다.
*   **입력 시스템**: Unreal Engine의 Enhanced Input System을 사용하여 입력을 처리합니다.

## 3. 조작 방법

| 키 | 기능 |
| --- | --- |
| W, S | 전진 / 후진 |
| A, D | 좌 / 우 이동 |
| E | 왼쪽으로 구르기 (Roll Left) |
| Q | 오른쪽으로 구르기 (Roll Right) |
| 마우스 이동 | 시점 전환 (드론 회전) |
| 스페이스 바 | 하강 |
| 왼쪽 Shift | 상승 |

## 4. 프로젝트 구조

### C++ 클래스

*   `ADR_Drone`: 플레이어가 조종하는 드론 Pawn 클래스입니다. 이동, 회전 등 드론의 핵심 로직을 포함합니다.
*   `ADR_PlayerController`: 플레이어의 입력을 받아 드론을 제어하는 클래스입니다. Enhanced Input System을 설정하고 입력 액션을 각 기능에 매핑합니다.
*   `ADR_GameMode`: 프로젝트의 기본 게임 모드입니다.
*   `ADR_GameState`: 프로젝트의 게임 상태를 관리합니다.

### 블루프린트 및 에셋

*   **BP_DR_Drone**: `ADR_Drone` C++ 클래스를 기반으로 생성된 블루프린트입니다. 드론의 메시, 머티리얼 등 시각적 요소를 설정합니다.
*   **Input Actions & Mapping Context**: `Content/Blueprints/System` 폴더에 `IA_...` 및 `IMC_Drone` 에셋으로 저장되어 있습니다.
*   **Map**: `Content/Maps/MainLevel` 에서 기본 레벨을 확인할 수 있습니다.

---

## 5. 상세 로직 흐름 (주석처럼 보기)

이 섹션에서는 프로젝트의 핵심 로직이 어떻게 작동하는지 코드 레벨에서 자세히 설명합니다.

### 5.1. 입력 처리 (`ADR_PlayerController` & `ADR_Drone`)

1.  **게임 시작 (`ADR_PlayerController::BeginPlay`)**:
    *   게임이 시작되면 `ADR_PlayerController`의 `BeginPlay` 함수가 호출됩니다.
    *   `UEnhancedInputLocalPlayerSubsystem`에 접근하여 `DroneMappingContext`를 추가합니다.
    *   `DroneMappingContext`는 어떤 키가 어떤 `InputAction`에 매핑되는지를 정의하는 에셋입니다. (예: W키 -> `IA_Move`)

2.  **입력 컴포넌트 설정 (`ADR_Drone::SetupPlayerInputComponent`)**:
    *   플레이어가 드론(`ADR_Drone`)을 조종하기 시작하면 이 함수가 호출됩니다.
    *   `UEnhancedInputComponent`를 가져와 각 `InputAction`과 그에 해당하는 처리 함수를 `BindAction`으로 연결합니다.
    *   **예시**: `PlayerController->MoveAction` (`IA_Move`)이 트리거되면, `ADR_Drone::Move` 함수를 호출하도록 바인딩합니다.
    *   모든 조작(Move, Look, UP, DOWN, Roll)에 대해 이 과정이 반복됩니다.

### 5.2. 드론 움직임 (`ADR_Drone`)

#### 매 프레임 실행 (`Tick` 함수)

`ADR_Drone::Tick` 함수는 매 프레임마다 호출되며, 드론의 상태를 지속적으로 업데이트합니다.

1.  **지면 확인 (`CheckGrounded`)**:
    *   `SweepSingleByChannel`을 사용하여 드론 아래 방향으로 캡슐 형태의 보이지 않는 선(Raycast)을 쏴서 지면과 충돌하는지 확인합니다.
    *   충돌하면 `bIsGrounded` 변수를 `true`로, 아니면 `false`로 설정합니다.
    *   이 변수는 중력 적용 및 공중 컨트롤을 결정하는 데 사용됩니다.

2.  **중력 적용 (`ApplyGravity`)**:
    *   `bIsGrounded`가 `false`일 때 (즉, 공중에 떠 있을 때)만 작동합니다.
    *   `VerticalVelocity` (수직 속도)에 `GravityForce` (중력 가속도)를 매 프레임 더해줍니다. (`DeltaTime`을 곱해 프레임 속도에 관계없이 일정한 가속도를 유지)
    *   `VerticalVelocity`가 `MaxFallSpeed` (최대 낙하 속도)를 넘지 않도록 제한합니다.
    *   계산된 `VerticalVelocity`만큼 드론을 아래로 이동시킵니다 (`AddActorWorldOffset`).

3.  **지면 충돌 처리 (`HandleGroundCollision`)**:
    *   `bIsGrounded`가 `true`이고, `VerticalVelocity`가 0보다 작을 때 (즉, 땅에 닿았고 하강 중일 때) 호출됩니다.
    *   `VerticalVelocity`를 0으로 만들어 드론이 땅을 뚫고 내려가는 것을 방지하고, 바닥에 안정적으로 서 있도록 합니다.

#### 입력에 따른 움직임 처리 함수

*   **`Move` (전후좌우 이동)**:
    1.  `FInputActionValue`에서 `FVector2D` 형식의 입력 값(X: W/S, Y: A/D)을 가져옵니다.
    2.  `ProcessHorizontalMovement` 함수를 호출합니다.
    3.  이 함수 내에서 `bIsGrounded` 상태에 따라 `MoveSpeed` 또는 `MoveSpeed * AirControlMultiplier`를 현재 속도로 결정합니다.
    4.  입력 값을 정규화(Normalize)하여 대각선 이동 시 속도가 빨라지는 것을 방지합니다.
    5.  계산된 이동 벡터만큼 드론을 로컬 좌표계 기준으로 이동시킵니다 (`AddActorLocalOffset`). 로컬 좌표계를 사용하므로 드론이 바라보는 방향을 기준으로 전후좌우로 움직입니다.

*   **`Look` (시점 회전)**:
    1.  마우스 움직임 값을 `FVector2D`로 가져옵니다.
    2.  쿼터니언(Quaternion)을 사용하여 회전을 계산합니다. 짐벌락(Gimbal Lock) 현상을 방지하고 부드러운 회전을 구현하기 위함입니다.
    3.  `Yaw` (좌우 회전)와 `Pitch` (상하 회전)에 대한 쿼터니언을 각각 생성합니다.
    4.  `YawQuat * CurrentQuat * PitchQuat` 순서로 쿼터니언을 곱하여 새로운 회전 값을 계산합니다. 이 순서는 월드 축을 기준으로 Yaw를 먼저 적용한 후, 로컬 축을 기준으로 Pitch를 적용하여 직관적인 조작감을 제공합니다.
    5.  계산된 새로운 쿼터니언 값으로 드론의 회전을 설정합니다 (`SetActorRotation`).

*   **`RollRight` / `RollLeft` (좌우 기울기)**:
    1.  입력 값(float)을 받아 `RollSpeed`와 곱하여 회전 각도를 계산합니다.
    2.  드론의 전방 벡터(`FVector::ForwardVector`)를 축으로 하는 롤(Roll) 쿼터니언을 생성합니다.
    3.  `CurrentQuat * RollQuat` 순서로 곱하여 현재 회전에 롤 회전을 추가합니다.
    4.  새로운 회전 값으로 드론의 회전을 설정합니다.

*   **`MoveUP` / `MoveDOWN` (상승/하강)**:
    1.  입력 값(float)을 받아 `ProcessVerticalMovement` 함수를 호출합니다. (`MoveDOWN`은 값을 음수로 바꿔서 호출)
    2.  이 함수는 `AddActorLocalOffset`을 사용하여 드론의 Z축(로컬 기준)으로 이동시킵니다.
    3.  상승 입력이 있을 경우, `VerticalVelocity`를 0 이상으로 만들어 중력에 의해 바로 떨어지지 않고 상승할 수 있도록 합니다.

---
드론 엔진 기능 추가
---
이 업데이트를 통해 드론에 엔진 ON/OFF 기능이 추가되었습니다. 이제 P 키를 눌러 드론의 엔진을 켜고 끌 수 있으며, 엔진 상태에 따라 드론의 움직임과 카메라 제어 방식이 달라집니다.

⚙️ 주요 변경 사항
엔진 ON/OFF 토글: P 키를 눌러 엔진을 켜거나 끌 수 있습니다.

엔진 ON 상태:

중력 비활성화: 엔진이 켜져 있을 때는 드론에 중력이 적용되지 않아 공중에서 안정적인 호버링이 가능합니다.

자유로운 기동: WASD 키를 사용한 수평 이동과 Spacebar/Shift를 사용한 수직 이동이 가능합니다. Q, E 키를 통한 롤(Roll) 회전도 작동하여 보다 세밀한 조작이 가능합니다.

쿼터니언 기반 카메라 제어: 마우스 움직임에 따라 드론 자체의 회전(Yaw, Pitch)이 쿼터니언 방식으로 제어되어, 드론의 진행 방향을 직접 조종하는 듯한 직관적인 경험을 제공합니다.

엔진 OFF 상태:

중력 적용: 엔진이 꺼지면 드론에 중력이 다시 적용되어 지면으로 하강합니다.

이동 제한: 수평 및 수직 이동, 롤 회전 기능이 비활성화됩니다.

컨트롤러 기반 카메라 제어: 마우스 움직임이 플레이어 컨트롤러의 회전을 제어합니다. 이는 드론 자체는 고정된 상태에서 주변을 둘러보는 듯한 시점을 제공합니다는 아직 미구현ㅋ

이 기능은 드론 시뮬레이션에 현실감을 더하고, 플레이어가 비행과 착륙을 더욱 전략적으로 관리할 수 있도록 합니다.

---

### 5.3. HOLE 인터페이스 및 자식 클래스

이 프로젝트에는 `HOLE`이라는 개념이 도입되어 있습니다. 이는 플레이어가 특정 오브젝트와 상호작용하여 '구멍'을 생성하고 관리하는 시스템입니다.

*   **`IHOLE_Interface` (인터페이스)**:
    *   `HOLE_Interface.h`에 정의된 순수 가상 함수(Pure Virtual Function)를 포함하는 인터페이스입니다.
    *   `RandomizePosition()`: 구멍의 위치를 무작위로 설정합니다.
    *   `RandomizeRotate()`: 구멍의 회전을 무작위로 설정합니다.
    *   `OnOverlap()`: 다른 액터와 겹쳤을 때 호출되는 이벤트 핸들러입니다.
    *   `DestroyHole()`: 구멍을 파괴하는 함수입니다.

*   **`AHole` (기본 클래스)**:
    *   `Hole.h`에 정의된 `AActor`를 상속받고 `IHOLE_Interface`를 구현하는 기본 클래스입니다.
    *   **컴포넌트**: `HoleCollision` (충돌 감지), `HoleMesh1`, `HoleMesh2` (시각적 메시)를 포함합니다.
    *   **속성**: `Position`, `Rotation`, `ReverseTime` 등의 속성을 가집니다.
    *   **메서드**: `RandomizeTime()`, `ReverseRandomizePosition()` 등을 구현하여 구멍의 동작을 제어합니다.

*   **`AHL_Box` (자식 클래스)**:
    *   `HL_Box.h`에 정의되어 있으며 `AHole` 클래스를 상속받습니다.
    *   `DestroyHole()` 및 `ReverseRandomizePosition()` 함수를 오버라이드하여 박스 형태의 구멍에 특화된 동작을 구현합니다.

*   **`AHL_Circle` (자식 클래스)**:
    *   `HL_Circle.h`에 정의되어 있으며 `AHole` 클래스를 상속받습니다.
    *   `DestroyHole()` 및 `ReverseRandomizePosition()` 함수를 오버라이드하여 원 형태의 구멍에 특화된 동작을 구현합니다.

*   **`AHL_CircleSpawner` (스포너 클래스)**:
    *   `HoleSpawner.h`에 정의되어 있으며 `AHole` 타입의 오브젝트를 생성하는 역할을 합니다.

이 시스템은 다양한 형태의 '구멍' 오브젝트를 유연하게 생성하고 관리할 수 있도록 설계되었습니다.

---
# 🔄 3D 회전 순서 데모

👉 [라이브 데모 보기](https://seowooyoung119.github.io/Rotation_Order_Demo/)
