/**************************************************************************
 * 
 * Maker Edu Firmware
 * 
 * 아두이노 기반의 스크래치 보드
 * 
 * Author : Makezone
 * Begin  : 2020.12.24
 * 
 * Modify :
 * - Beep 쪽 추가 필요
 * 
 * 2021.01.24
 * - LED Strip 코드 추
 * 
 * 2021.02.05
 * - #include <makeredu.h>
 * - start() 함수 호출
 * - Buzzer, servo, 모터 추가 
 * - Note는 파라미터 전달 문제있음
 * 
 * 2021.02.14
 * - servo 모터 다중제어 완성
 * - led strip 함수 수정 
 * 
 * 2021.02.15
 * - LED strip 버그 수정
 * 
 * 2021.04.29
 * - makerEdu_lib로 교체
 * - Adafruit_NeoPixel.h 주석처리 (makerEdu_lib와 충돌)
 *
 * 2021.09.23
 * - pinmode에 pullup추가 : rqtStr.equals("dip_m")
 * 
 * 
 **************************************************************************/

#include <Servo.h>
#include <ArduinoJson.h>
//#include <Adafruit_NeoPixel.h>
#include <makerEdu_lib.h>


String inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete
StaticJsonDocument<200> doc;

// led 스트립제어를 위한 광역변수 - 기존 버전에서 strip인스턴스를 loop에 설정하면 핀에 노이즈가 생기는 문제가 있는것으로 추정됨
// 그러므로 인스턴스 설정을 먼저 해주고 loop에서 픽셀개수와 사용핀을 네오픽셀 라이브러리 내 픽셀수와 핀설정 명령함수로 재설정함
int NUM_LEDS; 
int portNo;

Adafruit_NeoPixel strip_m = Adafruit_NeoPixel(NUM_LEDS, portNo, NEO_GRB + NEO_KHZ800);

Servo srv[13];

void setup() {
  // initialize serial:
  Serial.begin(115200);
  
  // reserve 200 bytes for the inputString:
  inputString.reserve(200);

  //start();
}

void loop() {
  // put your main code here, to run repeatedly:

  // print the string when a newline arrives:
  if (stringComplete) {
    // Serial.println("OK : " + inputString);
    // clear the string:

    // Deserialize the JSON document
    DeserializationError error = deserializeJson(doc, inputString);
  
    // Test if parsing succeeds.
    if (error) {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.c_str());
      inputString = "";
      stringComplete = false;
      return;
    }

    char* rqt = doc["rqt"];
    
    String rqtStr(rqt);
    
    //Serial.println("OK : " + rqtStr);

    if(rqtStr.equals("li"))  // LED strip_m initialize
    {
      NUM_LEDS = doc["c"];
      portNo = doc["p"];

      strip_m.updateLength(NUM_LEDS); // 픽셀개수 재설정
      strip_m.setPin(portNo);         // 사용포트 재설정
      strip_m.begin();
      strip_m.show();
      
    }
    else if(rqtStr.equals("ls"))  // LED strip_m set color
    {
      int index = doc["i"];
      int red = doc["r"];
      int green = doc["g"];
      int blue = doc["b"];
      
      strip_m.setPixelColor(index-1, strip_m.Color(red, green, blue));          //  Neopixel 색상 설정 ( 첫번째 소자위치 , 색상설정(Red) , 0 , 0 )
      strip_m.show();
    }
    else if(rqtStr.equals("lb"))  // LED strip_m bright
    {
      int bright = doc["v"];
      strip_m.setBrightness(bright);    //  BRIGHTNESS 만큼 밝기 설정
      // strip_m.show();
    } 
    else if(rqtStr.equals("buzzer"))  // Buzzer
    {
      //Serial.println("do");
      
      int pinNO = doc["p"];
      int time1 = doc["ondelay"];
      int time2 = doc["offdelay"];

      buzzer(time1, time2);
    }
    else if(rqtStr.equals("melody_sp"))  // Melody Speed
    {
      //Serial.println("do");
      
      int spd = doc["s"];

      tempo(spd);
    }
    else if(rqtStr.equals("note"))  // Note on
    {
      //Serial.println("do");
      
      int pinNO = doc["p"];
      int _t = doc["tempo"];
      int _b = doc["beat"];

      note(_t, _b);
    }
    else if(rqtStr.equals("do"))  // Digital Wirte
    {
      //Serial.println("do");
      
      int pinNO = doc["p"];
      int _value = doc["v"];

      digitalWrite(pinNO,_value);
    }
    else if(rqtStr.equals("do_m")) // Digital output mode
    {
      //Serial.println("do_m");
      int pinNO = doc["p"];

      pinMode(pinNO,OUTPUT);
    }
    else if(rqtStr.equals("pwm")) // PWM
    {
      //Serial.println("pwm");
      int pinNO = doc["p"];
      int _value = doc["v"];
      
      analogWrite(pinNO, _value);
    }
    else if(rqtStr.equals("pwm_m")) // digital output mode
    {
      //Serial.println("pwm_m");
      int pinNO = doc["p"];
  
      pinMode(pinNO,OUTPUT);
    }
    else if(rqtStr.equals("motor")) // Motor
    {

      int _lspd = doc["lspd"];
      int _rspd = doc["rspd"];
      int _t = doc["t"];

      motor(_lspd, _rspd, _t);
      
    }
    else if(rqtStr.equals("servo")) // servo control
    {
      int pinNO = doc["p"];
      int _v = doc["v"];
      
      srv[pinNO-1].write(_v);
      delay(15);
    }
    else if(rqtStr.equals("servo_m")) // servo mode
    {
      int pinNO = doc["p"];
      
      srv[pinNO-1].attach(pinNO);
    }
    else if(rqtStr.equals("ai")) // read analog
    {
      //Serial.println("ai");
      int pinNO = doc["p"];
      
      double _value = analogRead(pinNO);

      StaticJsonDocument<200> docRsp;
      docRsp["rsp"] = "ai";
      docRsp["p"] = pinNO;
      docRsp["v"] = _value;
      serializeJson(docRsp, Serial);
      Serial.write('\n');
      
    }
    else if(rqtStr.equals("ai_m"))
    {
      //Serial.println("ai_m");
    }
    else if(rqtStr.equals("di"))// read digital
    {
      //Serial.println("di");
      int pinNO = doc["p"];
      
      int _value = digitalRead(pinNO);

      StaticJsonDocument<200> docRsp;
      docRsp["rsp"] = "di";
      docRsp["p"] = pinNO;
      docRsp["v"] = _value;
      serializeJson(docRsp, Serial);
      Serial.write('\n');
    }
    else if(rqtStr.equals("di_m")) // digital input mode
    {
      //Serial.println("di_m");
      int pinNO = doc["p"];
      
      pinMode(pinNO,INPUT);
    }
    else if(rqtStr.equals("dip_m")) // digital input pullup mode
    {
      //Serial.println("di_m");
      int pinNO = doc["p"];
      
      pinMode(pinNO,INPUT_PULLUP);
    }

    
    inputString = "";
    stringComplete = false;
  }

}

/*
  SerialEvent occurs whenever a new data comes in the hardware serial RX. This
  routine is run between each time loop() runs, so using delay inside loop can
  delay response. Multiple bytes of data may be available.
*/
void serialEvent() {
  while (Serial.available()) 
  {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '}') {
      stringComplete = true;
    }
  }
}
