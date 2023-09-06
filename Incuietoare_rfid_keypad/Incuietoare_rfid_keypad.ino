#include <Keypad.h>
#include <Servo.h>
#include <SPI.h>
#include <MFRC522.h>

Servo servo;

#define SS_PIN 10
#define RST_PIN 9

MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class

MFRC522::MIFARE_Key key; 

const byte ROWS = 4; 
const byte COLS = 3; 

const String pass = "2008";
String pass_try_string = "";

char Keys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};

byte rowPins[ROWS] = {2, 3, 4, 5}; 
byte colPins[COLS] = {6, 7, 8}; 

Keypad keypad = Keypad(makeKeymap(Keys), rowPins, colPins, ROWS, COLS); 

void setup(){
  Serial.begin(9600);
  SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522 

  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }
  
  servo.attach(A0);  
  servo.write(0);
}
  
void loop(){
  char Key = keypad.getKey();
  
  if (Key == '*'){
    pass_try_string = "";
    servo.write(0);
    for (int foo = 0; foo < 4; ++foo) {
      char Key1 = keypad.getKey();
      while (!Key1) {
        Key1 = keypad.getKey();
      }
      if (Key1 != '#' && Key1 != '*') {
        pass_try_string = pass_try_string + Key1;
        Serial.println(pass_try_string);
      }
    }
    if (pass_try_string == pass) {
    Serial.println("CORECT");
        servo.write(90);
        delay(2000);
        servo.write(0);
    }
    else {
      Serial.println("GRESIT"); 
        servo.write(0);
    }
  }
  if (rfid.PICC_IsNewCardPresent()) {
    if (rfid.PICC_ReadCardSerial()) {
      if (rfid.uid.uidByte[0] != 161 || rfid.uid.uidByte[1] != 20 || rfid.uid.uidByte[2] != 152 || rfid.uid.uidByte[3] != 36) {
        Serial.println("GRESIT");
        servo.write(0);
      }
      if (rfid.uid.uidByte[0] == 161 || rfid.uid.uidByte[1] == 20 || rfid.uid.uidByte[2] == 152 || rfid.uid.uidByte[3] == 36) {
        Serial.println("CORECT");
        servo.write(90);
        delay(2000);
        servo.write(0);
      }
    }
  }
}
