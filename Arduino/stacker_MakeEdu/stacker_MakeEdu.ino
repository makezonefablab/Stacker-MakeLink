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
 * 
 **************************************************************************/

#include <Servo.h>
#include <ArduinoJson.h>
#include <Adafruit_NeoPixel.h>
#include <makeredu.h>


String inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete
StaticJsonDocument<200> doc;

Adafruit_NeoPixel strip ;

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
    Serial.println("OK : " + inputString);
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

    if(rqtStr.equals("li"))  // LED Strip initialize
    {
      int _count = doc["c"];
      int _pin = doc["p"];

      strip = Adafruit_NeoPixel(_count, _pin, NEO_GRB + NEO_KHZ800);
      
      strip.begin();
      
      for(int i = 0 ; i < _count ; i++)
      {
        strip.setPixelColor(i, strip.Color(0, 0, 0));
      }
      
      strip.show();
      
    }
    else if(rqtStr.equals("ls"))  // LED Strip set color
    {
      int index = doc["i"];
      int red = doc["r"];
      int green = doc["g"];
      int blue = doc["b"];
      
      strip.setPixelColor(index-1, strip.Color(red, green, blue));          //  Neopixel 색상 설정 ( 첫번째 소자위치 , 색상설정(Red) , 0 , 0 )
      strip.show();
    }
    else if(rqtStr.equals("lb"))  // LED Strip bright
    {
      int bright = doc["v"];
      strip.setBrightness(bright);    //  BRIGHTNESS 만큼 밝기 설정
      strip.show();
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

    
    inputString = "";
    stringComplete = false;
  }

  delay(10);

}

/*
  SerialEvent occurs whenever a new data comes in the hardware serial RX. This
  routine is run between each time loop() runs, so using delay inside loop can
  delay response. Multiple bytes of data may be available.
*/
void serialEvent() {
  while (Serial.available()) {
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
