    #include <SD.h>
    #include <SPI.h>
    #include <Arduino.h>
    
    
    char incomingByte [9]; // storing the data coming from the serial
    long output[600]; // data
    long int_no=0;
    String st = "" , data_sec = "";
    int p=0,s=0;

    File navi_file;
    int CS_pin = 53; 
  
    
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
//*******************************
void storing_the_data_into_CSV_file()
{
  navi_file = SD.open("navida2.csv", FILE_WRITE);  // if the file opened okay, write to it:
  if (navi_file) {
    navi_file.println(data_sec); 
    // close the file:
    navi_file.close();
  }
  else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }
}

//*****************************

    void setup() {
    Serial.begin(9600);
  Serial.print("Initializing SD card...");
  pinMode(SC, OUTPUT);
  //
  // see if the card is present and can be initialized:
  if (!SD.begin(SC)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }
  Serial.println("card initialized.");

     }
   
void loop() {
  // put your main code here, to run repeatedly:
s++;
getting_ascii_value_from_the_program();

storing_data_in_the_string();  

int_no=changing_the_string_into_int();

data_sec = String(int_no)+ ","+String(s)   ;

storing_the_data_into_CSV_file();

output[p]=int_no;
p++;
if(p=600)
p=0;
}
