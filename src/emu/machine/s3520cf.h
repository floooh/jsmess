/***************************************************************************

Template for skeleton device

***************************************************************************/

#pragma once

#ifndef __S3520CFDEV_H__
#define __S3520CFDEV_H__



//**************************************************************************
//  INTERFACE CONFIGURATION MACROS
//**************************************************************************

#define MCFG_S3520CF_ADD(_tag) \
	MCFG_DEVICE_ADD(_tag, S3520CF, XTAL_32_768kHz) \


//**************************************************************************
//  TYPE DEFINITIONS
//**************************************************************************

typedef enum
{
	RTC_SET_ADDRESS = 0,
	RTC_SET_DATA
} s3520cf_state_t;


// ======================> s3520cf_device

class s3520cf_device :	public device_t
{
public:
	// construction/destruction
	s3520cf_device(const machine_config &mconfig, const char *tag, device_t *owner, UINT32 clock);

	// I/O operations
	READ_LINE_MEMBER( read_bit );
	WRITE_LINE_MEMBER( set_dir_line );
	WRITE_LINE_MEMBER( set_cs_line );
	WRITE_LINE_MEMBER( set_clock_line );
	WRITE_LINE_MEMBER( write_bit );

protected:
	// device-level overrides
	virtual void device_validity_check(validity_checker &valid) const;
	virtual void device_start();
	virtual void device_reset();
	inline UINT8 rtc_read(UINT8 offset);
	inline void rtc_write(UINT8 offset,UINT8 data);

	int m_dir;
	int m_latch;
	int m_reset_line;
	int m_read_latch;
	UINT8 m_current_cmd;
	UINT8 m_cmd_stream_pos;
	UINT8 m_rtc_addr;

	s3520cf_state_t m_rtc_state;

};


// device type definition
extern const device_type S3520CF;



//**************************************************************************
//  GLOBAL VARIABLES
//**************************************************************************



#endif
