function init() {
  script.log(" - Module initialisé !");
}

/*
  This convert color based on float to int32
  0.0 => 0 to 1.0 => 255
  This return RGB values
*/
function colorFloatToInt(color){
  return { r: Math.round(color[0] * 255), g : Math.round(color[1] * 255), b : Math.round(color[2] * 255)};
}

/*
  Function used to send 1 update to 1 specific fixture
*/
function sendFixture(x,y,f,color) {
  color = colorFloatToInt(color);
  local.send("/fixture/"+x+"/"+y+"/"+f, color.r, color.g, color.b);
}

/*
  Function used to send 1 update to 1 entire square (8 fixtures)
*/
function sendSquare(x,y,color) {
  color = colorFloatToInt(color);
  local.send("/carre/"+x+"/"+y, color.r, color.g, color.b);
}

/*
  Print function, because the Hardware is slow, updates / LED's changes are updated by this function.
  So you can send hundred of requests, and THEN apply with this print
  ref : https://www.reddit.com/r/FastLED/comments/aqlb94/troubleshooting_slow_performance_tied_to/
*/
function print(){
  local.send("/print");
}

/*
  Clear function, this will instant turn off the LED's
*/
function clear(){
  local.send("/clear");
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

/*
  Send an unique Fixture and turn it on
*/
function lightFixture(x,y,f,color){
  sendFixture(x,y,f,color);
  print();
}

/*
  Send an unique Square and turn it on
*/
function lightSquare(x,y,color){
  sendSquare(x,y,color);
  print();
}

/*
  Function used to send an entire Matrix update based on each squares 
*/
function lightMatrix(color) {
  for(y = 1; y <= 8; y++){
    for(x = 1; x <= 8; x++){
      sendSquare(x,y,color);
    }
  }
  print();
}