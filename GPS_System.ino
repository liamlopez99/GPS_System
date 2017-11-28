
#include <Math.h>

void setup() {
 Serial.begin(9600);
  while(!Serial){;}
 
 setupVenusGPS();
 setupKenwood();
   // put your setup code here, to run once:

}

void loop() {
   
   readSentence();
   readKenwoodGPS();
   float testX = getDistX(getKenwoodLatitude(),getVenusLatitude(), getKenwoodLongitude(), getVenusLongitude());
   float testY =(getDistY(getKenwoodLatitude(),getVenusLatitude()));
   float testZ =(getDistZ(getKenwoodAltitude(),getVenusAltitude()));
   Serial.print("Theta = \t");
   Serial.println(getTheta(testX,testY,testZ));
   Serial.print("Gamma = \t");
   Serial.println(getGamma(testX,testY));
   Serial.println();
  /* Serial.println("Venus: \t");
   Serial.print("\t Latitude: ");
   Serial.println(getVenusLatitude());
  
   
   Serial.print("\t Longitude: ");
   Serial.println(getVenusLongitude());
  
   Serial.print("\t Altitude: ");
   Serial.println(getVenusAltitude());

   Serial.println(); 
   Serial.println();
   
   Serial.println("Kenwood: \t");
   Serial.print("\t Latitude: ");
   Serial.println(getKenwoodLatitude());
  
   
   Serial.print("\t Longitude: ");
   Serial.println(getKenwoodLongitude());
  
   Serial.print("\t Altitude: ");
   Serial.println(getKenwoodAltitude());

   Serial.println(); 
   Serial.println(); */
   delay(5000); 
   // put your main code here, to run repeatedly:

}

float getLatft(float lat){
  lat *= .01;
  lat = 365222.0 *lat;
  return lat;
  }

float getLongft(float lat, float longi)
{
 lat *=.01;
 longi *=.01;
 longi = 365217.6*longi*cos(lat*((2*3.14159)/360));
 return longi;
}

float getDistY(float kenLat, float venusLat){
  kenLat = getLatft(kenLat);
  venusLat = getLatft(venusLat);
  return kenLat-venusLat;
  }

float getDistX(float kenLat, float venusLat, float kenLong, float venusLong){
  kenLong = getLongft(kenLat, kenLong);
  venusLong = getLongft(venusLat, venusLong);
  return kenLong - venusLong;
  }
float getDistZ(float kenAlt, float venusAlt){
  return kenAlt - venusAlt;
  }
float getTheta(float X, float Y, float Z){
  if( Z < 0){
    return (-1 *asin ( (sqrt(pow(X,2) + pow(Y,2)) / sqrt(pow(X,2) + pow(Y,2) + pow(Z,2)))) + 3.14159) *180/3.14159;
    }
  else
    return (asin((sqrt(pow(X,2) + pow(Y,2)) / sqrt(pow(X,2) + pow(Y,2) + pow(Z,2))))) * 180/3.14159;
  }
float getGamma(float X, float Y){
  return (asin(Y / (sqrt(pow(X,2) + pow(Y,2))))) * 180/3.14159;
  }
