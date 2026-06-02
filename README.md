# Galaga - Game Programming Project

## 1. 프로젝트 소개

본 프로젝트는 고전 아케이드 슈팅 게임인 **Galaga** 스타일을 참고하여 제작한 2D 아케이드 슈팅 게임입니다.

플레이어는 우주선을 조작하여 화면 위에서 등장하는 적 기체들을 공격하고, 적의 총알과 충돌을 피하면서 점수를 획득합니다.  
게임은 wave 단위로 진행되며, 최종 목표는 **30 wave까지 도달하여 엔딩 화면을 보는 것**입니다.

프로젝트에서는 단순한 슈팅 구조뿐만 아니라, wave 진행, enemy AI, tractor beam, capture 연출, bonus enemy, score / high score, game over / ending 화면 등을 구현하여 하나의 완성된 게임 흐름을 만드는 것을 목표로 했습니다.

---

## 2. 팀 정보

- **팀 이름:** Galaga

| 학번 | 이름 |
| :--- | :--- |
| 12230350 | Pak Denis |
| 12215560 | NURMAISARAH BINTI AZAHAR |
| 12211608 | 박상하 |

---

## 3. 역할 분담

| 이름 | 학번 | 주요 담당 |
| :--- | :--- | :--- |
| Pak Denis | 12230350 | 프로젝트 기본 구조 설계, GitHub 관리, player / bullet / enemy 기본 구조, collision 및 hitbox, wave system, Type4 bonus star, lives icon 조정, final score / high score 표시, Game Over 및 Ending 점수 화면 정리 |
| 박상하 | 12211608 | enemy logic, enemy movement, enemy attack pattern, score system 기본 구현, tractor beam / capture logic, window focus pause 처리, 30 wave ending 연출, bug fix, PPT 준비 |
| NURMAISARAH BINTI AZAHAR | 12215560 | StartupScreen, font texture, Galaga title / start / exit UI, lives rendering, image resources, UI 수정, visual elements, background / screen 관련 작업, bug fix |

---

## 4. 개발 환경

- **Language:** C++
- **Graphics API:** DirectX 11
- **IDE:** Visual Studio
- **Version Control:** Git / GitHub
- **Platform:** Windows

---

## 5. 게임 조작 방법

| 기능 | 키 |
| :--- | :--- |
| 이동 | `← / →` 또는 `A / D` |
| 공격 | `Space Bar` |
| 일시정지 | `P` |
| 재시작 | `R` |
| 종료 | `ESC` |
| 30 wave 테스트 | `Z` |
| Game Over 테스트 | `X` |

`Z`와 `X` 키는 발표 및 테스트를 위한 debug shortcut입니다.

---

## 6. 게임 규칙

- 플레이어는 적 기체를 처치하여 점수를 획득합니다.
- 적 기체의 타입에 따라 획득 점수가 다릅니다.
- 플레이어는 기본적으로 3개의 생명(lives)을 가지고 시작합니다.
- 적의 총알에 맞거나 적 기체와 충돌하면 생명이 1 감소합니다.
- 모든 생명을 잃으면 Game Over 상태가 됩니다.
- 각 wave의 모든 적을 처치하면 다음 wave로 넘어갑니다.
- 30 wave의 모든 적을 처치하면 Ending 화면이 출력됩니다.

---

## 7. 주요 기능

### 7.1 Player System

- 플레이어 좌우 이동
- Space Bar를 이용한 총알 발사
- 발사 쿨타임 적용
- 최대 총알 수 제한
- 피격 시 생명 감소
- 리스폰 후 일정 시간 무적 처리
- 무적 상태 시 깜빡임 효과 적용

---

### 7.2 Enemy System

적 기체는 타입과 상태에 따라 서로 다른 움직임과 역할을 가집니다.

| Type | 설명 | 점수 |
| :--- | :--- | :--- |
| Type 1 | 기본 적 기체 | 100 |
| Type 2 | 공격형 적 기체 | 200 |
| Type 3 | 보스 기체 / Tractor Beam 사용 | 500 |
| Type 4 | Bonus Star | 1000 또는 생명 회복 |

Enemy는 내부적으로 상태 머신을 사용하여 다음과 같은 상태를 가집니다.

- Idle
- Diving
- Looping
- Beaming
- Capturing
- Returning

---

### 7.3 Tractor Beam & Capture

Type 3 보스 기체는 특정 위치에서 tractor beam을 발사할 수 있습니다.

- 빔은 아래로 퍼지는 형태의 시각 효과로 렌더링됩니다.
- 빔의 길이와 너비에 맞춰 hitbox가 갱신됩니다.
- 플레이어가 빔에 닿으면 capture 상태가 됩니다.
- capture된 플레이어 기체는 보스 기체 옆에 뒤집힌 상태로 매달립니다.
- 이미 기체를 납치한 Type 3는 추가로 두 번째 기체를 납치하지 않도록 처리했습니다.

---

### 7.4 Wave System

- 게임은 wave 단위로 진행됩니다.
- 각 wave마다 적 기체들이 배치됩니다.
- 모든 적을 처치하면 wave clear 상태가 됩니다.
- 10 wave 이후부터는 적의 속도와 공격 관련 수치가 점진적으로 증가합니다.
- 30 wave 클리어 시 Ending 화면으로 전환됩니다.

---

### 7.5 Bonus Star

Type 4 Bonus Star는 5 wave 단위로 등장하는 특별한 보너스 객체입니다.

- 플레이어의 생명이 3개 미만일 경우, 일정 확률로 생명을 회복하거나 1000점을 획득합니다.
- 생명이 이미 가득 차 있다면 1000점을 획득합니다.
- 이를 통해 단순 점수 획득뿐만 아니라 생존과 관련된 보너스 요소를 추가했습니다.

---

### 7.6 Score & High Score

- 적을 처치하면 score가 증가합니다.
- Game Over 화면에서 `FINAL SCORE`와 `HIGH SCORE`가 표시됩니다.
- Ending 화면에서도 `FINAL SCORE`와 `HIGH SCORE`가 표시됩니다.
- `HighScore`는 프로그램 실행 중에만 유지되며, 별도의 파일 저장 기능은 포함하지 않았습니다.

---

### 7.7 UI / Visual System

- 시작 화면
- Game Over 화면
- Ending 화면
- Score 표시
- Lives icon 표시
- Wave Clear 표시
- Background star scrolling
- Screen shake effect
- Floating text animation

---

## 8. 프로젝트 구조

프로젝트는 다음과 같은 주요 클래스를 중심으로 구성되어 있습니다.

| Class | 역할 |
| :--- | :--- |
| `Game` | 전체 게임 흐름 제어 |
| `WindowContext` | 윈도우 생성 및 메시지 처리 |
| `GraphicsContext` | DirectX 렌더링 처리 |
| `Player` | 플레이어 이동 및 렌더링 |
| `Enemy` | 적 기체 상태 및 AI 처리 |
| `PlayerBulletSystem` | 플레이어 총알 관리 |
| `EnemyBulletSystem` | 적 총알 관리 |
| `Bullet` | 총알 데이터 및 이동 처리 |
| `StartupScreen` | 시작 화면 처리 |

---

## 9. Object Component Map

게임 내 오브젝트는 크게 다음과 같이 분류했습니다.

### Ally

- Player
- Player Bullet

### Enemy

- Enemy Type 1 / Type 2
- Enemy Type 3
- Enemy Type 4 / Bonus Star
- Enemy Bullet
- Tractor Beam

### Background

- Star Background
- Screen Shake

### System / UI

- Game Controller
- HUD
- Start UI
- Game Over UI
- Ending UI

---

## 10. 실행 방법

1. `게임플레이` 폴더를 엽니다.
2. `Galaga.exe` 파일을 실행합니다.
3. 별도의 Visual Studio 실행 없이 게임을 플레이할 수 있습니다.

실행 파일과 필요한 리소스 파일은 같은 폴더 안에 있어야 합니다.

예시:

```text
게임플레이/
├─ Galaga.exe
├─ ship.png
├─ enemy.png
├─ enemy2.png
├─ enemy3.png
├─ bonus_star.png
├─ numbers.png
└─ font.png
```

## 11. 개발 과정

본 프로젝트는 GitHub를 이용하여 팀원들이 각자 맡은 기능을 구현하고, pull request와 merge를 통해 코드를 통합하는 방식으로 진행했습니다.

초기에는 단순한 도형을 이용하여 player, enemy, bullet system을 먼저 구현했고, 이후 이미지 리소스, enemy logic, wave system, UI, score system, hitbox adjustment, ending effect 등을 단계적으로 추가했습니다.

프로젝트 후반에는 새로운 기능을 무리하게 늘리기보다, 이미 구현된 기능들이 안정적으로 동작하도록 수정하고 발표 및 제출을 위한 문서와 실행 파일을 정리하는 데 집중했습니다.
