#ifndef _COLOR_H_
#define _COLOR_H_

#include <string>

using namespace std;

class Color {
private:
  short _r, _g, _b;
  void checkValues();
  double delta() const;

public:
  Color(short r = 0, short g = 0, short b = 0);
  Color(string hex);
  string getHex() const;
  short getRed() const { return _r; };
  short getGreen() const { return _g; };
  short getBlue() const { return _b; };
  void darken(double amount);
  void lighten(double amount);
  short getHue() const;
  double getLightness() const;
  double getSaturation() const;
  void setHue(double);
  void setLightness(double);
  void setSaturation(double);
};

#endif
