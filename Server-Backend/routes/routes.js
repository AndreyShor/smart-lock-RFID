const express = require('express');

const con = require('./../DB/connect_db');
const crypto = require('../controller/cryptography');
const schedule = require('node-schedule');

const router = express.Router();

const fs = require('fs')

// Loging system
const entranceLogPath = "./Logs/entranceLog.log";
const errorLogPath = "./Logs/errorsLog.log";
let outputLog;
let errorsLog;

// Schedular for logging 
const rule = new schedule.RecurrenceRule();
let consoler;
// it will delete file logs first day of every month at 00:00:00
schedule.scheduleJob('0 0 0 1 */1 *', function () {
  if (fs.existsSync(entranceLogPath) && fs.existsSync(entranceLogPath) ) {
    fs.unlinkSync(entranceLogPath);
    fs.unlinkSync(errorLogPath);
  }
});

// if log files exist, it will append new logs to them, if files are not exist it will create it 
if (fs.existsSync(entranceLogPath) && fs.existsSync(errorLogPath) ) {
  outputLog = fs.createWriteStream(entranceLogPath, {
    flags: 'a'
  })
  errorsLog = fs.createWriteStream(errorLogPath, {
    flags: 'a'
  })
} else {
  outputLog = fs.createWriteStream(entranceLogPath);
  errorsLog = fs.createWriteStream(errorLogPath);
}

  consoler = new console.Console(outputLog, errorsLog);


  router.post('/validateAccess', (req, res) => {

    let uid = req.body.uid;
    try{
      // try to decrypt data
      uid = uid.replace(" ", "+")
      const stringDecrypt = crypto.aesDecrypt(uid)
      // check if user exist in db 
      con.query("SELECT users.FirstName FROM userstokey, users WHERE (users.UserID = userstokey.UserID) AND userstokey.UIDKey = '" + stringDecrypt + "'", 
      (err, result, fields) => {
        if (result.length === 0) {
          const timeElapsed = Date.now();
          const today = new Date(timeElapsed);
          // check if log file exist
          if (fs.existsSync(errorLogPath) ) {
            consoler.error("Not valid user with UID '" + stringDecrypt + "' try to open the door on " + today.toUTCString())
          } else {
            errorsLog = fs.createWriteStream(errorLogPath);
            consoler = new console.Console(errorsLog);
            consoler.error("Not valid user with UID '" + stringDecrypt + "' try to open the door on " + today.toUTCString())
          }
          const stringEncrypt = crypto.aesEncrypt("Invalid")
          res.send(stringEncrypt);

      } else {
          for (let value of result) {
            const timeElapsed = Date.now();
            const today = new Date(timeElapsed);
            // check if log file exist
            if (fs.existsSync(entranceLogPath) ) {
              consoler.log( "User " +  value.FirstName + " open the door on " + today.toUTCString())
            } else {
              entranceLog = fs.createWriteStream(entranceLogPath);
              consoler = new console.Console(entranceLog);
              consoler.log( "User " +  value.FirstName + " open the door on " + today.toUTCString())
            }

          }
          const stringEncrypt = crypto.aesEncrypt("Valid")
          res.send(stringEncrypt);

          
      }
        // Send back response from the server to the client e.g RFID lock or browser.
      })

    } catch (err) {
      console.log(err)
      const stringDecrypt = crypto.aesEncrypt("Invalid")
      res.send(stringDecrypt);
    }
  })

module.exports = router;