
#include <string.h>
#include <Servo.h>    // 声明调用Servo.h库


//ros::NodeHandle  nh;/

Servo myservo;        // 创建一个舵机对象
int pos = 0;          // 变量pos用来存储舵机位置

int sensorPin = A0;    // select the input pin for the potentiometer
int outPin = 8;      // select the pin for the LED
int sensorValue = 0;  // variable to store the value coming from the sensor

float thresholdVoltageValue = 0.8;

float ee_angle = 0;
int ee_angle_int = 0;
String inString = "0";
String stringDwonToUp="DTU";
String stringUpToDwon="UTD";
String outString = stringDwonToUp+inString+stringDwonToUp;
void setup()
{
  pinMode(outPin, OUTPUT);
  analogReference(INTERNAL); //调用板载1.1V基准源
  myservo.attach(2);
  Serial.begin(9600);
}

void loop()
{
  sensorValue = analogRead(sensorPin);
  float voltageValue = (1.1 * sensorValue) / 1024;//电压
  if (voltageValue > thresholdVoltageValue)
  {
    //吸上了
    digitalWrite(outPin, HIGH);
    //    Serial.print("吸上了\n");
  } else
  {
    //没有吸上
    digitalWrite(outPin, LOW);
    //    Serial.print("没有吸上\n");
  }
  while (Serial.available() > 0) {
    int inChar = Serial.read();
    if (isDigit(inChar)) {
      // convert the incoming byte to a char
      // and add it to the string:
      inString += (char)inChar;
    } else {
      inString = "";
    }     
    if (inChar == '\n') { 
      sscanf(inString.c_str(), "UTD%dUTD", &ee_angle_int);           
      ee_angle = ee_angle_int/ 10;
      outString="";
      outString+=stringDwonToUp;
      char temp_str[20];
      outString+=itoa(ee_angle_int,temp_str,10);
      outString+=stringDwonToUp;
      inString = "";
    }
  }
      Serial.println(outString);
  delay(100);
}
