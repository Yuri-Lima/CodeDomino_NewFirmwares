/* mifare ultralight example (25-02-2018)
    https://www.rubydoc.info/gems/mfrc522/0.0.1/Mfrc522
 * 
 *   RFID-RC522 (SPI connexion)
 *   
 *   CARD RC522      Arduino (UNO)
 *     SDA  -----------  10 (Configurable, see SS_PIN constant)
 *     SCK  -----------  13
 *     MOSI -----------  11
 *     MISO -----------  12
 *     IRQ  -----------  
 *     GND  -----------  GND
 *     RST  -----------  9 (onfigurable, see RST_PIN constant)
 *     3.3V ----------- 3.3V
 *     
 */

  #include <SPI.h>
  #include <MFRC522.h>
  
  
  #define SS_PIN          10
  #define RST_PIN         9
  #include <stdlib.h>
  #include <string.h>

  MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance
  MFRC522::StatusCode status; //variable to get card status
  
  byte buffer[18];  //data transfer buffer (16+2 bytes data+CRC)
  byte size = sizeof(buffer);

  uint8_t pageAddr = 0x06;  //In this example we will write/read 16 bytes (page 6,7,8 and 9).
                            //Ultraligth mem = 16 pages. 4 bytes per page.  
                            //Pages 0 to 4 are for special functions.           
  
void setup() {
  Serial.begin(9600); // Initialize serial communications with the PC
  SPI.begin(); // Init SPI bus
  mfrc522.PCD_Init(); // Init MFRC522 card  
  Serial.println(F("Sketch has been started!"));
  memcpy(buffer,"F0000000",8);
  //"0000 0000 0000 0000 0000 0000 0000 0000 0000 0000"
  //buffer[0x30,0x31,0x32,0x33,0x34,0x35,0x36]; => 48,49,50,51,52,53,54 => 0,1,2,3,4,5,6
  // frente "4   0000"
  // numerico "00005   "
  // direita "5   0000"
  // esquerda "6   0000"
  // fim    "2   0000"
  // inicio "1   0000"
  // repete "8   0000"
  // fimrepete "9   0000"
  // descecaneta "10  0000"
  // subircaneta "11  0000"
  // subircaneta "A   0000"
}

void loop() {
  // Look for new cards
 gravapeca();

}
void gravapeca(){
   if ( ! mfrc522.PICC_IsNewCardPresent())
    return;

  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial())
    return;

  // Write data ***********************************************
  Serial.println(F(" Write data ... "));
  for (int i=0; i < 10 ; i++) {// 0 - 9
    //data is writen in blocks of 4 bytes (4 bytes per page)
    status = (MFRC522::StatusCode) mfrc522.MIFARE_Ultralight_Write(pageAddr+i, &buffer[i*4], 4);
    if (status != MFRC522::STATUS_OK) {
      Serial.print(F("MIFARE_Read() failed: "));
      Serial.println(mfrc522.GetStatusCodeName(status));
      return;
    }
  }
  Serial.println(F("MIFARE_Ultralight_Write() OK "));
  Serial.println();


  // Read data ***************************************************
    Serial.println(F("Reading data ... "));
    //data in 4 block is readed at once.
    status = (MFRC522::StatusCode) mfrc522.MIFARE_Read(0x06, buffer, &size);
    if (status != MFRC522::STATUS_OK)
    {
        Serial.print(F("MIFARE_Read() failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
        return;
    }

  Serial.print(F("Readed data: "));
  //Dump a byte array to Serial
  for (byte i = 0; i < 8; i++)
  {
    Serial.write(buffer[i]);
  }
  Serial.println();
  mfrc522.PICC_HaltA();
}

/*
int NTAGWriteDefaultUserPages() {
	int status = 0;
	byte page4[NTAG_PAGELENGTH];
	byte page5[NTAG_PAGELENGTH];

	#if (CARD_TYPE == 203)
	page4[0] = 0x01; page4[1] = 0x03; page4[2] = 0xA0; page4[3] = 0x0C;
	page5[0] = 0x34; page5[1] = 0x03; page5[2] = 0x00; page5[3] = 0xFE;
	#endif
	#if (CARD_TYPE == 206)
	page4[0] = 0x03; page4[1] = 0x00; page4[2] = 0xFE; page4[3] = 0x00;
	page5[0] = 0x00; page5[1] = 0x00; page5[2] = 0x00; page5[3] = 0x00;
	#endif
	status += mfrc522.MIFARE_Ultralight_Write(4, page4, NTAG_PAGELENGTH);
	status += mfrc522.MIFARE_Ultralight_Write(5, page5, NTAG_PAGELENGTH);

	return status;
}
*/

