#include <Arduino.h>
#include <LinearRegression.h>
#include <SPI.h>
#include <SD.h>
//***************************
// 9 0 0 - 9 0 0 
char incomingByte [9]; // storing the data coming from the serial
long output[600]; // data
int sec_output[600]; // sec

String st = "";
String data_sec = "";

long SF = 0 , slope = 0, int_no = 0;

int  k = 0,p=0;
long s = 0000; // Second 
long x = 0, y = 0 ; //y - read  (data file ) , x -read (sec) file 
int inital_stage = 0 , final_stage = 1, final_stage_value=600;

int m=0;

File navi_file;
int SC = 53;

//***************************

void geting_ascii_value_from_the_program()
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

bool readLine(File &f, char* line, size_t maxLen) {
  for (size_t n = 0; n < maxLen; n++) {
    int c = f.read();
    if ( c < 0 && n == 0)
    {
      return false;  // EOF
    }                         // 0 1  2  3
    if (c < 0 || c == '\n') { // 0 , 900 \n
      line[n] = 0; // 5
      return true;
    }
    line[n] = c;
  }
  return false; // line too long
}

bool readVals(File &f, long* v1, long* v2) {
  char line[600], *ptr, *str; //line [0] = 3  , 900000 \0 ;// 0 900
  if (!readLine(navi_file, line, sizeof(line))) {
    return false;  // EOF or too long
  }
  *v1 = strtol(line, &ptr, 10);  // 0 , 9000
  if (ptr == line) return false;  // bad number if equal
  while (*ptr) {
    if (*ptr++ == ',') break;
  }
  *v2 = strtol(ptr, &str, 10);
  while (*ptr) {
    if (*ptr++ == ',') break;
  }

  return str != ptr;  // true if number found
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

//******************


void simpLinReg(float* x, float* y, float* lrCoef, int n){
  // pass x and y arrays (pointers), lrCoef pointer, and n.  The lrCoef array is comprised of the slope=lrCoef[0] and intercept=lrCoef[1].  n is length of the x and y arrays.

  // initialize variables
  float xbar=0;
  float ybar=0;
  float xybar=0;
  float xsqbar=0;
  
  // calculations required for linear regression
  for (int i=0; i<n; i++){
    xbar=xbar+x[i];
    ybar=ybar+y[i];
    xybar=xybar+x[i]*y[i];
    xsqbar=xsqbar+x[i]*x[i];
  }
  xbar=xbar/n;
  ybar=ybar/n;
  xybar=xybar/n;
  xsqbar=xsqbar/n;
  
  // simple linear regression algorithm
  lrCoef[0]=(xybar-xbar*ybar)/(xsqbar-xbar*xbar);
  lrCoef[1]=ybar-lrCoef[0]*xbar;

  slope= lrCoef[0];
}
//******************

void finding_the_SF()
{
  //sf =0
  if (slope > 0)
    SF = SF - slope;
  else
    SF += slope;
  //  Serial.print(" SF  ");
  Serial.println(SF);
  //   Serial.println();

}

//***********************

void setup() {
  // put your setup code here, to run once:
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
    
    geting_ascii_value_from_the_program(); // 1
    storing_data_in_the_string(); 
    int_no = changing_the_string_into_int();
    data_sec = String(int_no)+ ","+String(s)   ;
    storing_the_data_into_CSV_file();
    output[p]=int_no;
    p++;

     if(p==600)
     {
      Serial.println(output[599]);
      p=0;
      
      navi_file = SD.open("navida2.csv", FILE_WRITE);  // if the file opened okay, write to it:
      readVals( navi_file,&x, &y);
      output[p] = x; //data
      sec_output[p] = y; //sec  // 9 =600 , next stagwe =2 +1 , initaiil
      Serial.print("sec ");
      Serial.println(sec_output[p]);
      Serial.print(" data ");
      Serial.println(output[p]);
      p++;
      
      if(p==600) // k =0 
       p=0;
      simpLinReg(output, sec_output, lrCoef, 4);
     }
     
  delay(1000);  
      // done write 
 }
    

  
  
