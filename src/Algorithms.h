#ifndef _ALGO_H_
#define _ALGO_H_

#include "Color.h"
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Algorithm {
public:
  static void quicksort(vector<Color> &v, int lo, int hi);
  static int partition(vector<Color> &v, int lo, int hi);
  static int classifyCharacter(char);
  static int toNumber(string);

private:
};

#endif
