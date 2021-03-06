/***************************************************************************

  machine.c

  Functions to emulate general aspects of PMD-85 (RAM, ROM, interrupts,
  I/O ports)

  Krzysztof Strzecha

***************************************************************************/

#include "emu.h"
#include "imagedev/cassette.h"
#include "cpu/i8085/i8085.h"
#include "machine/i8255.h"
#include "includes/pmd85.h"
#include "machine/msm8251.h"
#include "machine/pit8253.h"
#include "machine/ram.h"



enum {PMD85_LED_1, PMD85_LED_2, PMD85_LED_3};
enum {PMD85_1, PMD85_2, PMD85_2A, PMD85_2B, PMD85_3, ALFA, MATO, C2717};



static void pmd851_update_memory(running_machine &machine)
{
	pmd85_state *state = machine.driver_data<pmd85_state>();
	address_space* space = machine.device("maincpu")->memory().space(AS_PROGRAM);
	UINT8 *ram = ram_get_ptr(machine.device(RAM_TAG));

	if (state->m_startup_mem_map)
	{
		UINT8 *mem = machine.region("maincpu")->base();

		space->unmap_write(0x0000, 0x0fff);
		space->nop_write(0x1000, 0x1fff);
		space->unmap_write(0x2000, 0x2fff);
		space->nop_write(0x3000, 0x3fff);

		space->nop_read(0x1000, 0x1fff);
		space->nop_read(0x3000, 0x3fff);

		memory_set_bankptr(machine, "bank1", mem + 0x010000);
		memory_set_bankptr(machine, "bank3", mem + 0x010000);
		memory_set_bankptr(machine, "bank5", ram + 0xc000);

		memory_set_bankptr(machine, "bank6", mem + 0x010000);
		memory_set_bankptr(machine, "bank7", mem + 0x010000);
		memory_set_bankptr(machine, "bank8", ram + 0xc000);
	}
	else
	{
		space->install_write_bank(0x0000, 0x0fff, "bank1");
		space->install_write_bank(0x1000, 0x1fff, "bank2");
		space->install_write_bank(0x2000, 0x2fff, "bank3");
		space->install_write_bank(0x3000, 0x3fff, "bank4");
		space->install_write_bank(0x4000, 0x7fff, "bank5");

		space->install_read_bank(0x1000, 0x1fff, "bank2");
		space->install_read_bank(0x3000, 0x3fff, "bank4");

		memory_set_bankptr(machine, "bank1", ram);
		memory_set_bankptr(machine, "bank2", ram + 0x1000);
		memory_set_bankptr(machine, "bank3", ram + 0x2000);
		memory_set_bankptr(machine, "bank4", ram + 0x3000);
		memory_set_bankptr(machine, "bank5", ram + 0x4000);
	}
}

static void pmd852a_update_memory(running_machine &machine)
{
	pmd85_state *state = machine.driver_data<pmd85_state>();
	address_space* space = machine.device("maincpu")->memory().space(AS_PROGRAM);
	UINT8 *ram = ram_get_ptr(machine.device(RAM_TAG));

	if (state->m_startup_mem_map)
	{
		UINT8 *mem = machine.region("maincpu")->base();

		space->unmap_write(0x0000, 0x0fff);
		space->unmap_write(0x2000, 0x2fff);

		memory_set_bankptr(machine, "bank1", mem + 0x010000);
		memory_set_bankptr(machine, "bank2", ram + 0x9000);
		memory_set_bankptr(machine, "bank3", mem + 0x010000);
		memory_set_bankptr(machine, "bank4", ram + 0xb000);
		memory_set_bankptr(machine, "bank5", ram + 0xc000);
		memory_set_bankptr(machine, "bank6", mem + 0x010000);
		memory_set_bankptr(machine, "bank7", ram + 0x9000);
		memory_set_bankptr(machine, "bank8", mem + 0x010000);
		memory_set_bankptr(machine, "bank9", ram + 0xb000);
		memory_set_bankptr(machine, "bank10", ram + 0xc000);

	}
	else
	{
		space->install_write_bank(0x0000, 0x0fff, "bank1");
		space->install_write_bank(0x2000, 0x2fff, "bank3");

		memory_set_bankptr(machine, "bank1", ram);
		memory_set_bankptr(machine, "bank2", ram + 0x1000);
		memory_set_bankptr(machine, "bank3", ram + 0x2000);
		memory_set_bankptr(machine, "bank4", ram + 0x5000);
		memory_set_bankptr(machine, "bank5", ram + 0x4000);
	}
}

static void pmd853_update_memory(running_machine &machine)
{
	pmd85_state *state = machine.driver_data<pmd85_state>();
	UINT8 *mem = machine.region("maincpu")->base();
	UINT8 *ram = ram_get_ptr(machine.device(RAM_TAG));

	if (state->m_startup_mem_map)
	{
		memory_set_bankptr(machine, "bank1", mem + 0x010000);
		memory_set_bankptr(machine, "bank2", mem + 0x010000);
		memory_set_bankptr(machine, "bank3", mem + 0x010000);
		memory_set_bankptr(machine, "bank4", mem + 0x010000);
		memory_set_bankptr(machine, "bank5", mem + 0x010000);
		memory_set_bankptr(machine, "bank6", mem + 0x010000);
		memory_set_bankptr(machine, "bank7", mem + 0x010000);
		memory_set_bankptr(machine, "bank8", mem + 0x010000);
		memory_set_bankptr(machine, "bank9", ram);
		memory_set_bankptr(machine, "bank10", ram + 0x2000);
		memory_set_bankptr(machine, "bank11", ram + 0x4000);
		memory_set_bankptr(machine, "bank12", ram + 0x6000);
		memory_set_bankptr(machine, "bank13", ram + 0x8000);
		memory_set_bankptr(machine, "bank14", ram + 0xa000);
		memory_set_bankptr(machine, "bank15", ram + 0xc000);
		memory_set_bankptr(machine, "bank16", ram + 0xe000);
	}
	else
	{
		memory_set_bankptr(machine,  "bank1", ram);
		memory_set_bankptr(machine,  "bank2", ram + 0x2000);
		memory_set_bankptr(machine,  "bank3", ram + 0x4000);
		memory_set_bankptr(machine,  "bank4", ram + 0x6000);
		memory_set_bankptr(machine,  "bank5", ram + 0x8000);
		memory_set_bankptr(machine,  "bank6", ram + 0xa000);
		memory_set_bankptr(machine,  "bank7", ram + 0xc000);
		memory_set_bankptr(machine,  "bank8", state->m_pmd853_memory_mapping ? mem + 0x010000 : ram + 0xe000);
	}
}

static void alfa_update_memory(running_machine &machine)
{
	pmd85_state *state = machine.driver_data<pmd85_state>();
	address_space* space = machine.device("maincpu")->memory().space(AS_PROGRAM);
	UINT8 *ram = ram_get_ptr(machine.device(RAM_TAG));

	if (state->m_startup_mem_map)
	{
		UINT8 *mem = machine.region("maincpu")->base();

		space->unmap_write(0x0000, 0x0fff);
		space->unmap_write(0x1000, 0x33ff);
		space->nop_write(0x3400, 0x3fff);

		memory_set_bankptr(machine, "bank1", mem + 0x010000);
		memory_set_bankptr(machine, "bank2", mem + 0x011000);
		memory_set_bankptr(machine, "bank4", ram + 0xc000);
		memory_set_bankptr(machine, "bank5", mem + 0x010000);
		memory_set_bankptr(machine, "bank6", mem + 0x011000);
		memory_set_bankptr(machine, "bank7", ram + 0xc000);
	}
	else
	{
		space->install_write_bank(0x0000, 0x0fff, "bank1");
		space->install_write_bank(0x1000, 0x33ff, "bank2");
		space->install_write_bank(0x3400, 0x3fff, "bank3");

		memory_set_bankptr(machine, "bank1", ram);
		memory_set_bankptr(machine, "bank2", ram + 0x1000);
		memory_set_bankptr(machine, "bank3", ram + 0x3400);
		memory_set_bankptr(machine, "bank4", ram + 0x4000);
	}
}

static void mato_update_memory(running_machine &machine)
{
	pmd85_state *state = machine.driver_data<pmd85_state>();
	address_space* space = machine.device("maincpu")->memory().space(AS_PROGRAM);
	UINT8 *ram = ram_get_ptr(machine.device(RAM_TAG));

	if (state->m_startup_mem_map)
	{
		UINT8 *mem = machine.region("maincpu")->base();

		space->unmap_write(0x0000, 0x3fff);

		memory_set_bankptr(machine, "bank1", mem + 0x010000);
		memory_set_bankptr(machine, "bank2", ram + 0xc000);
		memory_set_bankptr(machine, "bank3", mem + 0x010000);
		memory_set_bankptr(machine, "bank4", ram + 0xc000);
	}
	else
	{
		space->install_write_bank(0x0000, 0x3fff, "bank1");

		memory_set_bankptr(machine, "bank1", ram);
		memory_set_bankptr(machine, "bank2", ram + 0x4000);
	}
}

static void c2717_update_memory(running_machine &machine)
{
	pmd85_state *state = machine.driver_data<pmd85_state>();
	address_space* space = machine.device("maincpu")->memory().space(AS_PROGRAM);
	UINT8 *mem = machine.region("maincpu")->base();
	UINT8 *ram = ram_get_ptr(machine.device(RAM_TAG));

	if (state->m_startup_mem_map)
	{
		space->unmap_write(0x0000, 0x3fff);

		memory_set_bankptr(machine, "bank1", mem + 0x010000);
		memory_set_bankptr(machine, "bank2", ram + 0x4000);
		memory_set_bankptr(machine, "bank3", mem + 0x010000);
		memory_set_bankptr(machine, "bank4", ram + 0xc000);
	}
	else
	{
		space->install_write_bank(0x0000, 0x3fff, "bank1");
		memory_set_bankptr(machine, "bank1", ram);
		memory_set_bankptr(machine, "bank2", ram + 0x4000);
	}
}

/*******************************************************************************

    Motherboard 8255 (PMD-85.1, PMD-85.2, PMD-85.3, Didaktik Alfa)
    --------------------------------------------------------------
        keyboard, speaker, LEDs

*******************************************************************************/

static  READ8_DEVICE_HANDLER ( pmd85_ppi_0_porta_r )
{
	return 0xff;
}

static  READ8_DEVICE_HANDLER ( pmd85_ppi_0_portb_r )
{
	pmd85_state *state = device->machine().driver_data<pmd85_state>();
	static const char *const keynames[] = {
		"KEY0", "KEY1", "KEY2", "KEY3", "KEY4", "KEY5", "KEY6", "KEY7",
		"KEY8", "KEY9", "KEY10", "KEY11", "KEY12", "KEY13", "KEY14", "KEY15"
	};

	return input_port_read(device->machine(), keynames[(state->m_ppi_port_outputs[0][0] & 0x0f)]) & input_port_read(device->machine(), "KEY15");
}

static  READ8_DEVICE_HANDLER ( pmd85_ppi_0_portc_r )
{
	return 0xff;
}

static WRITE8_DEVICE_HANDLER ( pmd85_ppi_0_porta_w )
{
	pmd85_state *state = device->machine().driver_data<pmd85_state>();
	state->m_ppi_port_outputs[0][0] = data;
}

static WRITE8_DEVICE_HANDLER ( pmd85_ppi_0_portb_w )
{
	pmd85_state *state = device->machine().driver_data<pmd85_state>();
	state->m_ppi_port_outputs[0][1] = data;
}

static WRITE8_DEVICE_HANDLER ( pmd85_ppi_0_portc_w )
{
	pmd85_state *state = device->machine().driver_data<pmd85_state>();
	state->m_ppi_port_outputs[0][2] = data;
	set_led_status(device->machine(), PMD85_LED_2, (data & 0x08) ? 1 : 0);
	set_led_status(device->machine(), PMD85_LED_3, (data & 0x04) ? 1 : 0);
}

/*******************************************************************************

    Motherboard 8255 (Mato)
    -----------------------
        keyboard, speaker, LEDs, tape

*******************************************************************************/

static  READ8_DEVICE_HANDLER ( mato_ppi_0_portb_r )
{
	pmd85_state *state = device->machine().driver_data<pmd85_state>();
	int i;
	UINT8 data = 0xff;
	static const char *const keynames[] = { "KEY0", "KEY1", "KEY2", "KEY3", "KEY4", "KEY5", "KEY6", "KEY7" };

	for (i = 0; i < 8; i++)
	{
		if (!BIT(state->m_ppi_port_outputs[0][0], i))
			data &= input_port_read(device->machine(), keynames[i]);
	}
	return data;
}

static  READ8_DEVICE_HANDLER ( mato_ppi_0_portc_r )
{
	return input_port_read(device->machine(), "KEY8") | 0x8f;
}

static WRITE8_DEVICE_HANDLER ( mato_ppi_0_portc_w )
{
	pmd85_state *state = device->machine().driver_data<pmd85_state>();
	state->m_ppi_port_outputs[0][2] = data;
	set_led_status(device->machine(), PMD85_LED_2, BIT(data, 3));
	set_led_status(device->machine(), PMD85_LED_3, BIT(data, 2));
}

/*******************************************************************************

    I/O board 8255
    --------------
        GPIO/0 (K3 connector), GPIO/1 (K4 connector)

*******************************************************************************/

static READ8_DEVICE_HANDLER ( pmd85_ppi_1_porta_r )
{
	return 0xff;
}

static READ8_DEVICE_HANDLER ( pmd85_ppi_1_portb_r )
{
	return 0xff;
}

static READ8_DEVICE_HANDLER ( pmd85_ppi_1_portc_r )
{
	return 0xff;
}

static WRITE8_DEVICE_HANDLER ( pmd85_ppi_1_porta_w )
{
	pmd85_state *state = device->machine().driver_data<pmd85_state>();
	state->m_ppi_port_outputs[1][0] = data;
}

static WRITE8_DEVICE_HANDLER ( pmd85_ppi_1_portb_w )
{
	pmd85_state *state = device->machine().driver_data<pmd85_state>();
	state->m_ppi_port_outputs[1][1] = data;
}

static WRITE8_DEVICE_HANDLER ( pmd85_ppi_1_portc_w )
{
	pmd85_state *state = device->machine().driver_data<pmd85_state>();
	state->m_ppi_port_outputs[1][2] = data;
}

/*******************************************************************************

    I/O board 8255
    --------------
        IMS-2 (K5 connector)

    - 8251 - cassette recorder and V.24/IFSS (selectable by switch)

    - external interfaces connector (K2)

*******************************************************************************/

static READ8_DEVICE_HANDLER ( pmd85_ppi_2_porta_r )
{
	return 0xff;
}

static READ8_DEVICE_HANDLER ( pmd85_ppi_2_portb_r )
{
	return 0xff;
}

static READ8_DEVICE_HANDLER ( pmd85_ppi_2_portc_r )
{
	return 0xff;
}

static WRITE8_DEVICE_HANDLER ( pmd85_ppi_2_porta_w )
{
	pmd85_state *state = device->machine().driver_data<pmd85_state>();
	state->m_ppi_port_outputs[2][0] = data;
}

static WRITE8_DEVICE_HANDLER ( pmd85_ppi_2_portb_w )
{
	pmd85_state *state = device->machine().driver_data<pmd85_state>();
	state->m_ppi_port_outputs[2][1] = data;
}

static WRITE8_DEVICE_HANDLER ( pmd85_ppi_2_portc_w )
{
	pmd85_state *state = device->machine().driver_data<pmd85_state>();
	state->m_ppi_port_outputs[2][2] = data;
}

/*******************************************************************************

    I/O board 8251
    --------------
        cassette recorder and V.24/IFSS (selectable by switch)

*******************************************************************************/

/*******************************************************************************

    I/O board 8253
    --------------

    Timer 0:
        OUT0    - external interfaces connector (K2)
        CLK0    - external interfaces connector (K2)
        GATE0   - external interfaces connector (K2), default = 1
    Timer 1:
        OUT0    - external interfaces connector (K2), msm8251 (for V24 only)
        CLK0    - hardwired to 2 MHz system clock
        GATE0   - external interfaces connector (K2), default = 1
    Timer 2:
        OUT0    - unused
        CLK0    - hardwired to 1HZ signal generator
        GATE0   - hardwired to 5V, default = 1

*******************************************************************************/

const struct pit8253_config pmd85_pit8253_interface =
{
	{
		{ 0,		DEVCB_NULL,     DEVCB_NULL },
		{ 2000000,	DEVCB_NULL,     DEVCB_NULL },
		{ 1,		DEVCB_LINE_VCC, DEVCB_NULL }
	}
};

/*******************************************************************************

    I/O board external interfaces connector (K2)
    --------------------------------------------

*******************************************************************************/



/*******************************************************************************

    ROM Module 8255
    ---------------
        port A - data read
        ports B, C - address select

*******************************************************************************/

static READ8_DEVICE_HANDLER ( pmd85_ppi_3_porta_r )
{
	pmd85_state *state = device->machine().driver_data<pmd85_state>();
	if (device->machine().region("user1")->base() != NULL)
		return device->machine().region("user1")->base()[state->m_ppi_port_outputs[3][1] | (state->m_ppi_port_outputs[3][2] << 8)];
	else
		return 0;
}

static READ8_DEVICE_HANDLER ( pmd85_ppi_3_portb_r )
{
	return 0xff;
}

static READ8_DEVICE_HANDLER ( pmd85_ppi_3_portc_r )
{
	return 0xff;
}

static WRITE8_DEVICE_HANDLER ( pmd85_ppi_3_porta_w )
{
	pmd85_state *state = device->machine().driver_data<pmd85_state>();
	state->m_ppi_port_outputs[3][0] = data;
}

static WRITE8_DEVICE_HANDLER ( pmd85_ppi_3_portb_w )
{
	pmd85_state *state = device->machine().driver_data<pmd85_state>();
	state->m_ppi_port_outputs[3][1] = data;
}

static WRITE8_DEVICE_HANDLER ( pmd85_ppi_3_portc_w )
{
	pmd85_state *state = device->machine().driver_data<pmd85_state>();
	state->m_ppi_port_outputs[3][2] = data;
}

/*******************************************************************************

    I/O ports (PMD-85.1, PMD-85.2, PMD-85.3, Didaktik Alfa)
    -------------------------------------------------------

    I/O board
    1xxx11aa    external interfaces connector (K2)

    0xxx11aa    I/O board interfaces
        000111aa    8251 (casette recorder, V24)
        010011aa    8255 (GPIO/0, GPIO/1)
        010111aa    8253
        011111aa    8255 (IMS-2)

    Motherboard
    1xxx01aa    8255 (keyboard, speaker, LEDs)
            PMD-85.3 memory banking

    ROM Module
    1xxx10aa    8255 (ROM reading)

*******************************************************************************/

READ8_HANDLER ( pmd85_io_r )
{
	pmd85_state *state = space->machine().driver_data<pmd85_state>();
	if (state->m_startup_mem_map)
	{
		return 0xff;
	}

	switch (offset & 0x0c)
	{
		case 0x04:	/* Motherboard */
				switch (offset & 0x80)
				{
					case 0x80:	/* Motherboard 8255 */
							return space->machine().device<i8255_device>("ppi8255_0")->read(*space, offset & 0x03);
				}
				break;
		case 0x08:	/* ROM module connector */
				switch (state->m_model)
				{
					case PMD85_1:
					case PMD85_2:
					case PMD85_2A:
					case C2717:
					case PMD85_3:
						if (state->m_rom_module_present)
						{
							switch (offset & 0x80)
							{
								case 0x80:	/* ROM module 8255 */
									return space->machine().device<i8255_device>("ppi8255_3")->read(*space, offset & 0x03);
							}
						}
						break;
				}
				break;
		case 0x0c:	/* I/O board */
				switch (offset & 0x80)
				{
					case 0x00:	/* I/O board interfaces */
							switch (offset & 0x70)
							{
								case 0x10:	/* 8251 (casette recorder, V24) */
										switch (offset & 0x01)
										{
											case 0x00: return msm8251_data_r(space->machine().device("uart"), offset & 0x01);
											case 0x01: return msm8251_status_r(space->machine().device("uart"), offset & 0x01);
										}
										break;
								case 0x40:      /* 8255 (GPIO/0, GPIO/1) */
										return space->machine().device<i8255_device>("ppi8255_1")->read(*space, offset & 0x03);
								case 0x50:	/* 8253 */
										return pit8253_r( space->machine().device("pit8253"), offset & 0x03);
								case 0x70:	/* 8255 (IMS-2) */
										return space->machine().device<i8255_device>("ppi8255_2")->read(*space, offset & 0x03);
							}
							break;
					case 0x80:	/* external interfaces */
							break;
				}
				break;
	}

	logerror ("Reading from unmapped port: %02x\n", offset);
	return 0xff;
}

WRITE8_HANDLER ( pmd85_io_w )
{
	pmd85_state *state = space->machine().driver_data<pmd85_state>();
	if (state->m_startup_mem_map)
	{
		state->m_startup_mem_map = 0;
		(*state->update_memory)(space->machine());
	}

	switch (offset & 0x0c)
	{
		case 0x04:	/* Motherboard */
				switch (offset & 0x80)
				{
					case 0x80:	/* Motherboard 8255 */
							space->machine().device<i8255_device>("ppi8255_0")->write(*space, offset & 0x03, data);
							/* PMD-85.3 memory banking */
							if ((offset & 0x03) == 0x03)
							{
								state->m_pmd853_memory_mapping = data & 0x01;
								(*state->update_memory)(space->machine());
							}
							break;
				}
				break;
		case 0x08:	/* ROM module connector */
				switch (state->m_model)
				{
					case PMD85_1:
					case PMD85_2:
					case PMD85_2A:
					case C2717:
					case PMD85_3:
						if (state->m_rom_module_present)
						{
							switch (offset & 0x80)
							{
								case 0x80:	/* ROM module 8255 */
										space->machine().device<i8255_device>("ppi8255_3")->write(*space, offset & 0x03, data);
										break;
							}
						}
						break;
				}
				break;
		case 0x0c:	/* I/O board */
				switch (offset & 0x80)
				{
					case 0x00:	/* I/O board interfaces */
							switch (offset & 0x70)
							{
								case 0x10:	/* 8251 (casette recorder, V24) */
										switch (offset & 0x01)
										{
											case 0x00: msm8251_data_w(space->machine().device("uart"), offset & 0x01, data); break;
											case 0x01: msm8251_control_w(space->machine().device("uart"), offset & 0x01, data); break;
										}
										break;
								case 0x40:      /* 8255 (GPIO/0, GPIO/0) */
										space->machine().device<i8255_device>("ppi8255_1")->write(*space, offset & 0x03, data);
										break;
								case 0x50:	/* 8253 */
										pit8253_w(space->machine().device("pit8253"), offset & 0x03, data);
										logerror ("8253 writing. Address: %02x, Data: %02x\n", offset, data);
										break;
								case 0x70:	/* 8255 (IMS-2) */
										space->machine().device<i8255_device>("ppi8255_2")->write(*space, offset & 0x03, data);
										break;
							}
							break;
					case 0x80:	/* external interfaces */
							break;
				}
				break;
	}
}

/*******************************************************************************

    I/O ports (Mato)
    ----------------

    Motherboard
    1xxx01aa    8255 (keyboard, speaker, LEDs, tape)

*******************************************************************************/

 READ8_HANDLER ( mato_io_r )
{
	pmd85_state *state = space->machine().driver_data<pmd85_state>();
	if (state->m_startup_mem_map)
	{
		return 0xff;
	}

	switch (offset & 0x0c)
	{
		case 0x04:	/* Motherboard */
				switch (offset & 0x80)
				{
					case 0x80:	/* Motherboard 8255 */
							return space->machine().device<i8255_device>("ppi8255_0")->read(*space, offset & 0x03);
				}
				break;
	}

	logerror ("Reading from unmapped port: %02x\n", offset);
	return 0xff;
}

WRITE8_HANDLER ( mato_io_w )
{
	pmd85_state *state = space->machine().driver_data<pmd85_state>();
	if (state->m_startup_mem_map)
	{
		state->m_startup_mem_map = 0;
		(*state->update_memory)(space->machine());
	}

	switch (offset & 0x0c)
	{
		case 0x04:	/* Motherboard */
				switch (offset & 0x80)
				{
					case 0x80:	/* Motherboard 8255 */
							return space->machine().device<i8255_device>("ppi8255_0")->write(*space, offset & 0x03, data);
							break;
				}
				break;
	}
}

const i8255_interface pmd85_ppi8255_interface[4] =
{
	{
		DEVCB_HANDLER(pmd85_ppi_0_porta_r),
		DEVCB_HANDLER(pmd85_ppi_0_porta_w),
		DEVCB_HANDLER(pmd85_ppi_0_portb_r),
		DEVCB_HANDLER(pmd85_ppi_0_portb_w),
		DEVCB_HANDLER(pmd85_ppi_0_portc_r),
		DEVCB_HANDLER(pmd85_ppi_0_portc_w)
	},
	{
		DEVCB_HANDLER(pmd85_ppi_1_porta_r),
		DEVCB_HANDLER(pmd85_ppi_1_porta_w),
		DEVCB_HANDLER(pmd85_ppi_1_portb_r),
		DEVCB_HANDLER(pmd85_ppi_1_portb_w),
		DEVCB_HANDLER(pmd85_ppi_1_portc_r),
		DEVCB_HANDLER(pmd85_ppi_1_portc_w)
	},
	{
		DEVCB_HANDLER(pmd85_ppi_2_porta_r),
		DEVCB_HANDLER(pmd85_ppi_2_porta_w),
		DEVCB_HANDLER(pmd85_ppi_2_portb_r),
		DEVCB_HANDLER(pmd85_ppi_2_portb_w),
		DEVCB_HANDLER(pmd85_ppi_2_portc_r),
		DEVCB_HANDLER(pmd85_ppi_2_portc_w)
	},
	{
		DEVCB_HANDLER(pmd85_ppi_3_porta_r),
		DEVCB_HANDLER(pmd85_ppi_3_porta_w),
		DEVCB_HANDLER(pmd85_ppi_3_portb_r),
		DEVCB_HANDLER(pmd85_ppi_3_portb_w),
		DEVCB_HANDLER(pmd85_ppi_3_portc_r),
		DEVCB_HANDLER(pmd85_ppi_3_portc_w)
	}
};

const i8255_interface alfa_ppi8255_interface[3] =
{
	{
		DEVCB_HANDLER(pmd85_ppi_0_porta_r),
		DEVCB_HANDLER(pmd85_ppi_0_porta_w),
		DEVCB_HANDLER(pmd85_ppi_0_portb_r),
		DEVCB_HANDLER(pmd85_ppi_0_portb_w),
		DEVCB_HANDLER(pmd85_ppi_0_portc_r),
		DEVCB_HANDLER(pmd85_ppi_0_portc_w)
	},
	{
		DEVCB_HANDLER(pmd85_ppi_1_porta_r),
		DEVCB_HANDLER(pmd85_ppi_1_porta_w),
		DEVCB_HANDLER(pmd85_ppi_1_portb_r),
		DEVCB_HANDLER(pmd85_ppi_1_portb_w),
		DEVCB_HANDLER(pmd85_ppi_1_portc_r),
		DEVCB_HANDLER(pmd85_ppi_1_portc_w)
	},
	{
		DEVCB_HANDLER(pmd85_ppi_2_porta_r),
		DEVCB_HANDLER(pmd85_ppi_2_porta_w),
		DEVCB_HANDLER(pmd85_ppi_2_portb_r),
		DEVCB_HANDLER(pmd85_ppi_2_portb_w),
		DEVCB_HANDLER(pmd85_ppi_2_portc_r),
		DEVCB_HANDLER(pmd85_ppi_2_portc_w)
	}
};

I8255_INTERFACE( mato_ppi8255_interface )
{
	DEVCB_HANDLER(pmd85_ppi_0_porta_r),
	DEVCB_HANDLER(pmd85_ppi_0_porta_w),
	DEVCB_HANDLER(mato_ppi_0_portb_r),
	DEVCB_HANDLER(pmd85_ppi_0_portb_w),
	DEVCB_HANDLER(mato_ppi_0_portc_r),
	DEVCB_HANDLER(mato_ppi_0_portc_w)
};


static void pmd85_cassette_write(running_machine &machine, int id, unsigned long state)
{
	pmd85_state *drvstate = machine.driver_data<pmd85_state>();
	drvstate->m_cassette_serial_connection.input_state = state;
}

static TIMER_CALLBACK(pmd85_cassette_timer_callback)
{
	pmd85_state *state = machine.driver_data<pmd85_state>();
	int data;
	int current_level;

	if (!(input_port_read(machine, "DSW0") & 0x02))	/* V.24 / Tape Switch */
	{
		/* tape reading */
		if (machine.device<cassette_image_device>(CASSETTE_TAG)->get_state()&CASSETTE_PLAY)
		{
			switch (state->m_model)
			{
				case PMD85_1:
					if (state->m_clk_level_tape)
					{
						state->m_previous_level = ((machine.device<cassette_image_device>(CASSETTE_TAG))->input() > 0.038) ? 1 : 0;
						state->m_clk_level_tape = 0;
					}
					else
					{
						current_level = ((machine.device<cassette_image_device>(CASSETTE_TAG))->input() > 0.038) ? 1 : 0;

						if (state->m_previous_level!=current_level)
						{
							data = (!state->m_previous_level && current_level) ? 1 : 0;

							set_out_data_bit(state->m_cassette_serial_connection.State, data);
							serial_connection_out(machine, &state->m_cassette_serial_connection);
							msm8251_receive_clock(machine.device("uart"));

							state->m_clk_level_tape = 1;
						}
					}
					return;
				case PMD85_2:
				case PMD85_2A:
				case C2717:
				case PMD85_3:
				case ALFA:
					/* not hardware data decoding */
					return;
			}
		}

		/* tape writing */
		if (machine.device<cassette_image_device>(CASSETTE_TAG)->get_state()&CASSETTE_RECORD)
		{
			data = get_in_data_bit(state->m_cassette_serial_connection.input_state);
			data ^= state->m_clk_level_tape;
			machine.device<cassette_image_device>(CASSETTE_TAG)->output(data&0x01 ? 1 : -1);

			if (!state->m_clk_level_tape)
				msm8251_transmit_clock(machine.device("uart"));

			state->m_clk_level_tape = state->m_clk_level_tape ? 0 : 1;

			return;
		}

		state->m_clk_level_tape = 1;

		if (!state->m_clk_level)
			msm8251_transmit_clock(machine.device("uart"));
		state->m_clk_level = state->m_clk_level ? 0 : 1;
	}
}

static TIMER_CALLBACK( pmd_reset )
{
	machine.schedule_soft_reset();
}

DIRECT_UPDATE_HANDLER(pmd85_opbaseoverride)
{
	if (input_port_read(machine, "RESET") & 0x01)
		machine.scheduler().timer_set(attotime::from_usec(10), FUNC(pmd_reset));
	return address;
}

static void pmd85_common_driver_init (running_machine &machine)
{
	pmd85_state *state = machine.driver_data<pmd85_state>();
	state->m_previous_level = 0;
	state->m_clk_level = state->m_clk_level_tape = 1;
	state->m_cassette_timer = machine.scheduler().timer_alloc(FUNC(pmd85_cassette_timer_callback));
	state->m_cassette_timer->adjust(attotime::zero, 0, attotime::from_hz(2400));

	serial_connection_init(machine, &state->m_cassette_serial_connection);
	serial_connection_set_in_callback(machine, &state->m_cassette_serial_connection, pmd85_cassette_write);
}

DRIVER_INIT ( pmd851 )
{
	pmd85_state *state = machine.driver_data<pmd85_state>();
	state->m_model = PMD85_1;
	state->update_memory = pmd851_update_memory;
	pmd85_common_driver_init(machine);
}

DRIVER_INIT ( pmd852a )
{
	pmd85_state *state = machine.driver_data<pmd85_state>();
	state->m_model = PMD85_2A;
	state->update_memory = pmd852a_update_memory;
	pmd85_common_driver_init(machine);
}

DRIVER_INIT ( pmd853 )
{
	pmd85_state *state = machine.driver_data<pmd85_state>();
	state->m_model = PMD85_3;
	state->update_memory = pmd853_update_memory;
	pmd85_common_driver_init(machine);
}

DRIVER_INIT ( alfa )
{
	pmd85_state *state = machine.driver_data<pmd85_state>();
	state->m_model = ALFA;
	state->update_memory = alfa_update_memory;
	pmd85_common_driver_init(machine);
}

DRIVER_INIT ( mato )
{
	pmd85_state *state = machine.driver_data<pmd85_state>();
	state->m_model = MATO;
	state->update_memory = mato_update_memory;
}

DRIVER_INIT ( c2717 )
{
	pmd85_state *state = machine.driver_data<pmd85_state>();
	state->m_model = C2717;
	state->update_memory = c2717_update_memory;
	pmd85_common_driver_init(machine);
}

static TIMER_CALLBACK( setup_machine_state )
{
	pmd85_state *state = machine.driver_data<pmd85_state>();
	if (state->m_model != MATO)
	{
		msm8251_connect(machine.device("uart"), &state->m_cassette_serial_connection);
	}
}


MACHINE_RESET( pmd85 )
{
	pmd85_state *state = machine.driver_data<pmd85_state>();
	int i, j;

	/* checking for Rom Module */
	switch (state->m_model)
	{
		case PMD85_1:
		case PMD85_2A:
		case PMD85_3:
		case C2717:
			state->m_rom_module_present = (input_port_read(machine, "DSW0") & 0x01) ? 1 : 0;
			break;
		case ALFA:
		case MATO:
			break;
	}

	for (i = 0; i < 4; i++)
		for (j = 0; j < 3; j++)
			state->m_ppi_port_outputs[i][j] = 0;

	/* memory initialization */
	memset(ram_get_ptr(machine.device(RAM_TAG)), 0, sizeof(unsigned char)*0x10000);
	state->m_pmd853_memory_mapping = 1;
	state->m_startup_mem_map = 1;
	state->update_memory(machine);

	machine.scheduler().timer_set(attotime::zero, FUNC(setup_machine_state));

	machine.device("maincpu")->memory().space(AS_PROGRAM)->set_direct_update_handler(direct_update_delegate(FUNC(pmd85_opbaseoverride), &machine));
}
