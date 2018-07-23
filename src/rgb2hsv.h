#include <cstdint>

void HsvToRgb(const uint8_t& h_, const uint8_t& s_, const uint8_t& v_,
                    uint8_t& r_,       uint8_t& g_,       uint8_t& b_);

void HsvToRgb(const float& h_, const float& s_, const float& v_,
                  uint8_t& r_,     uint8_t& g_,     uint8_t& b_);
