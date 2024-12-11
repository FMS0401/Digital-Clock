#include <Wire.h>

// Date and time functions using a DS1307 RTC connected via I2C and Wire lib
#include "RTClib.h"

#include <LiquidCrystal_I2C.h>
#include <Wire.h>

#include "DHT.h"

LiquidCrystal_I2C lcd(0x27,16,2) ; //SCL A5 SDA A4
const int DHTPIN = A2;
const int DHTTYPE = DHT11;

const int nut1=3; //Chỉnh
const int nut2=2; //Tăng
const int nut3=4; //Giảm
 //Reset
DHT dht(DHTPIN, DHTTYPE);

RTC_DS1307 rtc;

char daysOfTheWeek[7][12] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
int editField = 0; //0: OFF, 1: Giờ, 2: Phút, 3: Ngày, 4: Tháng, 5:Năm
char fields[6][7] = {"    ","Hour", "Min ", "Day ", "Mon ", "Year"};

unsigned long lastDebounceTime1 = 0;
unsigned long lastDebounceTime2 = 0;
unsigned long lastDebounceTime3 = 0;
unsigned long debounceDelay = 500; // Thời gian chống rung phím (debounce)

void setup () {
    lcd.init();
    lcd.backlight();
    Wire.begin();
   Serial.begin(9600);
   dht.begin();
   Serial.begin(57600);
   
#ifndef ESP8266
  while (!Serial); // wait for serial port to connect. Needed for native USB
#endif
  
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1) delay(10);
  }

  if (! rtc.isrunning()) {
    Serial.println("RTC is NOT running, let's set the time!");
    // When time needs to be set on a new device, or after a power loss, the
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }

  // When time needs to be re-set on a previously configured device, the
  // following line sets the RTC to the date & time this sketch was compiled
  // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  // This line sets the RTC with an explicit date & time, for example to set
  // January 21, 2014 at 3am you would call:
  // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  pinMode(nut1,INPUT_PULLUP);
  pinMode(nut2,INPUT_PULLUP);
  pinMode(nut3,INPUT_PULLUP);
  
}

void loop () 
{
    int docnut1 = digitalRead(nut1);
    int docnut2 = digitalRead(nut2);
    int docnut3 = digitalRead(nut3);
    
    DateTime now = rtc.now();
 
    int t=dht.readTemperature();
    char hourminute[16];
    sprintf(hourminute, "%02d:%02d", now.hour(), now.minute());
    char daymonth[16];
    sprintf(daymonth, "%02d/%02d", now.day(), now.month());

    lcd.setCursor(0,0);
    lcd.print(daysOfTheWeek[now.dayOfTheWeek()]);
    lcd.setCursor(5,0);
    lcd.print(hourminute);
    lcd.setCursor(12,0);
    lcd.print(fields[editField]);

    lcd.setCursor(0,1);
    lcd.print(daymonth);
    lcd.print("/");
    lcd.print(now.year(),DEC);

    lcd.setCursor(11,1);
    lcd.print("T:");
    lcd.setCursor(13,1);
    lcd.print(t);
    lcd.print("C");
 

if (docnut1 == 0 && (millis() - lastDebounceTime1) > debounceDelay) {
    editField = (editField + 1) % 6; 
    lastDebounceTime1 = millis();
  }




 if (docnut2 == 0 && (millis() - lastDebounceTime2) > debounceDelay) { 
    switch (editField) 
    {
      case 1: // Giờ
        if (now.hour() < 23) 
        {
          rtc.adjust(DateTime(now.year(), now.month(), now.day(), now.hour() + 1, now.minute(), now.second()));
        } else 
        {
          rtc.adjust(DateTime(now.year(), now.month(), now.day(), 0, now.minute(), now.second()));
        }
        break;
      case 2: // Phút
        if (now.minute() < 59) 
        {
          rtc.adjust(DateTime(now.year(), now.month(), now.day(), now.hour(), now.minute() + 1, now.second()));
        } else 
        {
          rtc.adjust(DateTime(now.year(), now.month(), now.day(), now.hour() , 0, now.second()));
        }
        break;
      case 3: // Ngày
      switch (now.month()) // xét tháng 30 ngày và 31 ngày và tháng 2
      {
        case 2:
        if(now.year()%4==0 && now.year()%100!=0)
        {
          if (now.day() < 29 ) {
            rtc.adjust(DateTime(now.year(), now.month(), now.day() + 1, now.hour(), now.minute(), now.second()));
          } else {
            rtc.adjust(DateTime(now.year(), now.month(), 1, now.hour(), now.minute(), now.second()));
          }
        }  else 
        {
          if (now.day() < 28 ) 
          {
          rtc.adjust(DateTime(now.year(), now.month(), now.day() + 1, now.hour(), now.minute(), now.second()));
          } else 
          {
          rtc.adjust(DateTime(now.year(), now.month(), 1, now.hour(), now.minute(), now.second()));
          }
        } break;
        case 4:
        if (now.day() < 30) 
        {
          rtc.adjust(DateTime(now.year(), now.month(), now.day() + 1, now.hour(), now.minute(), now.second()));
        } else 
        {
          rtc.adjust(DateTime(now.year(), now.month(), 1, now.hour(), now.minute(), now.second()));
        }
        break;
        case 6:
        if (now.day() < 30) 
        {
          rtc.adjust(DateTime(now.year(), now.month(), now.day() + 1, now.hour(), now.minute(), now.second()));
        } else 
        {
          rtc.adjust(DateTime(now.year(), now.month(), 1, now.hour(), now.minute(), now.second()));
        }
        break;
         case 9:
        if (now.day() < 30) 
        {
          rtc.adjust(DateTime(now.year(), now.month(), now.day() + 1, now.hour(), now.minute(), now.second()));
        } else 
        {
          rtc.adjust(DateTime(now.year(), now.month(), 1, now.hour(), now.minute(), now.second()));
        }
        break;
         case 11:
        if (now.day() < 30) 
        {
          rtc.adjust(DateTime(now.year(), now.month(), now.day() + 1, now.hour(), now.minute(), now.second()));
        } else 
        {
          rtc.adjust(DateTime(now.year(), now.month(), 1, now.hour(), now.minute(), now.second()));
          }
        break;
        default:
        if (now.day() < 31) 
        {rtc.adjust(DateTime(now.year(), now.month(), now.day() + 1, now.hour(), now.minute(), now.second()));
        } else 
        {
          rtc.adjust(DateTime(now.year(), now.month(), 1, now.hour(), now.minute(), now.second()));
        }
      } break;
      case 4: // Tháng
        if (now.month() < 12) {
          rtc.adjust(DateTime(now.year(), now.month() + 1, now.day(), now.hour(), now.minute(), now.second()));
        } else {
          rtc.adjust(DateTime(now.year(), 1, now.day(), now.hour(), now.minute(), now.second()));
        } break;
      case 5: // Năm
        rtc.adjust(DateTime(now.year() + 1, now.month(), now.day(), now.hour(), now.minute(), now.second()));
        break;
    }
    lastDebounceTime2 = millis();
  }
if (docnut3 == 0 && (millis() - lastDebounceTime3) > debounceDelay) {
    switch (editField) 
    {
      case 1: // Giờ
          
       if(now.hour()>0 && now.hour()<24)
        {
          rtc.adjust(DateTime(now.year(), now.month(), now.day(), now.hour() - 1, now.minute(), now.second()));
        } else if(now.hour()==0)
        {
          rtc.adjust(DateTime(now.year(), now.month(), now.day(), 23, now.minute(), now.second()));
        } 
        break;  
        
      case 2: // Phút
        if(now.minute()>0 && now.minute()<60)
        {
         rtc.adjust(DateTime(now.year(), now.month(), now.day(), now.hour(), now.minute() - 1, now.second()));
        
        } else if(now.minute()==0)
        {
          rtc.adjust(DateTime(now.year(), now.month(), now.day(), now.hour() , 59, now.second()));
        }
         break; 
        
      case 3: // Ngày
       if(now.day()>1 && now.day()<=31 )
        {
          rtc.adjust(DateTime(now.year(), now.month(), now.day() -1 , now.hour(), now.minute(), now.second()));
        } else if(now.day()==1)
        { switch (now.month())
          {
          case 2 : rtc.adjust(DateTime(now.year(), now.month(), 30, now.hour() , now.minute(), now.second()));
          break;
          case 4 : rtc.adjust(DateTime(now.year(), now.month(), 30, now.hour() , now.minute(), now.second()));
          break;
          case 6 : rtc.adjust(DateTime(now.year(), now.month(), 30, now.hour() , now.minute(), now.second()));
          break;
          case 9 : rtc.adjust(DateTime(now.year(), now.month(), 30, now.hour() , now.minute(), now.second()));
          break;
          case 11 : rtc.adjust(DateTime(now.year(), now.month(), 30, now.hour() , now.minute(), now.second()));
          break;
          default:
          rtc.adjust(DateTime(now.year(), now.month(), 31, now.hour() , now.minute(), now.second()));
          }
        } break;
        
      case 4: // Tháng
       if (now.month() > 1 && now.month() <= 12) 
       {
          rtc.adjust(DateTime(now.year(), now.month() - 1, now.day(), now.hour(), now.minute(), now.second()));
        } else  if(now.month() == 1)
        {rtc.adjust(DateTime(now.year(), 12, now.day(), now.hour(), now.minute(), now.second())); 
        } break;
        
      case 5: // Năm
        rtc.adjust(DateTime(now.year() - 1, now.month(), now.day(), now.hour(), now.minute(), now.second()));
        break;
    }
    lastDebounceTime3 = millis();
  }
  
   
  

    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(" (");
    Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
    Serial.print(") ");
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();
}