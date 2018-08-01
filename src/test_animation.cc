#include <unistd.h>
#include <chrono>
#include <iostream>
#include "json/json.h"
#include "animation.h"
#include "linearpixels.h"
#include "errors.h"
#include "fileio.h"

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
  blue_pixel.h = 240;
  blue_pixel.s = 255;
  blue_pixel.v = 255;

  LinearPixels pixels(1, 51);

  // initialize to blue
  for (size_t i = 0; i < pixels.size(); i++) {
    pixels[i] = blue_pixel;
  }

  pixels.print();

  const float ch_duration = 2.0;
  const float fps = 1;
  const float t_cycle = 1.0 / fps;

  std::vector<uint16_t> group_indices = { 10,20,30,40 };

  double t_start = gettimef();
  while (true) {
    double t_now = gettimef();
    while (t_now > t_start + ch_duration) {
      t_start += ch_duration;
    }    

    hsv_vec_t pg = pixels.get_pixel_group(group_indices);
    float t_rel = t_now - t_start;

    LOGF("render: t_rel = %f", t_rel); 
    animation->render(t_rel, pg);
    pixels.set_pixel_group(group_indices, pg);
    pixels.print();

    double t2 = gettimef();
    double sleep_sec = t_cycle - (t2 - t_now);
    LOGF("sleep = %f", sleep_sec);
    if (sleep_sec > 0.0) {
      usleep(static_cast<int>(sleep_sec * 1e6));
    }
  }

  return 0;
}