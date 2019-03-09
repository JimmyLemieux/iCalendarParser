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

    var fileNames = ["Another.ics", "AnotherFile.ics", "SomeThing.ics", "MegaFile.ics", "HereIsAnotherFile.ics"];

    var fileVersions = [2.0, 5.1, 69.9, 23.2,12.2];

    // So far this is working as expected, this will later use ajax calls to get JSON from my parser
    for(var i = 0;i<500;i++) {
        $("#file-table-contents").append("<tr><th scope=\"row\">" + fileNames[0] + "</th><td>"+ fileVersions[0] + "</td><td>SOME ID</td><td>2</td><td>100000000000090909090990909090909090909090990909</td></tr>");
        console.log("Added to the table!");
    }


    //Button action for the file view panel
    $("#upload-file-btn").click(function(event) {
        event.preventDefault();
        console.log("The Event was called here!");
    });

    //File drop down title changer

    // Populate the drop down menu here

    for(var i = 0;i<fileNames.length;i++) {
        $(".dropdown-menu").append("<p class=\"item\">" + fileNames[i] + "</p>");
        console.log("Called");
    }

    $(".dropdown-menu").find(".item").each(function(element) {
        $(this).on("click", function() {
            var newTitle = $(this).text();
            $("#drop-title").text(newTitle);
        });
    });


    // var menuItems = [];
    // $(".dropdown-menu").find("#drop-item").each(function() {
    //     console.log($(this).text());
    //     menuItems.push($(this));
    // });

    // console.log(menuItems.length);

    // menuItems.forEach(element => {
    //     element.on("click", function() {
    //         console.log("Element was clicked!");
    //     });
    // });

});