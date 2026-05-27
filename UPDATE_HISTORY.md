# UPDATE HISTORY

이 문서는 프로젝트 작업 내역을 간단하게 기록하기 위한 파일입니다.

프로젝트를 업데이트한 경우, 예를 들어
- commit
- pull request
- 기능 추가
- 기능 수정
- 버그 수정
- 문서 업데이트

등의 작업을 진행한 뒤, 아래 형식에 맞추어 짧게 기록해 주세요.

이 기록은 이후에
- 프로젝트 진행 상황 확인
- 팀원별 작업 내용 정리
- 발표 준비
- 보고서 작성
- GitHub commit history 확인

을 더 쉽게 하기 위해 사용됩니다.

추가로, `UPDATE_HISTORY` 파일 자체를 수정하는 경우에는  
commit message를 **`UPDATE_HISTORY`**로 작성해 주세요.

그리고 **Extended Description**에는  
이 파일에서 구체적으로 어떤 내용을 수정했는지 짧게 적어 주세요.

예를 들면:
- 기록 형식 수정
- 예시 추가
- 구조 변경
- 작성 규칙 추가

이 규칙은 `UPDATE_HISTORY` 파일의 변경 내역도 일정한 형식으로 정리하기 위해 사용됩니다.

가능하면 작업한 날짜에 바로 기록해 주세요.

---

## 기록 형식

## YYYY-MM-DD

### **Name**

**Task:**
- Commit message (hash)
- Commit message (hash)

**Details:**  
해당 날짜에 수행한 작업 내용을 간단하게 설명합니다.

---

# Updates

* * *

## 2026-05-27

### Pak Denis

Task:

  * hitbox 추가 및 player/enemy collision 연결 (`51254a0`)

Details:
Player, Enemy, Bullet에 hitbox를 연결하고, player bullet vs enemy, enemy bullet vs player, player vs enemy 충돌 흐름을 현재 구조에 맞게 정리했습니다.
또한 적과 플레이어의 기본 상호작용을 실제 게임 흐름 안에서 확인할 수 있도록 연결했습니다.

### Pak Denis

Task:

  * collision 및 hitbox 기본 구조 구현 (`9bd2b21`)

Details:
기본 collision 구조를 프로젝트에 추가하고, hitbox 판정과 respawn, game state 흐름이 함께 동작하도록 정리했습니다.
이 작업을 통해 단순 렌더링과 발사 프로토타입 단계를 넘어서 실제 충돌과 life 처리까지 연결되는 기반을 만들었습니다.

### Pak Denis

Task:

  * manually integrate Sera UI into current main structure (`6056889`)

Details:
Sera가 작업한 시작 화면 UI 내용을 최신 main 구조에 맞게 수동으로 통합했습니다.
StartupScreen, 별 배경, 카운트다운 흐름, 텍스트 배치 등을 현재 collision, respawn, game state 로직과 호환되도록 다시 정리하고, 빌드 및 실행 가능한 상태를 확인했습니다.

---
## 2026-05-26

###Sera
**Task:**

StartupScreen 구현 및 게임 상태 흐름 정리 (`e5e6c1c`)
**Details:** 
StartupScreen.cpp / .h 를 별도 파일로 분리하여 구현했습니다.
Game.cpp 의 메인 루프 구조(Input / Update / Render)를 유지하면서
GameState enum 을 통해 화면 전환 흐름을 관리했습니다. Start 기능, Paused 기능, GlobalTime 구현했습니다.

### **Pak Denis**

**Task:**
- DEV_LOG 내용 보완 및 현재 작업 방향 정리 (`90741e5`)

**Details:**  
`DEV_LOG.md` 내용을 보완하고, 현재 단계에서 무엇을 중심으로 작업했는지 더 자연스럽고 명확하게 정리했습니다.  
특히 Player / Bullet / Enemy 구조 분리, 발사 시스템에 대한 고민, 적 배치와 렌더링, 그리고 collision 이전 단계까지의 흐름이 잘 드러나도록 내용을 정리했습니다.

### **Pak Denis**

**Task:**
- Player/Bullet/Enemy 구조 분리 및 적/발사 프로토타입 구현 (`3976697`)

**Details:**  
기존에 Game 안에 있던 플레이어와 총알 관련 로직을 정리하고, Player, Bullet, Enemy 구조를 분리했습니다.  
Player 클래스를 새로 만들고, Bullet 클래스를 기반으로 player bullet system과 enemy bullet system을 추가했습니다.  
또한 Enemy 클래스를 생성하고 3가지 적 타입을 테스트 형태로 배치했으며, square / down triangle / diamond 형태로 렌더링할 수 있도록 GraphicsContext를 확장했습니다.  
마지막으로 Type2 적이 아래 방향으로 발사하는 프로토타입까지 연결하여, 적과 발사 구조의 기본 동작을 확인할 수 있는 단계까지 구현했습니다.

---
## 2026-05-19
### **Pak Denis**

**Task:**
- DEV_LOG 내용 수정 및 5월 19일 개발 기록 정리 (8eed5bdfc78126b8a40969fe4e6b3a864a663f3c)

**Details:**  
`DEV_LOG.md`의 5월 19일 개발 기록을 수정하고 내용을 더 자연스럽고 읽기 쉽게 정리했습니다.  
프로젝트 기반 구성, 플레이어 프로토타입, 이동, 발사 시스템, 그리고 이후 발사 로직 확장 계획까지 조금 더 명확하게 설명하도록 보완했습니다.

**Task:**
- MEETING_NOTES 업데이트 및 다음 미팅 논의 계획 추가 (3ea2a0de0569c7d22dcfdac6a58a500a3a087b83)

**Details:**  
`MEETING_NOTES.md`를 업데이트하고, 다음 수업 시간에 팀과 함께 논의할 계획을 추가했습니다.  
다이어그램, 미래 구조, 발사 시스템 개선, 적 오브젝트, hitbox, collision, 그리고 다음 역할 분담과 관련된 논의 내용을 정리했습니다.

**Task:**
- DIAGRAM 구조 수정 및 내용 정리 (dbd7b7c8d2b4c96491bd585fb741091cd5722bc5)

**Details:**  
`DIAGRAM.md` 파일의 Mermaid 구조를 수정하고 전체 내용을 더 읽기 쉽게 정리했습니다.  
Current Structure와 Future Structure를 나누어 프로젝트의 현재 상태와 이후 방향을 더 명확하게 보여 줄 수 있도록 정리했습니다.

**Task:**
- DIAGRAM.md 파일에 향후 구조에 대한 세부 정보를 추가하여 보완합니다. (60615872ac7a4df933df0463afa4b1d2ba142cb8)

**Details:**  
`DIAGRAM.md` 파일에 앞으로 고려하고 있는 구조에 대한 세부 내용을 추가했습니다.  
현재 구조뿐만 아니라 이후에 확장할 수 있는 방향을 더 구체적으로 정리하여 팀과 논의하기 쉽도록 보완했습니다.

**Task:**
- DIAGRAM 초안 파일 추가 (613489b15b78997a23a8dd92b593b9acfad73dfb)

**Details:**  
프로젝트의 현재 구조와 앞으로 추가할 수 있는 구조를 시각적으로 정리하기 위한 `DIAGRAM.md` 초안 파일을 추가했습니다.  
이 파일은 다음 수업 시간에 팀원들과 함께 구조를 논의하기 위한 테스트 버전으로 사용됩니다.

**Task:**
- MEETING_NOTES 파일 추가 (8db4db111382cd7bd3c58730c6e2cc13ccde5af5)

**Details:**  
팀 미팅에서 나온 생각, 의견, 아이디어, 그리고 간단한 논의 내용을 기록하기 위한 `MEETING_NOTES.md` 파일을 추가했습니다.  
이 파일은 수업 시간이나 팀 미팅 중 이야기한 내용을 잊지 않고 정리하기 위한 용도로 만들었습니다.

**Task:**
- DEV_LOG 추가 및 5월 18~19일 작업 내용 정리 (6a5b83ef42afc950de965eea2a3635ba531343fd)

**Details:**  
프로젝트 개발 과정을 더 자세하게 기록하기 위한 `DEV_LOG.md` 파일을 추가했습니다.  
또한 5월 18일과 19일에 진행한 작업 내용을 정리하여, 단순한 commit 기록보다 더 자세한 형태로 남길 수 있도록 구성했습니다.

**Task:**
- 플레이어 첫 발사 프로토타입 추가 (4f048116e1085be617d769e6d37e7acb9e8a130b)

**Details:**  
플레이어의 기본 발사 시스템을 추가했습니다.  
현재 총알은 하나의 active 상태로 관리되며, 화면에 존재하는 동안에는 새로운 총알이 생성되지 않습니다.  
총알은 위쪽으로 이동하고 화면 밖으로 나가면 비활성화되도록 구현했습니다.

**Task:**
- Galaga Visual Studio 프로젝트 파일 추가 (f804a483687ede8eb824a980852392e54e17d63f)

**Details:**  
Galaga 프로젝트를 위한 Visual Studio 작업 환경을 구성하고 기본 프로젝트 파일을 추가했습니다.  
이제 solution이 정상적으로 열리며, Visual Studio에서 프로젝트 구조를 확인하고 이후 개발을 계속 진행할 수 있는 상태가 되었습니다.

**Task:**
- Visual Studio용 .gitignore 파일 추가 (dc491e9040992d3c6a77ff0e73c64012a00cd964)

**Details:**  
Visual Studio 프로젝트에서 생성되는 임시 파일, 빌드 결과물, 캐시 파일 등이 저장소에 포함되지 않도록 `.gitignore` 파일을 추가했습니다.  
이를 통해 프로젝트 저장소를 더 깔끔하게 유지하고, 불필요한 파일이 GitHub에 올라가지 않도록 정리했습니다.


**Task:**
- 프로젝트 업데이트 기록을 위한 UPDATE_HISTORY.md 파일 추가 (eef2438ed26726e50da1a894a4239b863fb8bec3)

**Details:**  
팀 프로젝트의 업데이트 내역을 정리하기 위해 `UPDATE_HISTORY.md` 파일을 생성했습니다.  
이 파일에는 작업 기록을 남기기 위한 기본 구조, 작성 형식, 그리고 예시를 추가했습니다.  
이후 팀원들이 날짜별 작업 내용과 commit 내역을 정리할 수 있도록 하기 위한 목적입니다.

---

## 2026-05-18

### **Pak Denis**

**Task:**
- Galaga 프로젝트 README 초안 추가 (fdbe6f15365aea5935a81fa725f83a4a106dd03e)

**Details:**  
Galaga 프로젝트의 첫 README 초안을 추가했습니다.  
프로젝트 주제, 장르, 핵심 플레이, 개발 목표, 개발 환경, 팀원 정보 등 기본적인 프로젝트 설명을 정리했습니다.  
이 문서는 이후 저장소 구조를 정리하고 프로젝트 방향을 구체화하는 기준이 되는 초안입니다.

---

## 2026-05-06

### **Pak Denis**

**Task:**
- Initial commit (8ebace9e0d166bc69adb9d6535ee247a8a3a2adb)

**Details:**  
프로젝트의 초기 commit을 생성했습니다.  
이 commit에는 LICENSE 파일과 README의 초기 구조가 포함되어 있으며, 이후 프로젝트를 정리하고 확장하기 위한 기본 시작점으로 사용됩니다.
