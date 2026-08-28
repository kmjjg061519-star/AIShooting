# AIShooting Task 실행 순서

각 Task는 이전 Task의 완료와 대응 체크리스트 통과를 전제로 한다. 한 번에 하나만 구현하고, Editor 빌드와 PIE 검증을 마친 후 다음 Task로 이동한다.

| 순서 | Task | 주요 클래스 | 검증 문서 |
| --- | --- | --- | --- |
| 01 | [프로젝트 기반 및 게임 상태](01_프로젝트기반_게임상태.md) | `AAIShootingGameModeBase`, `AAIShootingGameState` | [Checklist 01](../checklists/01_프로젝트기반_게임상태.md) |
| 02 | [직교 카메라](02_직교카메라.md) | `AAIShootingCameraActor` | [Checklist 02](../checklists/02_직교카메라.md) |
| 03 | [플레이어 이동](03_플레이어이동.md) | `AAIShootingPlayerPawn`, `AAIShootingPlayerController` | [Checklist 03](../checklists/03_플레이어이동.md) |
| 04 | [방향키 총알 발사](04_방향키총알발사.md) | `AAIShootingPlayerProjectile`, 플레이어 클래스 확장 | [Checklist 04](../checklists/04_방향키총알발사.md) |
| 05 | [적 생성 객체 및 추적 AI](05_적추적AI.md) | `AAIShootingEnemyPawn`, `AAIShootingEnemyAIController` | [Checklist 05](../checklists/05_적추적AI.md) |
| 06 | [전투 충돌 및 생명](06_전투충돌_생명.md) | 플레이어·총알·적 클래스 확장 | [Checklist 06](../checklists/06_전투충돌_생명.md) |
| 07 | [적 자동 생성](07_적자동생성.md) | `AAIShootingGameModeBase` 확장 | [Checklist 07](../checklists/07_적자동생성.md) |
| 08 | [게임 오버 및 재시작](08_게임오버_재시작.md) | GameMode·GameState·PlayerController 확장 | [Checklist 08](../checklists/08_게임오버_재시작.md) |

## 공통 완료 규칙

- 해당 Task에 적힌 범위만 구현한다.
- `AIShootingEditor Win64 Development` 빌드가 성공해야 한다.
- 대응 체크리스트의 필수 항목을 PIE에서 확인한다.
- 실패 또는 미검증 항목이 있으면 다음 Task를 시작하지 않는다.
- 체크리스트 결과에는 실행 환경, 성공 여부, 실패 원인을 기록한다.

