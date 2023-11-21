#include <Servo.h> // Include the Servo library for controlling the servo motor 
#include <Wire.h> // Include the Wire library for I2C communication 
#include <LiquidCrystal_I2C.h> // Include the LiquidCrystal_I2C library for controlling the LCD
LiquidCrystal_I2C lcd(0x27, 20, 4); // Create an LCD object with the I2C address 0x27 and 20 columns and 4 rows 
Servo myservo; // Create a servo object 
#define ir_enter 2 // Define the pin number connected to the IR sensor for detecting entering cars 
#define ir_back 4 // Define the pin number connected to the IR sensor for detecting exiting cars
#define ir_car1 5 // Define the pin number connected to the IR sensor for detecting car 1 
#define ir_car2 6 // Define the pin number connected to the IR sensor for detecting car 2 
#define ir_car3 7 // Define the pin number connected to the IR sensor for detecting car 3 
#define ir_car4 8 // Define the pin number connected to the IR sensor for detecting car 4 
int S1 = 0, S2 = 0, S3 = 0, S4 = 0; // Declare variables to store the sensor readings for each car
 int flag1 = 0, flag2 = 0; // Declare flags to control the servo motor movement 
int slot = 4; // Declare a variable to store the number of available parking slots 
void setup() 
{ 
	Serial.begin(9600);
	pinMode(ir_car1, INPUT); // Set the pin mode for car 1 sensor to input 
	pinMode(ir_car2, INPUT); // Set the pin mode for car 2 sensor to input 
	pinMode(ir_car3, INPUT); // Set the pin mode for car 3 sensor to input 
	pinMode(ir_car4, INPUT); // Set the pin mode for car 4 sensor to input 
	
	pinMode(ir_enter, INPUT); // Set the pin mode for enter sensor to input 
	pinMode(ir_back, INPUT); // Set the pin mode for back sensor to input 
	
	myservo.attach(3); // Attach the servo motor to pin 3 
	myservo.write(90); // Initialize the servo motor to the neutral position (90 degrees) 
	
	lcd.init(); // Initialize the LCD 
	lcd.backlight(); // Turn on the LCD backlight 
	lcd.begin(20, 4); // Set the LCD display to 20 columns and 4 rows 
	lcd.setCursor(0, 1); // Set the cursor position to the first line, second column 
	lcd.print(" Car parking "); // Print the "Car Parking System" header 
	lcd.setCursor(0, 2); // Set the cursor position to the second line, second column 
	lcd.print(" System "); // Print the "System" text 
	delay(2000); // Delay for 2 seconds to allow the text to be displayed 
	lcd.clear(); // Clear the LCD display 
	Read_Sensor(); // Call the Read_Sensor function to read the initial sensor values 
	int total = S1 + S2 + S3 + S4; // Calculate the total number of cars detected 
	slot = slot - total; // Update the number of available parking slots based on the detected cars 
}
void Read_Sensor() 
{ // Function to read the sensor values 
	S1 = 0, S2 = 0, S3 = 0, S4 = 0; // Reset the sensor readings 
	if (digitalRead(ir_car1) == LOW) { S1 = 1; } // Check if car 1 sensor is triggered and set the corresponding variable 
	if (digitalRead(ir_car2) == LOW) { S2 = 1; } // Check if car 2 sensor is triggered and set the corresponding variable 
	if (digitalRead(ir_car3) == LOW) { S3 = 1; } // Check if car 3 sensor is triggered and set the corresponding variable 
	if (digitalRead(ir_car4) == LOW) { S4 = 1; } // Check if car 4 sensor is triggered and set the corresponding variable 
}
void loop() 
{
	Read_Sensor(); // Call the Read_Sensor function to update the sensor readings
	lcd.setCursor(0, 0); // Set the cursor position to the first line, first column 
	lcd.print("Slots: "); // Print the "Slots:" label 
	lcd.print(slot); // Print the number of available parking slots 
	lcd.setCursor(0, 1); // Set the cursor position to the second line, first column 
	lcd.print("S1:"); // Print the "S1:" label 
	lcd.print(S1 == 1 ? "Fill " : "Empty"); // Print the "Fill" or "Empty" message based on car 1 sensor reading 
	lcd.setCursor(10, 1); // Set the cursor position to the second line, tenth column 
	lcd.print("S2:"); // Print the "S2:" label 
	lcd.print(S2 == 1 ? "Fill " : "Empty"); // Print the "Fill" or "Empty" message based on car 2 sensor reading 
	lcd.setCursor(0, 2); // Set the cursor position to the third line, first column 
	lcd.print("S3:"); // Print the "S3:" label 
	lcd.print(S3 == 1 ? "Fill " : "Empty "); // Print the "Fill" or "Empty" message based on car 3 sensor reading 
	lcd.setCursor(10, 2); // Set the cursor position to the third line, tenth column 
	lcd.print("S4:"); // Print the "S4:" label 
	lcd.print(S4 == 1 ? "Fill " : "Empty "); // Print the "Fill" or "Empty" message based on car 4 sensor reading
if (digitalRead(ir_enter) == 0 && flag1 == 0) 
	{ // Check if the enter sensor is triggered and the flag1 is not set 
		if (slot > 0) 
		{ // Check if there are available parking slots 
			flag1 = 1; // Set the flag1 to indicate an incoming car 
			if (flag2 == 0) 
			{ // Check if the flag2 is not set 
				myservo.write(180); // Rotate the servo motor to open the barrier 
				slot = slot - 1; // Update the number of available parking slots
			 }
		 }
		else
		 { // If there are no available parking slots 
			lcd.setCursor(0, 0); // Set the cursor position to the first line, first column 
			lcd.print("Sorry, Parking Full");// Print the "Sorry, Parking Full" message 
			delay(1500); // Delay for 1.5 seconds to allow the message to be displayed 
			lcd.clear(); // Clear the LCD display 
		}
	 }if (digitalRead(ir_back) == LOW && flag2 == 0)
	 { // Check if the back sensor is triggered and the flag2 is not set 
		flag2 = 1; // Set the flag2 to indicate an exiting car 
		if (flag1 == 0)
		 { // Check if the flag1 is not set 
			myservo.write(180); // Rotate the servo motor to open the barrier 
			slot = slot + 1; // Update the number of available parking slots 
		} 
	} 
	if (flag1 == 1 && flag2 == 1)
	 { // Check if both flags are set, indicating a car has entered or exited 
		delay(1000); // Delay for 1 second 
		myservo.write(90); // Rotate the servo motor back to the neutral position 
		flag1 = 0, flag2 = 0; // Reset the flags 
	} 
	delay(1); // Delay for 1 millisecond to reduce loop execution time
 } 