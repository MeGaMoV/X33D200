function init() {
  script.log(" - Module initialisé !");
}

/*
  This convert color based on float to int32
  0.0 => 0 to 1.0 => 255
  This return RGB values
*/
function colorRVB(colorFromTxt, colorAccurate, intensity) {
  intensity = intensity / 100;
  color = colorFromTxt;
  //If we have black color, use the second choice of color (in case the other one have a color)
  if (colorFromTxt[0] == 0.0 && colorFromTxt[1] == 0.0 && colorFromTxt[2] == 0.0) color = colorAccurate;
  return { r: Math.round(color[0] * 255 * intensity), g: Math.round(color[1] * 255 * intensity), b: Math.round(color[2] * 255 * intensity) };
}

/*
  Function used to send 1 update to 1 specific fixture
*/
function sendFixture(x, y, f, color) {
  local.send("/fixture/" + x + "/" + y + "/" + f, color.r, color.g, color.b);
}

/*
  Function used to send 1 update to 1 specific external "fixture" of a square
*/
function sendTour(x, y, f, color) {
  local.send("/tour/" + x + "/" + y + "/" + f, color.r, color.g, color.b);
}

/*
  Function used to send 1 update to 1 entire square (8 fixtures)
*/
function sendSquare(x, y, color) {
  local.send("/carre/" + x + "/" + y, color.r, color.g, color.b);
}

/*
  Print function, because the Hardware is slow, updates / LED's changes are updated by this function.
  So you can send hundred of requests, and THEN apply with this print
  ref : https://www.reddit.com/r/FastLED/comments/aqlb94/troubleshooting_slow_performance_tied_to/
*/
function sendPrint() {
  local.send("/print");
}

/*
  Clear function, this will instant turn off the LED's
*/
function sendClear() {
  local.send("/clear");
}

/*
  Clear function, this will instant turn off the LED's
*/
function sendIntensity(intensity) {
  local.send("/intensity", intensity * 2.55);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
/*
  Send an unique Fixture and turn it on
*/
function fixture(xy, f, colorFromTxt, colorAccurate, intensity) {
  color = colorRVB(colorFromTxt, colorAccurate, intensity);
  sendFixture(xy.substring(0, 1), xy.substring(1, 2), f, color);
  sendPrint();
}

/*
  Send an unique Square and turn it on
*/
function square(xy, colorFromTxt, colorAccurate, intensity) {
  color = colorRVB(colorFromTxt, colorAccurate, intensity);
  sendSquare(xy.substring(0, 1), xy.substring(1, 2), color);
  sendPrint();
}

/*
  Send 2 square fixture's and turn it on
*/
function squarebord(xy, fixtures, colorFromTxt, colorAccurate, intensity) {
  color = colorRVB(colorFromTxt, colorAccurate, intensity);
  x = xy.substring(0, 1);
  y = xy.substring(1, 2);
  f1 = fixtures.substring(0, 1);
  f2 = fixtures.substring(1, 2);

  sendFixture(x, y, f1, color);
  sendFixture(x, y, f2, color);

  sendPrint();
}

/*
  Send 2 square fixture's at edge of the Matrix and turn it on
*/
function bord(xy, fixtures, colorFromTxt, colorAccurate, intensity) {
  color = colorRVB(colorFromTxt, colorAccurate, intensity);
  x = xy.substring(0, 1);
  y = xy.substring(1, 2);
  f1 = fixtures.substring(0, 1);
  f2 = fixtures.substring(1, 2);

  sendTour(x, y, f1, color);
  sendTour(x, y, f2, color);

  sendPrint();
}

/*
  Send an entire edge of the Matrix
*/
function frame(frame, colorFromTxt, colorAccurate, intensity) {
  color = colorRVB(colorFromTxt, colorAccurate, intensity);

  if (frame == 1) {
    for (x = 1; x <= 8; x++) {
      sendTour(x, 1, 1, color);
      sendTour(x, 1, 2, color);
    }
  }

  if (frame == 2) {
    for (y = 1; y <= 8; y++) {
      sendTour(8, y, 3, color);
      sendTour(8, y, 4, color);
    }
  }

  if (frame == 3) {
    for (x = 1; x <= 8; x++) {
      sendTour(x, 8, 5, color);
      sendTour(x, 8, 6, color);
    }
  }

  if (frame == 4) {
    for (y = 1; y <= 8; y++) {
      sendTour(1, y, 7, color);
      sendTour(1, y, 8, color);
    }
  }

  sendPrint();
}

/*
  Function used to send an entire Matrix update based on each squares 
*/
function matrix(colorFromTxt, colorAccurate, intensity) {
  color = colorRVB(colorFromTxt, colorAccurate, intensity);

  //External tour of the Matrix
  for (x = 1; x <= 8; x++) {
    sendTour(x, 1, 1, color);
    sendTour(x, 1, 2, color);
    sendTour(x, 8, 5, color);
    sendTour(x, 8, 6, color);
  }
  for (y = 1; y <= 8; y++) {
    sendTour(1, y, 7, color);
    sendTour(1, y, 8, color);
    sendTour(8, y, 3, color);
    sendTour(8, y, 4, color);
  }

  //All squares
  for (y = 1; y <= 8; y++) {
    for (x = 1; x <= 8; x++) {
      sendSquare(x, y, color);
    }
  }
  sendPrint();
}

/*
  Function used to create a "spirale"
*/
function zoomsquare(taille, colorFromTxt, colorAccurate, intensity) {
  color = colorRVB(colorFromTxt, colorAccurate, intensity);

  if (taille == 4) {
    xyMin = 1;
    xyMax = 8;
  }
  if (taille == 3) {
    xyMin = 2;
    xyMax = 7;
  }
  if (taille == 2) {
    xyMin = 3;
    xyMax = 6;
  }
  if (taille == 1) {
    xyMin = 4;
    xyMax = 5;
  }

  for (pos = xyMin; pos <= xyMax; pos++) {
    //Left
    sendFixture(xyMin, pos, 7, color);
    sendFixture(xyMin, pos, 8, color);

    //Right
    sendFixture(pos, xyMin, 1, color);
    sendFixture(pos, xyMin, 2, color);

    //Top
    sendFixture(xyMax, pos, 3, color);
    sendFixture(xyMax, pos, 4, color);

    //Bottom
    sendFixture(pos, xyMax, 5, color);
    sendFixture(pos, xyMax, 6, color);
  }

  sendPrint();
}

/*
  Send a Wall between 2 squares
*/
function wall(xy1, xy2, fixtures, colorFromTxt, colorAccurate, intensity) {
  color = colorRVB(colorFromTxt, colorAccurate, intensity);

  x1 = xy1.substring(0, 1);
  y1 = xy1.substring(1, 2);
  x2 = xy2.substring(0, 1);
  y2 = xy2.substring(1, 2);

  f1 = fixtures.substring(0, 1);
  f2 = fixtures.substring(1, 2);

  lightOn = true;

  if (x2 != x1) {
    if (x2 < x1) {
      direction = "LEFT";
    } else {
      direction = "RIGHT";
    }
  }

  if (y2 != y1) {
    if (y2 < y1) {
      direction = "UP";
    } else {
      direction = "DOWN";
    }
  }
  
  if (direction == "LEFT") {
    for (x = parseInt(x2); x >= x1; x--) {
      sendFixture(x, y1, f1, color);
      sendFixture(x, y1, f2, color);
    }
  }
  if (direction == "RIGHT") {
    for (x = parseInt(x1); x <= x2; x++) {
      sendFixture(x, y1, f1, color);
      sendFixture(x, y1, f2, color);
    }
  }
  if (direction == "UP") {
    for (y = parseInt(y1); y <= y2; y++) {
      sendFixture(x1, y, f1, color);
      sendFixture(x1, y, f2, color);
    }
  }
  if (direction == "DOWN") {
    for (y = parseInt(y2); y >= y1; y--) {
      sendFixture(x1, y, f1, color);
      sendFixture(x1, y, f2, color);
    }
  }

  sendPrint();
}

/*
  Send color to an intersection between 2 squares
*/
function intersection(xy1, xy2, colorFromTxt, colorAccurate, intensity) {
  color = colorRVB(colorFromTxt, colorAccurate, intensity);

  x1 = xy1.substring(0, 1);
  y1 = xy1.substring(1, 2);
  x2 = xy2.substring(0, 1);
  y2 = xy2.substring(1, 2);

  if (x2 == x1) {
    posX = "MID";
  } else {
    if (x2 < x1) {
      posX = "LEFT";
    } else {
      posX = "RIGHT";
    }
  }

  if (y2 == y1) {
    posY = "MID";
  } else {
    if (y2 < y1) {
      posY = "UP";
    } else {
      posY = "DOWN";
    }
  }

  if (posX + posY == "LEFTMID") {
    sendFixture(x1, y1, 7, color);
    sendFixture(x1, y1, 8, color);
    sendFixture(x2, y2, 3, color);
    sendFixture(x2, y2, 4, color);
  }
  if (posX + posY == "RIGHTMID") {
    sendFixture(x1, y1, 3, color);
    sendFixture(x1, y1, 4, color);
    sendFixture(x2, y2, 7, color);
    sendFixture(x2, y2, 8, color);
  }
  if (posX + posY == "MIDUP") {
    sendFixture(x1, y1, 1, color);
    sendFixture(x1, y1, 2, color);
    sendFixture(x2, y2, 5, color);
    sendFixture(x2, y2, 6, color);
  }
  if (posX + posY == "MIDDOWN") {
    sendFixture(x1, y1, 5, color);
    sendFixture(x1, y1, 6, color);
    sendFixture(x2, y2, 1, color);
    sendFixture(x2, y2, 2, color);
  }
  if (posX + posY == "LEFTUP") {
    sendFixture(x1, y1, 1, color);
    sendFixture(x1, y1, 8, color);
    sendFixture(x2, y2, 4, color);
    sendFixture(x2, y2, 5, color);
  }
  if (posX + posY == "RIGHTUP") {
    sendFixture(x1, y1, 2, color);
    sendFixture(x1, y1, 3, color);
    sendFixture(x2, y2, 6, color);
    sendFixture(x2, y2, 7, color);
  }
  if (posX + posY == "LEFTDOWN") {
    sendFixture(x1, y1, 6, color);
    sendFixture(x1, y1, 7, color);
    sendFixture(x2, y2, 2, color);
    sendFixture(x2, y2, 3, color);
  }
  if (posX + posY == "RIGHTDOWN") {
    sendFixture(x1, y1, 4, color);
    sendFixture(x1, y1, 5, color);
    sendFixture(x2, y2, 1, color);
    sendFixture(x2, y2, 8, color);
  }

  sendPrint();
}