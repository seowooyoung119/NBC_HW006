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
| E | 상승 |
| Q | 하강 |
| 마우스 이동 | 시점 전환 (드론 회전) |
| 스페이스 바 | 오른쪽으로 구르기 (Roll Right) |
| 왼쪽 Shift | 왼쪽으로 구르기 (Roll Left) |

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

