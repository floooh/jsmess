################################################################################
# Contains Makefile logic required when building KC85/3
################################################################################

# KC85/3 has a bios that is required to run the system.
BIOS := kc85_3.zip
# SUBTARGET for the MESS makefile.
SUBTARGET := kc85_3
# MESS_ARGS := ["kc85_3","-verbose","-rompath",".","-window","-resolution","320x256","-dump",gamename,"-nokeepaspect"]
MESS_ARGS := ["kc85_3","-verbose","-rompath",".","-quik",gamename,"-window","-resolution","640x512","-nokeepaspect"]

# System-specific flags that should be passed to MESS's makefile.
# MESS_FLAGS +=
