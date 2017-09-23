#include "Algorithms.h"
#include <cmath>

using namespace std;

int Algorithm::partition(vector<Color> &v, int lo, int hi) {
  Color pivot = v[hi];
  int i = lo - 1;
  for (int j = lo; j < hi; j++) {
    if (/*v[j].getHue() * */v[j].getLightness() + v[j].getSaturation() <=
        /*pivot.getHue() * */pivot.getLightness() + pivot.getSaturation()) {
      i++;
      swap(v[i], v[j]);
    }
  }
  if (/*v[hi].getHue() * */ v[hi].getLightness() + v[hi].getSaturation() <
      /*v[i + 1].getHue() * */ v[i + 1].getLightness() + v[i + 1].getSaturation()) {
    swap(v[hi], v[i + 1]);
  }
  return i + 1;
}

void Algorithm::quicksort(vector<Color> &v, int lo, int hi) {
  double avgSat = 0;
  for (int i = 0; i < v.size(); i++) {
    avgSat += v[i].getSaturation();
  }

  avgSat /= v.size();

  if (lo < hi) {
    int p = Algorithm::partition(v, lo, hi);
    Algorithm::quicksort(v, lo, p - 1);
    Algorithm::quicksort(v, p + 1, hi);
  }
}

int Algorithm::classifyCharacter(char c) {
  if (c == '1')
    return 1;
  else if (c == '2')
    return 2;
  else if (c == '3')
    return 3;
  else if (c == '4')
    return 4;
  else if (c == '5')
    return 5;
  else if (c == '6')
    return 6;
  else if (c == '7')
    return 7;
  else if (c == '8')
    return 8;
  else if (c == '9')
    return 9;
  else
    return 0;
}

int Algorithm::toNumber(string s) {
  int n = 0;
  for (int i = 0; i < s.size(); i++) {
    n += Algorithm::classifyCharacter(s[i]) * pow(10, s.size() - 1 - i);
  }
  return n;
}
