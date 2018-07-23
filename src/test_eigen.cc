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
  
  UDPSender udpsender("192.168.1.195", 2000);

  uint32_t frame_id = 0;
  for (float t = 0.0; t < 100.0; t += 0.1) {
    
    blink(hsv_group, t, 1.0, 0.0);
    hsv_group *= 255;

    cout << hsv_group << endl;

    float * fbuf = hsv_group.data();
    std::vector<uint8_t> ubuf(hsv_group.size());

    for (size_t i = 0; i < hsv_group.cols(); i++) {
      HsvToRgb(fbuf[3*i], fbuf[3*i+1], fbuf[3*i+2],
        ubuf[3*i], ubuf[3*i+1], ubuf[3*i+2]);
    }

    LOGF("Sending t=%.3f ...", t);
    for (size_t strip_id = 0; strip_id < 48; strip_id++) {
      udpsender.send(strip_id, frame_id++, &ubuf[0], ubuf.size());
    }
    usleep(1000 * 50);
  }

  return 0;
}
