let openings = document.getElementById("openings");
let openingsDropDown = document.getElementById("openingsDropDown");
openingsDropDown.style.display = "none";
function showOpenings() {
  openingsDropDown.style.display = "block";
}
function hideOpenings() {
  openingsDropDown.style.display = "none";
}
openings.addEventListener("mouseover", showOpenings);
openings.addEventListener("mouseleave", hideOpenings);
