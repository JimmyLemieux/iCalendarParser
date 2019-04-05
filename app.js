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
       var newObjArr = [];
       newObjArr.push(JSONObject["fileName"]);
       newObjArr.push(JSONObject["version"]);
       newObjArr.push(JSONObject["prodID"]); 
       fileListObj.push(newObjArr);
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
                    + fileName + "', '"
                    + eventNumber + "', '"
                    + cal_file_id + "')";

  var tableToBeInserted = "INSERT INTO EVENT " + heading + " VALUES " + values + ";";
  return tableToBeInserted;
}

function alarmToSQL(data, eventFieldID) {
  var heading = "(action, `trigger`, event)";
//Adding
  var values = "('"+ data.action + "', '"
                    + data.trigger + "', '"
                    + eventFieldID + "')";
  var tableToBeInserted = "INSERT INTO ALARM " + heading + " VALUES " + values + ";";
  console.log(tableToBeInserted);
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
    if(err){ 
      console.log("FILE TABLE ALREADY EXISTS!"); 
      return;
    } else {
      console.log("Table FILE Created");
      connection.query(sql2, function(err, result) {
        if(err){ 
          console.log("FILE EVENT ALREADY EXISTS!"); 
          return;
        } else{
          console.log("Table EVENT Created");
          connection.query(sql3, function(err, result) {
            if(err){ 
            console.log("FILE ALARM ALREADY EXISTS!"); 
            return;
            } else {
              console.log("Table ALARM Created");

              connection.query("DELETE FROM FILE", function(err) {
                if(err) {
                  console.log("Something went wrong");
                } else {
                  console.log("Table deleted");
                  connection.query("DELETE FROM EVENT" , function(err) {
                    if(err) {
                      console.log("Something went wrong");
                    } else {
                      console.log("Table deleted");
                      connection.query("DELETE FROM ALARM", function(err) {
                        if(err) {
                          console.log("Something went wrong");
                        } else {
                          console.log("Table deleted");
                        }
                      });
                    }
                  });
                }
              });
            }
          });
        }
      });
    }
  });
});


//This is for adding things to the database
// All of the tables are populated and added here
app.get('/dbSaveFiles', function(req, res) {

  //You can save multiple files into the table
  console.log(fileListObj);

  var sql = "INSERT INTO FILE (file_Name, version, prod_id) VALUES ?";
  console.log("here");
  connection.query(sql, [fileListObj], function(err, result) {
    if(err) throw err;
    else {
      console.log("Pushed");
      //res.send({error: "OK"});
      //Make a query to get all of the files
      var pullFile = "SELECT * FROM FILE";
      connection.query(pullFile, function(err, rows, result) {
        if(err) throw err
        else {
          var eventArrPush = [];
          for(let row of rows) {
            var fileName = row.file_Name;
            var cal_id_ref = row.cal_id; 
            console.log( "Some file name" + " "+ fileName);
             var eventList = sharedLib.eventJSONWrapper(fileName);
             var propList = sharedLib.eventPropWrapper(fileName);
            // var eventListObj = JSON.parse(eventList);
            console.log(eventList);
            console.log("---------------");
            console.log(propList);
            


            // var propListObj = JSON.parse(propList);
            // var eventLocation = null;
            // var eventOrganizer = null;
            // var tempObj = [];
            // for(var i = 0;i=eventListObj.length;i++) {
            //   for(var x = 0;x<propListObj.length;x++) {
            //     var  jsonText  = JSON.stringify(propListObj[x]);
            //     if(propListObj[x]["event"] == (i+1)) {
            //       if(propListObj[x]["name"].toUpperCase() == "LOCATION") {
            //         eventLocation = propListObj[x]["description"];
            //       }
            //       if(propListObj[x]["name"].toUpperCase() == "ORGANIZER") {
            //         eventOrganizer = propListObj[x]["description"];
            //       }
            //       // Here we need to make a query and add these into the event table
            //     }
            //   }
            //   console.log( "This is some event " +" "+ eventListObj[i]);
            //   var startDate = eventListObj[i]["startDT"]["date"];
            //   var startTime = eventListObj[i]["startDT"]["time"];
            //   //"(summary, start_time, location, organizer, cal_file)"
            //   tempObj.push(eventListObj[i]["summary"], startDate + startTime, eventLocation, eventOrganizer, cal_id_ref);
            //   //eventArrPush.push()
            // }
            //eventArrPush.push(tempObj);
          }

          //Push the content of the events

          // //"(summary, start_time, location, organizer, cal_file)"
          // var sql = "INSERT INTO EVENT (summary, start_time, location, organizer, cal_file) VALUES ?";
          // connection.query(sql, [eventArrPush], function(err) {
          //   if(err) throw err;
          //   else {
          //     console.log("events pushed");
          //   }
          // });


        }
      });
    }
  });


  //res.send(fileListObj);
});
//This will clear all of the files from the database 
app.get('/dbClearFiles', function(req, res) {
  var deleteFileTable = "DELETE FROM FILE";

  var tableSize = "SELECT COUNT(*) FROM FILE";

  var clearRet = {};


  connection.query("DELETE FROM FILE", function(err) {
    if(err) {
      console.log("Something went wrong");
    } else {
      clearRet["FILE"] = "DELETED";
      console.log("yee Table deleted");
      connection.query("DELETE FROM EVENT" , function(err) {
        if(err) {
          console.log("Something went wrong");
        } else {
          clearRet["EVENT"] = "DELETED";
          console.log("yee Table deleted");
          connection.query("DELETE FROM ALARM", function(err) {
            if(err) {
              console.log("Something went wrong");
            } else {
              clearRet["ALARM"] = "DELETED";
              res.send(clearRet); 
              console.log("uee Table deleted");
            }
          });
        }
      });
    }
  });
});

app.get('/getDBStatus', function(req,res) {

  var fileSize = "SELECT count(file_Name) FROM FILE";
  var eventSize = "SELECT count(event_id) FROM EVENT";
  var alarmSize = "SELECT count(alarm_id) FROM ALARM";
  var respObj = {};

  connection.query(fileSize, function(err,rows, result) {
    if(err) {
      console.log("Something went wrong");
    } else {
      let jsonText = JSON.stringify(rows).replace("(file_Name)", "");
      var jsonObj = JSON.parse(jsonText);
      respObj["fileCount"] = jsonObj[0]["count"];
    }
  }); 

  connection.query(eventSize, function(err, rows, result) {
    if(err) {
      console.log("Something went wrong with the query");
    } else {
      let jsonText = JSON.stringify(rows).replace("(event_id)", "");
      let jsonObj = JSON.parse(jsonText);
      respObj["eventCount"] = jsonObj[0]["count"];
    }
  });

  connection.query(alarmSize, function(err, rows, result) {
    if(err) {

      console.log("Something went wrong");
    } else {
      let jsonText = JSON.stringify(rows).replace("(alarm_id)", "");
      let jsonObj = JSON.parse(jsonText);
      respObj["alarmCount"] = jsonObj[0]["count"];
      
      res.send(respObj);
    }
  });


});


app.listen(portNum);
console.log('Running app at localhost: ' + portNum);
