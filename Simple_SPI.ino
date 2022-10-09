#include "support.h"

uint8_t SPI_State, dataReceived, dataToSend;
unsigned long previousMillis, currentMillis;
const long interval = 10000;

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  DDRB = (INPUT << DDB5) | (OUTPUT << DDB4) | (INPUT << DDB3) | (INPUT << DDB2);
  SPCR = (1 << SPIE) | (1 << SPE) | (0 << DORD) | (0 << MSTR) | (0 << CPOL) | (0 << CPHA);

  SPI_State = PrepareDataToSend;
  previousMillis = 0;
  currentMillis = 0;
  dataReceived = 0;
  dataToSend = 48;

  Serial.println("Program Started");

  sei();
}

void loop() {
  // put your main code here, to run repeatedly:
  SPIStateMachine();
}

ISR(SPI_STC_vect) {
  dataReceived = 1;
}

void SPIStateMachine() {
  switch (SPI_State) {
    case PrepareDataToSend:
      currentMillis = millis();
      SPDR = dataToSend;
      dataToSend++;
      if (dataToSend > 90) {
        dataToSend = 48;
      }
      SPI_State = WaitForReception;
      break;

    case WaitForReception:
      if (dataReceived) {
        Serial.println(SPDR);
        SPI_State = PrepareDataToSend;
        dataReceived = 0;
        previousMillis = currentMillis;
      }
      else if ((currentMillis - previousMillis) >= interval) {
        Serial.println(currentMillis);
        previousMillis = currentMillis;
      }
      currentMillis = millis();
      break;
  }
}
