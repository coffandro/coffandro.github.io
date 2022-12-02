// dark mode detection //
const useDark = window.matchMedia("(prefers-color-scheme: dark)");
function toggleDarkMode(state) {
    document.documentElement.classList.toggle("dark-mode", state);
}
toggleDarkMode(useDark.matches);
// Listening for the changes in the OS settings and auto switching the mode
useDark.addListener((evt) => toggleDarkMode(evt.matches));
// Toggles the "dark-mode" class on click
function DarkBTN() {
    document.documentElement.classList.toggle("dark-mode");
}
// touch navbar detection
if ("ontouchstart" in document.documentElement)
{
  // content for touch-screen (mobile) devices
}
else
{
  // everything else (desktop)
}