//  Marian Bodnar 
//  Semestrálna úloha 1  Architektura PC
//........................................................................................................................................

#include <Wire.h>                      //Kniznice 
#include <LiquidCrystal_I2C.h> 
#include <DHT.h>

#define DHTPIN 4        // pripojene na pin
#define DHTTYPE DHT22   // DHT 22  (AM2302)
#define Led1 7          // vystup LED alebo SSR rele 
#define Led2 9
#define Led3 10
#define Led4 12

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 20, 4);     // urcenie display 20x4 znakov , adresa i2c 0x27



float hum;  //desatinne cislo vlhkost
int temp; // cele cislo teplota , moze byt aj desatinne float

const int hore = 6;
const int dole = 11;

int reklama = 0;
int hodnota = 22;
int nastav;

byte celzia[] = {            // Vytvorenie znaku
  B10000,
  B00110,
  B01001,
  B10000,
  B10000,
  B10000,
  B01001,
  B00110
};


void setup() {
  
  lcd.init();                  // display start
  lcd.backlight();             // zapnutie podsvietenia
  lcd.clear();                 // vymazanie display
  dht.begin();                 // senzor DHT start
  lcd.createChar(0 , celzia);  // znak
  pinMode(Led1, OUTPUT);
  pinMode(Led2, OUTPUT);
  pinMode(Led3, OUTPUT);
  pinMode(Led4, OUTPUT);
  pinMode(hore, INPUT_PULLUP); // pripojenie interneho rezistora PULLUP
  pinMode(dole, INPUT_PULLUP); // pripojenie interneho rezistora PULLUP

  
  for (int reklama = 0; reklama < 2; reklama++)    // verzia termostatu
  {
    lcd.setCursor(2, 1);
    lcd.print (" TERMOSTAT 1.0") ;
    delay(1500);
    lcd.clear ();
  }
  
  lcd.clear();                 //vymazat display po reklame 
}


void loop() {
  delay(100);                      // nizka hodnota pre dobru odozvu gombika senzor cita iba delay2000 aj napriek hodnote delay100
    
    hum = dht.readHumidity();
    temp= dht.readTemperature();
    nastav = hodnota ;
    
    
    lcd.setCursor(3, 0);           // Display rozlozenie
    lcd.print("TERMOSTAT SIMI"); 
    lcd.setCursor(0, 1); 
    lcd.print("IN TEMP:");
    lcd.setCursor(12, 1); 
    lcd.write(0);                  // pouzitie znaku
    lcd.setCursor(9, 1); 
    lcd.print(temp);               // premenna teplota
    lcd.setCursor(0, 2); 
    lcd.print("IN HUM: ");
    lcd.setCursor(9, 2); 
    lcd.print(hum);                // premenna vlhkost
    lcd.setCursor(14, 2); 
    lcd.print("%");
    lcd.setCursor(0, 3); 
    lcd.print("SET TEMP:");  
    lcd.setCursor(9, 3); 
    lcd.print(nastav);             // premenna Nastav teplotu
    

  
  if (digitalRead(6) == LOW)   
  {
    hodnota = hodnota + 1;    // nastavenie teploty +
  }

  if (digitalRead(11) == LOW)
  {
    hodnota = hodnota -1;     // nastavenie teploty -
  }

   if ( isnan(temp) || isnan(hum) )        // Jednoducha kontrola cidla 
  {
    lcd.setCursor(13,3);
    lcd.print("Error  ");
    digitalWrite(Led1, LOW);
    digitalWrite(Led2, LOW);
    digitalWrite(Led3, LOW);
    digitalWrite(Led4, LOW);
  }
  
  else if (temp  > nastav)                 // podmienky pre Chladenie
  {
    digitalWrite(Led1, LOW);
    digitalWrite(Led2, HIGH);
    digitalWrite(Led3, HIGH);
    digitalWrite(Led4, LOW);
    lcd.setCursor(13,3);
    lcd.print("Cooling");
  } 
  else if (temp  < nastav)                 // podmienky pre Kurenie
  {
    digitalWrite(Led1, HIGH);
    digitalWrite(Led2, LOW);
    digitalWrite(Led3, LOW);
    digitalWrite(Led4, HIGH);
    lcd.setCursor(13,3);
    lcd.print("Heating ");
  }
  else if (temp == nastav)                 // podmienky pre cakanie
  {
    digitalWrite(Led1, HIGH);
    digitalWrite(Led2, LOW);
    digitalWrite(Led3, HIGH);
    digitalWrite(Led4, LOW);
    lcd.setCursor(13,3);
    lcd.print("Standby ");
  }
  
}

//........................................................................................................................................................
