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
        console.log(element.width + " " + element.height);
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

    //Start to fill in the array values to the table
    //Some testing data will be used for inserting into the table

    var fileNames = [];
    // So far this is working as expected, this will later use ajax calls to get JSON from my parser
    // $.ajax({url: "http://localhost:32628/fileNames",
    //     dataTyoe: 'json',
    //     success: function(data) {
    //         for(var i = 0;i<data.length;i++) {
    //             // console.log(data[i]);
    //         }
    //     }
    // });


    $.ajax({url: "http://localhost:8800/obj",
        dataType: 'json',
        async: false,
        success: function(data) {
            for(var i = 0;i<data.length;i++) {
                console.log(data[i]);
                var retData = JSON.parse(data[i]);
                if(!retData["isValid"]) continue;
                $("#file-table-contents").append("<tr><th scope=\"row\"><a href=\"http://localhost:8800/uploads/" + retData["fileName"] + "\">" + retData["fileName"] + "</a></th><td>"+ retData["version"] + "</td><td>" + retData['prodID'] + "</td><td>" +retData['numProps']+"</td><td>" + retData['numEvents'] + "</td></tr>");
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
            let url = "http://localhost:8800/eventList/" + newTitle;
            $.ajax({
                url: url,
                dataType: 'json',
                async: false,
                success: function(data) {
                    console.log(data);
                    $("#calendar-table-contents").empty(); // Clear the table of its contents
                    for(var i = 0;i<data.length;i++) {
                        $("#calendar-table-contents").append("<tr><td>" + (i+1) +"</th><td>" + data[i]["startDT"]["date"] + "</td><td>" + data[i]["startDT"]["time"] +"</td><td>" + data[i]["summary"] + "</td><td>" + data[i]["numProps"] + "</td><td>" + data[i]["numAlarms"] + "</td></tr>"); // Add the content we will get from the server and parsing
                    }

                }
            });
            $("#drop-title").text(newTitle);
        });
    });
});