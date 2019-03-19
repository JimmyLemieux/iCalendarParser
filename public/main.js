
$(document).ready(function() {
    //This is to make the logo increase
    var homeImage = $("#home-img");
    var navObjs = [];


    $(".col").find("#nav-item").each(function() {
        navObjs.push({
            objRef: $(this),
            width: $(this).width(),
            height: $(this).height()
        });
    });

    navObjs.forEach(element => {
        var navItemObj = element.objRef;
        navItemObj.on("mouseenter", function() {
            $(this).animate({fontSize: 25}, 100);
        });
        navItemObj.on("mouseleave", function() {
            $(this).animate({fontSize: 20}, 100);
        });
    });

    var ht = homeImage.height();
    var wd = homeImage.width();
    var mult = 1.4;
    homeImage.on("mouseenter", function() {
        $(this).animate({height: ht*mult, width: wd*mult}, 200); 
    });
    homeImage.on("mouseleave", function() {
        $(this).animate({height: ht, width: wd}, 100); 
    });

    $.ajax({url: "http://localhost:32629/obj",
        dataType: 'json',
        async: false,
        success: function(data) {
            for(var i = 0;i<data.length;i++) {
                var retData = JSON.parse(data[i]);
                if(!retData["isValid"]) continue;
                $("#file-table-contents").append("<tr><th scope=\"row\"><a href=\"http://localhost:32629/uploads/" + retData["fileName"] + "\">" + retData["fileName"] + "</a></th><td>"+ retData["version"] + "</td><td>" + retData['prodID'] + "</td><td>" +retData['numProps']+"</td><td>" + retData['numEvents'] + "</td></tr>");
                $(".dropdown-menu").append("<p class=\"item\">" + retData["fileName"] + "</p>");
            }
        }
    });

    //For the drop down table here
    $(".dropdown-menu").find(".item").each(function() {
        $(this).on("click", function() {
            var newTitle = $(this).text();
            //We are going to parse the event list contents and then put them into the table
            // Make an ajax call to our parser and then do what you do
            let url = "http://localhost:32629/eventList/" + newTitle;
            $.ajax({
                url: url,
                dataType: 'json',
                async: false,
                success: function(data) {
                    $("#calendar-table-contents").empty(); // Clear the table of its contents
                    for(var i = 0;i<data.length;i++) {
                        $("#calendar-table-contents").append("<tr><td>" + (i+1) +"</th><td>" + data[i]["startDT"]["date"] + "</td><td>" + data[i]["startDT"]["time"] +"</td><td>" + data[i]["summary"] + "</td><td>" + data[i]["numProps"] + "</td><td>" + data[i]["numAlarms"] + "</td></tr>"); // Add the content we will get from the server and parsing
                    }

                }
            });

            var alarmURL = "http://localhost:32629/alarmList/" + newTitle;
            $.ajax({
                url: alarmURL,
                dataType: 'json',
                async: false,
                success: function(data) {
                    //console.log(data);
                }
            });

            console.log("calling props");
            var propURL = "http://localhost:32629/propList/" + newTitle;
            $.ajax({
                url: propURL,
                dataType: 'json',
                async: false,
                success: function(data) {
                    //Now place these into the table
                    $("#event-properties-table-contents").empty();
                    for(var i = 0;i<data.length;i++) {
                        $("#event-properties-table-contents").append("<tr><td>" + data[i]["event"] +"</th><td>" + data[i]["name"] + "</td><td>" + data[i]["description"] +"</td></tr>");
                    }
                    console.log(data);
                }
            });


            $("#drop-title").text(newTitle);
        });
    });


    // Find all of the nav tabs and apply an action to them
    $(".nav").find(".nav-item").each(function() {
        $(this).on("click", function() {
            //Find all of the tables in the calendar-view-div
            var id = $(this).find("a").attr("href");
            id = id.substring(1);
            $(".calendar-table-view").find("div").each(function() {
                var divID =  $(this).attr("id");
                if(id == divID) {
                    $(this).show();
                } else {
                    $(this).hide();
                }
            });
        });
    });
});