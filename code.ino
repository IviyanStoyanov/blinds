#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Servo.h>

const char* ssid = "STOYANOVI_3";
const char* password = "101424241410";

ESP8266WebServer server(80);
Servo servo;

int servoPin = 5;  // GPIO5 for servo control
bool isRotating = false;

void setup() {
  Serial.begin(115200);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("WiFi connected");
  Serial.print("ESP IP Address: ");
  Serial.println(WiFi.localIP());

  // Initialize servo
  servo.attach(servoPin);

  // Initialize web server routes
  server.on("/", handleRoot);
  server.on("/startRotation", handleStartRotation);
  server.on("/stopRotation", handleStopRotation);
  server.begin();
  Serial.println("Web server started");
}

void loop() {
  server.handleClient();
  if (isRotating) {
    // Perform any additional rotation actions if needed
  }
}

void handleRoot() {
  String html = "<html><head><style>h1 { text-align: center; }";
  html += "button { font-size: 24px; padding: 10px 20px; }</style></head>";
  html += "<body><h1>Welcome to the Servo Control Page</h1>";
  html += "<button onclick=\"startRotation('down')\">DOWN</button>";
  html += "<button onclick=\"startRotation('up')\">UP</button>";
  html += "<button onclick=\"stopRotation()\">STOP</button>";
  html += "<script>function startRotation(direction) {";
  html += "var xhttp = new XMLHttpRequest();";
  html += "xhttp.open('GET', '/startRotation?dir=' + direction, true);";
  html += "xhttp.send();";
  html += "}";
  html += "function stopRotation() {";
  html += "var xhttp = new XMLHttpRequest();";
  html += "xhttp.open('GET', '/stopRotation', true);";
  html += "xhttp.send();";
  html += "}</script></body></html>";
  server.send(200, "text/html", html);
}

void handleStartRotation() {
  if (!isRotating) {
    String direction = server.arg("dir");
    if (direction == "down") {
      isRotating = true;
      server.send(200, "text/plain", "Servo rotation started (DOWN)");
      servo.attach(servoPin); // Re-attach servo if previously detached
      servo.write(180); 
      delay(20000);
      servo.detach();   
    } else if (direction == "up") {
      isRotating = true;
      server.send(200, "text/plain", "Servo rotation started (UP)");
      servo.attach(servoPin); // Re-attach servo if previously detached
      servo.write(0); 
      delay(20000);
      servo.detach();  
    } else {
      isRotating = false;
      server.send(400, "text/plain", "Invalid direction");
      return;
    }
    
  } else {
    server.send(400, "text/plain", "Servo already rotating");
  }
}

void handleStopRotation() {
  if (isRotating) {
    isRotating = false;
    servo.detach(); // Detach the servo to stop its movement
    server.send(200, "text/plain", "Servo rotation stopped (OFF)");
  } else {
    server.send(400, "text/plain", "Servo not currently rotating");
  }
}
