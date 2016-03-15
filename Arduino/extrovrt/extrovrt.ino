//wifi stuff
#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
//servo stuff
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

/************************* WiFi Access Point *********************************/

#define WLAN_SSID       "YOUR_WIFI_SSID"
#define WLAN_PASS       "YOUR_WIFI_PASSWORD"

/************************* Adafruit.io Setup *********************************/

#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883
#define AIO_USERNAME    "YOUR_AIO_USERNAME"
#define AIO_KEY         "YOUR_AIO_KEY"


/************ Global State (you don't need to change this!) ******************/

// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;

// Store the MQTT server, client ID, username, and password in flash memory.
// This is required for using the Adafruit MQTT library.
const char MQTT_SERVER[] PROGMEM    = AIO_SERVER;
// Set a unique MQTT client ID using the AIO key + the date and time the sketch
// was compiled (so this should be unique across multiple devices for a user,
// alternatively you can manually set this to a GUID or other random value).
const char MQTT_CLIENTID[] PROGMEM  = __TIME__ AIO_USERNAME;
const char MQTT_USERNAME[] PROGMEM  = AIO_USERNAME;
const char MQTT_PASSWORD[] PROGMEM  = AIO_KEY;

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, MQTT_SERVER, AIO_SERVERPORT, MQTT_CLIENTID, MQTT_USERNAME, MQTT_PASSWORD);


/****************************** Feeds ***************************************/
// Setup a feed called 'extrovrt-direction' for subscribing to changes.
const char EV_FEED[] PROGMEM = AIO_USERNAME "/feeds/extrovrt-direction";
Adafruit_MQTT_Subscribe extrovrt_direction = Adafruit_MQTT_Subscribe(&mqtt, EV_FEED);


/****************************** Servo ***************************************/
// Called this way, it uses the default address 0x40
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
// We are using continuous rotation servos, so in this case "minimum" is fully
// backwards, "maximum" is fully forwards, and "stop" is the middle inflection
// point
// NOTE: Plan on tweaking these up or down with your particular set of motors 
//       until it works
// 339 is bare-minimum (slowest)
#define MOTORMIN 339
// this is the "inflection point" on a CR servo where it stops in the middle
#define MOTORSTOP 355 
// 362 is bare-maximum (slowest)
#define MOTORMAX 366
#define LEFTSERVO 0
#define RIGHTSERVO 1


/*************************** Sketch Code ************************************/

// Bug workaround for Arduino 1.6.6, it seems to need a function declaration
// for some reason (only affects ESP8266, likely an arduino-builder bug).
void MQTT_connect();
void moveWheels(uint16_t left, uint16_t right);

void setup() {
  Serial.begin(115200);

  // Setup servo
  pwm.begin();
  pwm.setPWMFreq(60);  // Analog servos run at ~60 Hz updates
  moveWheels(MOTORSTOP, MOTORSTOP);
  yield();
  
  // Connect to WiFi access point.
  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.println("WiFi connected");
  Serial.println("IP address: "); Serial.println(WiFi.localIP());

  // Setup MQTT subscription for onoff feed.
  mqtt.subscribe(&extrovrt_direction);

}

/*
 * test loop
void loop() {
  moveWheels(MOTORMIN,MOTORMIN);
  delay(1000);
  moveWheels(MOTORSTOP,MOTORSTOP);
  delay(1000);
  moveWheels(MOTORMAX,MOTORMAX);
  delay(1000);
  moveWheels(MOTORSTOP,MOTORSTOP);
  delay(1000);
}
*/


void loop() {
  // Ensure the connection to the MQTT server is alive (this will make the first
  // connection and automatically reconnect when disconnected).  See the MQTT_connect
  // function definition further below.
  MQTT_connect();

  // this is our 'wait for incoming subscription packets' busy subloop
  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(1000))) {
    if (subscription == &extrovrt_direction) {
      Serial.print (F("Got: "));
      Serial.println((char *)extrovrt_direction.lastread);

      // check directions
      if (0 == strcmp((char *)extrovrt_direction.lastread, "F")) {
        Serial.println("Moving Forward");
        moveWheels(MOTORMAX, MOTORMAX);
      } else if (0 == strcmp((char *)extrovrt_direction.lastread, "B")) {
        Serial.println("Moving Backward");
        moveWheels(MOTORMIN, MOTORMIN);
      } else if (0 == strcmp((char *)extrovrt_direction.lastread, "R")) {
        Serial.println("Moving Right");
        moveWheels(MOTORMAX, MOTORMIN);
      } else if (0 == strcmp((char *)extrovrt_direction.lastread, "L")) {
        Serial.println("Moving Left");
        moveWheels(MOTORMIN, MOTORMAX);
      } else if (0 == strcmp((char *)extrovrt_direction.lastread, "S")) {
        Serial.println("Stopping");
        moveWheels(MOTORSTOP, MOTORSTOP);
      }
    } 
  }

  delay(1000);
}


// send this the PWM values you want for each wheel + it'll handle the movement
// it will also handle the fact that the right wheel has its direction reversed
void moveWheels(uint16_t left, uint16_t right) {
  // reverse the right motor
  if (right == MOTORMIN) { right = MOTORMAX; }
  else if (right == MOTORMAX) { right = MOTORMIN; }
  
  Serial.print("Left: ");
  Serial.println(left);
  Serial.print("Right: ");
  Serial.println(right);
  pwm.setPWM(LEFTSERVO, 0,  left);
  pwm.setPWM(RIGHTSERVO, 0, right);
}

// Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care if connecting.
void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(5000);  // wait 5 seconds
  }
  Serial.println("MQTT Connected!");
}
