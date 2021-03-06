/**********************************************************************

    Commodore 1581/1563 Single Disk Drive emulation

    Copyright MESS Team.
    Visit http://mamedev.org for licensing and usage restrictions.

**********************************************************************/

#pragma once

#ifndef __C1581__
#define __C1581__

#define ADDRESS_MAP_MODERN

#include "emu.h"
#include "cpu/m6502/m6502.h"
#include "imagedev/flopdrv.h"
#include "formats/d81_dsk.h"
#include "machine/6526cia.h"
#include "machine/cbmiec.h"
#include "machine/wd17xx.h"



//**************************************************************************
//  MACROS / CONSTANTS
//**************************************************************************

#define C1581_TAG			"c1581"



//**************************************************************************
//  INTERFACE CONFIGURATION MACROS
//**************************************************************************

#define MCFG_C1581_ADD(_tag, _address) \
    MCFG_DEVICE_ADD(_tag, C1581, 0) \
	base_c1581_device::static_set_config(*device, _address);


#define MCFG_C1563_ADD(_tag, _address) \
    MCFG_DEVICE_ADD(_tag, C1563, 0) \
	base_c1581_device::static_set_config(*device, _address);



//**************************************************************************
//  TYPE DEFINITIONS
//**************************************************************************

// ======================> base_c1581_device

class base_c1581_device :  public device_t,
						   public device_cbm_iec_interface
{

public:
    // construction/destruction
    base_c1581_device(const machine_config &mconfig, device_type type, const char *name, const char *tag, device_t *owner, UINT32 clock, UINT32 variant);

	enum
	{
		TYPE_1563,
		TYPE_1581
	};

	// inline configuration helpers
	static void static_set_config(device_t &device, int address);

	// optional information overrides
	virtual const rom_entry *device_rom_region() const;
	virtual machine_config_constructor device_mconfig_additions() const;
	// not really public
	DECLARE_WRITE_LINE_MEMBER( cnt_w );
	DECLARE_WRITE_LINE_MEMBER( sp_w );
	DECLARE_READ8_MEMBER( cia_pa_r );
	DECLARE_WRITE8_MEMBER( cia_pa_w );
	DECLARE_READ8_MEMBER( cia_pb_r );
	DECLARE_WRITE8_MEMBER( cia_pb_w );

protected:
    // device-level overrides
    virtual void device_start();
	virtual void device_reset();
    virtual void device_config_complete();

	// device_cbm_iec_interface overrides
	void cbm_iec_srq(int state);
	void cbm_iec_atn(int state);
	void cbm_iec_data(int state);
	void cbm_iec_reset(int state);

	inline void set_iec_data();
	inline void set_iec_srq();

	required_device<cpu_device> m_maincpu;
	required_device<mos6526_device> m_cia;
	required_device<device_t> m_fdc;
	required_device<device_t> m_image;
	cbm_iec_device *m_bus;

	int m_data_out;				// serial data out
	int m_atn_ack;				// attention acknowledge
	int m_fast_ser_dir;			// fast serial direction
	int m_sp_out;				// fast serial data out
	int m_cnt_out;				// fast serial clock out

    int m_address;
	int m_variant;
};


// ======================> c1563_device

class c1563_device :  public base_c1581_device
{
public:
    // construction/destruction
    c1563_device(const machine_config &mconfig, const char *tag, device_t *owner, UINT32 clock);
};


// ======================> c1581_device

class c1581_device :  public base_c1581_device
{
public:
    // construction/destruction
    c1581_device(const machine_config &mconfig, const char *tag, device_t *owner, UINT32 clock);
};


// device type definition
extern const device_type C1563;
extern const device_type C1581;



#endif
