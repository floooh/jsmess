/**********************************************************************

    Magic Formel cartridge emulation

    Copyright MESS Team.
    Visit http://mamedev.org for licensing and usage restrictions.

**********************************************************************/

#pragma once

#ifndef __MAGIC_FORMEL__
#define __MAGIC_FORMEL__


#include "emu.h"
#include "machine/c64exp.h"
#include "machine/6821pia.h"



//**************************************************************************
//  TYPE DEFINITIONS
//**************************************************************************

// ======================> c64_magic_formel_cartridge_device

class c64_magic_formel_cartridge_device : public device_t,
										  public device_c64_expansion_card_interface
{
public:
	// construction/destruction
	c64_magic_formel_cartridge_device(const machine_config &mconfig, const char *tag, device_t *owner, UINT32 clock);

	// optional information overrides
	virtual machine_config_constructor device_mconfig_additions() const;
	virtual ioport_constructor device_input_ports() const;

	DECLARE_INPUT_CHANGED_MEMBER( freeze );

	// not really public
	DECLARE_WRITE8_MEMBER( pia_pa_w );
	DECLARE_WRITE8_MEMBER( pia_pb_w );
	DECLARE_WRITE_LINE_MEMBER( pia_cb2_w );

protected:
	// device-level overrides
	virtual void device_start();
	virtual void device_reset();
	virtual void device_config_complete() { m_shortname = "c64_magic_formel"; }

	// device_c64_expansion_card_interface overrides
	virtual UINT8 c64_cd_r(address_space &space, offs_t offset, int roml, int romh, int io1, int io2);
	virtual void c64_cd_w(address_space &space, offs_t offset, UINT8 data, int roml, int romh, int io1, int io2);
	virtual int c64_game_r(offs_t offset, int ba, int rw, int hiram);

private:
	required_device<pia6821_device> m_pia;

	UINT8 m_rom_bank;
	UINT8 m_ram_bank;
	int m_pb7_ff;
	int m_cb2_ff;
	int m_rom_oe;
	int m_ram_oe;
	int m_pb7;
};


// device type definition
extern const device_type C64_MAGIC_FORMEL;


#endif
