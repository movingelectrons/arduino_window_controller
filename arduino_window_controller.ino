#include <DHT.h>
//these are connected to the wires that control the relays for opening and closing the windows
#define openPIN  2
#define closePIN 3
//temp/humidity sensor (dht11 based, digital pin input..)
#define DHTPIN 12
#define DHTTYPE DHT11
// Initialize DHT sensor.
DHT dht(DHTPIN, DHTTYPE);

float h,t,f;
String windowStatus = String("unknown");

void setup() {
  Serial.begin(9600);
  dht.begin();
  pinMode(openPIN, OUTPUT);
  pinMode(closePIN, OUTPUT);
  powerOffWindow();
}

void loop() {
  //get current tempreature/humidity from dht11 sensor, takes about 250ms
  //sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  //h = dht.readHumidity();
  t = dht.readTemperature();    // Read temperature as Celsius
  f = dht.readTemperature(true);// Read temperature as Fahrenheit
  if (isnan(h) || isnan(t) || isnan(f)) {// Check if any reads failed and exit early
    Serial.println("Failed to read from DHT sensor!");
    return;
  }else{
    Serial.print("DHT sensor says its ");
    Serial.println(f);
  }
  if(f >= 75 && windowStatus != "open"){
    openWindow();
  }
  if(f <= 70 && windowStatus != "closed"){
    closeWindow();  
  }
  Serial.print("Current window status is ");
  Serial.println(windowStatus);
  delay(60000); //one minute
}

void openWindow(){
  Serial.println("Opening window");
  digitalWrite(openPIN, LOW); //oppsite values, because we are using low-level triggered relays
  digitalWrite(closePIN, HIGH);
  delay(60000);
  delay(60000);
  powerOffWindow();
  windowStatus = "open";
  Serial.println("Finished opening window");
}

void closeWindow(){
  Serial.println("Closing window");
  digitalWrite(openPIN, HIGH);
  digitalWrite(closePIN, LOW);
  delay(60000);
  delay(60000);
  powerOffWindow();
  windowStatus = "closed";
  Serial.println("Finished closing window");
}

void powerOffWindow(){
  digitalWrite(openPIN, HIGH);
  digitalWrite(closePIN, HIGH);
}
