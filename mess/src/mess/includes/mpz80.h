#pragma once

#ifndef __MPZ80__
#define __MPZ80__

#define ADDRESS_MAP_MODERN

#include "emu.h"
#include "cpu/z80/z80.h"
#include "formats/basicdsk.h"
#include "imagedev/flopdrv.h"
#include "machine/ram.h"
#include "machine/s100.h"
#include "machine/s100_dj2db.h"
#include "machine/s100_djdma.h"
#include "machine/s100_mm65k16s.h"
#include "machine/s100_wunderbus.h"
#include "machine/terminal.h"

#define Z80_TAG			"17a"
#define AM9512_TAG		"17d"
#define TERMINAL_TAG	"terminal"

class mpz80_state : public driver_device
{
public:
	mpz80_state(const machine_config &mconfig, device_type type, const char *tag)
		: driver_device(mconfig, type, tag),
		  m_maincpu(*this, Z80_TAG),
		  m_ram(*this, RAM_TAG),
		  m_terminal(*this, TERMINAL_TAG),
		  m_s100(*this, S100_TAG),
		  m_nmi(1),
		  m_pint(1),
		  m_int_pend(0),
		  m_pretrap(0),
		  m_trap(0),
		  m_trap_reset(0),
		  m_trap_void(1),
		  m_trap_halt(1),
		  m_trap_int(1),
		  m_trap_stop(1),
		  m_trap_aux(1)
	{ }

	required_device<cpu_device> m_maincpu;
	required_device<device_t> m_ram;
	required_device<device_t> m_terminal;
	required_device<s100_device> m_s100;

	virtual void machine_start();
	virtual void machine_reset();

	inline offs_t get_address(offs_t offset);
	inline offs_t get_io_address(offs_t offset);

	inline void check_traps();
	inline void check_interrupt();

	DECLARE_READ8_MEMBER( mmu_r );
	DECLARE_WRITE8_MEMBER( mmu_w );
	DECLARE_READ8_MEMBER( mmu_io_r );
	DECLARE_WRITE8_MEMBER( mmu_io_w );
	DECLARE_READ8_MEMBER( trap_addr_r );
	DECLARE_READ8_MEMBER( keyboard_r );
	DECLARE_READ8_MEMBER( switch_r );
	DECLARE_READ8_MEMBER( status_r );
	DECLARE_WRITE8_MEMBER( disp_seg_w );
	DECLARE_WRITE8_MEMBER( disp_col_w );
	DECLARE_WRITE8_MEMBER( task_w );
	DECLARE_WRITE8_MEMBER( mask_w );
	DECLARE_WRITE8_MEMBER( terminal_w );
	DECLARE_WRITE_LINE_MEMBER( s100_pint_w );
	DECLARE_WRITE_LINE_MEMBER( s100_nmi_w );

	// memory state
	UINT32 m_addr;
	UINT8 *m_map_ram;
	UINT8 m_task;
	UINT8 m_mask;

	// interrupt state
	int m_nmi;
	int m_pint;
	int m_int_pend;

	// trap state
	UINT8 m_pretrap_addr;
	UINT8 m_trap_addr;
	UINT8 m_status;
	UINT16 m_trap_start;
	int m_pretrap;
	int m_trap;
	int m_trap_reset;
	int m_trap_void;
	int m_trap_halt;
	int m_trap_int;
	int m_trap_stop;
	int m_trap_aux;
};

#endif
