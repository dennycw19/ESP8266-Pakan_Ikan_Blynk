#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <Servo.h>

#include <LiquidCrystal_I2C.h>
#include <DallasTemperature.h>
#include <OneWire.h>

#include <BlynkSimpleEsp8266.h>

#include <RTClib.h>
#include <Wire.h>

#define BLYNK_TEMPLATE_ID " " //insert blynk template id
#define BLYNK_TEMPLATE_NAME " " //insert blynk template name
#define BLYNK_AUTH_TOKEN " " // insert blynk auth token
#define BLYNK_PRINT Serial

LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x3F for a 16 chars and 2 line display

Servo myservo ;

RTC_DS3231 rtc;

#define ONE_WIRE_BUS 2 // DS18B20 on arduino pin2 (GPIO2) corresponds to D4 on physical board "D4 pin on the ndoemcu Module"
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature DS18B20(&oneWire);
float celsius;

int StatusPakan;
const char* ssid = " " ; //insert wifi ssid
const char* pass = " " ; //insert wifi password

byte suhu[8] =
{
  B00100,
  B01010,
  B01010,
  B01110,
  B11111,
  B11111,
  B01110,
  B00000
};

void spinner() {
  static int8_t counter = 0;
  const char* glyphs = "\xa1\xa5\xdb";
  lcd.setCursor(15, 1);
  lcd.print(glyphs[counter++]);
  if (counter == strlen(glyphs)) {
    counter = 0;
  }
}

void BeriPakan(){
  for(int posisi=0; posisi<=180; posisi+=5){
    myservo.write(posisi);
    delay(10);
  }

  for(int posisi=180; posisi>=0; posisi-=5){
    myservo.write(posisi);
    delay(10);
  }
}

void PrintSuhu(){
  DS18B20.requestTemperatures(); 
  celsius = DS18B20.getTempCByIndex(0); // Celcius
  // Fahrenheit = DS18B20.toFahrenheit(temp); // Fahrenheit
  Serial.println(celsius);

  lcd.setCursor(0,0);
  lcd.write(1);
  lcd.setCursor(2,0);
  lcd.print("Suhu:");
  lcd.setCursor(8,0);
  lcd.print(celsius);
  lcd.setCursor(14,0);
  lcd.print((char)223);
  lcd.setCursor(15,0);
  lcd.print("C");
}

void PrintRTC(){
  DateTime now = rtc.now();       //Menampilkan RTC pada variable now
  
  lcd.setCursor(0,0);   //Set cursor to character col 0, row 0
  lcd.print(now.day());
  lcd.setCursor(2,0);   //Set cursor to character col 2, row 0
  lcd.print("/");
  lcd.setCursor(3,0);   //Set cursor to character col 3, row 0
  lcd.print(now.month());
  lcd.setCursor(5,0);   //Set cursor to character col 5, row 0
  lcd.print("/");
  lcd.setCursor(6,0);   //Set cursor to character col 6, row 0
  lcd.print(now.year());

  lcd.setCursor(0,1);   //Set cursor to character col 0, row 1
  lcd.print(now.hour());
  lcd.setCursor(2,1);   //Set cursor to character col 2, row 1
  lcd.print(":");
  lcd.setCursor(3,1);   //Set cursor to character col 3, row 1
  lcd.print(now.minute());
  lcd.setCursor(5,1);   //Set cursor to character col 5, row 1
  lcd.print(":");
  lcd.setCursor(6,1);   //Set cursor to character col 6, row 1
  lcd.print(now.second());

  delay(1000);
}

void setup() {
  myservo.attach(D5,500,2400);
  myservo.write(0);

  Serial.begin(9600);

  Wire.begin(4, 5);   //Setting wire (4 untuk SDA dan 5 untuk SCL)
  rtc.begin();
  rtc.adjust(DateTime(F(__DATE__),F(__TIME__)));  //Setting Time

  //TES LCD
  lcd.init(); //inisiasi LCD
  lcd.backlight();  //menyalakan backlight
  lcd.createChar(1, suhu);
  lcd.setCursor(3,0); //Set cursor to character col 3, row 0
  lcd.print("DN_FISHERY"); //menulis kata
  delay(2000);//menunggu 2000 milidetik
  lcd.clear();//menghapus tampilan LCD

  // Connect to WiFi
  lcd.setCursor(0, 0);
  lcd.print("Connecting to ");
  lcd.setCursor(0, 1);
  lcd.print("WiFi ");

  WiFi.begin(ssid, pass);
  while(WiFi.status() != WL_CONNECTED){
    // Serial.println(".");
    spinner();
    delay(500);
  }
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.println("WiFi Connected");
  delay(2000);
  lcd.clear();
  Serial.println("Wifi Terkoneksi");

  //Connect to Blynk
  lcd.setCursor(0, 0);
  lcd.print("Connecting to ");
  lcd.setCursor(0, 1);
  lcd.print("Blynk ");

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.println("Blynk Connected");
  delay(2000);
  lcd.clear();
  Serial.println("Blynk Terkoneksi");

  PrintSuhu();
  delay(5000);
  lcd.clear();
  PrintRTC();
  delay(5000);
  lcd.clear();
  lcd.noDisplay();
  lcd.noBacklight();
  
}



void loop() {
  Blynk.run();
  
  Serial.println("Status Pakan :  "+ String(StatusPakan));
  if(StatusPakan  == 1){
    lcd.display();
    lcd.backlight();
    PrintSuhu();
    delay(5000);
    lcd.clear();
    PrintRTC();
    delay(5000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.println("MAKAN CUUYY!!");
    Serial.println("MAKAN CUY..!!!");
    // delay(2000);
    BeriPakan();
    lcd.clear();
    lcd.noDisplay();
    lcd.noBacklight();
    
    Blynk.virtualWrite(V0 ,0);
    StatusPakan = 0;
    delay(2000);
  }
}
BLYNK_WRITE(V0){
  StatusPakan = param.asInt();
}