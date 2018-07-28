#include "linearpixels.h"


int main()
{
  LinearPixels pixels(3, 9);
  pixels.disable_pixel(pixels.get_linaddr(0, 3));
  pixels.disable_strip(1);
  pixels.print();
  return 0;
}

