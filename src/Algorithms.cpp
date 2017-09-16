#include "Algorithms.h"

using namespace std;

int Algorithm::partition(vector<Color> &v, int lo, int hi) {
  Color pivot = v[hi];
  int i = lo - 1;
  for (int j = lo; j < hi; j++) {
    if (v[j].getHue() * v[j].getLightness() * v[j].getSaturation() <=
        pivot.getHue() * pivot.getLightness() * pivot.getSaturation()) {
      i++;
      swap(v[i], v[j]);
    }
  }
  if (v[hi].getHue() * v[hi].getLightness() * v[hi].getSaturation() <
      v[i + 1].getHue() * v[i + 1].getLightness() * v[i + 1].getSaturation()) {
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
