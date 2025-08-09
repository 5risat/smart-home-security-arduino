#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>

// إعداد شاشة LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

// إعداد لوحة المفاتيح
const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {9, 8, 7, 6};
byte colPins[COLS] = {5, 4, 3, 2};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// كلمة السر
String password = "1234";
String inputPassword = "";

// الحساسات والمخرجات
int pirPin = 10;
int buzzer = 11;
int ledRed = 12;
int ledGreen = 13;

bool systemArmed = false;

void setup() {
  lcd.init();
  lcd.backlight();
  pinMode(pirPin, INPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(ledRed, OUTPUT);
  pinMode(ledGreen, OUTPUT);

  lcd.setCursor(0,0);
  lcd.print("Smart Security");
  lcd.setCursor(0,1);
  lcd.print("Enter Passcode");
  Serial.begin(9600);
}

void loop() {
  char key = keypad.getKey();

  if (key) {
    if (key == '#') { // تحقق من كلمة السر
      if (inputPassword == password) {
        systemArmed = !systemArmed;
        lcd.clear();
        if (systemArmed) {
          lcd.print("System Armed!");
          digitalWrite(ledGreen, HIGH);
          digitalWrite(ledRed, LOW);
          Serial.println("ALERT: System Activated");
        } else {
          lcd.print("System Disarmed");
          digitalWrite(ledGreen, LOW);
          digitalWrite(ledRed, HIGH);
          Serial.println("INFO: System Deactivated");
        }
      } else {
        lcd.clear();
        lcd.print("Wrong Password!");
        delay(2000);
        lcd.clear();
        lcd.print("Enter Passcode");
      }
      inputPassword = "";
    }
    else if (key == '*') { // مسح الإدخال
      inputPassword = "";
      lcd.clear();
      lcd.print("Enter Passcode");
    }
    else { // إدخال الأرقام
      inputPassword += key;
      lcd.setCursor(inputPassword.length()-1, 1);
      lcd.print("*");
    }
  }

  // إذا كان النظام مسلح وحدثت حركة
  if (systemArmed) {
    if (digitalRead(pirPin) == HIGH) {
      lcd.clear();
      lcd.print("Intruder Alert!");
      digitalWrite(buzzer, HIGH);
      digitalWrite(ledRed, HIGH);
      digitalWrite(ledGreen, LOW);
      Serial.println("WARNING: Intrusion Detected!");
      delay(2000);
    } else {
      digitalWrite(buzzer, LOW);
    }
  }
}
