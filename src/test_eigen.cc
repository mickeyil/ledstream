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

// typedef Matrix<float, 3, Dynamic, ColMajor> Mtx;
typedef MatrixXf Mtx;

static const int STRIP_SIZE = 51;

void blink(Mtx& data, float t, float freq_Hz, float offset, float h, float s)
{
  float A = freq_Hz * 2 * M_PI;
  float phi = offset * 2 * M_PI;
  float val = 255*0.5*(cos(A*t + phi) + 1.0);
  data.row(0) = RowVectorXf::Constant(data.cols(), h);
  data.row(1) = RowVectorXf::Constant(data.cols(), s);
  data.row(2) = RowVectorXf::Constant(data.cols(), val);
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



int main(int argc, char *argv[])
{
  Mtx hsv_group(3, STRIP_SIZE);	
  
  UDPSender udpsender("10.0.0.202", 2000);
  
  uint32_t frame_id = 0;
  uint32_t strip_id = 8;
  if (argc > 1) {
    strip_id = atoi(argv[1]);
    frame_id = atoi(argv[1]) * 1000;
  } else{
    frame_id = 0;
  }

  LOGF("strip_id = %u,  frame_id = %u", strip_id, frame_id);

  for (float t = 0.0; t < 100.0; t += 0.01) {
    
    blink(hsv_group, t, 1.0, 0.0, 200.0, 255.0);

    float * fbuf = hsv_group.data();
    std::vector<uint8_t> ubuf(hsv_group.size());

    for (size_t i = 0; i < hsv_group.cols(); i++) {
      HsvToRgb(fbuf[3*i], fbuf[3*i+1], fbuf[3*i+2],
        ubuf[3*i], ubuf[3*i+1], ubuf[3*i+2]);
    }

    LOGF("[%u]: t=%.3f -- sending to strip: %d", frame_id, t, strip_id);
    usleep(1000 * 15);
    udpsender.send(strip_id, frame_id++, &ubuf[0], ubuf.size());
  }

  return 0;
}
