#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <Servo.h>

// use dedicated header to hide personal data
#include "mywifi.h"

WiFiUDP udp;
unsigned int localPort = 1200;  // local port to listen on

char packetBuffer[UDP_TX_PACKET_MAX_SIZE];  // buffer to hold incoming packet,

float coords[2];
char buf[100];
int cnt = 0;

Servo servo;
#define SERVO_PIN 15

#define MOTOR_PIN_A 12
#define MOTOR_PIN_B 13

// floating-point version of Arduino's map() function
float map_f(float x, float in_min, float in_max, float out_min, float out_max) {
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


void setup() {
  Serial.begin(115200);

  servo.attach(SERVO_PIN);
  servo.write(90);  // start position - at the middle

  WiFi.mode(WIFI_STA);
  WiFi.begin(STASSID, STAPSK);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(500);
  }
  Serial.print(" Connected! IP address: ");
  Serial.println(WiFi.localIP());
  Serial.printf("UDP server on port %d\n", localPort);
  udp.begin(localPort);
}


void loop() {
  // if there's data available, read a packet
  int packetSize = udp.parsePacket();
  if (packetSize) {
    cnt++;

    // Serial.print("Received packet of size ");
    // Serial.print(packetSize);
    // Serial.print(" from ");
    // IPAddress remote = udp.remoteIP();
    // for (int i = 0; i < 4; i++) {
    //   Serial.print(remote[i], DEC);
    //   if (i < 3) {
    //     Serial.print(".");
    //   }
    // }
    // Serial.print(", port ");
    // Serial.println(udp.remotePort());

    udp.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE);  // read a packet into packetBufffer
    memcpy(coords, packetBuffer, 2*sizeof(float));  // extract coordinates
    // sprintf(buf, "%d: %d, %d", cnt,
    //         (int)map_f(coords[0], -1.0f, 1.0f, 0.0f, 180.0f),
    //         (int)map_f(coords[1], -1.0f, 1.0f, 0.0f, 1023.0f));
    // Serial.println(buf);

    // left/right
    servo.write((int)map_f(coords[0], -1.0f, 1.0f, 0.0f, 180.0f));

    if (coords[1] > 0) {
      // forward
      digitalWrite(MOTOR_PIN_B, 0);
      analogWrite(MOTOR_PIN_A, (int)map_f(coords[1], 0.0f, 1.0f, 0.0f, 1023.0f));
    } else {
      // reverse
      digitalWrite(MOTOR_PIN_A, 0);
      analogWrite(MOTOR_PIN_B, (int)map_f(coords[1], -1.0f, 0.0f, 1023.0f, 0.0f));
    }

  }

  delay(90);

  // TODO: add no-messages counter (timeout)
}
