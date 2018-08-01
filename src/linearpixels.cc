#include "linearpixels.h"


LinearPixels::LinearPixels(size_t number_of_strips_, size_t max_leds_per_strip_) :
  number_of_strips(number_of_strips_), max_leds_per_strip(max_leds_per_strip_)
{
  // initialize vectors
  data.resize(number_of_strips * max_leds_per_strip);
  active_mask.resize(data.size());

  // by default all pixels are enabled
  std::fill(active_mask.begin(), active_mask.end(), 1);

  hsv_t color;
  color.h = 0;
  color.s = 0;
  color.v = 0;
  std::fill(data.begin(), data.end(), color);
}


void LinearPixels::print() const
{
  std::cout << std::left << std::setw(15) << "strip \\ index";
  for (size_t i = 0; i < max_leds_per_strip; i++) {
    std::cout << std::left << std::setw(4) << i;
  }
  std::cout << std::endl;
  std::cout << std::endl;

  for (size_t s_idx = 0; s_idx < number_of_strips; s_idx++) {

    // print H values
    std::cout << std::left << std::setw(15) << s_idx;
    for (size_t i = 0; i < max_leds_per_strip; i++) {
      size_t linaddr = this->get_linaddr(s_idx, i);
      if (active_mask[linaddr] == 0)
        std::cout << std::left << std::setw(4) << "---";
      else
        std::cout << std::left << std::setw(4) << static_cast<int>(data[linaddr].h);
    }
    std::cout << std::endl;

    // print S values
    std::cout << std::left << std::setw(15) << "";
    for (size_t i = 0; i < max_leds_per_strip; i++) {
      size_t linaddr = this->get_linaddr(s_idx, i);
      if (active_mask[linaddr] == 0)
        std::cout << std::left << std::setw(4) << "---";
      else
        std::cout << std::left << std::setw(4) << static_cast<int>(data[linaddr].s);
    }
    std::cout << std::endl;

    // print V values
    std::cout << std::left << std::setw(15) << "";
    for (size_t i = 0; i < max_leds_per_strip; i++) {
      size_t linaddr = this->get_linaddr(s_idx, i);
      if (active_mask[linaddr] == 0)
        std::cout << std::left << std::setw(4) << "---";
      else
        std::cout << std::left << std::setw(4) << static_cast<int>(data[linaddr].v);
    }
    std::cout << std::endl;
    std::cout << std::endl;
  }
}


hsv_vec_t LinearPixels::get_pixel_group(const std::vector<uint16_t>& v_indices) const
{
  hsv_vec_t vec(v_indices.size());
  for (size_t i = 0; i < v_indices.size(); i++) {
    vec[i] = data[v_indices[i]];
  }
  return vec;
}


void LinearPixels::set_pixel_group(const std::vector<uint16_t>& v_indices, 
                                   const hsv_vec_t& hsv_vec)
{
  if (v_indices.size() != hsv_vec.size())
    throw "size mismatch on pixel assignment";

  for (size_t i = 0; i < v_indices.size(); i++) {
    data[v_indices[i]] = hsv_vec[i];
  }
}

