 /* 
This program is written by Saurabh Kumar.
This is an Arduino UNO R3 program
*/

#include <SoftwareSerial.h>
#include <LiquidCrystal.h>


LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
SoftwareSerial mySerial(9, 10);

int sensor=A5;
int speaker=8;
int gas_value,Gas_alert_val, Gas_shut_val;
int Gas_Leak_Status;
int sms_count=0;

void setup()
{
  pinMode(sensor,INPUT);
  pinMode(speaker,OUTPUT);
  mySerial.begin(9600);
  Serial.begin(9600);
  lcd.begin(16,2);
  delay(500);
}

void loop()
{
  CheckGas();
  CheckShutDown();
  gas_value= analogRead(A5); // read analog input pin A5
  Serial.println("Flammable Gas in ppm = ");
  Serial.println(gas_value, DEC); // prints the value read delay(3000);
}

void CheckGas()
{
  lcd.setCursor(0,0);
  lcd.print("Gas Scan - ON");
  lcd.setCursor(0,1);
  lcd.print(".........................");
  Gas_alert_val=ScanGasLevel();

  if(Gas_alert_val>150)
  {
    Serial.println("Flammable Gas in ppm = ");
    Serial.println(gas_value, DEC); // prints the value read 
    lcd.setCursor(0,1);
    lcd.print("Leakage Detected..!!");
    SetAlert();
  }
}

int ScanGasLevel()
{
  gas_value=analogRead(sensor);
  return gas_value;
}

void SetAlert()
{
  digitalWrite(speaker,HIGH);
  while(sms_count<3)
  {
    SendTextMessage();
  }

  Gas_Leak_Status=1;
  lcd.setCursor(0,1);
  lcd.print("Gas AlerSMS Sent!");
}

void CheckShutDown()
{
  if(Gas_Leak_Status==1)
  {
    Gas_shut_val=ScanGasLevel();
    if(Gas_shut_val<150)
    {
      lcd.setCursor(0,1);
      lcd.print("Gas Leak");
      digitalWrite(speaker,LOW);
      sms_count=0;
      Gas_Leak_Status=0;
    }
  }
}

void SendTextMessage()
{
  mySerial.println("AT+CMGF=1");
  delay(1000);
  mySerial.println("AT+CMGS=\"+91XXXXXXXXXX\"\r");

  /*
       if you are from India, Just replace XXXX with your mobile number
       if you are from other country, Replace +91 with your country code and 
       followed by XXXX with your mobile number
  */

  delay(1000);
  mySerial.println("Gas Leaking!");
  delay(200);
  mySerial.println((char)26);
  delay(1000);

  mySerial.println("AT+CMGS=\"+91XXXXXXXXXX\"\r");

  /*
      if you are from India, Just replace XXXX with your mobile number
      if you are from other country, Replace +91 with your country code and 
      followed by XXXX with your mobile number
  */

  delay(1000);
  mySerial.println("Gas Leaking!");
  delay(200);
  mySerial.println((char)26);
  delay(1000);
  sms_count++;
 }
 
 