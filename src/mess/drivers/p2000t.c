/************************************************************************
Philips P2000 1 Memory map

    CPU: Z80
        0000-0fff   ROM
        1000-4fff   ROM (appl)
        5000-57ff   RAM (Screen T ver)
        5000-5fff   RAM (Screen M ver)
        6000-9fff   RAM (system)
        a000-ffff   RAM (extension)

    Interrupts:

    Ports:
        00-09       Keyboard input
        10-1f       Output ports
        20-2f       Input ports
        30-3f       Scroll reg (T ver)
        50-5f       Beeper
        70-7f       DISAS (M ver)
        88-8B       CTC
        8C-90       Floppy ctrl
        94          RAM Bank select

    Display: SAA5050

************************************************************************/

#include "includes/p2000t.h"


/* port i/o functions */
static ADDRESS_MAP_START( p2000t_io, AS_IO, 8, p2000t_state )
	ADDRESS_MAP_GLOBAL_MASK(0xff)
	AM_RANGE(0x00, 0x0f) AM_READ(p2000t_port_000f_r)
	AM_RANGE(0x10, 0x1f) AM_WRITE(p2000t_port_101f_w)
	AM_RANGE(0x20, 0x2f) AM_READ(p2000t_port_202f_r)
	AM_RANGE(0x30, 0x3f) AM_WRITE(p2000t_port_303f_w)
	AM_RANGE(0x50, 0x5f) AM_WRITE(p2000t_port_505f_w)
	AM_RANGE(0x70, 0x7f) AM_WRITE(p2000t_port_707f_w)
	AM_RANGE(0x88, 0x8b) AM_WRITE(p2000t_port_888b_w)
	AM_RANGE(0x8c, 0x90) AM_WRITE(p2000t_port_8c90_w)
	AM_RANGE(0x94, 0x94) AM_WRITE(p2000t_port_9494_w)
ADDRESS_MAP_END

/* Memory w/r functions */
static ADDRESS_MAP_START( p2000t_mem, AS_PROGRAM, 8, p2000t_state )
	AM_RANGE(0x0000, 0x0fff) AM_ROM
	AM_RANGE(0x1000, 0x4fff) AM_ROM
	AM_RANGE(0x5000, 0x57ff) AM_DEVREADWRITE_LEGACY("saa5050", saa5050_videoram_r, saa5050_videoram_w)
	AM_RANGE(0x5800, 0x9fff) AM_RAM
	AM_RANGE(0xa000, 0xffff) AM_NOP
ADDRESS_MAP_END

static ADDRESS_MAP_START( p2000m_mem, AS_PROGRAM, 8, p2000t_state )
	AM_RANGE(0x0000, 0x0fff) AM_ROM
	AM_RANGE(0x1000, 0x4fff) AM_ROM
	AM_RANGE(0x5000, 0x5fff) AM_RAM AM_BASE(m_p_videoram)
	AM_RANGE(0x6000, 0x9fff) AM_RAM
	AM_RANGE(0xa000, 0xffff) AM_NOP
ADDRESS_MAP_END

/* graphics output */

static const gfx_layout p2000m_charlayout =
{
	6, 10,
	256,
	1,
	{ 0 },
	{ 2, 3, 4, 5, 6, 7 },
	{ 0*8, 1*8, 2*8, 3*8, 4*8,
	  5*8, 6*8, 7*8, 8*8, 9*8 },
	8 * 10
};

static PALETTE_INIT( p2000m )
{
	palette_set_color(machine,0,RGB_WHITE); /* white */
	palette_set_color(machine,1,RGB_BLACK); /* black */
	palette_set_color(machine,2,RGB_BLACK); /* black */
	palette_set_color(machine,3,RGB_WHITE); /* white */
}

static GFXDECODE_START( p2000m )
	GFXDECODE_ENTRY( "gfx1", 0x0000, p2000m_charlayout, 0, 2 )
GFXDECODE_END

/* Keyboard input */

/* 2008-05 FP:
TO DO: verify position of the following keys: '1/4 3/4', '-> <-', '@ up', 'Clrln'
Also, notice that pictures of p2000 units shows slightly different key mappings, suggesting
many different .chr roms could exist

Small note about natural keyboard support: currently,
- "Keypad 00" and "Keypad ," are not mapped
- "Code" is mapped to 'F1'
- "Clrln" is mapped to 'F2'
*/

static INPUT_PORTS_START (p2000t)
	PORT_START("KEY0")
	PORT_BIT (0x01, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_CODE(KEYCODE_LEFT)		PORT_CHAR(UCHAR_MAMEKEY(LEFT))
	PORT_BIT (0x02, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_CODE(KEYCODE_6)			PORT_CHAR('6') PORT_CHAR('&')
	PORT_BIT (0x04, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_CODE(KEYCODE_UP)			PORT_CHAR(UCHAR_MAMEKEY(UP))
	PORT_BIT (0x08, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_CODE(KEYCODE_Q)			PORT_CHAR('q') PORT_CHAR('Q')
	PORT_BIT (0x10, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_CODE(KEYCODE_3)			PORT_CHAR('3') PORT_CHAR('\xA3')
	PORT_BIT (0x20, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_CODE(KEYCODE_5)			PORT_CHAR('5') PORT_CHAR('%')
	PORT_BIT (0x40, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_CODE(KEYCODE_7)			PORT_CHAR('7') PORT_CHAR('\'')
	PORT_BIT (0x80, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_CODE(KEYCODE_4)			PORT_CHAR('4') PORT_CHAR('$')

	PORT_START("KEY1")
	PORT_BIT (0x01, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_CODE(KEYCODE_TAB)			PORT_CHAR('\t')
	PORT_BIT (0x02, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_CODE(KEYCODE_H)			PORT_CHAR('h') PORT_CHAR('H')
	PORT_BIT (0x04, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_CODE(KEYCODE_Z)			PORT_CHAR('z') PORT_CHAR('Z')
	PORT_BIT (0x08, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_CODE(KEYCODE_S)			PORT_CHAR('s') PORT_CHAR('S')
	PORT_BIT (0x10, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_CODE(KEYCODE_D)			PORT_CHAR('d') PORT_CHAR('D')
	PORT_BIT (0x20, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_CODE(KEYCODE_G)			PORT_CHAR('g') PORT_CHAR('G')
	PORT_BIT (0x40, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_CODE(KEYCODE_J)			PORT_CHAR('j') PORT_CHAR('J')
	PORT_BIT (0x80, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_CODE(KEYCODE_F)			PORT_CHAR('f') PORT_CHAR('F')

	PORT_START("KEY2")
	PORT_BIT (0x01, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_NAME("Keypad ,")	PORT_CODE(KEYCODE_ENTER_PAD)
	PORT_BIT (0x02, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_CODE(KEYCODE_SPACE)		PORT_CHAR(' ')
	PORT_BIT (0x04, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_NAME("Keypad 00") PORT_CODE(KEYCODE_DEL_PAD)
	PORT_BIT (0x08, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_CODE(KEYCODE_0_PAD)		PORT_CHAR(UCHAR_MAMEKEY(0_PAD))
	PORT_BIT (0x10, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_NAME("#  \xE2\x96\xAA") PORT_CODE(KEYCODE_BACKSLASH) PORT_CHAR('#')
	PORT_BIT (0x20, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_CODE(KEYCODE_DOWN)		PORT_CHAR(UCHAR_MAMEKEY(DOWN))
	PORT_BIT (0x40, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_CODE(KEYCODE_COMMA)		PORT_CHAR(',')
	PORT_BIT (0x80, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_CODE(KEYCODE_RIGHT)		PORT_CHAR(UCHAR_MAMEKEY(RIGHT))

	PORT_START("KEY3")
	PORT_BIT (0x01, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_NAME("Shift Lock") PORT_CODE(KEYCODE_CAPSLOCK) PORT_CHAR(UCHAR_MAMEKEY(CAPSLOCK))
	PORT_BIT (0x02, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_CODE(KEYCODE_N)			PORT_CHAR('n') PORT_CHAR('N')
	PORT_BIT (0x04, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_CODE(KEYCODE_TILDE)		PORT_CHAR('<') PORT_CHAR('>')
	PORT_BIT (0x08, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_CODE(KEYCODE_X)			PORT_CHAR('x') PORT_CHAR('X')
	PORT_BIT (0x10, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_CODE(KEYCODE_C)			PORT_CHAR('c') PORT_CHAR('C')
	PORT_BIT (0x20, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_CODE(KEYCODE_B)			PORT_CHAR('b') PORT_CHAR('B')
	PORT_BIT (0x40, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_CODE(KEYCODE_M)			PORT_CHAR('m') PORT_CHAR('M')
	PORT_BIT (0x80, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_CODE(KEYCODE_V)			PORT_CHAR('v') PORT_CHAR('V')

	PORT_START("KEY4")
	PORT_BIT (0x01, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_NAME("Code") PORT_CODE(KEYCODE_ESC) PORT_CHAR(UCHAR_MAMEKEY(F1))
	PORT_BIT (0x02, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_CODE(KEYCODE_Y)			PORT_CHAR('y') PORT_CHAR('Y')
	PORT_BIT (0x04, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_CODE(KEYCODE_A)			PORT_CHAR('a') PORT_CHAR('A')
	PORT_BIT (0x08, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_CODE(KEYCODE_W)			PORT_CHAR('w') PORT_CHAR('W')
	PORT_BIT (0x10, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_CODE(KEYCODE_E)			PORT_CHAR('e') PORT_CHAR('E')
	PORT_BIT (0x20, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_CODE(KEYCODE_T)			PORT_CHAR('t') PORT_CHAR('T')
	PORT_BIT (0x40, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_CODE(KEYCODE_U)			PORT_CHAR('u') PORT_CHAR('U')
	PORT_BIT (0x80, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_CODE(KEYCODE_R)			PORT_CHAR('r') PORT_CHAR('R')

	PORT_START("KEY5")
	PORT_BIT (0x01, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_NAME("Clrln") PORT_CODE(KEYCODE_END) PORT_CHAR(UCHAR_MAMEKEY(F2))
	PORT_BIT (0x02, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_CODE(KEYCODE_9)			PORT_CHAR('9') PORT_CHAR(')')
	PORT_BIT (0x04, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_CODE(KEYCODE_ASTERISK)	PORT_CHAR(UCHAR_MAMEKEY(PLUS_PAD)) PORT_CHAR(UCHAR_MAMEKEY(ASTERISK))
	PORT_BIT (0x08, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_CODE(KEYCODE_SLASH_PAD)	PORT_CHAR(UCHAR_MAMEKEY(MINUS_PAD)) PORT_CHAR(UCHAR_MAMEKEY(SLASH_PAD))
	PORT_BIT (0x10, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_CODE(KEYCODE_BACKSPACE)	PORT_CHAR(8)
	PORT_BIT (0x20, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_CODE(KEYCODE_0)			PORT_CHAR('0') PORT_CHAR('=')
	PORT_BIT (0x40, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_CODE(KEYCODE_1)			PORT_CHAR('1') PORT_CHAR('!')
	PORT_BIT (0x80, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_CODE(KEYCODE_MINUS)		PORT_CHAR(0xFF0D)

	PORT_START("KEY6")
	PORT_BIT (0x01, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_CODE(KEYCODE_9_PAD)		PORT_CHAR(UCHAR_MAMEKEY(9_PAD))
	PORT_BIT (0x02, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_CODE(KEYCODE_O)			PORT_CHAR('o') PORT_CHAR('O')
	PORT_BIT (0x04, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_CODE(KEYCODE_8_PAD)		PORT_CHAR(UCHAR_MAMEKEY(8_PAD))
	PORT_BIT (0x08, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_CODE(KEYCODE_7_PAD)		PORT_CHAR(UCHAR_MAMEKEY(7_PAD))
	PORT_BIT (0x10, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_CODE(KEYCODE_ENTER)		PORT_CHAR(13)
	PORT_BIT (0x20, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_CODE(KEYCODE_P)			PORT_CHAR('p') PORT_CHAR('P')
	PORT_BIT (0x40, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_CODE(KEYCODE_8)			PORT_CHAR('8') PORT_CHAR('(')
	PORT_BIT (0x80, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_NAME("@  \xE2\x86\x91") PORT_CODE(KEYCODE_OPENBRACE) PORT_CHAR('@')

	PORT_START("KEY7")
	PORT_BIT (0x01, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_CODE(KEYCODE_3_PAD)		PORT_CHAR(UCHAR_MAMEKEY(3_PAD))
	PORT_BIT (0x02, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_CODE(KEYCODE_STOP)		PORT_CHAR('.')
	PORT_BIT (0x04, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_CODE(KEYCODE_2_PAD)		PORT_CHAR(UCHAR_MAMEKEY(2_PAD))
	PORT_BIT (0x08, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_CODE(KEYCODE_1_PAD)		PORT_CHAR(UCHAR_MAMEKEY(1_PAD))
	PORT_BIT (0x10, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_NAME("\xE2\x86\x92  \xE2\x86\x90") PORT_CODE(KEYCODE_CLOSEBRACE)
	PORT_BIT (0x20, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_CODE(KEYCODE_SLASH)		PORT_CHAR('/') PORT_CHAR('?')
	PORT_BIT (0x40, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_CODE(KEYCODE_K)			PORT_CHAR('k') PORT_CHAR('K')
	PORT_BIT (0x80, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_CODE(KEYCODE_2)			PORT_CHAR('2') PORT_CHAR('"')

	PORT_START("KEY8")
	PORT_BIT (0x01, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_CODE(KEYCODE_6_PAD)		PORT_CHAR(UCHAR_MAMEKEY(6_PAD))
	PORT_BIT (0x02, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_CODE(KEYCODE_L)			PORT_CHAR('l') PORT_CHAR('L')
	PORT_BIT (0x04, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_CODE(KEYCODE_5_PAD)		PORT_CHAR(UCHAR_MAMEKEY(5_PAD))
	PORT_BIT (0x08, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_CODE(KEYCODE_4_PAD)		PORT_CHAR(UCHAR_MAMEKEY(4_PAD))
	PORT_BIT (0x10, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_CODE(KEYCODE_EQUALS)		PORT_CHAR(0x00BC) PORT_CHAR(0x00BE)
	PORT_BIT (0x20, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_CODE(KEYCODE_COLON)		PORT_CHAR(';') PORT_CHAR('+')
	PORT_BIT (0x40, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_CODE(KEYCODE_I)			PORT_CHAR('i') PORT_CHAR('I')
	PORT_BIT (0x80, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_CODE(KEYCODE_QUOTE)		PORT_CHAR(':') PORT_CHAR('*')

	PORT_START("KEY9")
	PORT_BIT (0x01, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_NAME("Shift (Left)") PORT_CODE(KEYCODE_LSHIFT) PORT_CHAR(UCHAR_SHIFT_1)
	PORT_BIT (0x02, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_NAME("N/C")
	PORT_BIT (0x04, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_NAME("N/C")
	PORT_BIT (0x08, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_NAME("N/C")
	PORT_BIT (0x10, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_NAME("N/C")
	PORT_BIT (0x20, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_NAME("N/C")
	PORT_BIT (0x40, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_NAME("N/C")
	PORT_BIT (0x80, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_NAME("Shift (Right)") PORT_CODE(KEYCODE_RSHIFT) PORT_CHAR(UCHAR_SHIFT_1)
INPUT_PORTS_END


static INTERRUPT_GEN( p2000_interrupt )
{
	cputag_set_input_line(device->machine(), "maincpu", 0, HOLD_LINE);
}


static SCREEN_UPDATE_IND16( p2000t )
{
	device_t *saa5050 = screen.machine().device("saa5050");

	saa5050_update(saa5050, bitmap, cliprect);
	return 0;
}

static const saa5050_interface p2000t_saa5050_intf =
{
	"screen",
	0,	/* starting gfxnum */
	40, 24 - 1, 80,  /* x, y, size */
	0	/* rev y order */
};

/* Machine definition */
static MACHINE_CONFIG_START( p2000t, p2000t_state )
	/* basic machine hardware */
	MCFG_CPU_ADD("maincpu", Z80, 2500000)
	MCFG_CPU_PROGRAM_MAP(p2000t_mem)
	MCFG_CPU_IO_MAP(p2000t_io)
	MCFG_CPU_VBLANK_INT("screen", p2000_interrupt)

	/* video hardware */
	MCFG_SCREEN_ADD("screen", RASTER)
	MCFG_SCREEN_REFRESH_RATE(50)
	MCFG_SCREEN_VBLANK_TIME(ATTOSECONDS_IN_USEC(SAA5050_VBLANK))
	MCFG_SCREEN_SIZE(40 * 12, 24 * 20)
	MCFG_SCREEN_VISIBLE_AREA(0, 40 * 12 - 1, 0, 24 * 20 - 1)
	MCFG_SCREEN_UPDATE_STATIC(p2000t)
	MCFG_GFXDECODE(saa5050)
	MCFG_PALETTE_LENGTH(128)
	MCFG_PALETTE_INIT(saa5050)

	MCFG_SAA5050_ADD("saa5050", p2000t_saa5050_intf)

	/* sound hardware */
	MCFG_SPEAKER_STANDARD_MONO("mono")
	MCFG_SOUND_ADD(SPEAKER_TAG, SPEAKER_SOUND, 0)
	MCFG_SOUND_ROUTE(ALL_OUTPUTS, "mono", 0.25)
MACHINE_CONFIG_END


/* Machine definition */
static MACHINE_CONFIG_START( p2000m, p2000t_state )
	/* basic machine hardware */
	MCFG_CPU_ADD("maincpu", Z80, 2500000)
	MCFG_CPU_PROGRAM_MAP(p2000m_mem)
	MCFG_CPU_IO_MAP(p2000t_io)
	MCFG_CPU_VBLANK_INT("screen", p2000_interrupt)
	MCFG_QUANTUM_TIME(attotime::from_hz(60))

	/* video hardware */
	MCFG_SCREEN_ADD("screen", RASTER)
	MCFG_SCREEN_REFRESH_RATE(50)
	MCFG_SCREEN_VBLANK_TIME(ATTOSECONDS_IN_USEC(2500)) /* not accurate */
	MCFG_SCREEN_SIZE(80 * 12, 24 * 20)
	MCFG_SCREEN_VISIBLE_AREA(0, 80 * 12 - 1, 0, 24 * 20 - 1)
	MCFG_VIDEO_START(p2000m)
	MCFG_SCREEN_UPDATE_STATIC(p2000m)
	MCFG_GFXDECODE( p2000m )
	MCFG_PALETTE_LENGTH(4)
	MCFG_PALETTE_INIT(p2000m)

	/* sound hardware */
	MCFG_SPEAKER_STANDARD_MONO("mono")
	MCFG_SOUND_ADD(SPEAKER_TAG, SPEAKER_SOUND, 0)
	MCFG_SOUND_ROUTE(ALL_OUTPUTS, "mono", 0.25)
MACHINE_CONFIG_END


ROM_START(p2000t)
	ROM_REGION(0x10000, "maincpu",0)
	ROM_LOAD("p2000.rom", 0x0000, 0x1000, CRC(650784a3) SHA1(4dbb28adad30587f2ea536ba116898d459faccac))
	ROM_LOAD("basic.rom", 0x1000, 0x4000, CRC(9d9d38f9) SHA1(fb5100436c99634a2592a10dff867f85bcff7aec))
	ROM_REGION(0x01000, "gfx1",0)
	ROM_LOAD("p2000.chr", 0x0140, 0x08c0, BAD_DUMP CRC(78c17e3e) SHA1(4e1c59dc484505de1dc0b1ba7e5f70a54b0d4ccc))
ROM_END

ROM_START(p2000m)
	ROM_REGION(0x10000, "maincpu",0)
	ROM_LOAD("p2000.rom", 0x0000, 0x1000, CRC(650784a3) SHA1(4dbb28adad30587f2ea536ba116898d459faccac))
	ROM_LOAD("basic.rom", 0x1000, 0x4000, CRC(9d9d38f9) SHA1(fb5100436c99634a2592a10dff867f85bcff7aec))
	ROM_REGION(0x01000, "gfx1",0)
	ROM_LOAD("p2000.chr", 0x0140, 0x08c0, BAD_DUMP CRC(78c17e3e) SHA1(4e1c59dc484505de1dc0b1ba7e5f70a54b0d4ccc))
ROM_END

/*      YEAR    NAME    PARENT  COMPAT  MACHINE     INPUT       INIT      COMPANY     FULLNAME */
COMP ( 1980,    p2000t, 0,      0,      p2000t,     p2000t,     0,       "Philips", "Philips P2000T", 0)
COMP ( 1980,    p2000m, p2000t, 0,      p2000m,     p2000t,     0,       "Philips", "Philips P2000M", 0)
