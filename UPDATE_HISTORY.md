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

## 2026-05-19

### **Pak Denis**

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
