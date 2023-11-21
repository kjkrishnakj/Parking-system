#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C>

LiquidCrystal_I2C lcd(0x27, 20, 4);
Servo myservo;

#define ir_enter 2
#define ir_back 4
#define ir_car1 5
#define ir_car2 6
#define ir_car3 7
#define ir_car4 8

int S1 = 0, S2 = 0, S3 = 0, S4 = 0;
int flag1 = 0, flag2 = 0;
int slot = 4;

void setup() {
  Serial.begin(9600);

  pinMode(ir_car1, INPUT);
  pinMode(ir_car2, INPUT);
  pinMode(ir_car3, INPUT);
  pinMode(ir_car4, INPUT);

  pinMode(ir_enter, INPUT);
  pinMode(ir_back, INPUT);

  myservo.attach(3);
  myservo.write(90);

  lcd.init();
  lcd.backlight();
  lcd.begin(20, 4);
  lcd.setCursor(0, 1);
  lcd.print(" Car parking ");
  lcd.setCursor(0, 2);
  lcd.print(" System ");
  delay(2000);
  lcd.clear();
  Read_Sensor();
  int total = S1 + S2 + S3 + S4;
  slot = slot - total;
}

void Read_Sensor() {
  S1 = 0, S2 = 0, S3 = 0, S4 = 0;
  if (digitalRead(ir_car1) == LOW) { S1 = 1; }
  if (digitalRead(ir_car2) == LOW) { S2 = 1; }
  if (digitalRead(ir_car3) == LOW) { S3 = 1; }
  if (digitalRead(ir_car4) == LOW) { S4 = 1; }
}

void loop() {
  Read_Sensor();
  lcd.setCursor(0, 0);
  lcd.print("Slots: ");
  lcd.print(slot);
  lcd.setCursor(0, 1);
  lcd.print("S1:");
  lcd.print(S1 == 1 ? "Fill " : "Empty");
  lcd.setCursor(10, 1);
  lcd.print("S2:");
  lcd.print(S2 == 1 ? "Fill " : "Empty");
  lcd.setCursor(0, 2);
  lcd.print("S3:");
  lcd.print(S3 == 1 ? "Fill " : "Empty ");
  lcd.setCursor(10, 2);
  lcd.print("S4:");
  lcd.print(S4 == 1 ? "Fill " : "Empty ");

  if (digitalRead(ir_enter) == 0 && flag1 == 0) {
    if (slot > 0) {
      flag1 = 1;
      if (flag2 == 0) {
        myservo.write(180);
        slot = slot - 1;
      }
    } else {
      lcd.setCursor(0, 0);
      lcd.print("Sorry, Parking Full");
      delay(1500);
      lcd.clear();
    }
  }

  if (digitalRead(ir_back) == LOW && flag2 == 0) {
    flag2 = 1;
    if (flag1 == 0) {
      myservo.write(180);
      slot = slot + 1;
    }
  }

  if (flag1 == 1 && flag2 == 1) {
    delay(1000);
    myservo.write(90);
    flag1 = 0, flag2 = 0;
  }

  delay(1);
}
