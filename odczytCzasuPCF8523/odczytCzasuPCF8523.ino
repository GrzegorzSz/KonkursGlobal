#include "Arduino.h"
#include "RTClib.h"
#include "Wire.h"
#include "SD.h"
#include "UTFT.h"

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

class ClockManager
{
private:
	RTC_PCF8523 rtc;
	DateTime timePoint;
	boolean begined, initial;
	String addLeadZero(uint8_t wartosc)
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
	DateTime getTimeObject()
	{
		return timePoint;
	}
	String getTimeString()
	{
		timePoint = rtc.now();
		String zformatowanyCzas = "";

		zformatowanyCzas += timePoint.year();
		zformatowanyCzas += ".";
		zformatowanyCzas += addLeadZero(timePoint.month());
		zformatowanyCzas += ".";
		zformatowanyCzas += addLeadZero(timePoint.day());
		zformatowanyCzas += ";";
		zformatowanyCzas += daysOfTheWeek[timePoint.dayOfTheWeek()];
		zformatowanyCzas += ";";
		zformatowanyCzas += addLeadZero(timePoint.hour());
		zformatowanyCzas += ":";
		zformatowanyCzas += addLeadZero(timePoint.minute());
		zformatowanyCzas += ":";
		zformatowanyCzas += (addLeadZero(timePoint.second()));
		return zformatowanyCzas;
	}
	ClockManager()
	{
		begined = rtc.begin();
		initial = rtc.initialized();
		timePoint = rtc.now();
	}

	boolean begin()
	{
		return begined;
	}
	boolean initialized()
	{
		return initial;
	}
};
class SDCardManager : SDClass
{
private:
	boolean initial;
	File root;
public:
	void checkForRequestedDirs(String fileNamesTable[])
	{
		for(unsigned int indeks=0; indeks < fileNamesTable->length(); indeks++)
		{
			if(fileNamesTable[indeks].equals("empl.txt"))
			{
				if(! SD.exists(fileNamesTable[indeks]))
				{
					Serial.println("Brak spisu pracowników");
				}
			} else if (! SD.exists(fileNamesTable[indeks]))
			{
				File file = SD.open(fileNamesTable[indeks], FILE_WRITE);
				file.close();
			}
		}
	}
	SDCardManager()
	{
		initial = SD.begin();
		root = SD.open("/");

	}
	boolean initialized()
	{
		return initial;
	}

};

ClockManager *clockMan;
SDCardManager *cardMan;
UTFT screen(ILI9481,38,39,40,41);
extern uint8_t BigFont[];

void setup () {

  while (!Serial) {
    delay(1);  // for Leonardo/Micro/Zero
  }
  Serial.begin(9600);
  clockMan = new ClockManager;
  if(! clockMan->begin())
  {
	  Serial.println("Nie znaleziono zegara RTC");
  }
  if(!clockMan->initialized())
  {
	  Serial.println("Zegar nie pracuje");
  }
  //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

  cardMan = new SDCardManager;
  if(! cardMan->initialized())
  {
	  Serial.println("Czytnik kart SD nie pracuje");
  }

  screen.InitLCD();
  screen.clrScr();
  screen.setColor(0,0,0);
  screen.setFont(BigFont);
  screen.print("ekran OK", LEFT, 20);
  Serial.println("setup done.");
  delay(1000);
}

void loop () {
	Serial.println(clockMan->getTimeString());
    Serial.println();
    delay(3000);
}


