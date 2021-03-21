# Stacker-MakerEdu
scratch IO Board based on Arduino

<img src="https://github.com/makezonefablab/Stacker-MakerEdu/blob/main/img/makeredu.png" width="380px" title="메이커 에듀 " alt="RubberDuck"></img><br/>

> **Note:** 이 소프트웨어는 현재 계속 업데이트 중 입니다. 현재 버전은 1.0 입니다.





## 스태커, 스크래치 기반의 STEAM 코딩 툴

스크래치 기반의 STEAM 코딩 툴 , 스태커(Stacker) 입니다.

아두이노 기반의 IO보드와 연동이되며 다양한 하드웨어와 연동이 되도록 구성되었습니다.

[*스태커 소개*](http://makezone.co.kr/blog/2021/01/01/%ec%8a%a4%ed%81%ac%eb%9e%98%ec%b9%98-%ea%b8%b0%eb%b0%98%ec%9d%98-sw-%ec%bd%94%eb%94%a9-%ed%88%b4-%ec%8a%a4%ed%83%9c%ec%bb%a4-stacker/)



메이커 에듀보드의 소개는 아래 링크를 확인하세요.

[*메이커 에듀 보드 소개*](http://makezone.co.kr/blog/2018/10/25/%eb%88%84%ea%b5%ac%eb%82%98-%ec%93%b0%eb%8a%94-%eb%a9%94%ec%9d%b4%ed%81%ac-%ec%97%90%eb%93%80make-edu-%eb%b3%b4%eb%93%9c/)



[*스태커 체험하기*](https://stacker.fun)

![Stacker Intro](https://github.com/makezonefablab/Stacker-MakerEdu/blob/main/img/stacker.png)  


---------------------------------------

## 메이커 링크 (for Microsoft Windows 10)

스크래치의 Scratch Link와 같은 기능이지만, 시리얼통신을 하도록 구성되었습니다.

스태커 실행 전 먼저 메이커 링크를 실행 후 메이커 에듀보드(아두이노 나노호환보드)와 연결합니다. 메이커 에듀보드 익스텐션과 연동하여 콘텐츠를 제작할 수 있습니다.

메이커 링크는 윈도우10 (windows 10)이상에서 안정되게 작동합니다.


![Stacker Intro](https://github.com/makezonefablab/Stacker-MakerEdu/blob/main/img/makerlink.png)  


메이커 링크가 실행되면 tray로 이동됩니다.

tray에서 더블클릭하면 숨겨졌던 메이커에듀가 나타나고, "장치 연결"에서 알맞은 아두이노 장치를 선택합니다.

그리고, com포트를 찾아 연결합니다.

장치를 선택하고,com포트가 연결되면 이제 사용가능 상태가 됩니다.


![Stacker in_Tray](https://github.com/makezonefablab/Stacker-MakerEdu/blob/main/img/tray.png) 

### 1. 디바이스 선택
>> 연결 사용할 아두이노 종류를 선택합니다. 메이커 에듀보드는 Arduino Nano입니다.


![Stacker devicelist](https://github.com/makezonefablab/Stacker-MakerEdu/blob/main/img/selectdevice.png)



### 2. 시리얼포트 선택
>> 아두이노를 연결한 시리얼포트 번호를 찾아 선택합니다.

![Stacker devicelist](https://github.com/makezonefablab/Stacker-MakerEdu/blob/main/img/selectserial.png)



### 3. 시리얼포트 열기
>> 아두이노를 연결한 시리얼포트 번호를 선택 후 "connect" 버튼을 누르면 "disconnect"로 바뀝니다.


>> 이렇게 바뀌면 정상적인 연결입니다.

![Stacker devicelist](https://github.com/makezonefablab/Stacker-MakerEdu/blob/main/img/openserial.png)



### 4. 펌웨어 업데이트
>> 메이커에듀 블럭과 연동을 위해서는 전용의 펌웨어를 업로드합니다.


>> Arduino Nano를 선택 후 "펌웨어 업로드" 메뉴를 클릭하면 업로드가 자동으로 됩니다.


>> 단, Arduino Nano에 맞게 개발된 "메이커 에듀보드" 용 입니다.

![Stacker devicelist](https://github.com/makezonefablab/Stacker-MakerEdu/blob/main/img/firmware.png)





아래 링크를 클릭하면 다운로드 됩니다. 메이커 링크는 현재 windows 버전만 개발되었습니다.

## [*Maker Link Download*](http://naver.me/F9QWKdOv)


## 메이커 링크 (by Processing)

메이커 링크의 간단한 기능을 mac OS 용으로 구성하였습니다.

Processing으로 개발되었으며, www.processing.org에서 그 개발환경을 다운받을 수 있습니다.

스태커 펌웨어를 업로드 하는 기능이 없이 단지, 블럭과의 통신을 통해 프로토콜만 주고 받는 기능으로 이루어졌습니다.

### 1. 실행

![Maker Link Start](https://github.com/makezonefablab/Stacker-MakerEdu/blob/main/img/makerlink_mac1.png)

### 2. 시리얼포트 선택 후 open

![Maker Link Start](https://github.com/makezonefablab/Stacker-MakerEdu/blob/main/img/makerlink_mac2.png)

### 3. 시리얼포트 열기 성공

![Maker Link Start](https://github.com/makezonefablab/Stacker-MakerEdu/blob/main/img/makerlink_mac3.png)


아래 링크를 클릭하면 다운로드 됩니다.

## [*Maker Link Download*](http://naver.me/xScNuPKt)



## Quickstart

1. 메이커 링크 실행 (시리얼 포트 접속)
2. 스태커 실행
3. 메이커 에듀 익스텐션 추가
4. 코딩 !!!
