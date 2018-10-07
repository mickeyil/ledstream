#include <time.h>
#include <unistd.h>
#include <cstdlib>
#include <chrono>
#include <iostream>
#include <algorithm>
#include "json/json.h"
#include "animation.h"
#include "linearpixels.h"
#include "errors.h"
#include "fileio.h"
#include "udpsender.h"
#include "rgb2hsv.h"

using std::cout;

static double gettimef()
{
  using namespace std::chrono;
  long int ms = duration_cast<milliseconds>(
      system_clock::now().time_since_epoch()).count();
  return static_cast<double>(ms) / 1000.0;
}

int main(int argc, char *argv[])
{
  if (argc < 2) {
    EXIT_ERROR("error: test requires JSON file as an argument.\n")
  }
  std::string jsondata = file2str(argv[1]);

  Json::Value root;
  Json::Reader reader;
  
  if (!reader.parse(jsondata.c_str(), root)) {
    EXIT_ERROR("Failed parsing json.\n>>\n%s\n<<\n", jsondata.c_str());
  }

  Animation* animation = new Blender(root);  
  
  hsv_t blue_pixel;
  blue_pixel.h = 180;
  blue_pixel.s = 255;
  blue_pixel.v = 255;

  LinearPixels pixels(1, 51);

  // initialize to blue
  for (size_t i = 0; i < pixels.size(); i++) {
    pixels[i] = blue_pixel;
  }

  pixels.print();

  const float ch_duration = 0.4;
  const float fps = 80;
  const float t_cycle = 1.0 / fps;

  // std::vector<uint16_t> group_indices = { 10,20,30,40 };
  std::vector<uint16_t> group_all(51);
  std::vector<uint16_t> group_indices;
  for (size_t i = 0; i < 51; i++)
    group_all[i] = i;

  UDPSender udpsender("10.0.0.202", 2000);
  unsigned int frame_id = time(NULL) % 1000000;
  LOGF("frame_id initialized to %d", frame_id);
  std::vector<uint8_t> rgb_vec(51*3);

  double t_start = gettimef();
  while (true) {
    double t_now = gettimef();

    // increment t_start to form a repeated animation channel duration
    while (t_now > t_start + ch_duration) {
      t_start += ch_duration;
      group_indices = group_all;
      std::random_shuffle(group_indices.begin(), group_indices.end());
      size_t grp_size = rand() % 25 + 1; 
      group_indices.resize(grp_size);

    }

#if 0
    for (size_t i = 0; i < pixels.size(); i++) {
      if (pixels[i].h == 255)
        pixels[i].h = 0;
      else
        pixels[i].h += 1;
    }
    #endif

    hsv_vec_t pg = pixels.get_pixel_group(group_indices);
    for (size_t i = 0; i < pg.size(); i++) {
      pg[i] = blue_pixel;
    }
    float t_rel = t_now - t_start;

    LOGF("render: t_rel = %f", t_rel); 
    animation->render(t_rel, pg);
    pixels.set_pixel_group(group_indices, pg);
    // pixels.print();
    for (size_t i = 0; i < pixels.size(); i++) {
      hsv_t pix = pixels[i];
      uint8_t r,g,b;
      HsvToRgb(pix.h, pix.s, pix.v, r, g, b);
      rgb_vec[3*i] = r;
      rgb_vec[3*i+1] = g;
      rgb_vec[3*i+2] = b;
    }

    uint8_t * pdata = &rgb_vec[0];
    size_t data_size = pixels.size() * 3;
    LOGF("data size: %d", (int)data_size);
    udpsender.send(8, frame_id++, pdata, data_size);


    double t2 = gettimef();
    double sleep_sec = t_cycle - (t2 - t_now);
    LOGF("sleep = %f", sleep_sec);
    if (sleep_sec > 0.0) {
      usleep(static_cast<int>(sleep_sec * 1e6));
    }
  }

  return 0;
}