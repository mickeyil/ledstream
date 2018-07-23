#include <cmath>
#include <unistd.h>
#include <iostream>
#include <Eigen/Dense>

#include "slog.h"
#include "udpsender.h"
#include "rgb2hsv.h"

using namespace Eigen;
using std::cout;
using std::endl;

typedef Matrix<float, 3, Dynamic, ColMajor> Mtx;

static const int STRIP_SIZE = 51;

void blink(Mtx& data, float t, float freq_Hz, float offset)
{
  float A = freq_Hz * 2 * M_PI;
  float phi = offset * 2 * M_PI;
  float val = 0.5*(cos(A*t + phi) + 1.0);
  data = Mtx::Constant(3, data.cols(), val);
}


#if 0
void wave(Mtx& data, float t, float freq_Hz, float offset)
{
  float A = freq_Hz * 2 * M_PI;
  float phi = offset * 2 * M_PI;
  for (size_t i = 0; i < 3; i++) {
    data.row(i) = data.row(i).setLinspaced(A*(t + offset, A*t + offset + freq_Hz);
  }
}
#endif



int main()
{
  Mtx hsv_group(3, STRIP_SIZE);	
  
  #if 0
  LOGF("start");
  for (size_t i = 0; i < 500; i++) {
    blink(hsv_group, float(i) / 1000.0, 1.0, 0.0);
  }
  LOGF("end");
  #endif

  UDPSender udpsender("10.0.0.202", 2000);

  for (float t = 0.0; t < 10.0; t += 0.1) {
    
    blink(hsv_group, 0.0, 1.0, 0.0);
    hsv_group *= 255;

    cout << hsv_group << endl;

    float * fbuf = hsv_group.data();
    std::vector<uint8_t> ubuf(hsv_group.size());

    LOGF("fbuf: MxN = %dx%d", hsv_group.rows(), hsv_group.cols());

    for (size_t i = 0; i < hsv_group.cols(); i++) {
      HsvToRgb(fbuf[3*i], fbuf[3*i+1], fbuf[3*i+2],
        ubuf[3*i], ubuf[3*i+1], ubuf[3*i+2]);
    }

    LOGF("Sending t=%.3f ...", t);
    for (size_t strip_id = 0; strip_id < 1; strip_id++) {
      udpsender.send(strip_id, 0, &ubuf[0], ubuf.size());
    }
    usleep(1000 * 500);
    break;
  }

  return 0;
}
