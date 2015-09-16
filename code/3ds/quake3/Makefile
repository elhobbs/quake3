#---------------------------------------------------------------------------------
.SUFFIXES:
#---------------------------------------------------------------------------------

ifeq ($(strip $(DEVKITARM)),)
$(error "Please set DEVKITARM in your environment. export DEVKITARM=<path to>devkitARM")
endif

include $(DEVKITARM)/3ds_rules

#---------------------------------------------------------------------------------
# TARGET is the name of the output
# BUILD is the directory where object files & intermediate files will be placed
# SOURCES is a list of directories containing source code
# DATA is a list of directories containing data files
# INCLUDES is a list of directories containing header files
#---------------------------------------------------------------------------------
#GDIR		:=	/c/devl/quake3/code/game
GDIR		:=	../../../game
CLDIR		:=	../../../client
QCOMMONDIR	:=	../../../qcommon
SERVERDIR	:=	../../../server
TARGET		:=	$(shell basename $(CURDIR))
BUILD		:=	build
SOURCES		:=	source
DATA		:=	data
INCLUDES	:=	include

#---------------------------------------------------------------------------------
# options for code generation
#---------------------------------------------------------------------------------
ARCH	:=	-march=armv6k -mtune=mpcore -mfloat-abi=hard

CFLAGS	:=	-g -Wall -O2\
		$(ARCH)

CFLAGS	+=	$(INCLUDE) -DARM11 -D_3DS
CXXFLAGS	:= $(CFLAGS) -fno-rtti -fno-exceptions

ASFLAGS	:=	-g $(ARCH)

#---------------------------------------------------------------------------------
# list of directories containing libraries, this must be the top level containing
# include and lib
#---------------------------------------------------------------------------------
LIBDIRS	:=	$(CTRULIB)

#---------------------------------------------------------------------------------
# no real need to edit anything past this point unless you need to add additional
# rules for different file extensions
#---------------------------------------------------------------------------------
ifneq ($(BUILD),$(notdir $(CURDIR)))
#---------------------------------------------------------------------------------

export OUTPUT	:=	$(CURDIR)/lib/lib$(TARGET).a

export VPATH	:=	$(foreach dir,$(SOURCES),$(CURDIR)/$(dir)) \
			$(foreach dir,$(DATA),$(CURDIR)/$(dir))

export DEPSDIR	:=	$(CURDIR)/$(BUILD)

CLFILES		:=	\
	$(CLDIR)/cl_cgame.c \
	$(CLDIR)/cl_cin.c \
	$(CLDIR)/cl_console.c \
	$(CLDIR)/cl_input.c \
	$(CLDIR)/cl_keys.c \
	$(CLDIR)/cl_main.c \
	$(CLDIR)/cl_net_chan.c \
	$(CLDIR)/cl_parse.c \
	$(CLDIR)/cl_scrn.c \
	$(CLDIR)/cl_ui.c

SNDFILES	:=	\
	$(CLDIR)/snd_adpcm.c \
	$(CLDIR)/snd_dma.c \
	$(CLDIR)/snd_mem.c \
	$(CLDIR)/snd_mix.c \
	$(CLDIR)/snd_wavelet.c

	
QCOMMONFILES	:=	\
	$(QCOMMONDIR)/cm_load.c \
	$(QCOMMONDIR)/cm_patch.c \
	$(QCOMMONDIR)/cm_polylib.c \
	$(QCOMMONDIR)/cm_test.c \
	$(QCOMMONDIR)/cm_trace.c \
	$(QCOMMONDIR)/cmd.c \
	$(QCOMMONDIR)/cvar.c \
	$(QCOMMONDIR)/files.c \
	$(QCOMMONDIR)/huffman.c \
	$(QCOMMONDIR)/md4.c \
	$(QCOMMONDIR)/msg.c \
	$(QCOMMONDIR)/net_chan.c \
	$(QCOMMONDIR)/unzip.c

GFILES		:= \
	$(GDIR)/q_math.c \
	$(GDIR)/q_shared.c

SERVERFILES	:= \
	$(SERVERDIR)/sv_bot.c \
	$(SERVERDIR)/sv_ccmds.c \
	$(SERVERDIR)/sv_client.c \
	$(SERVERDIR)/sv_game.c \
	$(SERVERDIR)/sv_init.c \
	$(SERVERDIR)/sv_main.c \
	$(SERVERDIR)/sv_net_chan.c \
	$(SERVERDIR)/sv_snapshot.c \
	$(SERVERDIR)/sv_world.c 

CFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.c))) \
				$(CLFILES) \
				$(SNDFILES) \
				$(QCOMMONFILES) \
				$(GFILES) \
				$(SERVERFILES)
CPPFILES	:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.cpp)))
SFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.s)))
BINFILES	:=	$(foreach dir,$(DATA),$(notdir $(wildcard $(dir)/*.*)))

#---------------------------------------------------------------------------------
# use CXX for linking C++ projects, CC for standard C
#---------------------------------------------------------------------------------
ifeq ($(strip $(CPPFILES)),)
#---------------------------------------------------------------------------------
	export LD	:=	$(CC)
#---------------------------------------------------------------------------------
else
#---------------------------------------------------------------------------------
	export LD	:=	$(CXX)
#---------------------------------------------------------------------------------
endif
#---------------------------------------------------------------------------------

export OFILES	:=	$(addsuffix .o,$(BINFILES)) \
			$(CPPFILES:.cpp=.o) $(CFILES:.c=.o) $(SFILES:.s=.o)

export INCLUDE	:=	$(foreach dir,$(INCLUDES),-I$(CURDIR)/$(dir)) \
			$(foreach dir,$(LIBDIRS),-I$(dir)/include) \
			-I$(CURDIR)/$(BUILD)

.PHONY: $(BUILD) clean all

#---------------------------------------------------------------------------------
all: $(BUILD)

lib:
	@[ -d $@ ] || mkdir -p $@
	
$(BUILD): lib
	@[ -d $@ ] || mkdir -p $@
	@$(MAKE) --no-print-directory -C $(BUILD) -f $(CURDIR)/Makefile

#---------------------------------------------------------------------------------
clean:
	@echo clean ...
	@rm -fr $(BUILD) lib

#---------------------------------------------------------------------------------
else

DEPENDS	:=	$(OFILES:.o=.d)

#---------------------------------------------------------------------------------
# main targets
#---------------------------------------------------------------------------------
$(OUTPUT)	:	$(OFILES)

#---------------------------------------------------------------------------------
%.bin.o	:	%.bin
#---------------------------------------------------------------------------------
	@echo $(notdir $<)
	@$(bin2o)


-include $(DEPENDS)

#---------------------------------------------------------------------------------------
endif
#---------------------------------------------------------------------------------------
