function darkmode() {
    // Get the checkbox
    var checkBox = document.getElementById("darkmodeCheck");

    // If the checkbox is checked, turns on dark mode
    if (checkBox.checked == true){
        var element = document.body;
        element.classList.toggle("dark-mode");
    } else {
        var element = document.body;
        element.classList.toggle("dark-mode");
    }
} 