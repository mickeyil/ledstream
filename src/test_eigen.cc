#include <cmath>
#include <iostream>
#include <Eigen/Dense>

#include "slog.h"

using namespace Eigen;
using std::cout;
using std::endl;

typedef Matrix<float, 3, Dynamic, RowMajor> Mtx;

static const int STRIP_SIZE = 144;

void blink(Mtx& data, float t, float freq_Hz, float offset)
{
  float A = freq_Hz * 2 * M_PI;
  float phi = offset * 2 * M_PI;
  float val = cos(A*t + phi);
  data = Mtx::Constant(3, data.cols(), val);
}

int main()
{
  Mtx hsv_group(3, STRIP_SIZE);	
  
  LOGF("start");
  for (size_t i = 0; i < 1000; i++) {
    blink(hsv_group, float(i) / 1000.0, 1.0, 0.0);
  }
  LOGF("end");

  cout << hsv_group << endl;
  return 0;
}
