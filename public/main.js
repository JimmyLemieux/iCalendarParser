
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
        url: "obj",
        dataType: 'json',
        async: false,
        success: function (data) {
            for (var i = 0; i < data.length; i++) {
                var retData = JSON.parse(data[i]);
                if (!retData["isValid"]) continue;
                $("#file-table-contents").append("<tr><th scope=\"row\"><a href=\"uploads/" + retData["fileName"] + "\">" + retData["fileName"] + "</a></th><td>" + retData["version"] + "</td><td>" + retData['prodID'] + "</td><td>" + retData['numProps'] + "</td><td>" + retData['numEvents'] + "</td></tr>");
                $(".dropdown-menu").append("<p class=\"item\">" + retData["fileName"] + "</p>");
            }
        }
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

        $(this).find("#fileNameInput").val("");
        $(this).find("#versionInput").val("");
        $(this).find("#idInput").val("");
        $(this).find("#uidInput").val("");
        $(this).find("#startDateInput").val("");
        $(this).find("#startDateTimeInput").val("");
        $(this).find("#createDate").val("");
        $(this).find("#createDateTime").val("");

        //Make an ajax call to the server to add this to the event

        if(!fileName || !calVersion || !prodIDString || !uidInput || !eventDate || !eventTime || !createDate || !createTime) {
            console.log("Invalid fields");
            return;
        }

        //We need to validate the strings so that they are valid
        if(!fileName.includes(".ics") ) {
            console.log("This is an invalid File");
        } else if(fileName.includes(".ics") && fileName.length > 4) {
            //This is a good calendar so you want to make a JSON string and push
            //Make a function for check version and update the functions for the checkDate and checkTime
            var version = parseInt(calVersion, 10);
            if(!checkDate(eventDate) || !checkDate(createDate) || !checkTime(eventTime) || !checkTime(createTime)) {
                return;
            }


            var calJSON = [];
            var fileJSON = {"fileName": fileName};
            calJSON.push(fileJSON);

            var calJSONObj = {"version": version,"prodID": prodIDString};
            calJSON.push(calJSONObj);


            var calJSONEventObj = {"uid":uidInput,"dateStartDate":eventDate,"dateStartTime":eventTime, "dateCreateDate":createDate, "dateCreateTime":createTime};
            calJSON.push(calJSONEventObj);
            console.log(calJSON);

            $.ajax({
                type:'post',
                dataType: 'json',
                contentType: "application/json",
                data: JSON.stringify(calJSON),
                url: "/createCalendar",
                success: function(msg) {
                    console.log(msg.error);
                },
                fail: function(error) {
                    console.log(error);
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


        $(this).find("#fileNameInput").val("");
        $(this).find("#UIDEventInput").val("");
        $(this).find("#eventStartDateInput").val("");
        $(this).find("#eventStartTimeInput").val("");
        $(this).find("#eventCreateDate").val("");
        $(this).find("#eventCreateTime").val("");
        $(this).find("#eventSummary").val("");
    
        if(!fileName || !uid || !eventStartDate || !eventStartTime || !eventCreateDate || !eventCreateTime) {
            console.log("Invalid Event Fields!");
            return;
        }

        if(!fileName.includes(".ics")) {
            console.log("The file does not include the .ics");
            return; 
        } else if(fileName.includes(".ics") && fileName.length > 4) {

            if(!checkDate(eventStartDate) || !checkTime(eventStartTime) || !checkDate(eventCreateDate) || !checkTime(eventCreateTime)) {
                return;
            }


            //The json data
            eventJSON = [];
            var fileJSON = {"fileName":fileName};
            eventJSON.push(fileJSON);
            var eJSON = {"uid":uid,"dateStartDate":eventStartDate,"dateStartTime":eventStartTime,"dateCreateDate":eventCreateDate,"dateCreateTime":eventCreateTime,"summary":summaryValue};
            eventJSON.push(eJSON);
            $.ajax({
                type:'post',
                dataType:'json',
                contentType:'application/json',
                data: JSON.stringify(eventJSON),
                url: '/createEvent',
                success: function(msg) {
                    console.log(msg);
                }
            });
        }

    });


    $("#event-form").submit(function(event) {
        event.preventDefault();
        console.log("The event form was submitted!");
    });
});