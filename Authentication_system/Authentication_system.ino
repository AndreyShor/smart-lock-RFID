// Basic library

// WI-fI CONNECTION
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

// RFID

#include <SPI.h>
#include <MFRC522.h>
#include <string>

// Encryption
#include "Base64A.h"

uint8_t cipher_key[16] = {0, 2, 4, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
uint8_t cipher_iv[16] = {0, 6, 6, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};

// PIN SETTINGS
#define RST_PIN         D1          // Configurable, see typical pin layout above
#define SS_PIN          D2         // Configurable, see typical pin layout above
#define LIGHT_PIN       D4         // Configurable, see typical pin layout above
#define RELAY_PIN       0         // Configurable, see typical pin layout above

// Comman c++ utility
using std::vector;
using std::string;


// Wi-Fi Settings

char* ssid = "TP-LINK_6926";
char* password = "29328123";

//Your Domain name with URL path or IP address with path
String serverName = "http://192.168.0.101:8080/validateAccess";

unsigned long lastTime = 0;
unsigned long timerDelay = 5000;
vector<string> arrayString = {};
char splitCharacter = ',';


MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance
MFRC522::MIFARE_Key key; 

const int lockopeningdelay = 4000; 


void setup() {
  Serial.begin(9600);
  while (!Serial);
  SPI.begin();

  mfrc522.PCD_Init();
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH); 
  pinMode(LIGHT_PIN, OUTPUT);
  delay(4);
// Setting Key for AES
  
  netowekCnnectWifi(ssid, password);
}

void loop() {

  if (WiFi.status() != WL_CONNECTED) {
    lightBlink(2, 500);
  }
  //Check if new tag is read by the reader 
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }
  // Check if it is able read Serial of the tag
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    Serial.print("Can t read card serial");
    return;
  }

  String ID = "";

  // Read UID of the tag
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    ID.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ")); // copy UID
    ID.concat(String(mfrc522.uid.uidByte[i], HEX));
    delay(50);
  } 
  ID.toUpperCase();

  // if lock is open - card will close it, if lock os closed - card will open it

  if (validateUID(ID.substring(1))) {
    Serial.print("Door is open ");

    digitalWrite(RELAY_PIN, LOW);
    digitalWrite(LIGHT_PIN, HIGH);

    delay(lockopeningdelay);
    Serial.clearWriteError();

    digitalWrite(LIGHT_PIN, LOW); 
    digitalWrite(RELAY_PIN, HIGH); 
    Serial.print("Door is closed");
  } else {
    lightBlink(2,200);
    Serial.print("Wrong Card");
    Serial.clearWriteError();
  }

  digitalWrite(RELAY_PIN, HIGH); 

  Serial.println("");
  mfrc522.PICC_HaltA();
}

  // Compare UID with database

 bool validateUID(String input) {
    if ((millis() - lastTime) > timerDelay) {
    //Check WiFi connection status
      if (WiFi.status() == WL_CONNECTED) {
        WiFiClient client;
        HTTPClient http;


        String serverPath = serverName;

        // Your Domain name with URL path or IP address with path
        http.begin(client, serverPath.c_str());

        // If you need Node-RED/server authentication, insert user and password below
        //http.setAuthorization("REPLACE_WITH_SERVER_USERNAME", "REPLACE_WITH_SERVER_PASSWORD");

        // Send HTTP GET request
        http.addHeader("Content-Type", "application/x-www-form-urlencoded");
        
        //Data to send with HTTP POST
        // Serial.println("Before Encryption text main function");
        // Serial.println(input);

        // Serial.println("Encrypted text main function");
        String encdata = encrypt(input);
        Serial.println(encdata);

        // Serial.println("Decrypted text main function");
        // String decdata = decrypt(encdata);
        // Serial.println(decdata);

        String httpRequestData = "uid=" + encdata;

        // Send HTTP POST request
        int httpResponseCode = http.POST(httpRequestData);

        std::vector<String> serverData = {};

        if (httpResponseCode == -1) {
          lightBlink(6,500);
          Serial.print("Can't connect to server. Server potentially id down ");
          return false;
        }

        if (httpResponseCode > 0) {
          Serial.print("HTTP Response code: ");
          Serial.println(httpResponseCode);
          // get data
          String payload = http.getString();
          String decdata = decrypt(payload);

          // transform data to char array       
          if(decdata == "Valid" ) {
            return true;
          } else {
            return false;            
          }


        } else {
          Serial.print("Error code: ");
          Serial.println(httpResponseCode);
          return false;
        }
        // Free resources
        http.end();
      } else {
        Serial.println("WiFi Disconnected");
        return false;
      }
      
      lastTime = millis();
    }

  return false;
 }

 // AES CBC Encryption
String encrypt(String plain_data){
  int i;
  // PKCS#7 Padding (Encryption), Block Size : 16
  int len = plain_data.length();
  int n_blocks = len / 16 + 1;
  uint8_t n_padding = n_blocks * 16 - len;
  uint8_t data[n_blocks*16];
  memcpy(data, plain_data.c_str(), len);
  for(i = len; i < n_blocks * 16; i++){
    data[i] = n_padding;
  }
  
  uint8_t key[16], iv[16];
  memcpy(key, cipher_key, 16);
  memcpy(iv, cipher_iv, 16);

  // encryption context
  br_aes_big_cbcenc_keys encCtx;

  // reset the encryption context and encrypt the data
  br_aes_big_cbcenc_init(&encCtx, key, 16);
  br_aes_big_cbcenc_run( &encCtx, iv, data, n_blocks*16 );

  // Base64 encode
  len = n_blocks*16;
  char encoded_data[ base64_enc_len(len) ];
  base64_encode(encoded_data, (char *)data, len);
  
  return String(encoded_data);
}

// AES CBC Decryption
String decrypt(String encoded_data_str){  
  int input_len = encoded_data_str.length();
  char *encoded_data = const_cast<char*>(encoded_data_str.c_str());
  int len = base64_dec_len(encoded_data, input_len);
  uint8_t data[ len ];
  base64_decode((char *)data, encoded_data, input_len);
  
  uint8_t key[16], iv[16];
  memcpy(key, cipher_key, 16);
  memcpy(iv, cipher_iv, 16);

  int n_blocks = len / 16;

  br_aes_big_cbcdec_keys decCtx;

  br_aes_big_cbcdec_init(&decCtx, key, 16);
  br_aes_big_cbcdec_run( &decCtx, iv, data, n_blocks*16 );

  // PKCS#7 Padding (Decryption)
  uint8_t n_padding = data[n_blocks*16-1];
  len = n_blocks*16 - n_padding;
  char plain_data[len + 1];
  memcpy(plain_data, data, len);
  plain_data[len] = '\0';

  return String(plain_data);
}

 void netowekCnnectWifi(char* ssid, char* password) {
   
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    lightBlink(2, 500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
 }

 void lightBlink(int times, int delayTime){
   for (int i=0; i <= times; i++){
    digitalWrite(LIGHT_PIN, HIGH);
    delay(delayTime);
    digitalWrite(LIGHT_PIN, LOW);
    delay(delayTime);
   }
 }




