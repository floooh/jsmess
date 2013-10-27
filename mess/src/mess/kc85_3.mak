###########################################################################
#
#   kc85_3.mak
#
# KC85/3-specific Makefile
#
###########################################################################

# disable messui for tiny build
MESSUI = 0

# include MESS core defines
include $(SRC)/mess/messcore.mak
include $(SRC)/mess/osd/$(OSD)/$(OSD).mak


#-------------------------------------------------
# Specify all the CPU cores necessary for the
# drivers referenced in colecovision.lst
#-------------------------------------------------

CPUS += Z80
CPUS += MCS48

#-------------------------------------------------
# Specify all the sound cores necessary for the
# drivers referenced in colecovision.lst
#-------------------------------------------------

SOUNDS += SPEAKER
SOUNDS += WAVE
SOUNDS += AY8910

#-------------------------------------------------
# This is the list of files that are necessary
# for building all of the drivers referenced
# in colecovision.lst
#-------------------------------------------------

DRVLIBS = $(EMUOBJ)/drivers/emudummy.o

DRVLIBS += $(MESSOBJ)/drivers/kc.o
DRVLIBS += $(MESSOBJ)/machine/kc.o
DRVLIBS += $(MESSOBJ)/video/kc.o

# device_ref=z80
DRVLIBS += $(EMUOBJ)/cpu/z80/z80.o

# device_ref=z80pio
DRVLIBS += $(EMUOBJ)/machine/z80pio.o

# device_ref=z80ctc
DRVLIBS += $(EMUOBJ)/machine/z80ctc.o

# device_ref=screen
DRVLIBS += $(EMUOBJ)/screen.o

# device_ref=timer
DRVLIBS += $(EMUOBJ)/timer.o

# device_ref=kc_keyboard
# DRVLIBS += $(MESSOBJ)/machine/kc_keyb.o

# device_ref=speaker
DRVLIBS += $(EMUOBJ)/speaker.o

# device_ref=wawe
DRVLIBS += $(EMUOBJ)/sound/wave.o

# device_ref=speaker_sound
DRVLIBS += $(EMUOBJ)/sound/speaker.o

# device_ref=quickload
DRVLIBS += $(EMUOBJ)/imagedev/snapquik.o

# device_ref=casette_image
DRVLIBS += $(EMUOBJ)/imagedev/cassette.o

# device_ref=kccart_slot
# device_ref=kcexp_slot
# DRVLIBS += $(MESSOBJ)/machine/kcexp.o

# device_ref=kc_m011
# DRVLIBS += $(MESSOBJ)/machine/kc_ram.c
# DRVLIBS += $(MESSOBJ)/machine/kc_rom.c

# device_ref=software_list
DRVLIBS += $(EMUOBJ)/softlist.o

# device_ref=ram
DRVLIBS += $(EMUOBJ)/machine/ram.o

DRVLIBS += $(MESSOBJ)/machine/upd765.o
DRVLIBS += $(OBJ)/lib/formats/basicdsk.o
DRVLIBS += $(OBJ)/lib/formats/flopimg.o
DRVLIBS += $(OBJ)/lib/formats/td0_dsk.o
DRVLIBS += $(OBJ)/lib/formats/imd_dsk.o
DRVLIBS += $(OBJ)/lib/formats/cqm_dsk.o
DRVLIBS += $(OBJ)/lib/formats/d88_dsk.o
DRVLIBS += $(OBJ)/lib/formats/dsk_dsk.o
DRVLIBS += $(OBJ)/lib/formats/fdi_dsk.o
DRVLIBS += $(MESSOBJ)/devices/appldriv.o


#-------------------------------------------------
# MESS special OSD rules
#-------------------------------------------------

include $(SRC)/mess/osd/$(OSD)/$(OSD).mak
