/* Sample program to read the Beeline GPS AX.25 serial date form the Kenwood TH-D7
 *  
 *  
 *  
*/

//------------------------------------------------------------------------------

#include <SoftwareSerial.h>

SoftwareSerial SerialGPS(10, 11); //RX, TX

const int kenwoodSentenceSize = 60;
char kenwoodSentence[kenwoodSentenceSize];
String kenwoodList;

//------------------------------------------------------------------------------

void setupKenwood() {
 Serial.begin(9600);
 while(!Serial){;} // Wait for port to connect
 SerialGPS.begin(9600);

}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------

void readKenwoodGPS(){

  short i = 0;
  int timeout = 1000;        // the maximum time to wait for a new character on the serial interface (micro sec) 
  char ch = ' ';
  long delta_t = 0;
  
  do{          
    long tv = micros();
    while((SerialGPS.available()==0) && ((micros() - tv) < timeout));   //wait here until the next character is available to read (needed) or timeout occures 
    delta_t = micros() - tv;                                          // timeout is required because the Venus GGA sentence does not always complete when locked
    if((micros() - tv) > timeout){                                    // break out of while loop if the timeout has occured
      //SerialUSB.println("Break Occured");               
      break;                                           
    }
    else{
      ch = SerialGPS.read();
      kenwoodSentence[i] = ch;
      i++;
    }
  } while (ch != '\n' && (i < kenwoodSentenceSize));  
  
  i = 0;
  makeKenwoodString();
 Serial.println(kenwoodList);
}
//------------------------------------------------------------------------------
void makeKenwoodString(){
  kenwoodList = "";
  for(int i = 0; i < kenwoodSentenceSize; i++){             //changes sentence[char] into string 
    kenwoodList += kenwoodSentence[i];
    }
   
  }

float getKenwoodLatitude()
{
  makeKenwoodString();
 
  int start = kenwoodList.indexOf("!")+1;
  int endString = kenwoodList.lastIndexOf("N");           //retrieves substring from kenwoodList, starting with "!" and ending with "N" counting backwards as call sign contains "N"
  String lat = kenwoodList.substring(start, endString);
  return lat.toFloat();
  
  }

float getKenwoodLongitude()
{
  makeKenwoodString();
  int start = kenwoodList.lastIndexOf("N")+2;
  int endString = kenwoodList.lastIndexOf("W");
  String longi = kenwoodList.substring(start, endString); // retrieves substring from kenwoodList, starting with "N" counting backwards, and "W"
  return longi.toFloat();            //fix
  }

float getKenwoodAltitude(){
  makeKenwoodString();
  int start = kenwoodList.lastIndexOf("=")+1;
  int endString = kenwoodList.lastIndexOf("F");         // retrieves substring from kenwoodList, starting with "=" and ending with "F"
  String alt = kenwoodList.substring(start, endString);
  return alt.toFloat();  

}
