#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUdp.h>
#include <IRremote.h>
#include <PString.h>
#include <CapacitiveSensor.h>

/* Hardware */
#define IR_PIN                   4
#define PIR_PIN                  6
#define CS_SEND_PIN              8
#define CS_RECV_PIN              9

/* Configuration */
#define PACKET_SIZE              128
#define CYCLE_MAX                20000
#define CYCLE_MOTION_REQUIRED    10000
#define CS_TOUCH_MIN             100
#define CS_TOUCH_CYCLES_MIN      10
#define CS_TOUCH_CYCLES_LONG_MIN 80
#define CS_TOUCH_ALLOWED_LOW     10

IRrecv irrecv(IR_PIN);
CapacitiveSensor cs(CS_SEND_PIN, CS_RECV_PIN);

decode_results results;

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192,168,1,82);
IPAddress remote_ip(192,168,1,81);
int remote_port = 12001;

// buffers for sending data
char ReplyBuffer[PACKET_SIZE];
PString ReplyBufferString(ReplyBuffer, sizeof(ReplyBuffer));
char IRMessage[] = "IRCODE=";
char MotionMessage[] = "MOTION";
char CSMessage[] = "TOUCH";
char CSLongMessage[] = "LONGTOUCH";

// counters for the motion sensor
int cycle_count = 0;
int cycle_motion = 0;

// counters for the Capacitive Sensor
int cycle_cs_high = 0;
int cycle_cs_low = 0;

// An EthernetUDP instance to let us send and receive packets over UDP
EthernetUDP Udp;

void setup() {
  // Start Serial
  Serial.begin(9600);
  Serial.println("UPDUpdater");
  Serial.print("Built: ");
  Serial.println(__TIMESTAMP__);
  // Start IR reciever
  irrecv.enableIRIn();
  // Start PIR sensor
  pinMode(PIR_PIN, INPUT);
  // Start Ethernet
  Ethernet.begin(mac, ip);
  Udp.begin(12001);
  delay(1);

  Serial.println("setup() complete");
}

void loop() {
  /* IR Remote */
  if (irrecv.decode(&results)) {
    Serial.print("IR Recieved: ");
    Serial.println(results.value, HEX);
    if (results.decode_type == NEC) {
      // Filters out bad IR recieves
      Udp.beginPacket(remote_ip, remote_port);
      Udp.write(IRMessage);
      ReplyBufferString.print(results.value, HEX);
      Udp.write(ReplyBuffer);
      Udp.endPacket();
      ReplyBufferString.begin();
    }
    irrecv.resume(); // Receive the next value
  }

  /* PIR Motion */
  cycle_count++;
  if (digitalRead(PIR_PIN))
    cycle_motion++;

  if (cycle_count >= CYCLE_MAX) {

    if (cycle_motion >= CYCLE_MOTION_REQUIRED) {
      Serial.println("Motion");
      Udp.beginPacket(remote_ip, remote_port);
      Udp.write(MotionMessage);
      Udp.endPacket();
    }
    cycle_count = 0;
    cycle_motion = 0;
  }

  /* Capacitive Touch Sensor */
  if (cs.capacitiveSensor(200) > CS_TOUCH_MIN) {
    if (cycle_cs_low > CS_TOUCH_ALLOWED_LOW) {
      // Reset at start of new touch
      cycle_cs_high = 0;
      cycle_cs_low = 0;
    }
    cycle_cs_high++;
  } else {
    cycle_cs_low++;

     // if touch ended
    if (cycle_cs_low > CS_TOUCH_ALLOWED_LOW && cycle_cs_high > CS_TOUCH_CYCLES_MIN) {
      if (cycle_cs_high > CS_TOUCH_CYCLES_LONG_MIN) {
        // Long Press
        Serial.println("Long Touch");
        Udp.beginPacket(remote_ip, remote_port);
        Udp.write(CSLongMessage);
        Udp.endPacket();
      } else {
        // Short Press
        Serial.println("Touch");
        Udp.beginPacket(remote_ip, remote_port);
        Udp.write(CSMessage);
        Udp.endPacket();
      }
      cycle_cs_high = 0;
      cycle_cs_low = 0;
    }
  }
}
