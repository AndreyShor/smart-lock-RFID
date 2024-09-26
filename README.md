# RFID Lock System

## RFID Lock on ESP 8266 (Wemos D1 min)

Wifi version of the RFID system is working on Wemos D1 mini microcontroler.

Templates for 3D Printer is stored in "Case" foldier. This files can be used to 3d print case for the lock.

We was using Arduino framework in combination with C++ libraries to write code for microcontroller.

<strong>LED Commands: </strong>
<ul>  
 <li>1 long blink - door is open </li>
 <li>2 short blings - incorrect RFID card </li>
 <li>Lock is blinking non stop - lock is trying to connect to WIFI </li>
 <li>8 medium blinks and 2 short - lock is connected to Wifi bet can't access server </li>
</ul>

## Lock Side Code - C/C++ and Arduino

Microcontroller is used on lock side. Basic Function of microcontroller is to get UID of a user and send it to the server for validation and manipulate a relay of the lock.

<strong> Link to all libraries for Microcontroller: </strong>

    \\cseesrv04\tech\CSEEprojects\RFID Lock Library

<ol>

<li> <strong> RFID_TAG_UID_PRINT.ino </strong>- contains code to display UID of the tag. </li> 

<li> <strong>DB</strong> - contains files of DB design </li> 

<li> <strong>Authentication_system.ino</strong> - contains authentication systemm for ESP8266, it is validate user by UID stored in in MySQL datbase and opening door for 5 seconds. Data encrypted done by using AES-128 algorithm </li> 

<li> <strong>ESPwifi_http_connection.ino</strong> - contains code to send request to connect to Wi-Fi, send request to backend server, store data in local memory 
<ol> You must specify
<li><strong>ssid</strong> of Wi-fi </li>
<li><strong>password</strong> of Wi-fi </li>
<li><strong>server IP</strong> and path to send request</li>

 </ol>

</li> 

</ol>

## Backend - Server Side,JavaScript Node.js, Express, MySQL 

Backend is written on Node.js + Express framework. MySQl is used as a Databse. Server is written on REST architecture.

Server is proccesing all request from the lock and grant access to space. 

Versions:
<ol>
<li>Node.js - 19.6.1</li>
<li>Npm - 9.5.0</li>
<li>Express - 4.18.2</li>
<li>mysql2 -3.2.0 </li>
 </ol>


All code you can find in a foldier "Server-Backend"

<strong>Environment and package varibles</strong> is specified in <strong>package.json</strong> file

<strong>Connection to MySQL <strong/> is specified in "DB/connect_db.js"

<strong>Authentication keys for AES is specified in "controller/cryptography.js"

You can <strong>create database and table </strong>by running create_db.sql

All <strong>server routes </strong> is specified in routes.js

You can start server by typing "npm start" or "node server.js"

### How to install node.js and npm

#### Short Way

<p>You should be able to install all libraries by copying project from /Server-Backend
Use this 2 commands to install everething, Node and NPM should be installed for this method </p>

    1. npm init
    2. npm install or npm i 

#### Long Way

<ol>
<li> Node.js and NPM - follow this link and install LTS version of node.js </li>
    https://nodejs.org/en/download

<li>NVM (Node Version Manager ) - install NVM to change version of Node.js to 19.6.1 or any others. Code should work on any versions of Node </li>

    https://github.com/nvm-sh/nvm

    Command to install nvm:
    npm i -g nvm

<li>Express - install express, this framework simplify routing on Node.js </li>
    https://expressjs.com/
    npm i -g express

<li>MySQL library</li>
    npm i mysql2
    https://www.npmjs.com/package/mysql2
 </ol>

 
## Electrical part of RFID lock 

Box, lead and frame was design for this project. Tou can find .stl and .gz files in <strong> Case foldier </strong>

Schematic of circuit:

<img src="https://cseegit.essex.ac.uk/tech/ispace-smart-lock-system/-/blob/main/schematic.jpg" alt="Diagram" >

### Parts
<ol>
<li><strong> RELAY</strong> - KY-019 5V 1-Channel Relay Board Module for Arduino IOT, cost about 2 pounds </li>
<li><strong> Microcontroller</strong> - ESP8266, cost about 8 pounds </li>
<li><strong> RFID Reader</strong> - AZDelivery RFID Kit - MFRC522, about 4.5 pounds  </li>
<li><strong> DC-DC converter</strong> - 20W Adjustable DC-DC Buck Converter with Digital Display about 4.5 pounds </li>
<li><strong> Fuse</strong> </li>
<li><strong> Wifi Box</strong> - 3D printed </li>
<li><strong> Wifi Box Lead</strong> - 3D printed </li>
<li><strong> Wifi Box Frame</strong> - 3D printed </li>
<li><strong> Diode for lock</strong> - 0.2 pound </li>
<li><strong> Total Cost of RFID soluion:</strong> +- 20 pounds </li>

<li><strong>Lock</strong> - GEM GK310LS 12 VDC Electric Release, aboyt 50-60 pounds </li>
<li><strong>Total Cost of RFID soluion + Lock for 1 door:</strong> +- 70 - 80 pounds </li>
</ol>

### Pins

<p>You can find pins connection for ESP8266 and RFID Reader in ESP8266 Pins.docx</p>


# Encryption - C Language

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



