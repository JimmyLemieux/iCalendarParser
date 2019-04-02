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


// connection = mysql.createConnection({
//   host: "dursley.socs.uoguelph.ca",
//   user: "jlemie03",
//   password: "1014181",
//   database: "jlemie03"
// });

// connection.connect(function(err, result) {

//   if(err){ 
//     return;
//   }
//   console.log("Connected to DB");

//   //Making a table in the new database
//   var sql = "CREATE TABLE FILE (cal_id INT AUTO_INCREMENT PRIMARY KEY, file_name VARCHAR(60) NOT NULL, version INT NOT NULL, prod_id VARCHAR(256) NOT NULL)";
//   var sql2 = "CREATE TABLE EVENT (event_id INT AUTO_INCREMENT PRIMARY KEY, summary VARCHAR(1024), start_time DATETIME NOT NULL, location VARCHAR(60), organizer VARCHAR(256), cal_file INT NOT NULL)";
//   var sql3 = "CREATE TABLE ALARM (alarm_id INT AUTO_INCREMENT PRIMARY KEY, action VARCHAR(256) NOT NULL, trigger VARCHAR(256) NOT NULL, event INT NOT NULL FOREIGN KEY(event) REFERENCES EVENT(event_id) ON DELETE CASCADE)";
//   con.query(sql, function(err, result) {
//     if(err){ console.log("FILE TABLE ALREADY EXISTS!"); return;}
//     console.log("Table FILE Created");
//   });

//   con.query(sql2, function(err, result) {
//     if(err){ console.log("FILE EVENT ALREADY EXISTS!"); return;}
//     console.log("Table EVENT Created");
//   });

//   con.query(sql3, function(err, result) {
//     if(err){ console.log("FILE ALARM ALREADY EXISTS!"); return;}
//     console.log("Table ALARM Created");
//   });
// });


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
});


app.get('/dbSaveFiles', function(req, res) {

  console.log("Starting to save files to the db");

  for(var i = 0;i<fileListObj.length;i++) {
    console.log("Before the SQL " + fileListObj[i]);
    var jObj = JSON.parse(fileListObj[i]);
    var fileName = jObj.fileName;
    var stringSQLQuery = fileLogToSQL(jObj);

    var checkDupe = "SELECT * FROM FILE WHERE file_name='" + fileName + "'";
    connection.query(checkDupe, function(err,result, fields) {
      if(err) {
        console.log("Something went wrong! on dupe");
      } else {
        if(result.length == 0) {
          console.log("The record was not found");
        }
      }
    });


    connection.query(stringSQLQuery, function(err, rows, fields) {
      if(err) {
        console.log("Something went wrong!");
      }else {
        console.log("The table was made successfully");
      }
    })
  }


  //res.send(fileListObj);
});


app.get('/dbClearFiles', function(req, res) {
  var deleteFileTable = "DELETE FROM FILE";

  var tableSize = "SELECT COUNT(*) FROM FILE";

  connection.query(tableSize, function(err,result, fields) {
    if(err) {
      console.log("something went wrong");
    } else {
      console.log(result);
    }
  });


  connection.query(deleteFileTable, function(err, rows, fields) {
    if(err) {
      console.log("Something went wrong!");
    } else {
      console.log("Tabled cleared on success");
    }
  });


  connection.query(tableSize, function(err,result, fields) {
    if(err) {
      console.log("something went wrong");
    } else {
      console.log(result);
    }
  });

});


app.listen(portNum);
console.log('Running app at localhost: ' + portNum);
