//let theme = localStorage.getItem('data-theme');
//const changeThemeToDark = () => {
//    document.documentElement.setAttribute("data-theme", "dark") // set theme to dark
//    localStorage.setItem("data-theme", "dark") // save theme to local storage
//}
//
//const changeThemeToLight = () => {
//    document.documentElement.setAttribute("data-theme", "light") // set theme light
//    localStorage.setItem("data-theme", 'light') // save theme to local storage
//}
//
////// Get the element based on ID
//const checkbox = document.getElementById("switch");
////// Apply retrived them to the website
//checkbox.addEventListener('change', () => {
//    let theme = localStorage.getItem('data-theme'); // Retrieve saved them from local storage
//    if (theme ==='dark'){
//        changeThemeToLight()
//    }else{
//        changeThemeToDark()
//    }   
//});
/**
 * Created with JetBrains WebStorm.
 * User: ynonperek
 * Date: 9/23/12
 * Time: 9:42 AM
 * To change this template use File | Settings | File Templates.
 */


function visited() {
    var visits = Number( localStorage.getItem('visits') );
    if ( visits != null ) {
        visits += 1;
    } else {
        visits = 0;
    }

    localStorage.setItem('visits', visits );
}

function restoreData() {
    return localStorage.getItem('visits');
}

function clearData() {
    localStorage.clear();
}

visited();

var visits = restoreData();
document.querySelector('.visits').innerHTML = visits;

var clear_btn = document.querySelector('#btn-clear');

clear_btn.addEventListener('click', clearData );​