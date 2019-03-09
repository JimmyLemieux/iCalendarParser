 'use strict'

 const ffi = require("ffi"); //This is so the C functions can connect to our library

 const express = require("express"); // Express library
 const app = express();

 const path = require("path"); //This is the path utility library
 const fileUpload = require("express-fileupload"); //File upload library

 app.use(fileUpload());
 app.use(express.static(path.join(__dirname, 'public')));
 app.use(express.static(path.join(__dirname, 'assets')));


//File reading and manipulating enviornment
 const fs = require("fs");

 //JavaScript obfucator
 const JavaScriptObfuscator = require("javascript-obfuscator");


 app.get('/', function(req, res) {
    res.sendFile(__dirname + '/public/index.html');
 });

 app.listen(32629, function(error) {
    console.log("Running on PORT 32629!");
 });