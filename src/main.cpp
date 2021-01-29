/* Door+ software made by 3S Team 
// Author: Dominik Nuszkiewicz
//
*/
//Dla nazw funkcji uzywamy podlogi _, dla zmiennych zaczynamy od duzych liter

#include <Arduino.h>

//blibloteki do komunikacji sieciowej
#include <WiFi.h>
#include <PubSubClient.h>



//do oledu
#include <Wire.h>
// Biblioteka niskopoziomowych funkcji wyświetlacza SSD1306.
#include <Adafruit_SSD1306.h>
// Biblioteka wysokopoziomowych funkcji dla różnych wyświetlaczy.
#include <Adafruit_GFX.h>


const int WIDTH = 128; // Szerokość wyświetlacza w pikselach
const int HEIGHT  = 64;  // Wysokość wyświetlacza w pikselach

// Stworzenie obiektu odpowiadającego naszemu wyświetlaczowi.
Adafruit_SSD1306 ekranik(WIDTH, HEIGHT, &Wire, -1);
//do oledu end

//Zmienne globalne

//Odlegosci czujników od najblizszego przedmiotu
float distance1; //odleglosc najbliżeszego przedmiotu od czujnika nr 1
float distance2; //odleglosc najbliżeszego przedmiotu od czujnika nr 2

int default_distance=5.00; //odległość w jakiej czujnik wykrywa ruch(domyślnie=5cm)



String surface;
int surface_room;

int axis = 0; //kąt otwarcia
bool isOpen; 
String topic1 = ""; //Variables for mqtt topics
String topic2 = ""; 


//CONFIG MODE START(Wifi manager)

////biblioteki
#include <ESPAsyncWebServer.h>     //Local WebServer used to serve the configuration portal
#include <ESPAsyncWiFiManager.h>          //https://github.com/tzapu/WiFiManager WiFi Configuration Magic

AsyncWebServer server(80); //do wifi managera
DNSServer dns;

AsyncWiFiManager wifiManager(&server,&dns);
//CONFIG MODE END



//MQTT CONNECTING START
const char* mqtt_server = "broker.hivemq.com";
const char* message = "Door+ welcome";

void callback(char* topic, byte* message, unsigned int length) {
  
  
  
  String messageTemp;
  Serial.println(messageTemp);
  for (int i = 0; i < length; i++) {
    messageTemp += (char)message[i];
  }
  Serial.println(messageTemp);
  if (messageTemp != "[]") {
    
    
    surface = messageTemp;
    
    int delimiter = surface.indexOf("x");
    int delimiter_1 = surface.indexOf("x", delimiter + 1);
    int delimiter_2 = surface.indexOf("x", delimiter_1 +1);
    

    // Define variables to be executed on the code later by collecting information from the readString as substrings.
    String first = surface.substring(delimiter + 1, delimiter_1);
    String second = surface.substring(delimiter_1 + 1, delimiter_2);
    surface_room = first.toInt() * second.toInt();

    
    
    
  
  }
}


WiFiClient espClient; 
PubSubClient client(mqtt_server, 1833, callback, espClient);




void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
  
    // Attempt to connect
    if (client.connect("door_plus")) {
      Serial.println("connected");
      client.subscribe(topic1);
      client.subscribe(topic2);
    }
      
    else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
    // Wait 5 seconds before retrying  
      delay(5000);
    }
  }
}


//MQTT CONNECTING END




//przekazniki i czujniki odleglosci
const byte relay_open = 14; //pin przekaźnika
const byte relay_close = 16;
const byte TRIG1=5; // pin podłączony do pinu TRIG czujnika odlegosci 1
const byte TRIG2=23;
const byte ECHO1=13; // pin podlaczony do pinu ECHO czujnika odlegosci 1 //dlatego const byte zamiast define
const byte ECHO2=19;




//Funkcje globalne

float distance_meter(byte trig, byte echo) { //mierzenie odleglosci od czujnika
  unsigned long time;
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  time = pulseIn(echo, HIGH);
  return time / 58.00;

}



void open_door() {
  
  for(axis = 0; axis<60; axis++){
    digitalWrite(relay_open, LOW);
    delay(100);
   
  }
 
  digitalWrite(relay_open, HIGH);
  axis = 0;
  isOpen = true;


}

void close_door() {
  for(axis = 0; axis<80; axis++){
    digitalWrite(relay_close, LOW);
    delay(100);
  

  }
  
  digitalWrite(relay_close, HIGH);
  axis = 0;
  isOpen = false ;

}

void motion_sensor(const byte trig_number, const byte echo_number){ //reaction for detected thing
  if (distance_meter(trig_number, echo_number)<=default_distance) //sprwadza czy odlegosc jest mniejsza rowna zmiennej default_distance
  {
    if(isOpen==false){
      open_door();
    }
    else {
      close_door();
    }
  }
  
 
 
  delay(500);
}






//main part of code:

//main part of code:

void setup() { /////////////////////////////////////////////SETUP
  Serial.begin(115200); //Ustawienie baud-rate



  if (!client.connected()){
    status_mqtt="OK";
  }
  else {
    status_mqtt="?";
  }
  
//DEFINICJE PINOW START
  
  pinMode(TRIG1, OUTPUT);
  pinMode(TRIG2, OUTPUT);
  pinMode(ECHO1, INPUT);
  pinMode(ECHO2, INPUT);
  pinMode(relay_open, OUTPUT);
  pinMode(relay_close, OUTPUT);


  digitalWrite(relay_open, HIGH);
  digitalWrite(relay_close, LOW);
  delay(10000);
  digitalWrite(relay_close, HIGH);
  isOpen=false;
//DEFINICJE PINOW END


  

//OLED START
  // Inicjalizacja wyświetlacza.
  if(!ekranik.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
      Serial.println("Błąd inicjalizacji wyświetlacza!");
      exit(0); // Nie idź dalej.
  }
  // Domyślny kolor tekstu: jasny na ciemnym tle.
  ekranik.setTextColor(WHITE);
  // Użyj 8-bitowego kodowania CP437, pozwalającego korzystać
  // ze wszystkich 256 znaków ASCII.
  ekranik.cp437(true);

  
 
//OLED END
  
  
//CONFIG START
  AsyncWiFiManager wifiManager(&server,&dns);

  wifiManager.autoConnect("Door+", "1234");
  Serial.println("connected...yeey :)"); 
  
  
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  Serial.println("Connected to MQTT");
  
//CONFIG END

  client.subscribe(topic1, 1); //topic subscribing
  client.subscribe(topic2,1);
}




void loop() {/////////////////////////////////////////////LOOP
  
//OLED START
  
  // Wyczyść bufor wyświetlacza przed zapisem do niego nowej treści.
  ekranik.clearDisplay();
  // Użyj najmniejszej czcionki.
  ekranik.drawLine(0, 13, 128, 16, WHITE);
  ekranik.drawLine(0,43,128,47,WHITE);
  
  
  ekranik.setTextSize(1);
 
 
  ekranik.setCursor(0, 0);
  ekranik.print("DOOR+");
  ekranik.setCursor(50, 0);
  

  ekranik.print("M: "+ status_mqtt);
  ekranik.setCursor(90, 0);
  ekranik.print("W: OK");
  ekranik.setCursor(0,48);
  ekranik.print("Surface:"+ String(surface_room));
  
  ekranik.setCursor(80, 48);
  ekranik.print("Pm2/: 1");

  ekranik.setTextSize(1.5);
  ekranik.setCursor(48,28);
  ekranik.print(String(people_room) + "/" + String(surface_room));

  
  ekranik.display();
  //delay(250);
//OLED END
  
  
  client.loop();
  delay(10);
  //mqtt connecting
  if (!client.connected()) {
    reconnect();
  }
  //mqtt connecting end
  
  

  
  //motion sensor 
  motion_sensor(TRIG1, ECHO1);
  motion_sensor(TRIG2,ECHO2);
  

  
}