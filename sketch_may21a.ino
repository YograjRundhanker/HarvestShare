#include <ESPmDNS.h>
#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include <Adafruit_Sensor.h>

// Pin assignments for ultrasonic sensors
const int trigPinFront = 27;
const int echoPinFront = 14;
const int trigPinRight = 21;
const int echoPinRight = 19;
const int trigPinLeft = 26;
const int echoPinLeft = 25;
const int trigPinMLeft = 23;
const int echoPinMLeft = 22;
const int trigPinMRight = 4;
const int echoPinMRight = 2;
const int relayPin1=13;
const int relayPin2=12;
const int distanceThreshold=50;

// Constants for sound speed and distances
#define SOUND_SPEED 0.034
long duration;
float distanceFront;
float distanceLeft;
float distanceRight;
float distanceMLeft;
float distanceMRight;

const char* ssid = "DIGISOL";
const char* password = "12345678"; 
AsyncWebServer server(80);

String readDistance() {
  // Sensor Front
  digitalWrite(trigPinFront, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPinFront, LOW);
  duration = pulseIn(echoPinFront, HIGH);
  distanceFront = duration * SOUND_SPEED / 2;

  // Sensor Right
  digitalWrite(trigPinRight, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPinRight, LOW);
  duration = pulseIn(echoPinRight, HIGH);
  distanceLeft = duration * SOUND_SPEED / 2;

  // Sensor Left
  digitalWrite(trigPinLeft, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPinLeft, LOW);
  duration = pulseIn(echoPinLeft, HIGH);
  distanceRight = duration * SOUND_SPEED / 2;

  // Sensor MLeft
  digitalWrite(trigPinMLeft, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPinMLeft, LOW);
  duration = pulseIn(echoPinMLeft, HIGH);
  distanceMLeft = duration * SOUND_SPEED / 2;

  // Sensor MRight
  digitalWrite(trigPinMRight, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPinMRight, LOW);
  duration = pulseIn(echoPinMRight, HIGH);
  distanceMRight = duration * SOUND_SPEED / 2;

  // Create a string with the distances for all sensors
  String distanceString = "Distance Sensor Front: " + String(distanceFront) + " cm\n";
  distanceString += "Distance Sensor Right: " + String(distanceRight) + " cm\n";
  distanceString += "Distance Sensor Left: " + String(distanceLeft) + " cm\n";
  distanceString += "Distance Sensor MLeft: " + String(distanceMLeft) + " cm\n";
  distanceString += "Distance Sensor MRight: " + String(distanceMRight) + " cm\n";

  // Print the distances to the serial port
  Serial.println(distanceString);

  // Return the distances as a string
  return distanceString;
}

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.7.2/css/all.css" integrity="sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr" crossorigin="anonymous">
  <style>
    html {
     font-family: Arial;
     display: inline-block;
     margin: 0px auto;
     text-align: center;
    }
    h2 { font-size: 3.0rem; }
    p { font-size: 3.0rem; }
    .units { font-size: 1.2rem; }
    .dht-labels{
      font-size: 1.5rem;
      vertical-align:middle;
      padding-bottom: 15px;
    }
  </style>
</head>
<body>
  <h2>Ultrasonic Sensor Readings on Server</h2>
   
  <p>
    <i class="fa fa-road" style="color:#00add6;"></i>
    <span class="dht-labels">Distance Sensor Front:</span>
    <span id="Distance1">%DISTANCE1%</span>
    <sup class="units">Cm</sup>
  </p>
  <p>
    <i class="fa fa-road" style="color:#00add6;"></i>
    <span class="dht-labels">Distance Sensor Right:</span>
    <span id="Distance2">%DISTANCE2%</span>
    <sup class="units">Cm</sup>
  </p>
  <p>
    <i class="fa fa-road" style="color:#00add6;"></i>
    <span class="dht-labels">Distance Sensor Left:</span>
    <span id="Distance3">%DISTANCE3%</span>
    <sup class="units">Cm</sup>
  </p>
  <p>
    <i class="fa fa-road" style="color:#00add6;"></i>
    <span class="dht-labels">Distance Sensor MLeft:</span>
    <span id="Distance4">%DISTANCE4%</span>
    <sup class="units">Cm</sup>
  </p>
  <p>
    <i class="fa fa-road" style="color:#00add6;"></i>
    <span class="dht-labels">Distance Sensor MRight:</span>
    <span id="Distance5">%DISTANCE5%</span>
    <sup class="units">Cm</sup>
  </p>
</body>
<script>  
setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      var distances = this.responseText.split("\n");
      document.getElementById("Distance1").innerHTML = distances[0];
      document.getElementById("Distance2").innerHTML = distances[1];
      document.getElementById("Distance3").innerHTML = distances[2];
      document.getElementById("Distance4").innerHTML = distances[3];
      document.getElementById("Distance5").innerHTML = distances[4];
    }
  };
  xhttp.open("GET", "/distanceCm", true);
  xhttp.send();
}, 1000);
</script>
</html>)rawliteral";

String processor(const String& var) {
  if (var == "DISTANCE1") {
    return String(distanceFront);
  } else if (var == "DISTANCE2") {
    return String(distanceLeft);
  } else if (var == "DISTANCE3") {
    return String(distanceRight);
  } else if (var == "DISTANCE4") {
    return String(distanceMLeft);
  } else if (var == "DISTANCE5") {
    return String(distanceMRight);
  }
  return String();
}

void setup() {
  Serial.begin(115200);
  pinMode(relayPin1, OUTPUT);
  pinMode(relayPin2, OUTPUT);
  pinMode(trigPinFront, OUTPUT);
  pinMode(echoPinFront, INPUT);
  pinMode(trigPinRight, OUTPUT);
  pinMode(echoPinRight, INPUT);
  pinMode(trigPinLeft, OUTPUT);
  pinMode(echoPinLeft, INPUT);
  pinMode(trigPinMLeft, OUTPUT);
  pinMode(echoPinMLeft, INPUT);
  pinMode(trigPinMRight, OUTPUT);
  pinMode(echoPinMRight, INPUT);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  if (!MDNS.begin("esp32")) {
    Serial.println("Error starting mDNS");
    return;
  }

  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, [](AsyncWebServerRequest* request) {
    request->send_P(200, "text/html", index_html, processor);
  });
  server.on("/distanceCm", HTTP_GET, [](AsyncWebServerRequest* request) {
    request->send_P(200, "text/plain", readDistance().c_str());
  });

  server.begin();
}

void loop() {
  
  // Perform actions based on the distance readings
  // if (distanceFront < distanceThreshold) {
  //   // Do something when an object is detected in front
  //   // Serial.println(distanceFront); 
  //   // Serial.println(distanceThreshold) ;
  //   digitalWrite(relayPin1, HIGH);
  // }
  if ((distanceFront < distanceThreshold && distanceMLeft<distanceThreshold && distanceMRight<distanceThreshold)){
    // Obstacle detected in front, stop the car
    digitalWrite(relayPin1,LOW);
    digitalWrite(relayPin2,LOW);
    Serial.println("Obstacle detected in front, left, right, so stopping the car");
  } 
  else if (distanceLeft < distanceThreshold && distanceRight >= distanceThreshold && distanceFront<distanceThreshold) {
    // Obstacle detected on the left, turn right
    digitalWrite(relayPin1,HIGH);
    digitalWrite(relayPin2,LOW);
    Serial.println("Obstacle detected on the left, turning right");
  } 
  else if (distanceRight < distanceThreshold && distanceLeft >= distanceThreshold && distanceFront<distanceThreshold) {
    // Obstacle detected on the right, turn left
    digitalWrite(relayPin1,LOW);
    digitalWrite(relayPin2,HIGH);
    Serial.println("Obstacle detected on the right, turning left");
  } 
  else if (distanceMLeft < distanceThreshold && distanceMRight >= distanceThreshold && distanceFront<distanceThreshold) {
    // Obstacle detected in the middle-left, turn right
    digitalWrite(relayPin1,HIGH);
    digitalWrite(relayPin2,LOW);
    Serial.println("Obstacle detected in the middle-left, turning right");
  } 
  else if (distanceMRight < distanceThreshold && distanceMLeft >= distanceThreshold && distanceFront<distanceThreshold) {
    // Obstacle detected in the middle-right, turn left
    digitalWrite(relayPin1,LOW);
    digitalWrite(relayPin2,HIGH);
    Serial.println("Obstacle detected in the middle-right, turning left");
  } 
  else if(distanceFront<distanceThreshold){
  digitalWrite(relayPin1,LOW); 
  digitalWrite(relayPin2,LOW); 
  Serial.println("objected detected in front, so stopping the car");
  }
  else {
    // No obstacle detected, continue moving forward
    digitalWrite(relayPin1,HIGH);
    digitalWrite(relayPin2,HIGH);
    Serial.println("No obstacle detected, moving forward");
  }
  
  // Perform similar actions for other sensors and relays
  
  // Delay between readings
  delay(100);
}