#include "Arduino.h"
#include "RTClib.h"
#include "Wire.h"

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

class ObecnyCzas
{
private:
	RTC_PCF8523 rtc;
	String addZeroToSmallValue(uint8_t wartosc)
	{
		String liczba = "";
		if (wartosc < 10)
		{
			liczba += "0";
		}
		liczba += (String)wartosc;
		return liczba;
	}

public:
	String DataGodzina(void)
	{
		DateTime teraz = rtc.now();
		String zformatowanyCzas = "";

		zformatowanyCzas += teraz.year();
		zformatowanyCzas += ".";
		zformatowanyCzas += addZeroToSmallValue(teraz.month());
		zformatowanyCzas += ".";
		zformatowanyCzas += addZeroToSmallValue(teraz.day());
		zformatowanyCzas += ";";
		zformatowanyCzas += daysOfTheWeek[teraz.dayOfTheWeek()];
		zformatowanyCzas += ";";
		zformatowanyCzas += addZeroToSmallValue(teraz.hour());
		zformatowanyCzas += ":";
		zformatowanyCzas += addZeroToSmallValue(teraz.minute());
		zformatowanyCzas += ":";
		zformatowanyCzas += (addZeroToSmallValue(teraz.second()));
		return zformatowanyCzas;
	}
	ObecnyCzas()
	{

		Serial.println("KOnstruktor...");
	}
	boolean begin()
	{
		return rtc.begin();
	}
	boolean initialized()
	{
		return rtc.initialized();
	}
};

ObecnyCzas obecnyCzas;

void setup () {

  while (!Serial) {
    delay(1);  // for Leonardo/Micro/Zero
  }
  Serial.begin(9600);
  if(! obecnyCzas.begin())
  {
	  Serial.println("Nie znaleziono zegara RTC");
  }
  if(!obecnyCzas.initialized())
  {
	  Serial.println("Zegar nie pracuje");
  }
  //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  Serial.println("setup...");
}

void loop () {
	Serial.println(obecnyCzas.DataGodzina());
    Serial.println();
    delay(3000);
}


