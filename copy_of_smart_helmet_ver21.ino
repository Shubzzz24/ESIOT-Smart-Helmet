#include <LiquidCrystal.h>
#include <SoftwareSerial.h>

float t=0;
char data = 0;
String apiKey = "ZY6DICQ136QRGTH1";  // Write API key
// connect 8 to TX of ESP
// connect 9 to RX of ESP
SoftwareSerial ser(8,9); // RX, TX
const int rs = 13, en = 12, d4 = 6, d5 = 5, d6 = 4, d7 = 3;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
float temp = 0;
unsigned long start, finished, elapsed;
int period = 250;
unsigned long time_now = 0;
int const PINO_SGAS = A0;
int belt=A2;
int red=2;
int green=11;
int yellow=1;
int tilt=7;
int buzzer=A4;
int  valor;
void setup() {
ser.begin(9600); 
Serial.begin(9600);
//Serial.println(F("reading temperature begin. \n"));
  pinMode(red, OUTPUT);
  digitalWrite(red, LOW);
  pinMode(green, OUTPUT);
  digitalWrite(green, LOW);
  pinMode(yellow, OUTPUT);
  digitalWrite(yellow, LOW);
  pinMode(buzzer, OUTPUT);
  pinMode(belt, INPUT);
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2); 
  
  ser.println("AT");  // Attenuation

  delay(1000);

  ser.println("AT+GMR"); // To view version info for ESP-01 output: 00160901 and ESP-12 output: 0018000902-AI03

  delay(1000);

  ser.println("AT+CWMODE=3"); // To determine WiFi mode
/*
1 = Station mode (client)
2 = AP mode (host)
3 = AP + Station mode (ESP8266 has a dual mode) 
*/
   

  delay(1000);

  ser.println("AT+RST"); // To restart the module

  delay(5000);

  ser.println("AT+CIPMUX=1"); // Enable multiple connections
  /*

    0: Single connection
    1: Multiple connections (MAX 4)
 
*/  
  delay(1000);

  //String cmd="AT+CWJAP=\"SSID\",\"PASSWORD\""; // connect to Wi-Fi 
  String cmd="AT+CWJAP=\"Thiha Htun\",\"12345678\""; 
  ser.println(cmd);
  delay(1000);
  ser.println("AT+CIFSR"); // Return or get the local IP address
  delay(1000);
}
void loop() {
  digitalWrite(green, LOW);
  digitalWrite(red, LOW);
  lcd.clear();
  if(analogRead(belt)>=500){
 int start=millis()/1000;
 digitalWrite(green,HIGH);
 lcd.print("Helmet Wear Properly"); 
  }
 static unsigned long sensortStamp = 0;
 valor = analogRead(PINO_SGAS);
 t=valor;
    
 if(valor>800){
 gas();
  }  
 if(millis() - sensortStamp > 1000){
 sensortStamp = millis();
 int reading = analogRead(A1);
 Serial.print(F("Real Time Temp: ")); 
 // converting that reading to voltage, for 3.3v arduino use 3.3
 float voltage = reading * 5.0;
 voltage /= 1024.0;  
 // print out the voltage
// Serial.print(voltage); Serial.println(" volts"); 
 // now print out the temperature
 float temperatureC = (voltage - 0.5) * 100 ;  //converting from 10 mv per degree wit 500 mV offset
                                               //to degrees ((voltage - 500mV) times 100)
Serial.print(temperatureC); Serial.print(" degrees C     ");
   lcd.setCursor(0,1);
   //lcd.print("Helmet is now running");
  for (int positionCounter = 0; positionCounter < 13; positionCounter++) {
    // scroll one position left:
  lcd.scrollDisplayLeft();
    // wait a bit:
    delay(300);
  }
   lcd.setCursor(0,1);
   lcd.print(temperatureC);
   lcd.print(" ");
   lcd.print("C");
   lcd.print(" "); 
 // now convert to Fahrenheit
 float temperatureF = (temperatureC * 9.0 / 5.0) + 32.0;
// Serial.print(temperatureF); Serial.println(" degrees F");
   lcd.setCursor(9, 1);
   lcd.print(temperatureF);
   lcd.print(" ");
   lcd.print("F");
   lcd.print(" ");   
   
  if(digitalRead(tilt)==HIGH){
  lcd.clear();
  lcd.print("Fall Detect");
  lcd.setCursor(0,1);
  lcd.print("Wait 5 seconds");
  start=millis();
  delay(5000);
  finished=millis();
  Serial.println("Finished");
  elapsed=finished-start;
  Serial.print(elapsed);
  Serial.println(" milliseconds elapsed");
  Serial.println();
  delay(500);
  lcd.clear();
  tone(buzzer, 440, 250);
  lcd.print("Sending ALERT");
  lcd.setCursor(0,1);
  lcd.print("SMS & Emerg call");
  delay(3000);
  lcd.clear();
   }
  if(temperatureC > 38){
    time_now = millis();
    digitalWrite(red, HIGH); 
    lcd.clear();
  tone(buzzer, 440, 250);
  lcd.print("Alert Abnormal");
  lcd.setCursor(0,1);
  lcd.print("Body Temperature");
 while(millis() < time_now + period){
  //wait approx. [period] ms
    }     

  }
 
  
 }
  int stop=(millis()/1000);
  int escape=stop-start; 
  int percent=map(escape,0,36000,0,100);
 // Serial.print(percent);Serial.print(" %    ");
 // Serial.print(escape);Serial.println("s");
  
  
}


void esp_8266()

{

   // TCP connection AT+CIPSTART=4,"TCP","184.106.153.149",80 

    String cmd = "\nAT+CIPSTART=4,\"TCP\",\"";  // Establish TCP connection
    /*
     AT+CIPSTART=id,type,addr,port
     
    id: 0-4, id of connection
    type: String, “TCP” or “UDP”
    addr: String, remote IP
    port: String, remote port
 
    */
    cmd += "184.106.153.149"; // api.thingspeak.com

    cmd += "\",80";

    ser.println(cmd);

    Serial.println(cmd); 

    if(ser.find("Error"))

    {

      Serial.println("AT+CIPSTART error");

      return;

    }

  String getStr = "GET /update?api_key=";  // API key

  getStr += apiKey;

  //getStr +="&field1=";

  //getStr +=String(h);

  getStr +="&field1=";

  getStr +=String(t);

  getStr += "\r\n\r\n";

  // send data length

  cmd = "AT+CIPSEND="; // Send data AT+CIPSEND=id,length

  cmd += String(getStr.length());

  ser.println(cmd);

  Serial.println(cmd);

  delay(1000);

  ser.print(getStr);

  Serial.println(getStr);
  
  
 

  // thingspeak needs 16 sec delay between updates

  delay(17000);  

}
void gas(){
  if(valor>80){
    tone(buzzer, 440, 250);
     delay(2000);
     digitalWrite(red, HIGH);
     lcd.clear();
     lcd.print("Harmful Gas");
     lcd.setCursor(0,1);
     lcd.print("condition");
     delay(1000);
     lcd.clear();
     Serial.println(valor);
   }
}