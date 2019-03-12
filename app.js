 'use strict'

 const ffi = require("ffi"); //This is so the C functions can connect to our library

 const express = require("express"); // Express library
 const app = express();

 const path = require("path"); //This is the path utility library
 const fileUpload = require("express-fileupload"); //File upload library

 app.use(fileUpload());
 app.use(express.static(path.join(__dirname, 'public')));
 app.use(express.static(path.join(__dirname, 'assets')));
 app.use(express.static(path.join(__dirname, 'uploads')));
//File reading and manipulating enviornment
 const fs = require("fs");

 //JavaScript obfucator
 const JavaScriptObfuscator = require("javascript-obfuscator");

 // Important, pass in port as in `npm run dev 1234`, do not change
 const portNum = process.argv[2];


 // Obfuscating the javascript code
 // Send obfuscated JS, do not change
 
 //As of right now this is not working too proper, I think it has something to do with the file paths or something like that
 //There isn't that much that is going on here
app.get('/main.js',function(req,res){
   fs.readFile(path.join(__dirname+'/public/main.js'), 'utf8', function(err, contents) {
      console.log(__dirname + '/public/main.js');
     const minimizedContents = JavaScriptObfuscator.obfuscate(contents, {compact: true, controlFlowFlattening: true});
     res.contentType('application/javascript');
     res.send(minimizedContents._obfuscatedCode);
   });
   console.log("This should return obfuscated javscript!");
 });



 //Respond to POST requests that upload files to uploads/ directory
 app.post('/upload', function(req, res) {
    console.log("The file was uploaded");
   if(!req.files) {
     return res.status(400).send('No files were uploaded.');
   }
  
   let uploadFile = req.files.someUpload;
   // Use the mv() method to place the file somewhere on your server
   uploadFile.mv('uploads/' + uploadFile.name, function(err) { // This will move the file to the server
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
       res.sendFile(path.join(__dirname+'/uploads/' + req.params.name)); // This is actually downloading the file
     } else {
       res.send(''); //If there is an error
     }
   });
 });



 app.get('/', function(req, res) {
    res.sendFile(__dirname + '/public/index.html');
 });

 app.get("/something", function(req, res) {
   res.send("This is an end point");
 });

 app.listen(32629, function(error) {
    console.log("Running on PORT 32629!");
 });