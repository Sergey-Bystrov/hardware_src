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
 * E22		  ----- esp32
 * M0         ----- 19 (or GND)
 * M1         ----- 21 (or GND)
 * RX         ----- TX2 (PullUP)
 * TX         ----- RX2 (PullUP)
 * AUX        ----- 15  (PullUP)
 * VCC        ----- 3.3v/5v
 * GND        ----- GND
 *
 */
/*
ПРИЁМНИК
*/
// with this DESTINATION_ADDL 2 you must set
// FIXED SENDER configuration to the other device and
// FIXED RECEIVER to this device
#define DESTINATION_ADDL 2//3

// If you want use RSSI uncomment //#define ENABLE_RSSI true
// and use relative configuration with RSSI enabled
//#define ENABLE_RSSI true

#include "Arduino.h"
#include "LoRa_E22.h"

// ---------- esp32 pins --------------
LoRa_E22 e22ttl(&Serial2, 15, 19, 21); //  RX AUX M0 M1

void printParameters(struct Configuration configuration);

void setup() {
  Serial.begin(9600);
  delay(1000);

  // Startup all pins and UART
  e22ttl.begin();
  delay(1000);
/*
	ResponseStructContainer c;
	c = e22ttl.getConfiguration();
	// It's important get configuration pointer before all other operation
	Configuration configuration = *(Configuration*) c.data;
	Serial.println(c.status.getResponseDescription());
  delay(500);
	Serial.println(c.status.code);
  delay(500);
	printParameters(configuration);
  delay(500);
	c.close();
  //delay(500);
  //Serial.println("Hi, I'm going to receive message!");
  delay(1000);
  //Serial.println("Listen");
  */
}

void loop() {    
	// If something available
  if (e22ttl.available()>1) {    
	  // read the String message
#ifdef ENABLE_RSSI
	ResponseContainer rc = e22ttl.receiveMessageRSSI();
#else
	ResponseContainer rc = e22ttl.receiveMessage();
#endif
	// Is something goes wrong print error
	if (rc.status.code!=1){
    Serial.println("something goes wrong");
		Serial.println(rc.status.getResponseDescription());
	}else{
		//Print the data received    
		Serial.println(rc.data);
#ifdef ENABLE_RSSI
		Serial.print("RSSI: "); Serial.println(rc.rssi, DEC);
#endif
	}
  }
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
