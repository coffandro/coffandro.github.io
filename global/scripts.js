window.onload=function() {
    var darkMode = localStorage.getItem('darkMode');
    var checkBox = document.getElementById("darkmodeCheck");

    // If the checkbox is checked, turns on dark mode
    if (darkmode == true){
        var element = document.body;
        element.classList.toggle("dark-mode");
        localStorage.setItem('darkMode', 'true');
    } else {
        var element = document.body;
        element.classList.toggle("dark-mode");
        localStorage.setItem('darkMode', 'false');
    }
} 

function darkmode() {
    // Get the checkbox
    var checkBox = document.getElementById("darkmodeCheck");

    // If the checkbox is checked, turns on dark mode
    if (checkBox.checked == true){
        var element = document.body;
        element.classList.toggle("dark-mode");
        localStorage.setItem('darkMode', 'true');
    } else {
        var element = document.body;
        element.classList.toggle("dark-mode");
        localStorage.setItem('darkMode', 'false');
    }
} 