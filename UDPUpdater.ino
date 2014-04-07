#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUdp.h>
#include <IRremote.h>
#include <PString.h>

#define IR_PIN                   4
#define PIR_PIN                  6
#define PACKET_SIZE              128
#define CYCLE_MAX                20000
#define CYCLE_MOTION_REQUIRED    10000

IRrecv irrecv(IR_PIN);

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

// counters for the motion sensor
int cycle_count = 0;
int cycle_motion = 0;

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
  // IR handling
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

  cycle_count++;
  if (digitalRead(PIR_PIN))
    cycle_motion++;

  if (cycle_count >= CYCLE_MAX) {

    if (cycle_motion >= CYCLE_MOTION_REQUIRED) {
      Udp.beginPacket(remote_ip, remote_port);
      Udp.write(MotionMessage);
      Udp.endPacket();
    }
    cycle_count = 0;
    cycle_motion = 0;
  }
}
