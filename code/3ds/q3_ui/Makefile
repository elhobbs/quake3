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
GDIR		:=	../../../game
Q3UIDIR		:= ../../../q3_ui
TARGET		:=	$(shell basename $(CURDIR))
BUILD		:=	build
SOURCES		:=	source
DATA		:=	data
INCLUDES	:=	include $(GDIR)

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
			$(foreach dir,$(DATA),$(CURDIR)/$(dir)) \
			$(Q3UIDIR)

export DEPSDIR	:=	$(CURDIR)/$(BUILD)

Q3UIFILES		:=	\
	ui_addbots.c \
	ui_atoms.c \
	ui_cdkey.c \
	ui_cinematics.c \
	ui_confirm.c \
	ui_connect.c \
	ui_controls2.c \
	ui_credits.c \
	ui_demo2.c \
	ui_display.c \
	ui_gameinfo.c \
	ui_ingame.c \
	ui_loadconfig.c \
	ui_main.c \
	ui_menu.c \
	ui_mfield.c \
	ui_mods.c \
	ui_network.c \
	ui_options.c \
	ui_playermodel.c \
	ui_players.c \
	ui_playersettings.c \
	ui_preferences.c \
	ui_qmenu.c \
	ui_removebots.c \
	ui_saveconfig.c \
	ui_serverinfo.c \
	ui_servers2.c \
	ui_setup.c \
	ui_sound.c \
	ui_sparena.c \
	ui_specifyserver.c \
	ui_splevel.c \
	ui_sppostgame.c \
	ui_spskill.c \
	ui_startserver.c \
	ui_team.c \
	ui_teamorders.c \
	ui_video.c


CFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.c))) \
				$(Q3UIFILES)
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
