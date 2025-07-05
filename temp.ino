#include <Keypad.h>
#include <DHT.h>

#define DHTPIN 3  
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

const byte ROWS = 4; 
const byte COLS = 4; 

char keys[ROWS][COLS] = { 
  {'D', 'C', 'B', 'A'},
  {'#', '9', '6', '3'},
  {'0', '8', '5', '2'},
  {'*', '7', '4', '1'}
};

byte rowPins[ROWS] = {4, 5, 6, 7};  
byte colPins[COLS] = {8, 9, 10, 11}; 

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

int targetTemp = 0; 
int enteredTemp = 0; 
bool heating = false; 
bool cooling = false; 

void setup() {
  Serial.begin(9600);
  dht.begin();
}

void loop() {
  char key = keypad.getKey();
  int currentTemp = dht.readTemperature();

  if (key) {
    if (key >= '0' && key <= '9') {
      enteredTemp = enteredTemp * 10 + (key - '0');
      Serial.print("Внесена температура: ");
      Serial.println(enteredTemp);
    }

    if (key == '*') {
      targetTemp = enteredTemp;
      Serial.print("Целна температура: ");
      Serial.println(targetTemp);

      Serial.print("Тековна температура: ");
      Serial.println(currentTemp);


      if (currentTemp < targetTemp) {
        Serial.println("Вклучи греење!");
        heating = true;
        cooling = false;
      } else if (currentTemp > targetTemp) {
        Serial.println("Вклучи ладење!");
        cooling = true;
        heating = false;
      } else {
        Serial.println("Температурата е на зададената вредност.");
        heating = false;
        cooling = false;
      }

      enteredTemp = 0;
    }
  }

  if (heating && currentTemp >= targetTemp) {
    Serial.println("Исклучи греење!");
    heating = false;
    Serial.println(currentTemp);
  }

  if (cooling && currentTemp <= targetTemp) {
    Serial.println("Исклучи ладење!");
    cooling = false;
    Serial.println(currentTemp);
  }
}
