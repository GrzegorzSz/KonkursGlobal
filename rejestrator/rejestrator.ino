#include "Arduino.h"
#include "RTClib.h"
#include "Wire.h"
#include "SD.h"
#include "UTFT.h"
#include <RFID.h>

#define RFID_SS_PIN 6
#define RFID_RST_PIN 5
#define SD_CS_PIN 53

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
UTFT screen(ILI9481,38,39,40,41);


class Person
{
public:
	const String& getCardId() const {
		return cardID;
	}

	void setCardId(const String& cardId = "") {
		cardID = cardId;
	}

	const String& getName() const {
		return name;
	}

	void setName(const String& name = "") {
		this->name = name;
	}
	Person()
	{
		name = "";
		cardID = "";
	}

private:
	String name;
	String cardID;
};
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
		String sformatowany = "";

		sformatowany += timePoint.year();
		sformatowany += ".";
		sformatowany += addLeadZero(timePoint.month());
		sformatowany += ".";
		sformatowany += addLeadZero(timePoint.day());
		sformatowany += ";";
		sformatowany += daysOfTheWeek[timePoint.dayOfTheWeek()];
		sformatowany += ";";
		sformatowany += addLeadZero(timePoint.hour());
		sformatowany += ":";
		sformatowany += addLeadZero(timePoint.minute());
		sformatowany += ":";
		sformatowany += (addLeadZero(timePoint.second()));
		return sformatowany;
	}
	String getMonthString()
	{
		timePoint = rtc.now();
		return (addLeadZero(timePoint.month()) + "." + timePoint.year());
	}
	ClockManager()
	{
		begined = rtc.begin();
		initial = rtc.initialized();
		delay(100);
		timePoint = rtc.now();
		//rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
		Serial.println("Obiekt zegara utworzony");
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
class SDCardManager : public SDClass
{
private:
	boolean initial;
	File root, employes, monthList;
	String monthOfYear;
	ClockManager *clockMan;
public:

	void writeToMonthList (String cardID)
	{
		String monthOfYr = clockMan->getMonthString();
		String timePoint = clockMan->getTimeString();
		Serial.print("cardID: ");
		Serial.println(cardID);
		Serial.print("miesiac roku: ");
		Serial.println(monthOfYr);
		Serial.print("timepoint: ");
		monthOfYr.replace('.', '_');
		Serial.println(timePoint);
		monthList = SD.open(monthOfYr, FILE_WRITE);
		while(monthList.available())
		{
			monthList.readString();
		}
		monthList.print(timePoint);
		monthList.print(";");
		monthList.println(cardID);
		monthList.close();
	}
	boolean foundEmployeInFile(Person &person)
	{
		File file;
		String tmp = "";
		boolean entryFound = false;
		if(SD.exists("empl.txt"))
		{
			file = SD.open("empl.txt");
		}
		else
		{
			entryFound = false;
			Serial.println("Brak listy pracowników");
			return entryFound;
		}
		while(file.available())
		{
			tmp = file.readStringUntil('\n');
			if(tmp.indexOf(person.getCardId()) == -1)
			{
				continue;
			}
			else
			{
				tmp.replace(person.getCardId(), "");
				tmp.replace(";", "");
				person.setName(tmp);
				entryFound = true;
				break;
			}
		}
		file.close();
		return entryFound;
	}
	SDCardManager()
	{
		initial = SD.begin(SD_CS_PIN);
		root = SD.open("/", FILE_WRITE);
		clockMan = new ClockManager;
		if(! clockMan->begin())
		{
			Serial.println("Nie znaleziono zegara RTC");
		}
		if(!clockMan->initialized())
		{
			Serial.println("Zegar nie pracuje");
		}

		monthOfYear = "";
		Serial.println("Obiekt SDCardManager utworzony");
	}
	boolean initialized()
	{
		return initial;
	}

};

extern uint8_t BigFont[];

class RfidReader
{
private:
	RFID *reader;
	int prevSerialNumber[5];
	Person person;


	SDCardManager *cardMan;

	boolean IDsAreEqual()
	{
		for (int indeks = 0; indeks<5; indeks++)
		{
			if(prevSerialNumber[indeks] != reader->serNum[indeks])
			{
				return false;
			}
		}
		return true;
	}
	String serialNumberToString()
	{
		String serial = "";
		for(int indeks = 0; indeks < sizeof(reader->serNum)-1; indeks++)
		{
			serial += reader->serNum[indeks];
			if(indeks != sizeof(reader->serNum)-2)
				serial += ",";
		}
		Serial.print("serial to string: ");
		Serial.println(serial);
		return serial;
	}
	void drawStartupScreen()
	{
		screen.clrScr();
		screen.setBackColor(0,0,0);
		screen.setColor(255, 255, 255);
		screen.print("Podaj karte", CENTER, 150);
	}
	void drawTakeCard()
	{
		screen.clrScr();
		screen.setBackColor(59,188,46);		//green color
		screen.fillScr(59, 188,46);
		screen.setColor(255, 255, 255);
		screen.print("OK.", CENTER, 90);
		delay(1000);
	}
	void drawRecognizedCard(Person person)
	{
		int y1 = 80;
		int y2 = 110;
		screen.clrScr();
		screen.setBackColor(0,0,0);
		screen.setColor(255, 255, 255);
		screen.print("Smacznego", CENTER, y1);
		screen.print(person.getName(), CENTER, y2);
		delay(3000);
		drawStartupScreen();
	}
public:
	RfidReader()
	{
		reader = new RFID(RFID_SS_PIN, RFID_RST_PIN);
		reader->init();



		  cardMan = new SDCardManager;
		  if(! cardMan->initialized())
		  {
			  Serial.println("Czytnik kart SD nie pracuje");
		  }

		Serial.println("Obiekt Rfid reader utworzony");
		delay(500);
		screen.InitLCD();
		screen.clrScr();
		screen.setFont(BigFont);
		drawStartupScreen();
	}
	void rfRun()
	{
		if(reader->isCard())
		{
			if(reader->readCardSerial())
			{
				drawTakeCard();
				if(IDsAreEqual())
				{
					Serial.println("Już zarejestrowany");
					delay(2000);
					drawStartupScreen();
					return;
				}
				for (int indeks = 0; indeks < 5; indeks++)
				{
					prevSerialNumber[indeks] = reader->serNum[indeks];
				}
				person.setCardId(serialNumberToString());
				if(! cardMan->foundEmployeInFile(person))
				{
					person.setName("Nieznajomy");
				}
				cardMan->writeToMonthList(person.getCardId());
				drawRecognizedCard(person);
			}
		}
	}
};

RfidReader *rfidReader;

void setup () {
	pinMode(RFID_RST_PIN, OUTPUT);
	pinMode(RFID_SS_PIN, OUTPUT);
	//pinMode(SD_CS_PIN, OUTPUT);

  while (!Serial) {
    delay(1);  // for Leonardo/Micro/Zero
  }
  Serial.begin(9600);
  SPI.begin();
  Serial.println("Start setup...");


  rfidReader = new RfidReader();
  Serial.println("setup done.");
}

void loop () {
	rfidReader->rfRun();
}

