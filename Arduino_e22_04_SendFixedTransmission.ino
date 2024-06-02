/*
 * EBYTE LoRa E22
 * Send a string message to a fixed point ADDH ADDL CHAN 0 2 23
 *
 * Write a string on serial monitor or reset to resend default value.
 *
 * Send a fixed point message, you must check that the transmitter and receiver have different
 * CHANNEL ADDL or ADDH, check down the correct configuration
 *
 * Pai attention e22 support RSSI, if you want use that functionality you must enable RSSI on configuration
 * configuration.TRANSMISSION_MODE.enableRSSI = RSSI_ENABLED;
 *
 * and uncomment #define ENABLE_RSSI true in this sketch
 *
 * Renzo Mischianti <https://www.mischianti.org>
 * https://www.mischianti.org/category/my-libraries/ebyte-lora-e22-devices/
 *
 * E22		  ----- Arduino
 * M0         ----- 7 (or GND)
 * M1         ----- 6 (or GND)
 * RX         ----- 4 (PullUP)
 * TX         ----- 5 (PullUP)
 * AUX        ----- 3  (PullUP)
 * VCC        ----- 3.3v/5v
 * GND        ----- GND
 *
 */

// With FIXED SENDER configuration
// #define DESTINATION_ADDL 3

// With FIXED RECEIVER configuration
#define DESTINATION_ADDL 3//2
#define DHTTYPE DHT22
#define DHTPIN 2

// If you want use RSSI uncomment //#define ENABLE_RSSI true
// and use relative configuration with RSSI enabled
//#define ENABLE_RSSI true

#include "Arduino.h"
#include "LoRa_E22.h"
#include "DHT.h"
#include <floatToString.h>

#include <SoftwareSerial.h>
SoftwareSerial mySerial(10, 11); // Arduino RX <-- e22 TX, Arduino TX --> e22 RX
LoRa_E22 e22ttl(&mySerial, 3, 8, 9); // AUX M0 M1
DHT dht(DHTPIN, DHTTYPE);
// -------------------------------------

void printParameters(struct Configuration configuration);

void setup() {
  Serial.begin(9600);
  delay(500);
  dht.begin();
  
  // Startup all pins and UART
  e22ttl.begin();

	ResponseStructContainer c;
	c = e22ttl.getConfiguration();
	// It's important get configuration pointer before all other operation
	Configuration configuration = *(Configuration*) c.data;
	Serial.println(c.status.getResponseDescription());
	Serial.println(c.status.code);

	printParameters(configuration);
	c.close();


  Serial.println("Hi, I'm going to send message!");

  // Send message
  ResponseStatus rs = e22ttl.sendFixedMessage(0, DESTINATION_ADDL, 23, "Hello, world?");
  // Check If there is some problem of succesfully send
  Serial.println(rs.getResponseDescription());
}

void loop() {  
  char S[15];
  delay(2000);
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    //return;
  }
  String temp =  floatToString(t, S, sizeof(S), 1);
  String hmdt = floatToString(h, S, sizeof(S), 1);
  String res = temp+"/"+hmdt+"/"+"15";
  Serial.println(res);
  Serial.println(temp);
  Serial.println(hmdt);
  e22ttl.sendFixedMessage(0, DESTINATION_ADDL, 23, res);
  /*if (Serial.available()) {
	  String input = Serial.readString();
	  e22ttl.sendFixedMessage(0, DESTINATION_ADDL, 23, input);
    Serial.println(input);
  }*/
}

void printParameters(struct Configuration configuration) {
	Serial.println("----------------------------------------");

	Serial.print(F("HEAD : "));  Serial.print(configuration.COMMAND, HEX);Serial.print(" ");Serial.print(configuration.STARTING_ADDRESS, HEX);Serial.print(" ");Serial.println(configuration.LENGHT, HEX);
	Serial.println(F(" "));
	Serial.print(F("AddH : "));  Serial.println(configuration.ADDH, HEX);
	Serial.print(F("AddL : "));  Serial.println(configuration.ADDL, HEX);
	Serial.print(F("NetID : "));  Serial.println(configuration.NETID, HEX);
	Serial.println(F(" "));
	Serial.print(F("Chan : "));  Serial.print(configuration.CHAN, DEC); Serial.print(" -> "); Serial.println(configuration.getChannelDescription());
	Serial.println(F(" "));
	Serial.print(F("SpeedParityBit     : "));  Serial.print(configuration.SPED.uartParity, BIN);Serial.print(" -> "); Serial.println(configuration.SPED.getUARTParityDescription());
	Serial.print(F("SpeedUARTDatte     : "));  Serial.print(configuration.SPED.uartBaudRate, BIN);Serial.print(" -> "); Serial.println(configuration.SPED.getUARTBaudRateDescription());
	Serial.print(F("SpeedAirDataRate   : "));  Serial.print(configuration.SPED.airDataRate, BIN);Serial.print(" -> "); Serial.println(configuration.SPED.getAirDataRateDescription());
	Serial.println(F(" "));
	Serial.print(F("OptionSubPacketSett: "));  Serial.print(configuration.OPTION.subPacketSetting, BIN);Serial.print(" -> "); Serial.println(configuration.OPTION.getSubPacketSetting());
	Serial.print(F("OptionTranPower    : "));  Serial.print(configuration.OPTION.transmissionPower, BIN);Serial.print(" -> "); Serial.println(configuration.OPTION.getTransmissionPowerDescription());
	Serial.print(F("OptionRSSIAmbientNo: "));  Serial.print(configuration.OPTION.RSSIAmbientNoise, BIN);Serial.print(" -> "); Serial.println(configuration.OPTION.getRSSIAmbientNoiseEnable());
	Serial.println(F(" "));
	Serial.print(F("TransModeWORPeriod : "));  Serial.print(configuration.TRANSMISSION_MODE.WORPeriod, BIN);Serial.print(" -> "); Serial.println(configuration.TRANSMISSION_MODE.getWORPeriodByParamsDescription());
	Serial.print(F("TransModeTransContr: "));  Serial.print(configuration.TRANSMISSION_MODE.WORTransceiverControl, BIN);Serial.print(" -> "); Serial.println(configuration.TRANSMISSION_MODE.getWORTransceiverControlDescription());
	Serial.print(F("TransModeEnableLBT : "));  Serial.print(configuration.TRANSMISSION_MODE.enableLBT, BIN);Serial.print(" -> "); Serial.println(configuration.TRANSMISSION_MODE.getLBTEnableByteDescription());
	Serial.print(F("TransModeEnableRSSI: "));  Serial.print(configuration.TRANSMISSION_MODE.enableRSSI, BIN);Serial.print(" -> "); Serial.println(configuration.TRANSMISSION_MODE.getRSSIEnableByteDescription());
	Serial.print(F("TransModeEnabRepeat: "));  Serial.print(configuration.TRANSMISSION_MODE.enableRepeater, BIN);Serial.print(" -> "); Serial.println(configuration.TRANSMISSION_MODE.getRepeaterModeEnableByteDescription());
	Serial.print(F("TransModeFixedTrans: "));  Serial.print(configuration.TRANSMISSION_MODE.fixedTransmission, BIN);Serial.print(" -> "); Serial.println(configuration.TRANSMISSION_MODE.getFixedTransmissionDescription());


	Serial.println("----------------------------------------");
}
