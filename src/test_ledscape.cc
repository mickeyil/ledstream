#include "slog.h"

extern "C" {
  #include "ledscape.h"
}

static const char * path_pru0 = "/home/debian/dev/LedBurnBBB/pru/bin/ws281x-come-million-box-pru0.bin";
static const char * path_pru1 = "/home/debian/dev/LedBurnBBB/pru/bin/ws281x-come-million-box-pru1.bin";
static const int PIXELS_PER_STRAND = 144;



ledscape_frame_t * get_next_framebuffer(ledscape_t * leds)
{
  static int buffer_index = 0;
	ledscape_frame_t * frame = ledscape_frame(leds, buffer_index);
	buffer_index = (buffer_index + 1) % 2;
  return frame;
}


ledscape_t * StartLedScape()
{
  ledscape_t *leds = NULL;
	LOGF("[main] Starting LEDscape...");

	leds = ledscape_init_with_programs(
		PIXELS_PER_STRAND,
		path_pru0,
		path_pru1
	);		
	
	LOGF("[main] Done Starting LEDscape...");
  return leds;
}

int main()
{
  ledscape_t *leds = StartLedScape();
  return 0;
}
