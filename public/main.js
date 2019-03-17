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


    $.ajax({url: "http://localhost:32629/obj",
        dataType: 'json',
        success: function(data) {
            for(var i = 0;i<data.length;i++) {
                if(data[i] == "{}")continue;
                var retData = JSON.parse(data[i]);
                $("#file-table-contents").append("<tr><th scope=\"row\">" + "test" + "</th><td>"+ retData["version"] + "</td><td>" + retData['prodID'] + "</td><td>" +retData['numProps']+"</td><td>" + retData['numEvents'] + "</td></tr>")  
            }
        }
    });


    $("#uploadForm").on('submit', function(e) {
        $.ajax({url: "http://localhost:32629/upload",
            type: "POST",
            success: function(data, textStatus, xhr) {
                console.log(textStatus);
            }
        });

        console.log("Here"); 
    });

    //File drop down title changer
    // Populate the drop down menu here

    for(var i = 0;i<fileNames.length;i++) {
        $("#file-list").append("<p class=\"item\">Here</p>");
    }

    //For the drop down table here
    $(".dropdown-menu").find(".item").each(function() {
        $(this).on("click", function() {
            var newTitle = $(this).text();
            $("#drop-title").text(newTitle);
            $("#calendar-table-contents").empty(); // Clear the table of its contents
            $("#calendar-table-contents").append("<tr><td>" + newTitle +"</th><td>here</td><td>SOME ID</td><td>2</td><td>10</td><td>2</td></tr>"); // Add the content we will get from the server and parsing
        });
    });

    // Adding a populater for the calendar table
    //This should actually be null at first
    for(var i = 0;i<2;i++) {
        $("#calendar-table-contents").append("<tr><td>2</th><td>Here</td><td>SOME ID</td><td>2</td><td>10</td><td>2</td></tr>");
    }
});