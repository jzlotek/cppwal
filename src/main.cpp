#include "Algorithms.h"
#include "Color.h"
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <string>
#include <vector>

using namespace std;

const int NOT_EXISTANT_COMMAND = 32512;
const string HOME = string(getenv("HOME"));

// DECLARE FUNCTIONS
vector<Color> colorPallet(string filePath, int colorCount);
void processArgs(int argc, char **args);
int numLines(string fileLocation);
vector<Color> parseLines(string filePath);
void createResourceFiles(vector<Color> v, string fileLocation, string alpha);
void setWallpaper(string fileLocation);
void reloadColors(vector<vector<Color>> v, string alpha);
string popenCommand(string shellCommand);
void reload();
void cleanCachedFiles();
void autoGenerate(vector<Color> &v, int max);

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
    displayHelp();
    exit(0);
  }
  argparams[1] = "100";

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
      if (arguments[i] == "-a") {
        arg[1] = true;
        if (i < arguments.size() - 1 && arguments[i + 1][0] != '-' &&
            Algorithm::toNumber(arguments[i + 1]) <= 100 &&
            Algorithm::toNumber(arguments[i + 1]) >= 0) {
          argparams[1] = arguments[i + 1];
          i++;
        } else {
          argparams[1] = "100";
          exit(1);
        }
      }
      if (arguments[i] == "-c") {
        arg[2] = true;
      }
    }
  }

  if (arg[2]) {
    cleanCachedFiles();
  }

  if (arg[0]) {
    vector<Color> pallet = colorPallet(argparams[0], 8);
    // must replace 8 with another param later
    // system(xrdb -merge ./colors.txt)
    createResourceFiles(pallet, argparams[0], argparams[1]);
    setWallpaper(argparams[0]);
    vector<vector<Color>> fullPallet(2);

    fullPallet[1] = pallet;
    Color bg = pallet[0];
    Color fg = pallet[7];

    if (pallet[0].getLightness() < 0.5) {
      bg.darken(0.1);
      fg.lighten(0.1);
      for (int i = 0; i < fullPallet[1].size(); i++) {
        fullPallet[1][i].lighten(0.9);
      }
    } else {
      bg.lighten(0.1);
      fg.darken(0.1);
      for (int i = 0; i < fullPallet[1].size(); i++) {
        fullPallet[1][i].darken(0.9);
      }
    }

    fullPallet[0].push_back(bg);
    fullPallet[0].push_back(fg);
    reloadColors(fullPallet, argparams[1]);
  }
}

void setWallpaper(string fileLocation) {
  string syscall = "feh --bg-fill ~/.config/i3/wall.png";
  string copy = "cp " + fileLocation + " ~/.config/i3/wall.png";
  system(copy.c_str());
  system(syscall.c_str());
}

void cleanCachedFiles() {
  string clean = "rm " + HOME + "/.cache/cwal/schemes/*";
  system(clean.c_str());
}

void reloadColors(vector<vector<Color>> v, string alpha) {
  // make sure that there are 2 vectors: 1 for 16 color 1 for special
  if (v.size() == 2) {
    string a = "";
    if (alpha != "100") {
      a = "[" + alpha + "]";
    }
    string sequences = "";
    // string sequences = "";
    stringstream num;
    for (int i = 0; i < v[1].size(); i++) {
      num << i;
      sequences += "\033]4;" + num.str() + ";" + v[1][i].getHex() + "\007";
      num.str(std::string());
    }
    /* Foreground */
    sequences += "\033]10;" + a + "" + v[0][1].getHex() + "\007";
    sequences += "\033]12;" + a + "" + v[0][1].getHex() + "\007";
    sequences += "\033]13;" + a + "" + v[0][1].getHex() + "\007";
    /* Background */
    sequences += "\033]11;" + a + "" + v[0][0].getHex() + "\007";
    sequences += "\033]14;" + a + "" + v[0][0].getHex() + "\007";
    sequences += "\033]708;" + a + "" + v[0][0].getHex() + "\007";

    // ofstream file("sequences");
    // file << sequences;
    // file.close();
    // cout<<sequences<<endl;
    // system("sh scripts/colorterm.sh");

    string t = "for term in /dev/pts/[0-9]*\ndo\n\t[[ -w \"$term\" ]] && "
               "printf \"%b\" \"" +
               sequences + "\" > \"$term\" &\ndone";

    system(t.c_str());
    cout << "\n\nColor applied to terminals" << endl;
  } else {
    cout << "Error" << endl;
    exit(1);
  }
  reload();
}

void createResourceFiles(vector<Color> v, string fileLocation, string alpha) {
  string filename = HOME + "/.cache/cwal/schemes/image_";
  filename += fileLocation.substr(
      fileLocation.rfind('/') + 1,
      fileLocation.size() - fileLocation.rfind('/') -
          (fileLocation.size() - fileLocation.rfind(".") + 1));

  string a = "";
  if (alpha != "100") {
    a = "[" + alpha + "]";
  }
  Color bg = v[0];
  Color fg = v[15];
  // bg.lighten(.25);
  // fg.darken(.25);

  ofstream file(HOME + "/.cache/cwal/current_color_scheme_xrdb.Xresources");
  file << "*background: " << bg.getHex() << endl;
  file << "*foreground: " << fg.getHex() << endl;
  file << "URxvt.background: " << a << bg.getHex() << endl;
  file << "URxvt.foreground: " << a << fg.getHex() << endl;
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
    // for (int i = v.size() - 1; i >= 0; i--) {
    //   file << "*color" << v.size() - i - 1 << ": " << v[i].getHex() << endl;
    //   file << "*.color" << v.size() - i - 1 << ": " << v[i].getHex() << endl;
    // }
  }
  file.close();

  string copy = "cp " + HOME +
                "/.cache/cwal/current_color_scheme_xrdb.Xresources " + filename;
  system(copy.c_str());
}

void reload() {
  system("xrdb -merge ~/.cache/cwal/current_color_scheme_xrdb.Xresources");
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

// Returns stdoutput from calling imagemagick command
string popenCommand(string shellCommand) {
  int PATH_MAX = 100;
  FILE *file;
  int status;
  char path[PATH_MAX];
  string line;

  file = popen(shellCommand.c_str(), "r");

  while (fgets(path, PATH_MAX, file) != NULL) {
    line += path;
  }

  return line;
}

void autoGenerate(vector<Color> &v, int max) {
  Color lastInList = v[v.size() - 1];
  while (v.size() < max) {
    lastInList.setHue(lastInList.getHue() + 135.5g);
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

    string imagemagickText = popenCommand(fm);

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
    autoGenerate(pallet, 16);
  }

  Algorithm::quicksort(pallet, 0, pallet.size() - 1);
  // for (int i = 0; i < pallet.size(); i++) {
  //   cout << pallet[i].getHex() << " " << pallet[i].getHue() << " "
  //        << pallet[i].getLightness() << " " << pallet[i].getSaturation()
  //        << endl;
  // }
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
