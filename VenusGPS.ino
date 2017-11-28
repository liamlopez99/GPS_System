/* Venus GPS setup & reading 
   Returns Latitude, Longitude, & Altitude
   by: Tom Fetter
   date: 8/29/16

   For the Rocket logger, The Venus GPS is set up using a separtate setup sketch "VenusGPS_Rocket_Logger_Setup".
   The setup stores the programmed attributes in the VenusGPS Flash
   This is done to avoid the change in the BAUD rate at setup necessary for the higher sample rate 
   The BAUD rate must already be set to the higher rate if the chip is to start up automatically on re-powerup
   
   The setup is presumed to be:
   1. Request just the "GGA" NMEA message be sent
   2. Set the Serial Port BAUD rate  to 15200
   3. Set the position update rate (NMEA message readout rate) to 10Hz

   The NMEA message must still be turned on at setup
   
*/

//------------------------------------------------------------------------------

const int venusSentenceSize = 80;
char venusSentence[venusSentenceSize];

//------------------------------------------------------------------------------

void setupVenusGPS(){

  Serial1.begin(9600);
  while(!Serial1);

  delay(100);      // hangs here without a delay - 10 ms not long ernough

  //Turn on NMEA output messages
  Serial1.write(0xA0);        // start message
  Serial1.write(0xA1);        
  Serial1.write((byte)0x00);  // Payload length = 2 bytes
  Serial1.write(0x02);        // Payload length = 2 bytes
  Serial1.write(0x09);        // Message Number 09
  Serial1.write((byte)0x01);  // NMEA output
  Serial1.write(0x08);        // checksum = 1000 = 0x08
  Serial1.write(0x0D);        // end of message
  Serial1.write(0x0A);        
}

//------------------------------------------------------------------------------

String readGPS(){

  String GPS = "";
  readSentence();
  GPS += getString(2);    // Latitude
  GPS += "\t";
 
  GPS += getString(4);    // Longitude
  GPS += "\t";

  GPS += getString(9);    // Altitude
  GPS += "\t";
 
  GPS += getString(7);    // # Sats
  return GPS;
  
}
//------------------------------------------------------------------------------

String getString(int index)
{
  String str = "";
  int i = 0;
  int j = 0;
    
  while (i < venusSentenceSize)
  {
    if (venusSentence[i] == ',')
    {
      j ++;
      i ++;
    }
    if (j == index)
    {
      str += venusSentence[i];
    }
    i ++;
  }
  //Serial.println(str);
  return str;
}
//------------------------------------------------------------------------------

void readSentence()
{
 
 
  short i = 0;
  int timeout = 1000;        // the maximum time to wait for a new character on the serial interface (micro sec) 
  char ch = ' ';
  long delta_t = 0;
  
  do{          
    long tv = micros();
    while((Serial1.available()==0) && ((micros() - tv) < timeout));   //wait here until the next character is available to read (needed) or timeout occures 
    delta_t = micros() - tv;                                          // timeout is required because the Venus GGA sentence does not always complete when locked
    if((micros() - tv) > timeout){                                    // break out of while loop if the timeout has occured
      //SerialUSB.println("Break Occured");               
      break;                                           
    }
    else{
      ch = Serial1.read();
      venusSentence[i] = ch;
      i++;
    }
  } while (ch != '\n' && (i < venusSentenceSize));  
  
  i = 0;
Serial.println(venusSentence);
 }
//------------------------------------------------------------------------------ 
 float getVenusLatitude()
{
 return getString(2).toFloat();
}

float getVenusLongitude()
{
 return getString(4).toFloat();
}

float getVenusAltitude()
{
 return getString(9).toFloat()*3.28084;
}
