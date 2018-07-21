#include <cstdint>
#include <cstdio>
#include <vector>

#include "slog.h"

typedef uint8_t u8;
typedef std::vector<uint8_t> vec_u8;

typedef struct RgbColor
{
    unsigned char r;
    unsigned char g;
    unsigned char b;

    RgbColor() : r(0), g(0), b(0) { }
} RgbColor;

typedef struct HsvColor
{
    unsigned char h;
    unsigned char s;
    unsigned char v;
    HsvColor() : h(0), s(0), v(0) { }
} HsvColor;


void HsvToRgb(const vec_u8& vh, const vec_u8& vs, const vec_u8& vv,
              vec_u8& vr, vec_u8& vg, vec_u8& vb)
{
    // TODO: check vector sizes

    for (size_t i = 0; i < vh.size(); i++) {
       
        HsvColor hsv;
        hsv.h = vh[i];
        hsv.s = vs[i];
        hsv.v = vv[i];
        
        RgbColor rgb;
        unsigned char region, remainder, p, q, t;

        if (hsv.s == 0)
        {
            rgb.r = hsv.v;
            rgb.g = hsv.v;
            rgb.b = hsv.v;
            vr[i] = rgb.r;
            vg[i] = rgb.g;
            vb[i] = rgb.b;
            continue;
        }

        region = hsv.h / 43;
        remainder = (hsv.h - (region * 43)) * 6; 

        p = (hsv.v * (255 - hsv.s)) >> 8;
        q = (hsv.v * (255 - ((hsv.s * remainder) >> 8))) >> 8;
        t = (hsv.v * (255 - ((hsv.s * (255 - remainder)) >> 8))) >> 8;

        switch (region)
        {
            case 0:
                rgb.r = hsv.v; rgb.g = t; rgb.b = p;
                break;
            case 1:
                rgb.r = q; rgb.g = hsv.v; rgb.b = p;
                break;
            case 2:
                rgb.r = p; rgb.g = hsv.v; rgb.b = t;
                break;
            case 3:
                rgb.r = p; rgb.g = q; rgb.b = hsv.v;
                break;
            case 4:
                rgb.r = t; rgb.g = p; rgb.b = hsv.v;
                break;
            default:
                rgb.r = hsv.v; rgb.g = p; rgb.b = q;
                break;
        }

        vr[i] = rgb.r;
        vg[i] = rgb.g;
        vb[i] = rgb.b;
    }
}



RgbColor HsvToRgb(HsvColor hsv)
{
    RgbColor rgb;
    unsigned char region, remainder, p, q, t;

    if (hsv.s == 0)
    {
        rgb.r = hsv.v;
        rgb.g = hsv.v;
        rgb.b = hsv.v;
        return rgb;
    }

    region = hsv.h / 43;
    remainder = (hsv.h - (region * 43)) * 6; 

    p = (hsv.v * (255 - hsv.s)) >> 8;
    q = (hsv.v * (255 - ((hsv.s * remainder) >> 8))) >> 8;
    t = (hsv.v * (255 - ((hsv.s * (255 - remainder)) >> 8))) >> 8;

    switch (region)
    {
        case 0:
            rgb.r = hsv.v; rgb.g = t; rgb.b = p;
            break;
        case 1:
            rgb.r = q; rgb.g = hsv.v; rgb.b = p;
            break;
        case 2:
            rgb.r = p; rgb.g = hsv.v; rgb.b = t;
            break;
        case 3:
            rgb.r = p; rgb.g = q; rgb.b = hsv.v;
            break;
        case 4:
            rgb.r = t; rgb.g = p; rgb.b = hsv.v;
            break;
        default:
            rgb.r = hsv.v; rgb.g = p; rgb.b = q;
            break;
    }

    return rgb;
}

HsvColor RgbToHsv(RgbColor rgb)
{
    HsvColor hsv;
    unsigned char rgbMin, rgbMax;

    rgbMin = rgb.r < rgb.g ? (rgb.r < rgb.b ? rgb.r : rgb.b) : (rgb.g < rgb.b ? rgb.g : rgb.b);
    rgbMax = rgb.r > rgb.g ? (rgb.r > rgb.b ? rgb.r : rgb.b) : (rgb.g > rgb.b ? rgb.g : rgb.b);

    hsv.v = rgbMax;
    if (hsv.v == 0)
    {
        hsv.h = 0;
        hsv.s = 0;
        return hsv;
    }

    hsv.s = 255 * long(rgbMax - rgbMin) / hsv.v;
    if (hsv.s == 0)
    {
        hsv.h = 0;
        return hsv;
    }

    if (rgbMax == rgb.r)
        hsv.h = 0 + 43 * (rgb.g - rgb.b) / (rgbMax - rgbMin);
    else if (rgbMax == rgb.g)
        hsv.h = 85 + 43 * (rgb.b - rgb.r) / (rgbMax - rgbMin);
    else
        hsv.h = 171 + 43 * (rgb.r - rgb.g) / (rgbMax - rgbMin);

    return hsv;
}



void HsvToRgb(const u8& h_, const u8& s_, const u8& v_,
                    u8& r_,       u8& g_,       u8& b_)
{
  HsvColor hsv;
  hsv.h = h_;
  hsv.s = s_;
  hsv.v = v_;

  RgbColor rgb = HsvToRgb(hsv);
  r_ = rgb.r;
  g_ = rgb.g;
  b_ = rgb.b;
}




void bench2()
{
    const size_t BSZ = 7200;
    vec_u8 vr(BSZ, 0);
    vec_u8 vg(BSZ, 0);
    vec_u8 vb(BSZ, 0);
    vec_u8 vh(BSZ, 0);
    vec_u8 vs(BSZ, 0);
    vec_u8 vv(BSZ, 0);

    LOGF("start");
    for (int i=0 ; i < 1000; i++) {
        // HsvToRgb(vh, vs, vv, vr, vg, vb);
        HsvToRgb(vh[i], vs[i], vv[i], vr[i], vg[i], vb[i]);
    }
    LOGF("end");
    
    unsigned int sum = 0;
    for (size_t i = 0; i < vr.size(); i++) {
      sum += vr[i];
    }
}


void bench1()
{
    std::vector<HsvColor> vhsv(7200);
    std::vector<RgbColor> vrgb(7200);  

    size_t N = vhsv.size();

    LOGF("start");
    for (int i=0 ; i < 1000; i++) {
        for (size_t j = 0; j < N; j++) {
            vrgb[j] = HsvToRgb(vhsv[j]);
        }
    }
    LOGF("end");
    
    unsigned int sum = 0;
    for (size_t i =0; i < vrgb.size(); i++) {
        sum += vrgb[i].r;
    }
}

int main()
{
    bench2();
    return 0;
}
