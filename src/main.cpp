#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include "credentials.h"

#define PACKET_LEN 256
#define CLIENT_BUFF_SIZE 512
#define DEEPSLEEP_US 20000000
#define TX_TOPIC "aircon/packet/tx"
#define RX_TOPIC "aircon/packet/rx"
#define ERROR_TOPIC "aircon/packet/error"

WiFiClientSecure espClient;
PubSubClient client(espClient);

void setup_wifi() {
  unsigned long count = 0;
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (count > 9){
      esp_sleep_enable_timer_wakeup(DEEPSLEEP_US);
      esp_deep_sleep_start();
    }
    count++;
  }
  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  if (strcmp(topic, TX_TOPIC) == 0){
    Serial2.write((uint8_t *) payload, (size_t) length);
    for (int i = 0; i < length; i++) {
      Serial.printf("%02X ", payload[i]);
    }
  } else {
    for (int i = 0; i < length; i++) {
      Serial.print((char)payload[i]);
    }
  }
  Serial.println();
}

void reconnect() {
  while (!client.connected()) {
    if (WiFi.status() != WL_CONNECTED) {
      esp_sleep_enable_timer_wakeup(DEEPSLEEP_US);
      esp_deep_sleep_start();
    }
    Serial.print("Attempting MQTT connection...");
    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX);
    if (client.connect(clientId.c_str(), mqtt_user, mqtt_pass)) {
      Serial.println("connected");
      client.subscribe(TX_TOPIC);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
  setCpuFrequencyMhz(80);
  Serial.begin(115200);
  Serial2.setTimeout(10);
  Serial2.begin(2400, SERIAL_8E1, 19, 18);
  setup_wifi();

  //espClient.setInsecure();
  //client.setServer(mqtt_server, 1883);
  espClient.setCACert(root_ca);
  espClient.setCertificate(certificate);
  espClient.setPrivateKey(private_key);  
  client.setServer(mqtt_server, 8883);

  client.setCallback(callback);
  client.setBufferSize(CLIENT_BUFF_SIZE);
}

enum packet_check {ok, sb, fe, ce, se};
// ok, stand-by, framing error, checksum error, size error

void read_packet(uint8_t pkt[], int16_t &len, enum packet_check &res) {
  size_t length;
  int16_t i;
  uint8_t csum = 0;

  for (i=0; i<4; i++) {
    if (Serial2.readBytes(pkt+i, 1) != 1){
      res = sb;
      return;
    }
  }
  length = pkt[3] + 1;
  if (length > PACKET_LEN - 4) {
    res = se;
    return;
  }
  for (i=0; i<length; i++) {
    if (Serial2.readBytes(pkt+4+i, 1) != 1){
      res = fe;
      return;
    }
  }
  for (i=0; i<3+length; i++)
    csum ^= pkt[i];
  if (csum != pkt[3+length]){
    res = ce;
    return;
  }
  res = ok;
  len = 4 + length;
}

void loop() {
  int16_t len;
  uint8_t pkt[PACKET_LEN];
  enum packet_check res = sb;
  const char *msg;

  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  
  read_packet(pkt, len, res);
  if (res == ok){
    client.publish(RX_TOPIC, pkt, len);
    Serial.print("Message sent    [");
    Serial.print(RX_TOPIC);
    Serial.print("] ");
    for (int16_t i=0; i<len; i++){
      Serial.printf("%02X ", pkt[i]);
    }
    Serial.println();
  } else if (res != sb) {
    switch (res){
      case fe:
        msg = "FE";
        break;
      case ce:
        msg = "CE";
        break;
      case se:
        msg = "SE";
        break;
      default:
        msg = "NA";
    }
    client.publish(ERROR_TOPIC, msg);
    Serial.printf( "Packet error : %s\n", msg );
    Serial.print("Message sent    [");
    Serial.print(ERROR_TOPIC);
    Serial.print("] ");
    Serial.println(msg);
  }
}
