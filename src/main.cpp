#include "Algorithms.h"
#include "Color.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <string>
#include <vector>

using namespace std;

const int NOT_EXISTANT_COMMAND = 32512;

// DECLARE FUNCTIONS
vector<Color> colorPallet(string filePath, int colorCount);
void processArgs(int argc, char **args);
int numLines(string fileLocation);
vector<Color> parseLines(string filePath);
void createResourceFiles(vector<Color> v, string fileLocation);

// END DECLARATION

void displayHelp() { cout << "HELP TEXT" << endl; }

void processArgs(int argc, char **args) {
  vector<string> arguments;
  vector<bool> arg(10);
  vector<string> argparams(10);
  if (argc > 1) {

    for (int i = 1; i < argc; i++) {
      arguments.push_back(args[i]);
    }

  } else {
    cout << "No arguments supplied. Exiting..." << endl;
    exit(1);
  }

  for (int i = 0; i < arguments.size(); i++) {
    if (arguments[i] == "-h") {
      displayHelp();
      exit(0);
    } else {
      if (arguments[i] == "-i") {
        arg[0] = true;
        if (i < arguments.size() - 1 && arguments[i + 1][0] != '-') {
          argparams[0] = arguments[i + 1];
          i++;
        } else {
          cout << "Must specify file location after -i" << endl;
          exit(1);
        }
      }
    }
  }

  if (arg[0]) {
    vector<Color> pallet = colorPallet(
        argparams[0], 16); // must replace 8 with another param later
    // system(xrdb -merge ./colors.txt)
    createResourceFiles(pallet, argparams[0]);
  }
}

void createResourceFiles(vector<Color> v, string fileLocation) {
  string filename = "image_";
  filename +=
      fileLocation.substr(fileLocation.rfind('/') + 1,
                          fileLocation.size() - fileLocation.rfind('/'));

  cout << filename << endl;

  Color bg = v[0];
  Color fg = v[7];
  // bg.lighten(.25);
  // fg.darken(.25);

  ofstream file("current_color_scheme_xrdb");
  file << "*background: " << bg.getHex() << endl;
  file << "*foreground: " << fg.getHex() << endl;
  file << "URxvt*background: [85]" << bg.getHex() << endl;
  file << "URxvt*foreground: [85]" << fg.getHex() << endl;
  if (v.size() == 8) {
    for (int i = 0; i < v.size(); i++) {
      Color n = v[i];
      n.lighten(.8);
      file << "*color" << i << ": " << n.getHex() << endl;
      file << "*.color" << i << ": " << n.getHex() << endl;
      n.lighten(.8);
      file << "*color" << i + 8 << ": " << n.getHex() << endl;
      file << "*.color" << i + 8 << ": " << n.getHex() << endl;
    }
  } else {
    for (int i = 0; i < v.size(); i++) {
      file << "*color" << i << ": " << v[i].getHex() << endl;
      file << "*.color" << i << ": " << v[i].getHex() << endl;
    }
  }

  file.close();
  string copy = "cp current_color_scheme_xrdb " + filename;
  system(copy.c_str());
  system("xrdb -merge ./current_color_scheme_xrdb");
  system("i3-msg restart");
}

int numLines(string fileLocation) {
  ifstream inputFile;
  inputFile.open(fileLocation);
  int lines = 0;
  string currentLine;

  while (!inputFile.eof()) {
    getline(inputFile, currentLine);

    if (currentLine != "") {
      lines++;
    }
  }
  inputFile.close();
  return lines;
}

vector<Color> parseLines(string fileText) {
  vector<Color> colorPallet;
  string currentLine, color;
  stringstream sstream;
  sstream << fileText;
  getline(sstream, currentLine);

  while (!sstream.eof()) {
    getline(sstream, currentLine);

    if (currentLine != "") {
      color = currentLine.substr(currentLine.find('#'), 7);
      colorPallet.push_back(Color(color));
    }
  }
  return colorPallet;
}

string imagemagick(string filepath) {
  int PATH_MAX = 100;
  FILE *file;
  int status;
  char path[PATH_MAX];
  string line;

  file = popen(filepath.c_str(), "r");

  while (fgets(path, PATH_MAX, file) != NULL) {
    line += path;
  }

  return line;
}

void autoGenerate(vector<Color> &v) {
  Color lastInList = v[v.size() - 1];
  int size = v.size();
  for (int i = 0; i < 8 - size; i++) {
    lastInList.setHue(lastInList.getHue() + 135.5 * i);
    // cout<<lastInList.getHex()<<endl;
    v.push_back(lastInList);
  }
}

vector<Color> colorPallet(string filePath, int colorCount) {
  if (system("convert > /dev/null") == NOT_EXISTANT_COMMAND) {
    cout << "ImageMagick not found. Please install before using. Exiting..."
         << endl;
    exit(1);
  }

  vector<Color> pallet;

  while (pallet.size() != colorCount && colorCount > 0) {
    cout << "Trying pallet size of " << colorCount << endl;

    string colorCountS = to_string(colorCount);
    string fm = "convert " + filePath + " -resize 25% +dither -colors " +
                colorCountS + " -unique-colors txt:-";

    string imagemagickText = imagemagick(fm);

    pallet = parseLines(imagemagickText);
    if (pallet.size() < colorCount)
      colorCount -= 8;
  }

  if (pallet.size() < 8) {
    // run code to auto generate colorscheme
    cout << "Not enough colors in given picture\nNow auto-generating "
            "colorscheme"
         << endl;
    // Algorithm::quicksort(pallet, 0, pallet.size() - 1);
    // autoGenerate(pallet);
    autoGenerate(pallet);
  }

  Algorithm::quicksort(pallet, 0, pallet.size() - 1);
  for (int i = 0; i < pallet.size(); i++) {
    cout << pallet[i].getHex() << " " << pallet[i].getHue() << " "
         << pallet[i].getLightness() << " " << pallet[i].getSaturation()
         << endl;
  }
  return pallet;
}

int main(int argc, char **args) {

  processArgs(argc, args);

  // test << system("ls -al ~");
  // if return is 32512, command is non existant and must be installed
  // colorPallet("~/Pictures/Wallpapers/Landscapes/waterfall_pool.jpg", 16);
  // Detect proper installed programs
  if (system("feh -h > /dev/null") == NOT_EXISTANT_COMMAND) {
    cout << "Error: feh not detected. Please install feh before using" << endl;
    exit(1);
  }
  // inputFile.open("./colors.txt");

  // system("feh --bg-scale ~/.config/i3/wall.png");
  return 0;
}
