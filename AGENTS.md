# AIShooting 프로젝트 작업 지침

## 프로젝트 목표와 범위

- 이 프로젝트는 Unreal Engine 5.8 기반의 3D 탑뷰 슈팅 게임 프로토타입이다.
- 플레이어와 게임플레이 오브젝트의 이동 평면은 월드 `YZ` 평면이다. 이동 중 `X` 좌표는 고정하고, 이동 입력과 속도에는 `Y`, `Z` 성분만 사용한다.
- 카메라는 `YZ` 플레이 영역을 `X`축 방향에서 바라보도록 배치한다. 메시와 이펙트의 앞/뒤 방향도 이 좌표계를 기준으로 일관되게 정한다.
- 구현은 플레이 가능한 핵심 루프 검증에 필요한 프로토타입 범위로 제한한다. 핵심 루프는 이동, 조준/발사, 적 생성 및 단순 AI, 피격/처치, 재시작이다.
- 출시용 아키텍처, 과도한 추상화, 범용 프레임워크, 네트워크 플레이, 영구 저장, 상점, 라이브 서비스, 복잡한 최적화는 명시적으로 요청받지 않는 한 추가하지 않는다.

## 화면과 카메라

- 카메라 Projection Mode는 반드시 `Orthographic`을 사용한다. Perspective 카메라나 원근감에 의존하는 게임플레이를 도입하지 않는다.
- 기준 해상도는 가로 `1024`, 세로 `2048`의 세로형 화면이며, 기준 종횡비는 `1:2`(`0.5`)이다.
- 카메라 프레이밍, UI 배치, 월드-스크린 좌표 변환, 조준 범위 및 가시 영역 계산은 이 기준 해상도와 종횡비를 우선 사용한다.
- `Ortho Width`는 가로 가시 범위를 뜻하므로 세로 가시 범위가 기준 비율에 따라 가로의 2배가 된다는 점을 고려한다. 해상도 픽셀값을 월드 유닛값으로 직접 간주하지 말고 명시적인 변환 비율을 둔다.
- UI는 UMG 앵커를 사용해 세로형 화면에 배치하고, 중요한 조작 및 HUD가 화면 가장자리의 안전 영역을 벗어나지 않게 한다.
- 해상도나 종횡비에 종속된 숫자는 코드 곳곳에 하드코딩하지 말고 이름 있는 상수, 설정값 또는 한 곳의 계산 함수로 관리한다.

## 입력

- **Enhanced Input을 사용하지 않는다.** `UInputAction`, `UInputMappingContext`, `UEnhancedInputComponent`, `UEnhancedPlayerInput`에 의존하는 코드를 추가하지 않는다.
- Unreal의 기존 입력 시스템(Old/Legacy Input)을 사용한다.
  - 축과 액션 매핑은 `Config/DefaultInput.ini` 또는 Project Settings의 Input 항목에 정의한다.
  - C++에서는 `UInputComponent::BindAxis`와 `UInputComponent::BindAction`을 사용한다.
  - 매핑 이름은 의미가 드러나는 PascalCase로 통일한다. 예: `MoveHorizontal`, `MoveVertical`, `Fire`, `Restart`.
- 이동 입력은 화면 기준 좌우를 월드 `Y`, 상하를 월드 `Z`에 매핑하고 월드 `X` 이동은 항상 제거하거나 고정한다.
- 입력 시스템 관련 작업을 할 때는 `DefaultInput.ini`의 기본 클래스가 레거시 `PlayerInput`/`InputComponent`인지 확인하고, 모듈 의존성에서 불필요한 `EnhancedInput`을 제거한다.

## 설계 원칙

- C++에 핵심 규칙과 재사용 가능한 동작을 구현하고, Blueprint는 에셋 연결, 수치 튜닝, 단순 연출에 사용한다.
- 프로토타입의 주요 책임은 가능한 한 작게 나눈다.
  - Pawn/Character: YZ 평면 이동과 생존 상태
  - PlayerController: 입력 전달과 조준
  - Weapon/Projectile: 발사 주기, 투사체 이동, 충돌
  - Enemy/AIController: 단순 추적, 공격, 사망
  - GameMode/GameState: 스폰 및 라운드/재시작 흐름
- 기능을 추가하기 전에 기존 클래스를 확장할 수 있는지 확인한다. 프로토타입에서 인터페이스, 서브시스템, 컴포넌트 계층을 필요 이상으로 늘리지 않는다.
- Tick은 프레임마다 반드시 필요한 이동이나 조준에만 사용한다. 주기적 작업은 Timer를 우선하고, 사용하지 않는 Tick은 비활성화한다.
- 게임플레이 수치는 `UPROPERTY(EditDefaultsOnly)` 또는 `EditAnywhere`로 노출해 에디터에서 빠르게 조정할 수 있게 하되, 런타임 내부 상태는 외부에 불필요하게 노출하지 않는다.
- 객체 참조에는 Unreal의 수명 관리 방식을 따른다. UObject 포인터는 필요한 경우 `UPROPERTY()`로 추적하고, 비동기 또는 지연 참조에는 적절한 weak pointer를 고려한다.

## UE5 C++ 코딩 컨벤션

- Epic Games의 Unreal Engine C++ Coding Standard와 프로젝트의 기존 스타일을 따른다.
- Unreal 타입 접두사를 사용한다: `U`(UObject), `A`(Actor), `F`(구조체/일반 타입), `E`(enum), `I`(인터페이스), `T`(템플릿), `b`(bool).
- 타입과 함수는 PascalCase, 지역 변수와 멤버 변수도 Unreal 관례의 PascalCase를 사용한다. bool은 질문처럼 읽히는 `bIsAlive`, `bCanFire` 형태로 짓는다.
- 헤더에는 `#pragma once`를 두고, 대응하는 헤더를 `.cpp`의 첫 include로 포함한다. include는 필요한 것만 명시하고 헤더에서는 가능한 경우 전방 선언을 사용한다.
- 리플렉션 매크로(`UCLASS`, `USTRUCT`, `UENUM`, `UFUNCTION`, `UPROPERTY`)와 `GENERATED_BODY()`를 올바르게 사용한다. `.generated.h`는 해당 헤더의 마지막 include여야 한다.
- 공개 API를 먼저, 보호 API를 다음, 비공개 구현을 마지막에 배치한다. 접근 지정자를 명시하고 클래스의 책임을 작게 유지한다.
- 함수는 한 가지 일을 하도록 짧게 유지하며, 의도가 분명한 이름을 사용한다. 매직 넘버와 중복 로직을 피한다.
- 단위에는 이름으로 의미를 드러낸다(예: `MoveSpeedUnitsPerSecond`, `FireIntervalSeconds`). 프레임 독립적인 값에는 `DeltaTime`을 적용한다.
- 로그는 프로젝트 전용 로그 카테고리를 사용한다. 정상 플레이 중 반복되는 `LogTemp` 및 매 프레임 로그를 남기지 않는다.
- `check`는 복구 불가능한 개발자 불변식에만, `ensure`는 실행을 계속할 수 있는 예상 밖 상태에 사용한다. 사용자/에셋 설정 오류는 안전하게 처리하고 유용한 로그를 남긴다.
- 주석은 코드가 무엇을 하는지 반복하지 말고, 좌표계 선택이나 엔진 제약처럼 이유가 필요한 부분을 설명한다.
- 에디터에 노출하는 속성과 함수에는 적절한 Category를 지정하고, Blueprint 노출은 실제 디자이너 작업에 필요한 최소 범위로 제한한다.

## 에셋과 파일 명명

- C++ 파일은 타입명과 동일하게 이름 짓고 관련 모듈의 `Public`/`Private` 구조를 일관되게 사용한다.
- Unreal 에셋은 역할 접두사를 사용한다. 예: `BP_` Blueprint, `WBP_` Widget, `M_` Material, `MI_` Material Instance, `T_` Texture, `SM_` Static Mesh, `SK_` Skeletal Mesh, `NS_` Niagara System.
- 임시 에셋도 용도를 알 수 있게 명명하고, 검증이 끝난 폐기 후보는 별도 Prototype 폴더에 모은다.

## 작업 및 검증 규칙

- 변경 전에 관련 C++, Blueprint, Config의 기존 설정을 확인하고 사용자의 기존 변경을 보존한다.
- 작은 단위로 구현하며, 요청과 무관한 리팩터링이나 에셋 변경은 함께 수행하지 않는다.
- C++ 변경 후에는 최소한 Editor 타깃(`AIShootingEditor`, Win64, Development)이 컴파일되는지 확인한다.
- 입력 변경 후에는 PIE에서 좌우 입력이 `Y`, 상하 입력이 `Z`로만 움직이며 `X`가 변하지 않는지 확인한다.
- 카메라/UI 변경 후에는 1024x2048 Standalone 또는 PIE 뷰포트에서 직교 투영, 프레이밍, 클릭/조준 좌표, HUD 앵커를 확인한다.
- 게임플레이 변경 후에는 PIE에서 핵심 루프를 직접 점검한다. 자동화 테스트가 이미 있는 영역은 관련 테스트도 실행한다.
- 검증하지 못한 항목은 완료한 것처럼 표현하지 말고, 무엇을 검증했고 무엇이 남았는지 명시한다.

## 완료 기준

- 요청된 프로토타입 기능이 에디터에서 실행되고 핵심 경로가 동작한다.
- YZ 평면 제약, Orthographic 카메라, 1024x2048 기준 화면, Old Input 사용 조건을 위반하지 않는다.
- 새 컴파일 오류나 명백한 런타임 오류가 없고, 변경 범위가 요청에 필요한 수준으로 제한되어 있다.

## 핵심 실행 지침
너는 오케스트레이터로 역할을 수행하는거야.
사용자의 요청은 서브에이전트에 위임해서 실행 하도록해.
서브에이전트의 수행 결과를 단계별로 알려줘.
