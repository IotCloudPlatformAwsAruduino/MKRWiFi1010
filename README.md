
#💡스마트 에어컨시스템 

**[💦]**  
1인가구 주거공간의 여름철 온습도를 IoT 기기와 AWS 서비스를 이용한 IoT 클라우드 플랫폼을 구축하여 자동으로 제어하는 시스템 구축

___
## 적용된 AWS 서비스 
  <li>AWS IoT Core</li>
  <li>AWS Lambda</li>
  <li>Amazon DyanmoDB</li>
  <li>Amazon API Gateway</li>
  
## HW 구성
  <li>MKRWiFi1010 보드</li>
  <li>SG90 서보모터 : D3 </li>
  <li>DHT11 온도 습도 센서 : D2</li>
  <li>TEMT6000 조도센서 : A1</li>
  
  ![image](https://user-images.githubusercontent.com/72599051/144745942-1e4392ea-9d46-4c16-b18a-196ae228a6a1.png)

  
___ 

## 서비스 구성도
API Gateway를 통해 API를 호출하여  안드로이드 app상에서 사물의 상태를 확인 및 수정을 할 수 있습니다. 
또한 Dynamodb에 저장하여 사물의 로그를 조회하거나 나아가서 분석할 수 있는 데이터로 사용이 가능해 집니다.
아래와같은 다양한 AWS의 서비스들을 이용하여 사물을 좀 더 편리하고 쉽게 다룰 수 있게 됩니다.
![image](https://user-images.githubusercontent.com/72599051/143241035-e962d196-8519-418c-a038-5cf6dc6108d7.png)

___ 

## 안드로이드 APP 화면 예시 
센서에 의해 측정된 값들은 AWS상의 mqtt 클라이언트에 의해 구독되어져서 구축해둔 REST API 를 통해 사용자가 APP상에서 확인 및 사물의 상태를 제어할 수 있습니다.
![image](https://user-images.githubusercontent.com/72599051/144988777-40561d91-c9ee-4e11-b669-2ac964e43a50.png)

___ 


## 1. REST API 설계
다음 프로그램 코드를 실행시키기 위해서는 다음 형식의 REST API가 준비되어 있어야 합니다.
- 디바이스 로그 조회 
```
 GET /devices/{deviceId}/log?from=yyyy-mm-dd hh:mm:ss&to=yyyy-mm-dd hh:mm:ss
```
- 디바이스 상태 조회 : 온습도 , 에어컨 상태 확인(ON/OFF)
```
GET /devices/{deviceId}
```
- 디바이스 상태 변경 : 에어컨 ON/OFF
```
 PUT /devices/{deviceId}
```
  <li>message body (payload)</li>
  ```
  { 
 	"tags" : [
 		{
 			"attrName": "temperature",
 			"attrValue": "27.0"
 		},
 		{
 			"attrName": "LED",
 			"attrValue": "OFF"
 		}
 	]
 }
```

### Prerequisites / 선행 조건


```
- Arduino IDE 
- Android Mobile Phone or Android Studio IDE + Emulater
- AWS Account 

참고사항 : 
- AWS IoT 와 MKRWiFi1010 연결방법 : https://kwanulee.github.io/AWS_IoT_DHT11/
- 디바이스 섀도우 사용법 : https://kwanulee.github.io/AWS_IoT_DHT11/device-shadows.html
- AWS IoT 사용법 : https://kwanulee.github.io/AWS_IoT_DHT11/
- API Gateway 사용법 : https://kwanulee.github.io/APIGatewayPractice/

```


## Running the tests / 테스트의 실행

( 선행조건이 만족되었다고 가정 )
1. 센서 및 모터가 연결된 MKRWiFi1010 보드에 전원을 공급.
2. 안드로이드 App을 다운로드 혹은 에뮬레이터로 실행.
3. 안드로이드 App에 표현된 사물의 상태를 확인 및 제어.


### 테스트는 이런 식으로 동작합니다

```
적정 온도 및 습도를 벗어나면 스마트시스템 상태가 ON 일 경우에 적정온도 및 습도를 유지하기 위해 에어컨 및 제습기를 제어가능.
```
___ 

## 아두이노 코드 핵심 flow  
![image](https://user-images.githubusercontent.com/72599051/144989300-7d5f013e-73c2-4334-b0f4-92b863ca6a8a.png)

___ 



## Built With / 누구랑 만들었나요?

* [1791320_최종현](https://github.com/TonyJHC) 
   1. 안드로이드 APP 개발 ​

       -  온습도 , 에어컨 상태 확인  (완료)​

       -  스마트 에어컨 시스템 제어 (완료)​
     

   2.  AWS IoT 클라우드 플랫폼 구축​

       - GET : 온습도 , 에어컨 상태 확인   (완료)​

       - PUT : 스마트 에어컨 시스템 제어  (완료)​

       - Dynamodb table에 저장 (완료)​
      

   3. 아두이노 HW/SW 개발 ​

       - 온습도 , LED, 조도센서 , 서보모터 연결 및 기본설정(완료)​

       - APP 에서 스마트 에어컨 시스템 작동 시 사물의 상태에 따라 에어컨 ON/OFF (완료)​


   4.  ppt 제작 및 github 관리 및 read.me 작성​


* [1771391 유지만](https://github.com/jiman-you) - 작업 내용

   1. 안드로이드 APP 개발​

      -  로그조회 화면 연동​

   2.  AWS IoT 클라우드 플랫폼 구축​

       - 로그조회 화면 연동​

       - SNS이메일 연동​

   3. 시연 동영상 촬영​
      https://youtu.be/_r9dTBWbfAU​
      

## Contributiong / 기여
   1. 안드로이드 APP 개발 ​

         -  [1791320 최종현] 

         ![image](https://user-images.githubusercontent.com/72599051/144746833-3f8a75c0-0d32-4cd3-aa9d-660eb0970a34.png)   



         -  [1771391 유지만] 
          
         ![image](https://user-images.githubusercontent.com/72599051/144746687-d5807104-000e-490c-95b6-171c6c920afb.png)


   2.  AWS IoT 클라우드 플랫폼 구축​

         -  [1791320 최종현] 
        
          ![image](https://user-images.githubusercontent.com/72599051/144746754-36df46a4-048d-4c19-b56d-1b2648dbde3f.png)
  
      
         -  [1771391 유지만] 
         
          ![image](https://user-images.githubusercontent.com/72599051/144746766-fc33e95a-3b14-4a52-83c2-4ca500f05f04.png) 


   3. 아두이노 HW/SW 개발 ​

         -  [1791320 최종현] 
         
         ![image](https://user-images.githubusercontent.com/72599051/144746722-feed2e11-ab9b-4882-a2e0-555576392a09.png)


Please read [CONTRIBUTING.md](https://gist.github.com/PurpleBooth/b24679402957c63ec426) for details on our code of conduct, and the process for submitting pull requests to us. / [CONTRIBUTING.md](https://gist.github.com/PurpleBooth/b24679402957c63ec426) 를 읽고 이에 맞추어 pull request 를 해주세요.

## License / 라이센스

This project is licensed under the MIT License - see the [LICENSE.md](https://gist.github.com/PurpleBooth/LICENSE.md) file for details / 이 프로젝트는 MIT 라이센스로 라이센스가 부여되어 있습니다. 자세한 내용은 LICENSE.md 파일을 참고하세요.

