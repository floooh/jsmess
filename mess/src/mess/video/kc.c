/***************************************************************************

  kc.c

  Functions to emulate the video hardware of the kc85/4,kc85/3

***************************************************************************/

#include "emu.h"
#include "includes/kc.h"
#include "machine/ram.h"

/* KC85/4 and KC85/3 common graphics hardware */

static const unsigned short kc85_colour_table[KC85_PALETTE_SIZE] =
{
	0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
	16, 17, 18, 19, 20, 21, 22, 23
};

/*
    foreground:

        "full" of each component

        black,
        blue,
        red,
        magenta,
        green,
        cyan
        yellow
        white

        "full of each component + half of another component"
        black
        violet
        red/purple
        pastel green
        sky blue
        yellow/green
        white

    background:
        "half" of each component
        black
        dark blue
        dark red
        dark magenta
        dark green
        dark cyan
        dark yellow
        dark white (grey)

 */


static const unsigned char kc85_palette[KC85_PALETTE_SIZE * 3] =
{
		/* 3 bit colour value. bit 2->green, bit 1->red, bit 0->blue */

		/* foreground colours */
		0x00, 0x00, 0x00,
		0x00, 0x00, 0xFF,
		0xFF, 0x00, 0x00,
		0xFF, 0x00, 0xFF,
		0x00, 0xFF, 0x00,
		0x00, 0xFF, 0xFF,
		0xFF, 0xFF, 0x00,
		0xFF, 0xFF, 0xFF,

		0x00, 0x00, 0x00,
		0xA0, 0x00, 0xFF,
		0xFF, 0xA0, 0x00,
		0xFF, 0x00, 0xA0,
		0x00, 0xFF, 0xA0,
		0x00, 0xA0, 0xFF,
		0xA0, 0xFF, 0x00,
		0xFF, 0xFF, 0xFF,

		/* background colours are slightly darker than foreground colours */
		0x00, 0x00, 0x00,
		0x00, 0x00, 0xA0,
		0xA0, 0x00, 0x00,
		0xA0, 0x00, 0xA0,
		0x00, 0xA0, 0x00,
		0x00, 0xA0, 0xA0,
		0xA0, 0xA0, 0x00,
		0xA0, 0xA0, 0xA0

};

// video addressing lookup tables (lookup with Y coordinate)
struct kc85_3_video_addr_lookup_entry
{
	int left_pixel_offset;
	int left_color_offset;
	int right_pixel_offset;
	int right_color_offset;
};

static kc85_3_video_addr_lookup_entry kc85_3_video_addr_lookup_table[256];

// setup Y video address lookup tables
/* 1  0  1  0  0  V7 V6 V1  V0 V3 V2 V5 V4 H2 H1 H0 */
/* 1  0  1  1  0  0  0  V7  V6 V3 V2 V5 V4 H2 H1 H0 */

static void kc85_3_setup_video_lookup_tables()
{
	int y;
	for (y = 0; y < 256; y++)
	{
		kc85_3_video_addr_lookup_entry& entry = kc85_3_video_addr_lookup_table[y];
		entry.left_pixel_offset  = (((y>>2) & 0x03)<<5) | ((y & 0x03)<<7) | (((y>>4) & 0x0f)<<9);
		entry.left_color_offset  = (((y>>2) & 0x03f)<<5);
		entry.right_pixel_offset = (((y>>4) & 0x03)<<3) | (((y>>2) & 0x03)<<5) | ((y & 0x03)<<7) | (((y>>6) & 0x03)<<9);
		entry.right_color_offset = (((y>>4) & 0x03)<<3) | (((y>>2) & 0x03)<<5) | (((y>>6) & 0x03)<<7);
	}
}

/* Initialise the palette */
PALETTE_INIT( kc85 )
{
	int i;

	for ( i = 0; i < sizeof(kc85_palette) / 3; i++ ) {
		palette_set_color_rgb(machine, i, kc85_palette[i*3], kc85_palette[i*3+1], kc85_palette[i*3+2]);
	}
}

enum
{
	KC85_VIDEO_EVENT_SET_BLINK_STATE
};

/* set new blink state - record blink state in event list */
void kc85_video_set_blink_state(running_machine &machine, int data)
{
	//spectrum_EventList_AddItemOffset(machine, KC85_VIDEO_EVENT_SET_BLINK_STATE, ((data & 0x01)<<7), machine.firstcpu->attotime_to_cycles(machine.primary_screen->scan_period() * machine.primary_screen->vpos()));
}

void kc85_draw_8_pixels(kc_state *state, bitmap_t *bitmap,int x,int y, unsigned char colour_byte, unsigned char gfx_byte)
{
	int pens[4];
	int px;

	/* Low resolution: 2 colors for block */
	/* 16 foreground colours, 8 background colours */

	/* bit 7 = 1: flash between foreground and background colour 0: no flash */
	/* bit 6: adjusts foreground colours by adding half of another component */
	/* bit 5,4,3 = foreground colour */
		/* bit 5: background colour -> Green */
		/* bit 4: background colour -> Red */
		/* bit 3: background colour -> Blue */
	/* bit 2,1,0 = background colour */
		/* bit 2: background colour -> Green */
		/* bit 1: background colour -> Red */
		/* bit 0: background colour -> Blue */

	int background_pen = (colour_byte&7) + 16;
	int foreground_pen = ((colour_byte>>3) & 0x0f);

	if ((colour_byte & 0x80) && state->m_kc85_blink_state)
	{
		foreground_pen = background_pen;
	}

	pens[0] = background_pen;
	pens[1] = foreground_pen;

	px = x;

	const int bitmap_width = bitmap->width;
	const int bitmap_height = bitmap->height;
	for (int a=0; a<8; a++)
	{
		int pen = pens[(gfx_byte>>7) & 0x01];

		if ((px >= 0) && (px < bitmap_width) && (y >= 0) && (y < bitmap_height))
		{
			*BITMAP_ADDR16(bitmap, y, px) = pen;
		}
		px++;
		gfx_byte = gfx_byte<<1;
	}
}

/***************************************************************************
 KC85/4 video hardware
***************************************************************************/
static void kc85_common_vh_start(running_machine &machine)
{
	kc_state *state = machine.driver_data<kc_state>();
	state->m_kc85_blink_state = 0;
	//spectrum_EventList_Initialise(machine, 30000);
}

VIDEO_START( kc85_4 )
{
	kc_state *state = machine.driver_data<kc_state>();
	kc85_common_vh_start(machine);

	state->m_kc85_4_video_ram = auto_alloc_array(machine, UINT8,
		(KC85_4_SCREEN_COLOUR_RAM_SIZE*2) +
		(KC85_4_SCREEN_PIXEL_RAM_SIZE*2));

	state->m_kc85_4_display_video_ram = state->m_kc85_4_video_ram;
}

void kc85_4_video_ram_select_bank(running_machine &machine, int bank)
{
	kc_state *state = machine.driver_data<kc_state>();
	/* calculate address of video ram to display */
	unsigned char *video_ram;

	video_ram = state->m_kc85_4_video_ram;

	if (bank!=0)
	{
		video_ram +=
				   (KC85_4_SCREEN_PIXEL_RAM_SIZE +
				   KC85_4_SCREEN_COLOUR_RAM_SIZE);
	}

	state->m_kc85_4_display_video_ram = video_ram;
}

unsigned char *kc85_4_get_video_ram_base(running_machine &machine, int bank, int colour)
{
	kc_state *state = machine.driver_data<kc_state>();
	/* base address: screen 0 pixel data */
	unsigned char *addr = state->m_kc85_4_video_ram;

	if (bank!=0)
	{
		/* access screen 1 */
		addr += KC85_4_SCREEN_PIXEL_RAM_SIZE +
				KC85_4_SCREEN_COLOUR_RAM_SIZE;
	}

	if (colour!=0)
	{
		/* access colour information of selected screen */
		addr += KC85_4_SCREEN_PIXEL_RAM_SIZE;
	}

	return addr;
}

/***************************************************************************
  Draw the game screen in the given bitmap_t.
  Do NOT call osd_update_display() from this function,
  it will be called by the main emulation engine.
***************************************************************************/
SCREEN_UPDATE( kc85_4 )
{
	kc_state *state = screen->machine().driver_data<kc_state>();

	UINT8 *pixel_ram = state->m_kc85_4_display_video_ram;
	UINT8 *colour_ram = pixel_ram + 0x04000;

	const int min_y = cliprect->min_y;
	const int max_y = cliprect->max_y;
	const int min_x = cliprect->min_x >> 3;
	const int max_x = (cliprect->max_x + 1) >> 3;

	for (int y = min_y; y <= max_y; y++)
	{
		for (int x = min_x; x <= max_x; x++)
		{
			UINT16 offset = y | (x<<8);

			UINT8 colour_byte = colour_ram[offset];
			UINT8 gfx_byte = pixel_ram[offset];

			kc85_draw_8_pixels(state, bitmap, (x<<3), y, colour_byte, gfx_byte);
		}
	}
	return 0;
}

/***************************************************************************
 KC85/3 video
***************************************************************************/

VIDEO_START( kc85_3 )
{
	kc85_common_vh_start(machine);
	kc85_3_setup_video_lookup_tables();
}

/***************************************************************************
  Draw the game screen in the given bitmap_t.
  Do NOT call osd_update_display() from this function,
  it will be called by the main emulation engine.
***************************************************************************/
SCREEN_UPDATE( kc85_3 )
{
	kc_state *state = screen->machine().driver_data<kc_state>();

	UINT8* pixel_ram = ram_get_ptr(screen->machine().device(RAM_TAG))+0x08000;
	UINT8* colour_ram = pixel_ram + 0x02800;

	const int min_y = cliprect->min_y;
	const int max_y = cliprect->max_y;
	const int min_x = cliprect->min_x >> 3;
	const int max_x = cliprect->max_x >> 3;

	int pixel_offset;
	int colour_offset;
	for (int y = min_y; y <= max_y; y++)
	{
		// FLOH: why is the Y offset so complicated???

		/* 1  0  1  0  0  V7 V6 V1  V0 V3 V2 V5 V4 H2 H1 H0 */
		/* 1  0  1  1  0  0  0  V7  V6 V3 V2 V5 V4 H2 H1 H0 */
		const kc85_3_video_addr_lookup_entry& lookup_y = kc85_3_video_addr_lookup_table[y];

		for (int x = min_x; x <= max_x; x++)
		{
			if (x < 0x020)
			{
				// left 256x256 area of the screen
				pixel_offset  = x | lookup_y.left_pixel_offset;
				colour_offset = x | lookup_y.left_color_offset;
			}
			else
			{
				// right 64x256 area of the screen
				pixel_offset  = 0x02000 + ((x & 0x07) | lookup_y.right_pixel_offset);
				colour_offset = 0x0800 + ((x & 0x07) | lookup_y.right_color_offset);
			}

			UINT8 colour_byte = colour_ram[colour_offset];
			UINT8 gfx_byte = pixel_ram[pixel_offset];
			kc85_draw_8_pixels(state, bitmap, (x<<3), y, colour_byte, gfx_byte);
		}
	}
	return 0;
}
