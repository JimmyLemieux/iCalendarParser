'use strict'

// C library API
const ffi = require('ffi');

// Express App (Routes)
const express = require("express");
const app     = express();
const path    = require("path");
const fileUpload = require('express-fileupload');

app.use(fileUpload());

// Minimization
const fs = require('fs');
const JavaScriptObfuscator = require('javascript-obfuscator');

// Important, pass in port as in `npm run dev 1234`, do not change
const portNum = process.argv[2];

// Send HTML at root, do not change

 app.use(express.static(path.join(__dirname, 'public')));

 app.use(express.static(path.join(__dirname, 'assets')));

 app.use(express.static(path.join(__dirname, 'uploads')));


let sharedLib = ffi.Library('./libcal.dylib', {
  'makeObj' : ['string', ['string']],
  'eventJSONWrapper' : ['string', ['string']],
  'alarmJSONWrapper' : ['string', ['string']] 
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
    return res.status(400).send('No files were uploaded.');
  }
 
  let uploadFile = req.files.uploadFile;
 
  // Use the mv() method to place the file somewhere on your server
  uploadFile.mv('uploads/' + uploadFile.name, function(err) {
    if(err) {
      return res.status(500).send(err);
    }
    res.redirect('/');
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


//Sample endpoint
app.get('/someendpoint', function(req , res){
  res.send({
    foo: "bar"
  });
});

app.listen(portNum);
console.log('Running app at localhost: ' + portNum);
