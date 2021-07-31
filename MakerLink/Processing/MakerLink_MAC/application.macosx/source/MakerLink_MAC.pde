import websockets.*;
import processing.serial.*;

import controlP5.*;

ControlP5 cp5;
Textarea myTextarea;
DropdownList dSerialPorts, d2;

WebsocketServer ws;

JSONObject json;
PrintWriter output;

int cnt = 0;

Serial myPort;  // Create object from Serial class
int val;        // Data received from the serial port
int serialCount = 0;
char[] serialInArray = new char[30];
int now;
float x,y;

boolean checkOpen = false;
boolean isSerialOpen = false;

/////////////////////////////////////////////////////
void setup(){
  size(480,400);
  
  cp5 = new ControlP5(this);
  
  cp5.addButton("Clear")
     .setValue(0)
     .setPosition(20,50)
     .setSize(200,19)
     ;
     
  cp5.addButton("Upload")
     .setValue(0)
     .setPosition(260,50)
     .setSize(200,19)
     ;
     
  
  myTextarea = cp5.addTextarea("txt")
                  .setPosition(20,80)
                  .setSize(440,300)
                  .setFont(createFont("arial",12))
                  .setLineHeight(14)
                  .setColor(color(128))
                  .setColorBackground(color(255,100))
                  .setColorForeground(color(255,100));
                  ;
                  
  myTextarea.setText("Test");
  
  // create a DropdownList, 
  dSerialPorts = cp5.addDropdownList("Serial Ports")
          .setPosition(20, 20)
          .setSize(200,200)
          ;
          
  customize(dSerialPorts); // customize the first list
  
  cp5.addButton("Open")
     .setValue(0)
     .setPosition(260, 20)
     .setSize(200,19)
     ;
  
  ws  = new WebsocketServer(this,21000,"/MakerEdu");
  now = millis();
  x = 0;
  y = 0;
  
  //String portName = Serial.list()[4];
  //myPort = new Serial(this, portName, 115200);
}

/////////////////////////////////////////////////////
public void Clear(int theValue) {
  myTextarea.setText("");
}


/////////////////////////////////////////////////////
public void Upload(int theValue) {
  if(checkOpen != false)
  {

    launch("arduino-cli");
  }
}



/////////////////////////////////////////////////////
public void Open(int theValue) {
  println((int)dSerialPorts.getValue());
  
  int index = (int)dSerialPorts.getValue();
  
  if(checkOpen != false)
  {
    //String portName = Serial.list()[4];
    int count = Serial.list().length;
    
    if (count != index)
        myPort = new Serial(this, Serial.list()[index], 115200);
    else
        myPort = new Serial(this, "/dev/ttyTHS1", 115200); //for Ubuntu
    
    println("Serial Ports Open !!!");
    myTextarea.setText("Serial Ports Open !!!");
    isSerialOpen = true;
  }
  
  checkOpen = true;
  
  
}

/////////////////////////////////////////////////////
void customize(DropdownList ddl) {
  // a convenience function to customize a DropdownList
  ddl.setBackgroundColor(color(190));
  ddl.setItemHeight(20);
  ddl.setBarHeight(15);
  ddl.getCaptionLabel().set("Serial Ports");

  int count = Serial.list().length;
  
  for (int i=0;i<count;i++) {
    if(Serial.list()[i] != null)
    {
        ddl.addItem(Serial.list()[i], i);
    }
  }
  
  ddl.addItem("/dev/ttyTHS1",count); // for Jetson nano

  ddl.setColorBackground(color(60));
  ddl.setColorActive(color(255, 128));
}

/////////////////////////////////////////////////////
/*void keyPressed() {
  // some key events to change the properties of DropdownList d1
  if (key=='1') {
    // set the height of a pulldown menu, should always be a multiple of itemHeight
    d1.setHeight(210);
  } 
  else if (key=='2') {
    // set the height of a pulldown menu, should always be a multiple of itemHeight
    d1.setHeight(120);
  }
  else if (key=='3') {
    // set the height of a pulldown menu item, should always be a fraction of the pulldown menu
    d1.setItemHeight(30);
  } 
  else if (key=='4') {
    // set the height of a pulldown menu item, should always be a fraction of the pulldown menu
    d1.setItemHeight(12);
    d1.setBackgroundColor(color(255));
  } 
  else if (key=='5') {
    // add new items to the pulldown menu
    int n = (int)(random(100000));
    d1.addItem("item "+n, n);
  } 
  else if (key=='6') {
    // remove items from the pulldown menu  by name
    d1.removeItem("item "+cnt);
    cnt++;
  }
  else if (key=='7') {
    d1.clear();
  }
}*/

/////////////////////////////////////////////////////
void controlEvent(ControlEvent theEvent) {
  // DropdownList is of type ControlGroup.
  // A controlEvent will be triggered from inside the ControlGroup class.
  // therefore you need to check the originator of the Event with
  // if (theEvent.isGroup())
  // to avoid an error message thrown by controlP5.

  if (theEvent.isGroup()) {
    // check if the Event was triggered from a ControlGroup
    println("event from group : "+theEvent.getGroup().getValue()+" from "+theEvent.getGroup());
  } 
  else if (theEvent.isController()) {
    println("event from controller : "+theEvent.getController().getValue()+" from "+theEvent.getController());
  }
}

/////////////////////////////////////////////////////
void draw(){
  background(0);

  if(millis()>now+5000){
    ws.sendMessage("{\"rsp\":\"ai\",\"p\":\"0\",\"v\":\"500\"}"+'\n');
    now=millis();
  }
}

/////////////////////////////////////////////////////
void webSocketServerEvent(String msg){
 println(msg);
 myTextarea.setText(msg);
 
 if(isSerialOpen == true)
 {
     myPort.write(msg);
 }
 
 /*JSONObject json = parseJSONObject(msg);
 
 if (json == null) {
    myTextarea.setText("JSONObject could not be parsed");
  } else {
    String species = json.getString("msg");
    myTextarea.setText(species);
    
    output = createWriter("makeredu_test.ino");
    output.println(species); // Write the coordinate to the file
    output.flush(); // Writes the remaining data to the file
    output.close(); // Finishes the file
  }*/
 
 //myPort.write(msg);
 //myPort.write('\n');

}

/////////////////////////////////////////////////////
void serialEvent(Serial myPort) {
  int inByte = myPort.read();
  serialInArray[serialCount] = (char)inByte;
  serialCount++;
  
  if((char)inByte == '}')
  {
    serialCount = 0;
    println(serialInArray);
    
  }
}
