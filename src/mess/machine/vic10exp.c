/**********************************************************************

    Commodore VIC-10 Expansion Port emulation

    Copyright MESS Team.
    Visit http://mamedev.org for licensing and usage restrictions.

**********************************************************************/

#include "emu.h"
#include "emuopts.h"
#include "machine/vic10exp.h"


//**************************************************************************
//  GLOBAL VARIABLES
//**************************************************************************

const device_type VIC10_EXPANSION_SLOT = &device_creator<vic10_expansion_slot_device>;


//**************************************************************************
//  DEVICE VIC10_EXPANSION CARD INTERFACE
//**************************************************************************

//-------------------------------------------------
//  device_vic10_expansion_card_interface - constructor
//-------------------------------------------------

device_vic10_expansion_card_interface::device_vic10_expansion_card_interface(const machine_config &mconfig, device_t &device)
	: device_slot_card_interface(mconfig,device)
{
	m_slot = dynamic_cast<vic10_expansion_slot_device *>(device.owner());
}


//-------------------------------------------------
//  ~device_vic10_expansion_card_interface - destructor
//-------------------------------------------------

device_vic10_expansion_card_interface::~device_vic10_expansion_card_interface()
{
}



//**************************************************************************
//  LIVE DEVICE
//**************************************************************************

//-------------------------------------------------
//  vic10_expansion_slot_device - constructor
//-------------------------------------------------

vic10_expansion_slot_device::vic10_expansion_slot_device(const machine_config &mconfig, const char *tag, device_t *owner, UINT32 clock) :
        device_t(mconfig, VIC10_EXPANSION_SLOT, "VIC-10 expansion port", tag, owner, clock),
		device_slot_interface(mconfig, *this),
		device_image_interface(mconfig, *this)
{
}


//-------------------------------------------------
//  vic10_expansion_slot_device - destructor
//-------------------------------------------------

vic10_expansion_slot_device::~vic10_expansion_slot_device()
{
}


//-------------------------------------------------
//  device_config_complete - perform any
//  operations now that the configuration is
//  complete
//-------------------------------------------------

void vic10_expansion_slot_device::device_config_complete()
{
	// inherit a copy of the static data
	const vic10_expansion_slot_interface *intf = reinterpret_cast<const vic10_expansion_slot_interface *>(static_config());
	if (intf != NULL)
	{
		*static_cast<vic10_expansion_slot_interface *>(this) = *intf;
	}

	// or initialize to defaults if none provided
	else
	{
    	memset(&m_out_irq_cb, 0, sizeof(m_out_irq_cb));
    	memset(&m_out_sp_cb, 0, sizeof(m_out_sp_cb));
    	memset(&m_out_cnt_cb, 0, sizeof(m_out_cnt_cb));
    	memset(&m_out_res_cb, 0, sizeof(m_out_res_cb));
	}

	// set brief and instance name
	update_names();
}


//-------------------------------------------------
//  device_start - device-specific startup
//-------------------------------------------------

void vic10_expansion_slot_device::device_start()
{
	m_cart = dynamic_cast<device_vic10_expansion_card_interface *>(get_card_device());

	// resolve callbacks
	m_out_irq_func.resolve(m_out_irq_cb, *this);
	m_out_sp_func.resolve(m_out_sp_cb, *this);
	m_out_cnt_func.resolve(m_out_cnt_cb, *this);
	m_out_res_func.resolve(m_out_res_cb, *this);
}


//-------------------------------------------------
//  call_load -
//-------------------------------------------------

bool vic10_expansion_slot_device::call_load()
{
	if (m_cart)
	{
		size_t size = 0;

		if (software_entry() == NULL)
		{
			size = length();

			if (!mame_stricmp(filetype(), "80"))
			{
				fread(m_cart->vic10_lorom_pointer(), 0x2000);

				if (size == 0x4000)
				{
					fread(m_cart->vic10_uprom_pointer(), 0x2000);
				}
			}
			else if (!mame_stricmp(filetype(), "e0")) fread(m_cart->vic10_uprom_pointer(), size);
		}
		else
		{
			size = get_software_region_length("lorom");
			if (size) memcpy(m_cart->vic10_lorom_pointer(), get_software_region("lorom"), size);

			size = get_software_region_length("uprom");
			if (size) memcpy(m_cart->vic10_uprom_pointer(), get_software_region("uprom"), size);

			size = get_software_region_length("exram");
			if (size) m_cart->vic10_exram_pointer();
		}
	}

	return IMAGE_INIT_PASS;
}


//-------------------------------------------------
//  call_softlist_load -
//-------------------------------------------------

bool vic10_expansion_slot_device::call_softlist_load(char *swlist, char *swname, rom_entry *start_entry)
{
	load_software_part_region(this, swlist, swname, start_entry);

	return true;
}


//-------------------------------------------------
//  get_default_card_software -
//-------------------------------------------------

const char * vic10_expansion_slot_device::get_default_card_software(const machine_config &config, emu_options &options)
{
	return software_get_default_slot(config, options, this, "standard");
}


//-------------------------------------------------
//  screen_update -
//-------------------------------------------------

UINT32 vic10_expansion_slot_device::screen_update(screen_device &screen, bitmap_ind16 &bitmap, const rectangle &cliprect)
{
	bool value = false;

	if (m_cart != NULL)
	{
		value = m_cart->vic10_screen_update(screen, bitmap, cliprect);
	}

	return value;
}


READ8_MEMBER( vic10_expansion_slot_device::lorom_r ) { UINT8 data = 0; if (m_cart != NULL) data = m_cart->vic10_lorom_r(space, offset); return data; }
WRITE8_MEMBER( vic10_expansion_slot_device::lorom_w ) { if (m_cart != NULL) m_cart->vic10_lorom_w(space, offset, data); }
READ8_MEMBER( vic10_expansion_slot_device::uprom_r ) { UINT8 data = 0; if (m_cart != NULL) data = m_cart->vic10_uprom_r(space, offset); return data; }
WRITE8_MEMBER( vic10_expansion_slot_device::uprom_w ) { if (m_cart != NULL) m_cart->vic10_uprom_w(space, offset, data); }
READ8_MEMBER( vic10_expansion_slot_device::exram_r ) { UINT8 data = 0; if (m_cart != NULL) data = m_cart->vic10_exram_r(space, offset); return data; }
WRITE8_MEMBER( vic10_expansion_slot_device::exram_w ) { if (m_cart != NULL) m_cart->vic10_exram_w(space, offset, data); }

READ_LINE_MEMBER( vic10_expansion_slot_device::p0_r ) { int state = 0; if (m_cart != NULL) state = m_cart->vic10_p0_r(); return state; }
WRITE_LINE_MEMBER( vic10_expansion_slot_device::p0_w ) { if (m_cart != NULL) m_cart->vic10_p0_w(state); }
WRITE_LINE_MEMBER( vic10_expansion_slot_device::irq_w ) { m_out_irq_func(state); }
WRITE_LINE_MEMBER( vic10_expansion_slot_device::sp_w ) { m_out_sp_func(state); if (m_cart != NULL) m_cart->vic10_sp_w(state); }
WRITE_LINE_MEMBER( vic10_expansion_slot_device::cnt_w ) { m_out_cnt_func(state); if (m_cart != NULL) m_cart->vic10_cnt_w(state); }
WRITE_LINE_MEMBER( vic10_expansion_slot_device::res_w ) { m_out_res_func(state); }