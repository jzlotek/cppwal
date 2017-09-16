#include "Color.h"
#include <cmath>
#include <iostream>

using namespace std;

const string hexRef = "0123456789ABCDEF";

Color::Color(short r, short g, short b) {
  _r = r;
  _g = g;
  _b = b;
  checkValues();
}
void Color::checkValues() {
  short r = _r, g = _g, b = _b;
  _r = ((_r > 255) ? 255 : ((_r < 0) ? 0 : r));
  _g = ((_g > 255) ? 255 : ((_g < 0) ? 0 : g));
  _b = ((_b > 255) ? 255 : ((_b < 0) ? 0 : b));
  cout << getRed() << getGreen() << getBlue() << endl;
}
Color::Color(string hex) {
  if (hex.length() == 7 && hex[0] == '#') {
    // hex with # sign
    _r = hexRef.find(hex[1]) * 16 + hexRef.find(hex[2]);
    _g = hexRef.find(hex[3]) * 16 + hexRef.find(hex[4]);
    _b = hexRef.find(hex[5]) * 16 + hexRef.find(hex[6]);
  } else if (hex.length() == 6 && hex[0] != '#') {
    _r = hexRef.find(hex[0]) * 16 + hexRef.find(hex[1]);
    _g = hexRef.find(hex[2]) * 16 + hexRef.find(hex[3]);
    _b = hexRef.find(hex[4]) * 16 + hexRef.find(hex[5]);
  } else {
    _r = 0;
    _g = 0;
    _b = 0;
  }
}

string Color::getHex() const {
  string hex = "#";
  hex += hexRef[_r / 16];
  hex += hexRef[_r % 16];
  hex += hexRef[_g / 16];
  hex += hexRef[_g % 16];
  hex += hexRef[_b / 16];
  hex += hexRef[_b % 16];
  return hex;
}
void Color::darken(double amount) {
  _r *= (1 - amount);
  _g *= (1 - amount);
  _b *= (1 - amount);
  checkValues();
}

void Color::lighten(double amount) {
  _r += ((255 - _r) * (1 - amount));
  _g += ((255 - _g) * (1 - amount));
  _b += ((255 - _b) * (1 - amount));
  checkValues();
}

short Color::getHue() const {
  double r, g, b;
  r = static_cast<double>(_r) / 255.0;
  g = static_cast<double>(_g) / 255.0;
  b = static_cast<double>(_b) / 255.0;
  double hue;
  /* cout << endl
  << "R: " << r << endl << "G: " << g << endl << "B: " << b << endl;*/
  double min = fmin(r, fmin(g, b));
  double max = fmax(r, fmax(g, b));

  if (max == r)
    hue = (g - b) / (max - min);
  else if (max == g)
    hue = 2 + (b - r) / (max - min);
  else if (max == b)
    hue = 4 + (r - g) / (max - min);
  else
    hue = 0;

  hue *= 60;

  if (hue < 0)
    hue += 360;

  return static_cast<int>(round(hue));
}

double Color::getLightness() const {
  return (static_cast<double>(fmax(_r, fmax(_g, _b))) / 255.0 +
          static_cast<double>(fmin(_r, fmin(_g, _b))) / 255.0) /
         2;
}
double Color::getSaturation() const {
  if (delta() == 0)
    return 0;
  else
    return delta() / (1 - abs(2 * getLightness() - 1));
}
double Color::delta() const {
  return (static_cast<double>(fmax(_r, fmax(_g, _b))) -
          static_cast<double>(fmin(_r, fmin(_g, _b)))) /
         255.0;
}

void Color::setHue(double amount) {
  short r, g, b;
  double c = (1 - abs(2 * getLightness() - 1)) * getSaturation();
  double h = amount / 60.0;
  double x = c * (1 - abs(static_cast<int>(h) % 2 - 1));
  if (h >= 0 && h <= 1) {
    r = c;
    g = x;
    b = 0;
  } else if (h >= 1 && h <= 2) {
    r = x;
    g = c;
    b = 0;
  } else if (h >= 2 && h <= 3) {
    r = 0;
    g = c;
    b = x;
  } else if (h >= 3 && h <= 4) {
    r = 0;
    g = x;
    b = c;
  } else if (h >= 4 && h <= 5) {
    r = x;
    g = 0;
    b = c;
  } else if (h >= 5 && h < 6) {
    r = x;
    g = 0;
    b = c;
  } else {
    r = 0;
    g = 0;
    b = 0;
  }
  double m = getLightness() - 0.5 * c;
  r += m;
  g += m;
  b += m;
  _r = static_cast<short>(r);
  _g = static_cast<short>(g);
  _b = static_cast<short>(b);
  checkValues();
}
void Color::setLightness(double amount) {
  short r = _r, g = _g, b = _b;
  cout;
}
void Color::setSaturation(double amount) {
  short r = _r, g = _g, b = _b;
  cout;
}
