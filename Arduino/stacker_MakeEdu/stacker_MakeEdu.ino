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
 **************************************************************************/

#include <ArduinoJson.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif
#include <Servo.h>

String inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete
StaticJsonDocument<200> doc;

Adafruit_NeoPixel strip ;

void setup() {
  // initialize serial:
  Serial.begin(115200);
  
  // reserve 200 bytes for the inputString:
  inputString.reserve(200);
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
      
      strip.setPixelColor(index, strip.Color(red, green, blue));          //  Neopixel 색상 설정 ( 첫번째 소자위치 , 색상설정(Red) , 0 , 0 )
      strip.show();
    }
    else if(rqtStr.equals("lb"))  // LED Strip bright
    {
      int bright = doc["v"];
      strip.setBrightness(bright);    //  BRIGHTNESS 만큼 밝기 설정
      strip.show();
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
      
      char* motorNO = doc["p"];
      char* motorDir = doc["d"];
      int _value = doc["v"];

      String motorNOStr(motorNO);
      String motorDirStr(motorDir);

      if(motorNOStr.equals("M1"))
      {
        if(motorDirStr.equals("CW"))
        {
          
          if(_value > 200) _value=200;
          else if(_value < 45) _value=45;
          
          _value = 255-_value;   
          analogWrite(6,_value);
          digitalWrite(9,HIGH);      
        }
        else if(motorDirStr.equals("CCW"))
        {
          if(abs(_value)>=200) _value=210;
          else if(abs(_value)<=45) _value=45;
          
          analogWrite(6,abs(_value));
          digitalWrite(9,0);    
        }
        else
        {
           digitalWrite(6,0);    
           digitalWrite(9,0);          
        }
      }
      else if(motorNOStr.equals("M2"))
      {
        //Serial.println("M2");
        if(motorDirStr.equals("CW"))
        {
          if(_value > 200) _value=200;
          else if(_value < 45) _value=45;
          
          _value = 255-_value;        
          analogWrite(5,_value);
          digitalWrite(3,HIGH);
        }  
        else if(motorDirStr.equals("CCW"))
        {
          if(abs(_value)>=200) _value=210;
          else if(abs(_value)<=45) _value=45;
              
          analogWrite(5,abs(_value));
          digitalWrite(3,0);    
        }
        else
        {
           digitalWrite(3,0);    
           digitalWrite(5,0);  
        }
      }
      
    }
    else if(rqtStr.equals("servo")) // servo control
    {
      //Serial.println("servo");
    }
    else if(rqtStr.equals("servo_m")) // servo mode
    {
      //Serial.println("servo_m");
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
