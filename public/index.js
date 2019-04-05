
function checkDate(date) {
    console.log("Checking the date");
    if(date.length != 8){
        return false;
    }
    if(isNaN(date)) {
        console.log("This is not an integer");
        return false;
    }
    console.log(date);
    return true;
}

function checkTime(time) {
    console.log("Checking the time");
    if(time.length != 6) {
        return false;
    }
    if(isNaN(time)) {
        console.log("This is not an integer");
        return false;
    }
    console.log(time);
    return true;
}

function checkVersion(version) {
    console.log("Checking the version here");
    return  Number.isInteger(version);
}

$(document).ready(function () {

    //Declare a function here that will be called
    //This is to make the logo increase
    var homeImage = $("#home-img");
    var navObjs = [];


    $(".col").find("#nav-item").each(function () {
        navObjs.push({
            objRef: $(this),
            width: $(this).width(),
            height: $(this).height()
        });
    });

    navObjs.forEach(element => {
        var navItemObj = element.objRef;
        navItemObj.on("mouseenter", function () {
            $(this).animate({ fontSize: 25 }, 100);
        });
        navItemObj.on("mouseleave", function () {
            $(this).animate({ fontSize: 20 }, 100);
        });
    });

    var ht = homeImage.height();
    var wd = homeImage.width();
    var mult = 1.4;
    homeImage.on("mouseenter", function () {
        $(this).animate({ height: ht * mult, width: wd * mult }, 200);
    });
    homeImage.on("mouseleave", function () {
        $(this).animate({ height: ht, width: wd }, 100);
    });

    $.ajax({
        url: "obj/",
        dataType: 'json',
        async: false,
        success: function (data) {
            $("#file-table-contents").empty();
            for (var i = 0; i < data.length; i++) {
                var retData = JSON.parse(data[i]);
                if (!retData["isValid"]) continue;
                $("#file-table-contents").append("<tr><th scope=\"row\"><a href=\"uploads/" + retData["fileName"] + "\">" + retData["fileName"] + "</a></th><td>" + retData["version"] + "</td><td>" + retData['prodID'] + "</td><td>" + retData['numEvents'] + "</td><td>" + retData['numProps'] + "</td></tr>");
                $("#file-list").append("<p class=\"item\">" + retData["fileName"] + "</p>");
                $("#event-files").append("<p class=\"event-drop-item\">" + retData["fileName"] + "</p>");
            }
        }
    });

    $("#clear-status-panel").on("click", function(event) {
        event.preventDefault();
        $("#status-contents").empty();
        $("#status-contents").append("<tr><td>CLEARED</td></tr>");
        
    });


    //The upload for the client to the server

    $("#uploadForm").submit(function(event) {
        event.preventDefault();
        var uploadForm = $("#uploadForm")[0];
        var formData = new FormData(uploadForm);
        $.ajax({
            url: "/upload",
            type: 'post',
            contentType: false,
            processData: false,
            data: formData,
            success: function(msg) {
                console.log(msg.error);
                $("#status-contents").append("<tr><td>" + msg.error +"</td></tr>");
                $.ajax({
                    url: "obj/",
                    dataType: 'json',
                    async: false,
                    success: function (data) {
                        $("#file-table-contents").empty();
                        $("#file-list").empty();
                        $("#event-files").empty();
                        for (var i = 0; i < data.length; i++) {
                            var retData = JSON.parse(data[i]);
                            if (!retData["isValid"]) continue;
                            $("#file-table-contents").append("<tr><th scope=\"row\"><a href=\"uploads/" + retData["fileName"] + "\">" + retData["fileName"] + "</a></th><td>" + retData["version"] + "</td><td>" + retData['prodID'] + "</td><td>" + retData['numEvents'] + "</td><td>" + retData['numProps'] + "</td></tr>");
                            $("#file-list").append("<p class=\"item\">" + retData["fileName"] + "</p>");
                            $("#event-files").append("<p class=\"event-drop-item\">" + retData["fileName"] + "</p>");
                        }

                                                //For the drop down table here
                        $(".dropdown-menu").find(".item").each(function () {
                            $(this).on("click", function () {
                                var newTitle = $(this).text();
                                //We are going to parse the event list contents and then put them into the table
                                // Make an ajax call to our parser and then do what you do
                                let url = "eventList/" + newTitle;
                                $.ajax({
                                    url: url,
                                    dataType: 'json',
                                    async: false,
                                    success: function (data) {
                                        $("#calendar-table-contents").empty(); // Clear the table of its contents
                                        for (var i = 0; i < data.length; i++) {
                                            $("#calendar-table-contents").append("<tr><td>" + (i + 1) + "</th><td>" + data[i]["startDT"]["date"] + "</td><td>" + data[i]["startDT"]["time"] + "</td><td>" + data[i]["summary"] + "</td><td>" + data[i]["numProps"] + "</td><td>" + data[i]["numAlarms"] + "</td></tr>"); // Add the content we will get from the server and parsing
                                        }
                                    }
                                });

                                var alarmURL = "alarmList/" + newTitle;
                                $.ajax({
                                    url: alarmURL,
                                    dataType: 'json',
                                    async: false,
                                    success: function (data) {
                                        $("#event-alarm-table-contents").empty();
                                        for (var i = 0; i < data.length; i++) {
                                            $("#event-alarm-table-contents").append("<tr><td>" + data[i]["event"] + "</th><td>" + data[i]["action"] + "</td><td>" + data[i]["trigger"] + "</td><td>" + data[i]["numProps"] + "</td></tr>");
                                        }
                                        console.log(data);
                                    }
                                });

                                var propURL = "propList/" + newTitle;
                                $.ajax({
                                    url: propURL,
                                    dataType: 'json',
                                    async: false,
                                    success: function (data) {
                                        $("#event-properties-table-contents").empty();
                                        for (var i = 0; i < data.length; i++) {
                                            $("#event-properties-table-contents").append("<tr><td>" + data[i]["event"] + "</th><td>" + data[i]["name"] + "</td><td>" + data[i]["description"] + "</td></tr>");
                                        }
                                        console.log(data);
                                    }
                                });

                                $("#drop-title").text(newTitle, function () {
                                    $(".calendar-table-view").find("#main-event-table").show();
                                });
                            }); 
                        });

                        $("#event-files").find(".event-drop-item").each(function() {
                            $(this).on("click", function() {
                                var eventTitle = $(this).text();
                                console.log(eventTitle);
                                //Place this in the ent title form field
                                $(this).val(eventTitle);
                                $("#event-form").find("#fileNameInput").val(eventTitle);
                    
                            }); 
                    
                        });
                    }
                });
            }
        });
    });
    //For the drop down table here
    $(".dropdown-menu").find(".item").each(function () {
        $(this).on("click", function () {
            var newTitle = $(this).text();
            //We are going to parse the event list contents and then put them into the table
            // Make an ajax call to our parser and then do what you do
            let url = "eventList/" + newTitle;
            $.ajax({
                url: url,
                dataType: 'json',
                async: false,
                success: function (data) {
                    $("#calendar-table-contents").empty(); // Clear the table of its contents
                    for (var i = 0; i < data.length; i++) {
                        $("#calendar-table-contents").append("<tr><td>" + (i + 1) + "</th><td>" + data[i]["startDT"]["date"] + "</td><td>" + data[i]["startDT"]["time"] + "</td><td>" + data[i]["summary"] + "</td><td>" + data[i]["numProps"] + "</td><td>" + data[i]["numAlarms"] + "</td></tr>"); // Add the content we will get from the server and parsing
                    }
                }
            });

            var alarmURL = "alarmList/" + newTitle;
            $.ajax({
                url: alarmURL,
                dataType: 'json',
                async: false,
                success: function (data) {
                    $("#event-alarm-table-contents").empty();
                    for (var i = 0; i < data.length; i++) {
                        $("#event-alarm-table-contents").append("<tr><td>" + data[i]["event"] + "</th><td>" + data[i]["action"] + "</td><td>" + data[i]["trigger"] + "</td><td>" + data[i]["numProps"] + "</td></tr>");
                    }
                    console.log(data);
                }
            });

            var propURL = "propList/" + newTitle;
            $.ajax({
                url: propURL,
                dataType: 'json',
                async: false,
                success: function (data) {
                    $("#event-properties-table-contents").empty();
                    for (var i = 0; i < data.length; i++) {
                        $("#event-properties-table-contents").append("<tr><td>" + data[i]["event"] + "</th><td>" + data[i]["name"] + "</td><td>" + data[i]["description"] + "</td></tr>");
                    }
                    console.log(data);
                }
            });

            $("#drop-title").text(newTitle, function () {
                $(".calendar-table-view").find("#main-event-table").show();
            });
        }); 
    });

    $("#uploadForm").submit(function(event) {
        event.preventDefault();
        console.log("This was called!");
        var n = $("input[type=file]")[0].files[0].name;
    });

    // Find all of the nav tabs and apply an action to them
    $(".nav").find(".nav-item").each(function () {
        $(this).on("click", function () {
            //Find all of the tables in the calendar-view-div
            var id = $(this).find("a").attr("href");
            id = id.substring(1);
            $(".calendar-table-view").find("div").each(function () {
                var divID = $(this).attr("id");
                if (id == divID) {
                    $(this).show();
                } else {
                    $(this).hide();
                }
            });
        });
    });


    //The dropdown part for the createEvent Section

    $("#event-files").find(".event-drop-item").each(function() {
        $(this).on("click", function() {
            var eventTitle = $(this).text();
            console.log(eventTitle);
            //Place this in the ent title form field
            $(this).val(eventTitle);
            $("#event-form").find("#fileNameInput").val(eventTitle);

        }); 

    });

    $("#calendar-form").submit(function(event) {
        //Get the contents of all of the input fields here
        //Clear all of the input fields here
        event.preventDefault();
        var fileName = $(this).find("#fileNameInput").val();
        var calVersion = $(this).find("#versionInput").val();
        var prodIDString = $(this).find("#idInput").val();
        var uidInput = $(this).find("#uidInput").val();
        var eventDate = $(this).find("#startDateInput").val();
        var eventTime = $(this).find("#startDateTimeInput").val();
        var createDate = $(this).find("#createDate").val();
        var createTime = $(this).find("#createDateTime").val();

        var isUTCStart = $(this).find("#utcStart").is(":checked");
        var isUTCCreate = $(this).find("#utcCreate").is(":checked");
        var dateReg = /-/g;
        var timeReg = /:/g; 

        eventDate = eventDate.replace(dateReg, '');
        eventTime = eventTime.replace(timeReg, '');
        createDate = createDate.replace(dateReg, '');
        createTime = createTime.replace(timeReg,'');
        eventTime += "00";
        createTime += "00";
        console.log(eventDate);
        console.log(eventTime);


        $(this).find("#fileNameInput").val("");
        $(this).find("#versionInput").val("");
        $(this).find("#idInput").val("");
        $(this).find("#uidInput").val("");
        $(this).find("#startDateInput").val("");
        $(this).find("#startDateTimeInput").val("");
        $(this).find("#createDate").val("");
        $(this).find("#createDateTime").val("");

        //Make an ajax call to the server to add this to the event

        //We need to validate the strings so that they are valid
        if(!fileName.includes(".ics") ) {
            console.log("This is an invalid File");
            $("#status-contents").append("<tr><td>The file name"+ fileName +" is invalid!</td></tr>");
        } else if(fileName.includes(".ics") && fileName.length > 4) {
            //This is a good calendar so you want to make a JSON string and push
            //Make a function for check version and update the functions for the checkDate and checkTime
            var version = parseInt(calVersion, 10);

            var calJSON = [];
            var fileJSON = {"fileName": fileName};
            calJSON.push(fileJSON);

            var calJSONObj = {"version": version,"prodID": prodIDString};
            calJSON.push(calJSONObj);


            var calJSONEventObj = {"uid":uidInput,"dateStartDate":eventDate,"dateStartTime":eventTime,"dateStartUTC":isUTCStart, "dateCreateDate":createDate, "dateCreateTime":createTime,"dateCreateUTC":isUTCCreate};
            calJSON.push(calJSONEventObj);
            console.log(calJSON);

            $.ajax({
                type:'post',
                dataType: 'json',
                contentType: "application/json",
                data: JSON.stringify(calJSON),
                url: "/createCalendar",
                success: function(msg) {
                    $("#status-contents").append("<tr><td>The server returned " + msg.error + " on Create Calendar of fileName " + fileName +"</td></tr>");
                    $.ajax({
                        url: "obj/",
                        dataType: 'json',
                        async: false,
                        success: function (data) {
                            $("file-table-contents").empty();
                            for (var i = 0; i < data.length; i++) {
                                var retData = JSON.parse(data[i]);
                                if (!retData["isValid"]) continue;
                                $("#file-table-contents").append("<tr><th scope=\"row\"><a href=\"uploads/" + retData["fileName"] + "\">" + retData["fileName"] + "</a></th><td>" + retData["version"] + "</td><td>" + retData['prodID'] + "</td><td>" + retData['numProps'] + "</td><td>" + retData['numEvents'] + "</td></tr>");
                                $(".dropdown-menu").append("<p class=\"item\">" + retData["fileName"] + "</p>");
                            }
                        }
                    });
                },
                fail: function(error) {
                    console.log(error);
                    $("#status-contents").append("<tr><td>The server returned an error</td></tr>");

                }
            });

        } 
        console.log("The calendar form was submitted!");
    });


    $("#event-form").submit(function(event) {
        event.preventDefault();
        console.log("The event was submitted");

        var fileName = $(this).find("#fileNameInput").val();
        var uid = $(this).find("#UIDEventInput").val();
        var eventStartDate = $(this).find("#eventStartDateInput").val();
        var eventStartTime = $(this).find("#eventStartTimeInput").val();
        var eventCreateDate = $(this).find("#eventCreateDate").val();
        var eventCreateTime = $(this).find("#eventCreateTime").val();
        var summaryValue = $(this).find("#eventSummary").val();
        var utcStart = $(this).find("#utcStartEvent").is(":checked");
        var utcCreate = $(this).find("#utcCreateEvent").is(":checked");

        var dateReg = /-/g;
        var timeReg = /:/g;

        eventStartDate = eventStartDate.replace(dateReg, ''); 
        eventCreateDate = eventCreateDate.replace(dateReg, '');
        eventStartTime = eventStartTime.replace(timeReg,'');
        eventStartTime += "00";
        eventCreateTime = eventCreateTime.replace(dateReg, '');
        eventCreateTime += "00";

        $(this).find("#fileNameInput").val("");
        $(this).find("#UIDEventInput").val("");
        $(this).find("#eventStartDateInput").val("");
        $(this).find("#eventStartTimeInput").val("");
        $(this).find("#eventCreateDate").val("");
        $(this).find("#eventCreateTime").val("");
        $(this).find("#eventSummary").val("");
    
        if(!fileName || !uid || !eventStartDate || !eventStartTime || !eventCreateDate || !eventCreateTime) {
            console.log("Invalid Event Fields!");
            $("#status-contents").append("<tr><td>There is invalid event fields!</td></tr>");

            return;
        }
        if(!fileName.includes(".ics")) {
            console.log("The file does not include the .ics");
            $("#status-contents").append("<tr><td>The file name"+ fileName +" is invalid!</td></tr>");
            return; 
        } else if(fileName.includes(".ics") && fileName.length > 4) {

            //The json data
            eventJSON = [];
            var fileJSON = {"fileName":fileName};
            eventJSON.push(fileJSON);
            var eJSON = {"uid":uid,"dateStartDate":eventStartDate,"dateStartTime":eventStartTime,"dateStartUTC":utcStart,"dateCreateDate":eventCreateDate,"dateCreateTime":eventCreateTime,"dateCreateUTC":utcCreate,"summary":summaryValue};
            eventJSON.push(eJSON);
            $.ajax({
                type:'post',
                dataType:'json',
                contentType:'application/json',
                data: JSON.stringify(eventJSON),
                url: '/createEvent',
                success: function(msg) {
                    console.log(msg);
                },
                fail: function(error) {
                    console.log(error);
                    $("#status-contents").append("<tr><td>The server returned an error</td></tr>");
                }
            });
        }

    });


    $("#event-form").submit(function(event) {
        event.preventDefault();
        console.log("The event form was submitted!");
    });

    //****Starting the database section
    //Hide the query section initially
    $('.database-functions').hide();

    //Handle the login form to make the database credentials

    $('.login-form').submit(function(e) {
        e.preventDefault();
        var userName = $(this).find('#userNameInput').val();
        var passWord = $(this).find("#passWordInput").val();
        var databaseName = $(this).find("#databaseName").val();
        $(this).find("#userNameInput").val("");
        $(this).find("#passWordInput").val("");
        $(this).find("#databaseName").val("");
        var data = {
            user: userName,
            pass: passWord,
            dbName: databaseName
        }

        $(".console-output").append("<medium style=\"color:black;\"> Attempting login as " + data.user + " one moment..</medium></br>");

        $.ajax({
            type: 'get',
            dataType: 'json',
            url: '/loginDatabase',
            data: data,
            async: false,
            success: function(resp) {
                console.log(resp); //json
                if(resp.error == "SUCCESS") {
                    $(".console-output").append("<medium style=\"color:green;\"> Successful login as " + data.user + " you can now interact with DB..</medium></br>");
                    $('.database-functions').fadeIn();
                } else {
                    $(".console-output").append("<medium style=\"color:red;\"> Unsuccessful login as " + data.user + " you need to enter correct DB information..</medium></br>");
                }
            }
        })
    });


    //The button that will submit all of the forms
    $(".main-functions").find("#store-files-btn").on('click', function(e) {
        e.preventDefault();
        $.ajax({
            type: 'get',
            url: '/dbClearFiles',
            async: false,
            success: function(data) {
                $.ajax({
                    type: 'get',
                    url: '/dbSaveFiles',
                    success: function(data) {
                        console.log(data);
                    }
                });
            }
        }) 
    });


    $(".main-functions").find("#clear-db-files").on('click', function(e) {
        e.preventDefault();
        $.ajax({
            type: 'get',
            url: '/dbClearFiles',
            async: false,
            success: function(data) {
                console.log(data);
                $(".console-output").append("<medium style=\"color:green;\">Deleting All DB Contents</medium></br>");
                $(".console-output").append("<medium style=\"color:black;\"> FILE: " + data.FILE + ", EVENT: " + data.EVENT + ", ALARM: " + data.ALARM + "</medium></br>");
                console.log("DB Cleared");
            }
        });
    });

    $(".main-functions").find("#get-db-status").on('click', function(e) {
        e.preventDefault();
        console.log("Clicked!");
        $.ajax({
            type: 'get',
            url: '/getDBStatus',
            async: false,
            success: function(data) {
                console.log("Got the status");
                console.log(data);
                $(".console-output").append("<medium style=\"color:black;\">Fetching DB status..</medium></br>");
                $(".console-output").append("<medium style=\"color:black;\">Number of files: " + data.fileCount + ", Number of Events: " + data.eventCount + ", Number of Alarms: " + data.alarmCount + "</medium></br>");

            }
        });
    });

    $(".main-functions").find("#get-sort-events").on('click', function(e) {
        e.preventDefault();
        console.log("Clicked the sorted events");
        $.ajax({
            type: 'get',
            url: '/getSortedEvents',
            success: function(data) {
                console.log(data);
                $(".console-output").append("<medium style=\"color:black;\">-----BEGIN QUERY-----</medium></br>");
                if(data.length == 0) {
                    $(".console-output").append("<medium style=\"color:black;\">-----THERE IS NOTHING TO QUERY-----</medium></br>");
                }
                for(var i = 0;i<data.length;i++) {
                    var startTime = data[i].startTime; 
                    var location = data[i].location;
                    var organizer = data[i].organizer;
                    var summary = data[i].summary;
                    $(".console-output").append("<medium style=\"color:black;\">-----BEGIN EVENT-----</medium></br>");
                    $(".console-output").append("<medium style=\"color:black;\">Event StartTime: <b>" + startTime + "</b></br> Event Location: <b>" + location + "</b></br> Organizer <b>" + organizer + "</b></br> Summary: <b>" + summary + " </b></medium></br>");
                    $(".console-output").append("<medium style=\"color:black;\">-----END EVENT-----</medium></br>");
                }
                $(".console-output").append("<medium style=\"color:black;\">-----END QUERY-----</medium></br>");
            }
        });

    });

    //query to get events from a specific file
    $(".main-functions").find("#get-file-events").on('click', function(e) {
        e.preventDefault();
        var fileNameInput = $("#file-event-input").val();
        $("#file-event-input").val("");
        console.log(fileNameInput);
        console.log("Clicked the file events");
        var obj = {
            fileName: fileNameInput
        }
        $.ajax({
            type: 'get',
            dataType: 'json',
            url: "/getFileEvents",
            data: obj,
            success: function(data) {
                console.log(data);
                $(".console-output").append("<medium style=\"color:black;\">-----BEGIN QUERY-----</medium></br>");
                if(data.length == 0) {
                    $(".console-output").append("<medium style=\"color:black;\">-----THERE IS NOTHING TO QUERY OR FILE NOT FOUND!-----</medium></br>");
                }
                for(var i = 0;i<data.length;i++) {
                    var startDate = data[i].startDate; 
                    var summary = data[i].summary;
                    $(".console-output").append("<medium style=\"color:black;\">-----BEGIN EVENT-----</medium></br>");
                    $(".console-output").append("<medium style=\"color:black;\">Event StartTime: <b>" + startDate + "</b></br> Summary: <b>" + summary + " </b></medium></br>");
                    $(".console-output").append("<medium style=\"color:black;\">-----END EVENT-----</medium></br>");
                }
                $(".console-output").append("<medium style=\"color:black;\">-----END QUERY-----</medium></br>");
            }
        });

    });



    $(".main-functions").find("#get-conflict-events").on('click', function(e) {
        e.preventDefault();
        console.log("Clicked the conflict events");
        $.ajax({
            type: 'get',
            url: '/getConflictEvents', 
            success: function(data) {
                console.log(data);
                $(".console-output").append("<medium style=\"color:green;\">Displaying Conflict Events..</medium></br>");
                $(".console-output").append("<medium style=\"color:black;\">-----BEGIN QUERY-----</medium></br>");
                if(data.length == 0) {
                    $(".console-output").append("<medium style=\"color:black;\">-----THERE IS NOTHING TO QUERY OR NO CONFLICTS!-----</medium></br>");
                }
                for(var i = 0;i<data.length;i++) {
                    var summary = data[i].summary;
                    var organizer = data[i].organizer;
                    $(".console-output").append("<medium style=\"color:black;\">-----BEGIN EVENT-----</medium></br>");
                    $(".console-output").append("<medium style=\"color:black;\"> Summary: <b>" + summary + " </b> Organizer: <b>" + organizer + "</b></medium></br>");
                    $(".console-output").append("<medium style=\"color:black;\">-----END EVENT-----</medium></br>");
                }
                $(".console-output").append("<medium style=\"color:black;\">-----END QUERY-----</medium></br>");
                $(".console-output").append("<medium style=\"color:green;\">Done..</medium></br>");

            }
        });
    });





    //***Ending the database section
});