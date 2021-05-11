#include <Arduino.h>
#include <LinearRegression.h>
#include <SPI.h>
#include <SD.h>
#include <LiquidCrystal.h>


char incomingByte [9]; // storing the data coming from the serial
long output[600]; // data
long int_no=0;
String st = "";
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


//********************************************************

void getting_ascii_value_from_the_program()
{
  if (Serial.available() > 0) {
    for (int i = 0; i < 9; i++) {
      incomingByte[i] = Serial.read(); // 900 data coming -  9 0 0 data reading .
      if ( incomingByte[i] == -1)
        incomingByte[i] = ' ';
    }
  }
}

//***************************

void storing_data_in_the_string()
{
  st = "";
  for (int i = 0; i < 9; i++) //incomingByte[i] =   0 1 2 
    st += incomingByte[i]; // incoming data -       9 0 0 - ouput "900" 

}

//***************************

long changing_the_string_into_int()
{
  long m1 = st.toInt(); // "900" - 900
  return m1;
}

//***************************
void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
lcd.begin(16, 2);
}

void loop() {

getting_ascii_value_from_the_program();
storing_data_in_the_string();
int_no=changing_the_string_into_int();
// put ur storing the data into the array in this code .
{
        lcd.setCursor(0,0);
        lcd.print(int_no);
        delay(1000);
      }
}
