#include <WiFi.h>

const char* ssid = "SW";
const char* password = "02070465";

WiFiServer server(80);

const int led1 = 26;
const int led2 = 27;

String header;

void setup() {
  Serial.begin(115200);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected.");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.begin();
}

void loop(){
  WiFiClient client = server.available();
  if (client) {
    String currentLine = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        header += c;

        if (c == '\n') {
          if (currentLine.length() == 0) {

            if (header.indexOf("GET /led1/on") != -1) {
              digitalWrite(led1, LOW);    // ON 시 LOW 출력
            } else if (header.indexOf("GET /led1/off") != -1) {
              digitalWrite(led1, HIGH);   // OFF 시 HIGH 출력
            } else if (header.indexOf("GET /led2/on") != -1) {
              digitalWrite(led2, HIGH);
            } else if (header.indexOf("GET /led2/off") != -1) {
              digitalWrite(led2, LOW);
            }

            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            client.println("<!DOCTYPE html><html>");
            client.println("<head><title>ESP32 LED Control</title></head>");
            client.println("<body><h1>ESP32 LED Control</h1>");
            client.println("<p><a href=\"/led1/on\"><button>LED1 ON</button></a>");
            client.println("<a href=\"/led1/off\"><button>LED1 OFF</button></a></p>");
            client.println("<p><a href=\"/led2/on\"><button>LED2 ON</button></a>");
            client.println("<a href=\"/led2/off\"><button>LED2 OFF</button></a></p>");
            client.println("</body></html>");

            client.println();
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }
      }
    }

    header = "";
    client.stop();
  }
}
