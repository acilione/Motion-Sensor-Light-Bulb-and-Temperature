#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 8, 7, 6, 5, 4);
int pinRelay = 2;
int pinPIR = 3;
int n;
int min = 1023;
int max = 0;
int light;
int turnedOnTime;
int red_light_pin = 11;
int blue_light_pin = 10;
int green_light_pin = 9;

bool moving = false;

void movingStatusChanged()
{
  	moving = !moving;
}

void RGB_color(int red_light_value, int green_light_value, int blue_light_value)
{
  	analogWrite(red_light_pin, red_light_value);
  	analogWrite(green_light_pin, green_light_value);
  	analogWrite(blue_light_pin, blue_light_value);
}

void setup()
{
  	Serial.begin(9600);
	lcd.begin(16, 2);
  	pinMode(pinRelay, OUTPUT);
	pinMode(pinPIR, INPUT);
  	pinMode(A0, INPUT);
  	pinMode(A1, INPUT);
	pinMode(red_light_pin, OUTPUT);
  	pinMode(green_light_pin, OUTPUT);
  	pinMode(blue_light_pin, OUTPUT);
  	attachInterrupt(digitalPinToInterrupt(pinPIR), movingStatusChanged, CHANGE);
	while (millis() < 5000)
  	{
  		light = analogRead(A1);
    	if (light > max)
      		max = light;
    	if (light < min)
      		min = light;
  	}
  	Serial.println("Calibration ended");
  	Serial.print("Min: ");
  	Serial.println(min);
  	Serial.print("Max: ");
  	Serial.println(max);
 	delay(3000);
}

void loop()
{
	n = analogRead(A0);
	int light = analogRead(A1);
	float volt = (n / 1023.0) * 5;
	float temp = (volt - 0.5) * 100; 
	float ambientLightLevel = map(light, min, max, 0 ,100);
	Serial.println(ambientLightLevel);
	if (moving)
	{
		if (ambientLightLevel < 60)
			digitalWrite(pinRelay, HIGH);
		lcd.begin(16, 2);
		lcd.setCursor(0, 0);
		lcd.print("Temperatura:");
		lcd.setCursor(0,1);
		lcd.print(temp);
		if (temp < 0)
			RGB_color(0, 0, 255);
		else if (temp > 0 && temp < 30)
			RGB_color(255, 255, 0);
		else
		  	RGB_color(255, 0, 0);
		delay(5000);
	}
	else
	{
	 	digitalWrite(pinRelay, LOW);
	   	lcd.noDisplay();
	   	digitalWrite(red_light_pin, LOW);
	   	digitalWrite(blue_light_pin, LOW);
	   	digitalWrite(green_light_pin, LOW);
	}
	int val = digitalRead(3);
	Serial.println(val);
}