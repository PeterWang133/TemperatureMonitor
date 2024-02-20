/* 
SparkFun Homework 4 Part 2
H.Wang
Temperature Monitor
Displays the temperature reading, and switches between unit of C and F when pressing the pushbutton
*/

#include <LiquidCrystal.h>
LiquidCrystal lcd(12,11,5,4,3,2);
const int temperaturePin = A0;// Sets the analog pin for the temperature component
const int pushbuttonPin = 13; // Sets the pin for the pushbutton

int buttonState = 0; // Sets the initial buttonstate to 0
int mode = 0; // The mode indicates the unit that the lcd display is dislaying. 1 - DegreeC, 0 - DegreeF
int previousMode = 0; // Record the mode that the user has selected previously
int currentTime = 0; // The time since the program started running
int previousTime = 0; // The time since last time user interacted with the pushbutton

int readTime = 0; // Reads the time since the program started running
int previousreadTime = 0; // Records the last time that the program updates the temperature reading
int update = 0; // The program will start updating temperature reading once update value becomes 1

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  lcd.begin(16,2);
  pinMode(pushbuttonPin,INPUT_PULLUP); 
  lcd.setCursor(0,0); // Displays the welcome message
  lcd.print("Temperature"); 
  lcd.setCursor(0,1);
  lcd.print("Monitor");
  delay(2000); // keep the text on LCD 2 second
  lcd.clear(); // Clears the text on the display
  delay(500); // Wait for 5 milliseconds
  lcd.setCursor(0,0); // Tells reader how to use the program
  lcd.print("Press the button");
  lcd.setCursor(0,1);
  lcd.print("to switch C or F.");
  delay(3000); // keep the text on LCD for 3 second
  lcd.clear();// Clears LCD 
}

void loop() {
  buttonState = digitalRead(pushbuttonPin); // buttonState reads whether the pushbutton has been pressed or not
  float voltage, degreeC, degreeF; 
  voltage = getVoltage(temperaturePin); // Gets the voltage value
  degreeC = (voltage - 0.5) * 100.0; // Converts voltage value into temperature value with the unit of C
  degreeF = degreeC * (9.0/5.0) + 32.0; // Cnnverts the voltage value into temperature value with the unit of F
  currentTime=millis(); // Stats timing
  readTime = millis(); // Starts timing for updating temperature reading
  // IF the pushbutton has been pressed and the previous mode is DegreeF and 0.5 second has passed since the user pressed button last time
  if (buttonState == HIGH && previousMode == 0 && currentTime - previousTime > 500){
    // Displays the temperature reading in the unit of C
    lcd.setCursor(0,0);
    lcd.print("Degree C: ");
    lcd.print(degreeC);
    // Sets mode to 1, which is degreeC mode
    mode = 1;
    // Updates the time the user has pressed pushbutton
    previousTime = currentTime;
    // Starts updating temperature reading after every 5 seconds
    update = 1;
  }
  // ELSE IF the push button has been pressed, the previous mode is degreec, and 0.5 second has passed since the user pressed button last time
  else if (buttonState == HIGH && previousMode == 1&& currentTime - previousTime > 500) {
    // Displays the temperature reading in the unit of F
    lcd.setCursor(0,0);
    lcd.print("Degree F: ");
    lcd.print(degreeF);
    // Sets mode to 0, which is degreeF mode
    mode = 0;
    // Updates the time the user has pressed pushbutton
    previousTime = currentTime;
    // Starts updating temperature reading after every 5 second
    update = 1;
  }
  // Records the mode that the display has shown to the user
  previousMode = mode;
  // IF update value becomes one, and five seconds have been passed since the last time read temperature
  if(readTime-previousreadTime>=5000&&update == 1){
    previousreadTime=readTime;
    getVoltage(temperaturePin);
    // Updates degreeC value if the user is looking at degreeC value on the display
    if(mode==1){
      lcd.setCursor(0,0);
      lcd.print("Degree C: ");
      lcd.print(degreeC);
    }
    // Updates degreeF value if the user is looking at degreeF value on the display
    else if (mode==0){
      lcd.setCursor(0,0);
      lcd.print("Degree F: ");
      lcd.print(degreeF);
    }
  }
}


float getVoltage(int pin){ // The function that converts analog reading into voltage value
return (analogRead(pin)*0.004882814);

}