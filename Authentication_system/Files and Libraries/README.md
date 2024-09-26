# Encryption

## Encryption is build by using BearSSL framework written on C
<p>Link: </p>
<p>https://bearssl.org/ </p>

<p>I was using AES-128-CBC method of encryption.</p>

<p>Youd should make some minor changes to run code with ESP8266 </p>
<ul>

<li>0. Install MFRC522 for scanner </li>


<li>1. Add Base64A.h and Base64A.cpp files to C:\Users\%UserName%\AppData\Local\Arduino15\packages\esp8266\hardware\esp8266\3.1.2\cores\esp8266</li>
<li>2. Change Secret Key and IV Vector in Authentication_system.ino</li>

    uint8_t cipher_key[16] = {0, 2, 4, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    uint8_t cipher_iv[16] = {0, 6, 6, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};

<li>3. Change Secret Key and IV Vector cryptography.js</li>

    var cipher_key = Buffer.from([0, 2, 4, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15]);
    var cipher_iv = Buffer.from([0, 6, 6, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15]);

<li>3. Change IP Adress and port of server in Authentication_system.ino</li>

    String serverName = "http://192.168.0.102:8080/validateAccess";

<li>4. Change Wifi SSID and PASS in Authentication_system.ino</li>

    char* ssid = "TP-LINK_6926";
    char* password = "29328123";



</ul>

