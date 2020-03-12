/*
  Web Server

 A simple web server that shows the value of the analog input pins.
 using an Arduino Wiznet Ethernet shield.

 Circuit:
 * Ethernet shield attached to pins 10, 11, 12, 13
 * Analog inputs attached to pins A0 through A5 (optional)

 created 18 Dec 2009
 by David A. Mellis
 modified 9 Apr 2012
 by Tom Igoe
 modified 02 Sept 2015
 by Arturo Guadalupi
 
 */

#include <SPI.h>
#include <Ethernet.h>
#include <Arduino.h>
#include <math.h>

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = {
  0x90, 0xA2, 0xDA, 0x00, 0x7C, 0x74
};
IPAddress ip;

// Initialize the Ethernet server library
// with the IP address and port you want to use
// (port 80 is default for HTTP):
EthernetServer server(80);

void setup() {
  analogReference(EXTERNAL);
  // You can use Ethernet.init(pin) to configure the CS pin
  //Ethernet.init(10);  // Most Arduino shields
  //Ethernet.init(5);   // MKR ETH shield
  //Ethernet.init(0);   // Teensy 2.0
  //Ethernet.init(20);  // Teensy++ 2.0
  //Ethernet.init(15);  // ESP8266 with Adafruit Featherwing Ethernet
  //Ethernet.init(33);  // ESP32 with Adafruit Featherwing Ethernet

  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println("Kev's arduino server");

  // start the Ethernet connection and the server:
  Ethernet.begin(mac);

  // Check for Ethernet hardware present
  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
    while (true) {
      delay(1); // do nothing, no point running without Ethernet hardware
    }
  }
  if (Ethernet.linkStatus() == LinkOFF) {
    Serial.println("Ethernet cable is not connected.");
  }

  // start the server
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
}


void loop() 
{
  // listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
    Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
          client.println("Refresh: 5");  // refresh the page automatically every 5 sec
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          client.println("\nWelcome to Kev's arduino server, the sensor readings are as follows:\n");
  int readings = 1;
  int thermValue[8];
  float thermRes[8];
  float thermTemp[8];

  int sensorValue[8];
  float sensorVolt[8];
  float temp[8];
  int i;
  for(i=0; i < readings; i++)
  {
  // read the input on analog pin 0:
  thermValue[i] = analogRead(A0);
  // print out the value you read:
  client.println("\nThermistor value = ");
  client.println(thermValue[i]);

  thermRes[i] = 3000 / ((1023 / (float)thermValue[i]) - 1);

  client.println("\nThermistor res = ");
  client.println(thermRes[i]);

  thermTemp[i] = (1 / ((1 / 298.15) + ((1.0 / 3892) * logf(thermRes[i] / 3000)))) - 273.15;

  client.println("\nThermistor Temp = ");
  client.println(thermTemp[i]);
 
  // read the input on analog pin 1:
  sensorValue[i] = analogRead(A1);
  // print out the value you read:
  client.println("\n\nLM35 value = : ");
  client.println(sensorValue[i]);

  sensorVolt[i] = sensorValue[i] * 0.00323;
    
  client.println("\nLM35 Voltage = ");
  client.println(sensorVolt[i]);

  temp[i] = sensorVolt[i] / 0.01;

  client.println("\nLM35 Temp = ");
  client.println(temp[i]);

    delay(10);
  }
  client.println("\n\nDone");
  
  /*int total_therm = 0;
  int total_sensor =0;
  for(i = 0; i < readings; i++)
  {
    total_therm = total_therm + thermValue[i];
    total_sensor = total_sensor + sensorValue[i];
  }
  float avg = (float)total / (float)readings;
  Serial.print("\nThe average is: ");
  Serial.println(avg); */
  delay(10000);        // delay in between reads for stability
         
         
          client.println("</html>");
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
    Serial.println("client disconnected");
  }
  
}
