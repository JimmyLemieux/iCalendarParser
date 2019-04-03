'use strict'

// C library API
const ffi = require('ffi');

// Express App (Routes)
const express = require("express");
const app     = express();
const path    = require("path");
const fileUpload = require('express-fileupload');

const Promise = require('promise');

var connection;
var fileListObj = [];

const mysql = require('mysql');

app.use(fileUpload());
app.use(express.json());

// Minimization
const fs = require('fs');
const JavaScriptObfuscator = require('javascript-obfuscator');

// Important, pass in port as in `npm run dev 1234`, do not change
const portNum = process.argv[2];

// Send HTML at root, do not change

app.use(express.static(path.join(__dirname, 'public')));

app.use(express.static(path.join(__dirname, 'assets')));

app.use(express.static(path.join(__dirname, 'uploads')));


//Setting up some sql things



//This part will have to be apart of the login form endpoint, on success we will have to send an endpoint


let sharedLib = ffi.Library('./libcal.so', {
  'makeObj' : ['string', ['string']],
  'eventJSONWrapper' : ['string', ['string']],
  'alarmJSONWrapper' : ['string', ['string']],
  'eventPropWrapper' : ['string', ['string']],
  'createCalendarFromJSONWrapper' : ['string', ['string','int','string','string','string','string','string','string','int','int']],
  'createEventFromJSONWrapper' : ['string', ['string','string','string','string','string','string','string', 'int', 'int']]
});


app.get('/',function(req,res){
  res.sendFile(path.join(__dirname+'/public/index.html'));
});

// Send Style, do not change
app.get('/style.css',function(req,res){
  //Feel free to change the contents of style.css to prettify your Web app
  res.sendFile(path.join(__dirname+'/public/style.css'));
});

// Send obfuscated JS, do not change
app.get('/index.js',function(req,res){
  fs.readFile(path.join(__dirname+'/public/index.js'), 'utf8', function(err, contents) {
    const minimizedContents = JavaScriptObfuscator.obfuscate(contents, {compact: true, controlFlowFlattening: true});
    res.contentType('application/javascript');
    res.send(minimizedContents._obfuscatedCode);
  });
});

//Respond to POST requests that upload files to uploads/ directory
app.post('/upload', function(req, res) {
  if(!req.files) {
    var ret = {error: "No Files Sent"};
    res.send(ret);
  }

  let uploadFile = req.files.someUpload;

  // Use the mv() method to place the file somewhere on your server
  uploadFile.mv('uploads/' + uploadFile.name, function(err) {
    if(err) {
      var ret = {error:"Some error uploading"};
      res.send(ret);
    }
    var ret = {error:"Success on Upload of file " + uploadFile.name};
    res.send(ret);
  });
});



//Respond to GET requests for files in the uploads/ directory
app.get('/uploads/:name', function(req , res){
  fs.stat('uploads/' + req.params.name, function(err, stat) {
    console.log(err);
    if(err == null) {
      res.sendFile(path.join(__dirname+'/uploads/' + req.params.name));
    } else {
      res.send('');
    }
  });
});

//******************** Your code goes here ********************


app.get('/obj', function(req,res) {
   let files = fs.readdirSync('./uploads/');
   console.log(files);
   var arr = [];
   for(var i =0;i<files.length;i++) {
     let JSONString = sharedLib.makeObj(files[i]);
     let JSONObject = JSON.parse(JSONString);
     if (JSONString == "{}" ? JSONObject["isValid"] = 0 : JSONObject["isValid"] = 1);
     JSONObject["fileName"] = files[i];
     arr[i] = JSON.stringify(JSONObject);
     if(JSONObject["isValid"]) {
       fileListObj.push(JSON.stringify(JSONObject));
     }
   }
   //Sending the json from server to client side of the application
   res.send(arr);
});

app.get('/eventList/:name', function(req, res) {
  let fileName = req.params.name;
  let JSONString = sharedLib.eventJSONWrapper(fileName);
  res.send(JSONString);
});

app.get('/alarmList/:name', function(req, res) {
  let fileName = req.params.name;
  let JSONString = sharedLib.alarmJSONWrapper(fileName);
  res.send(JSONString);
});

app.get('/propList/:name', function(req, res) {
  let fileName = req.params.name;
  let JSONString = sharedLib.eventPropWrapper(fileName);
  res.send(JSONString);
});

//Endpoint for the create calendar JSON

app.post('/createCalendar', function(req,res) {
  //This is returning null

  //Check if the fileName already exists

  var jsonReq = req.body;
  var fileName = jsonReq[0].fileName;
  var version = jsonReq[1].version;
  var prodid = jsonReq[1].prodID;
  var uid = jsonReq[2].uid;
  var startDate = jsonReq[2].dateStartDate;
  var startTime = jsonReq[2].dateStartTime;
  var createDate = jsonReq[2].dateCreateDate;
  var createTime = jsonReq[2].dateCreateTime;
  var utcStart = jsonReq[2].dateStartUTC;
  var utcCreate = jsonReq[2].dateCreateUTC;

  console.log(fileName + " " + version + " " + prodid + " " + uid + " " + startDate + " " + startTime + " " + createDate + " " + createTime + " " + utcStart  + " " + utcCreate);


  let files = fs.readdirSync('./uploads/');

  for(var i = 0;i<files.length;i++) {
    if(files[i] == fileName) {
      var ret = {error:"File Already Exists!"};
      res.send(JSON.stringify(ret));
    }
  }

  //Make a call to our c function that will make the call to the write calendar and everything
  var r = sharedLib.createCalendarFromJSONWrapper(fileName, version, prodid, uid, startDate, startTime, createDate, createTime,utcStart,utcCreate);
  console.log(r);
  var ret = {error: r};
  res.send(JSON.stringify(ret));
});

app.post('/createEvent', function(req, res) {
  var jsonReq = req.body;
  console.log(jsonReq);
  var fileName = jsonReq[0].fileName;
  var uid = jsonReq[1].uid;
  var dateStartDate = jsonReq[1].dateStartDate;
  var dateStartTime = jsonReq[1].dateStartTime;
  var dateCreateDate = jsonReq[1].dateCreateDate;
  var dateCreateTime = jsonReq[1].dateCreateTime;
  var summaryValue = jsonReq[1].summary;
  var utcStart = jsonReq[1].dateStartUTC;
  var utcCreate = jsonReq[1].dateCreateUTC;

  console.log(utcStart);

  var r = sharedLib.createEventFromJSONWrapper(fileName, uid,dateStartDate,dateStartTime,dateCreateDate,dateCreateTime,summaryValue,utcStart,utcCreate);
  var ret = {error:r};
  res.send(JSON.stringify(ret));
});

//Some database helper functions

function fileLogToSQL(data) {
  var heading = "(file_name, version, prod_id)";
  var values = "('"+ data.fileName + "', '"
              + data.version + "', '"
              + data.prodID +"')";


  var tableToBeInserted = "INSERT INTO FILE " + heading + " VALUES " + values + ";";
  return tableToBeInserted;
}

function eventToSQL(data, summary,startTime,organizer, location, cal_file_id) {
  var heading = "(summary, start_time, location, organizer, cal_file)";
  var values = "('"+ summary + "', '"
                    + startTime + "', '"
                    + location + "', '"
                    + organizer + "', '"
                    + cal_file_id + "')";

  var tableToBeInserted = "INSERT INTO EVENT " + heading + " VALUES " + values + ";";
  return tableToBeInserted;
}

function alarmToSQL(data, eventFieldID) {
  var heading = "(action, trigger, event)";

  var values = "('"+ data.action + "', '"
                    + data.trigger + "', '"
                    + eventFieldID + "')";
  console.log(values);
  var tableToBeInserted = "INSERT INTO ALARM " + heading + " VALUES " + values + ";";
  return tableToBeInserted;
}



//Make a connection to the data base
app.get('/loginDatabase', function(req, res) {
  var isErr = 0;
  console.log(req.body);
  var jsonData = req.query;
  var userName = jsonData.user;
  var passWord = jsonData.pass;
  var dbName = jsonData.dbName;
  console.log(userName);
  console.log(passWord);
  console.log(dbName);
  connection = mysql.createConnection({
    host: "dursley.socs.uoguelph.ca",
    user: userName,
    password: passWord,
    database: dbName
  });

  connection.connect(function(err) {
    if(err) {
      res.send({error: "FAIL"});
    } else {
      res.send({error: "SUCCESS"});
    }
  });

  var sql = "CREATE TABLE IF NOT EXISTS FILE (cal_id INT AUTO_INCREMENT PRIMARY KEY, file_Name VARCHAR(60) NOT NULL, version INT NOT NULL, prod_id VARCHAR(256) NOT NULL)";
  var sql2 = "CREATE TABLE IF NOT EXISTS EVENT (event_id INT AUTO_INCREMENT PRIMARY KEY, summary VARCHAR(1024), start_time DATETIME NOT NULL, location VARCHAR(60), organizer VARCHAR(256), cal_file INT NOT NULL, FOREIGN KEY(cal_file) REFERENCES FILE(cal_id) ON DELETE CASCADE)";
  var sql3 = "CREATE TABLE IF NOT EXISTS ALARM (alarm_id INT AUTO_INCREMENT PRIMARY KEY, action VARCHAR(256) NOT NULL, `trigger` VARCHAR(256) NOT NULL, event INT NOT NULL, FOREIGN KEY(event) REFERENCES EVENT(event_id) ON DELETE CASCADE)";


  connection.query(sql, function(err, result) {
    if(err){ console.log("FILE TABLE ALREADY EXISTS!"); return;}
    console.log("Table FILE Created");
  });

  connection.query(sql2, function(err, result) {
    if(err){ console.log("FILE EVENT ALREADY EXISTS!"); return;}
    console.log("Table EVENT Created");
  });

  connection.query(sql3, function(err, result) {
    if(err){ console.log("FILE ALARM ALREADY EXISTS!"); return;}
    console.log("Table ALARM Created");
  });


  connection.query("DELETE FROM FILE", function(err) {
    if(err) {
      console.log("Something went wrong");
    } else {
      console.log("Table deleted");
    }
  });

  connection.query("DELETE FROM EVENT" , function(err) {
    if(err) {
      console.log("Something went wrong");
    } else {
      console.log("Table deleted");
    }
  });

  connection.query("DELETE FROM ALARM", function(err) {
    if(err) {
      console.log("Something went wrong");
    } else {
      console.log("Table deleted");
    }
  });
});


//This is for adding things to the database
// All of the tables are populated and added here
app.get('/dbSaveFiles', function(req, res) {

  console.log("Starting to save files to the db");
  for(var i = 0;i<fileListObj.length;i++) {
    var jObj = JSON.parse(fileListObj[i]);
    var fileName = jObj.fileName;
    var stringSQLQuery = fileLogToSQL(jObj);

    //This is pushing things to the table
    connection.query(stringSQLQuery, function(err, rows, fields) {
      if(err) {
        console.log("Something went wrong!");
      }else {
        console.log("The table was made successfully");
      }
    });
  }



  //Now I am going to have to get a list of fileEvents
  connection.query("SELECT * FROM FILE;", function(err, rows, fields) {
    if(err) {
      console.log("Something went wrong");
    } else {
      //Go through all of the rows in the query

      for(let row of rows) {

        var calID = row.cal_id;
        var eventList = sharedLib.eventJSONWrapper(row.file_Name);
        var propList = sharedLib.eventPropWrapper(row.file_Name);
        var eventLocation = null;
        var eventOrganizer = null;
        //Go through all of the indi events and put them into the table with reference to the cal_id

        var eventListObj = JSON.parse(eventList);
        var propListObj = JSON.parse(propList);

        for(var i = 0;i<eventListObj.length;i++) { //The event list for each file in the database
          //Each event will have a specific list of props
          for(var x = 0;x<propListObj.length;x++) {
            var  jsonText  = JSON.stringify(propListObj[x]);
            if(propListObj[x]["event"] == (i+1)) {
              if(propListObj[x]["name"].toUpperCase() == "LOCATION") {
                eventLocation = propListObj[x]["description"];
              }
              if(propListObj[x]["name"].toUpperCase() == "ORGANIZER") {
                eventOrganizer = propListObj[x]["description"];
              }
              // Here we need to make a query and add these into the event table
            }
          }
          //console.log("Start time " + eventListObj[i]["start"])
          var startTimeDate  = eventListObj[i]["startDT"]["date"];
          var starTime = eventListObj[i]["startDT"]["time"];

          var summary = null;
          if(eventListObj[i].summary != '') summary = eventListObj[i].summary; 
          var eventToSQLQuery = eventToSQL(eventListObj[i], summary, startTimeDate + starTime, eventOrganizer, eventLocation,calID);

          //Adding the event to the database 
          connection.query(eventToSQLQuery, function(err, rows,fields) {
            if(err) {
              console.log("There was an error with the event table");
            } else {
              console.log(fields);
              console.log("NICE");              
              // console.log(result);
              // console.log("THE CURRENT: " + row.file_Name);
              // var alarmList = sharedLib.alarmJSONWrapper(row.file_Name);
              // var alarmListObj = JSON.parse(alarmList);
              // //You want to push the alar table here
              // console.log("The event table was appended to");
              // console.log("------------------------------");
              // console.log("THE CURRENT EVENT IS " + (i));
              // for(var x = 0;x<alarmListObj.length;x++) {
              //   if(alarmListObj[x]["event"] == (i)) {
              //     //console.log("File: " + row.file_Name + " with alarm number: " + (x+1) + " has " + jsonText);
              //     var alarmToSQLQuery = alarmToSQL(alarmListObj[x], result.insertId);
              //     connection.query(alarmToSQLQuery, function(err) {
              //       if(err) {
              //         console.log("Something went wrong");
              //       } else {
              //         console.log("Alarm table was appended");
              //       }
              //     });
              //   }
              // }
              // console.log("-------------------------------");
            }
          });
        }
      }
    }
  });
  //Make a query to from the event table 

  //res.send(fileListObj);
});

app.get('/dbSaveFilesAlarms', function(req, res) {

});

//This will clear all of the files from the database 
app.get('/dbClearFiles', function(req, res) {
  var deleteFileTable = "DELETE FROM FILE";

  var tableSize = "SELECT COUNT(*) FROM FILE";


  connection.query("DELETE FROM FILE", function(err) {
    if(err) {
      console.log("Something went wrong");
    } else {
      console.log("Table deleted");
    }
  });

  connection.query("DELETE FROM EVENT" , function(err) {
    if(err) {
      console.log("Something went wrong");
    } else {
      console.log("Table deleted");
    }
  });

  connection.query("DELETE FROM ALARM", function(err) {
    if(err) {
      console.log("Something went wrong");
    } else {
      console.log("Table deleted");
    }
  });


});

app.get('/getDBStatus', function(req,res) {
  var fileSize = "SELECT COUNT(*) FROM FILE";
  var eventSize = "SELECT COUNT(*) FROM EVENT";
  var alarmSize = "SELECT COUNT(*) FROM ALARM";
});


app.listen(portNum);
console.log('Running app at localhost: ' + portNum);
