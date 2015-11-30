#---------------------------------------------------------------------------------
.SUFFIXES:
#---------------------------------------------------------------------------------

ifeq ($(strip $(DEVKITARM)),)
$(error "Please set DEVKITARM in your environment. export DEVKITARM=<path to>devkitARM")
endif

TOPDIR ?= $(CURDIR)
include $(DEVKITARM)/3ds_rules

NIHSTRO	:= /c/devl/nihstro/build/Debug

#---------------------------------------------------------------------------------
# TARGET is the name of the output
# BUILD is the directory where object files & intermediate files will be placed
# SOURCES is a list of directories containing source code
# DATA is a list of directories containing data files
# INCLUDES is a list of directories containing header files
#
# NO_SMDH: if set to anything, no SMDH file is generated.
# APP_TITLE is the name of the app stored in the SMDH file (Optional)
# APP_DESCRIPTION is the description of the app stored in the SMDH file (Optional)
# APP_AUTHOR is the author of the app stored in the SMDH file (Optional)
# ICON is the filename of the icon (.png), relative to the project folder.
#   If not set, it attempts to use one of the following (in this order):
#     - <Project name>.png
#     - icon.png
#     - <libctru folder>/default_icon.png
#---------------------------------------------------------------------------------
QCOMMONDIR	:=	../../../qcommon
SERVERDIR	:=	../../../server
GDIR		:=	../../../game
CLDIR		:=	../../../client
BOTDIR		:=	../../../botlib
RDIR		:=	../../../renderer
CGDIR		:=	../../../cgame
SPLINEDIR	:=	../../../splines
UIDIR		:=	../../../ui
Q3UIDIR		:=	../../../q3_ui

TARGET		:=	$(notdir $(CURDIR))
BUILD		:=	build
SOURCES		:=	source ctr_rend
DATA		:=	data
INCLUDES	:=	include ../ ../renderer/include $(GDIR) ctr_rend

#---------------------------------------------------------------------------------
# options for code generation
#---------------------------------------------------------------------------------
ARCH	:=	-march=armv6k -mtune=mpcore -mfloat-abi=hard

CFLAGS	:=	-g -Wall -O0 -mword-relocations \
			-fomit-frame-pointer -ffast-math \
			$(ARCH)

CFLAGS	+=	$(INCLUDE) -DARM11 -D_3DS -DBOTLIB -DQ3_STATIC

CXXFLAGS	:= $(CFLAGS) -fno-rtti -fno-exceptions -std=gnu++11

ASFLAGS	:=	-g $(ARCH)
LDFLAGS	=	-specs=3dsx.specs -g $(ARCH) -Wl,-Map,$(notdir $*.map)

LIBS	:=	-ljpeg -lctru -lstdc++ -lm

#---------------------------------------------------------------------------------
# list of directories containing libraries, this must be the top level containing
# include and lib
#---------------------------------------------------------------------------------
LIBDIRS	:=	$(PORTLIBS) \
			$(CTRULIB)


#---------------------------------------------------------------------------------
# no real need to edit anything past this point unless you need to add additional
# rules for different file extensions
#---------------------------------------------------------------------------------
ifneq ($(BUILD),$(notdir $(CURDIR)))
#---------------------------------------------------------------------------------

export OUTPUT	:=	$(CURDIR)/$(TARGET)
export TOPDIR	:=	$(CURDIR)

export VPATH	:=	$(foreach dir,$(SOURCES),$(CURDIR)/$(dir)) \
			$(foreach dir,$(DATA),$(CURDIR)/$(dir)) \
			$(QCOMMONDIR) \
			$(SERVERDIR) \
			$(CLDIR) \
			$(BOTDIR) \
			$(RDIR) \
			$(CGDIR) \
			$(GDIR) \
			$(Q3UIDIR) \
			$(UIDIR)

export DEPSDIR	:=	$(CURDIR)/$(BUILD)

QCOMMONFILES	:=	\
	cmd.c \
	common.c \
	cvar.c \
	files.c \
	huffman.c \
	md4.c \
	msg.c \
	cm_load.c \
	cm_patch.c \
	cm_polylib.c \
	cm_test.c \
	cm_trace.c \
	net_chan.c \
	unzip.c \
	vm.c \
	vm_interpreted.c

SERVERFILES	:= \
	sv_bot.c \
	sv_ccmds.c \
	sv_client.c \
	sv_game.c \
	sv_init.c \
	sv_main.c \
	sv_net_chan.c \
	sv_snapshot.c \
	sv_world.c 

CLFILES		:=	\
	cl_cgame.c \
	cl_cin.c \
	cl_console.c \
	cl_input.c \
	cl_keys.c \
	cl_main.c \
	cl_net_chan.c \
	cl_parse.c \
	cl_scrn.c \
	cl_ui.c

SNDFILES	:=	\
	snd_adpcm.c \
	snd_dma.c \
	snd_mem.c \
	snd_mix.c \
	snd_wavelet.c

BOTFILES		:=	\
	be_aas_bspq3.c \
	be_aas_cluster.c \
	be_aas_debug.c \
	be_aas_entity.c \
	be_aas_file.c \
	be_aas_main.c \
	be_aas_move.c \
	be_aas_optimize.c \
	be_aas_reach.c \
	be_aas_route.c \
	be_aas_routealt.c \
	be_aas_sample.c \
	be_ai_char.c \
	be_ai_chat.c \
	be_ai_gen.c \
	be_ai_goal.c \
	be_ai_move.c \
	be_ai_weap.c \
	be_ai_weight.c \
	be_ea.c \
	be_interface.c \
	l_crc.c \
	l_libvar.c \
	l_log.c \
	l_memory.c \
	l_precomp.c \
	l_script.c \
	l_struct.c

RFILES		:=	\
	tr_animation.c \
	tr_backend.c \
	tr_bsp.c \
	tr_cmds.c \
	tr_curve.c \
	tr_flares.c \
	tr_font.c \
	tr_image.c \
	tr_init.c \
	tr_light.c \
	tr_main.c \
	tr_marks.c \
	tr_mesh.c \
	tr_model.c \
	tr_noise.c \
	tr_scene.c \
	tr_shade.c \
	tr_shade_calc.c \
	tr_shader.c \
	tr_shadows.c \
	tr_sky.c \
	tr_surface.c \
	tr_world.c

CGFILES		:=	\
	cg_consolecmds.c \
	cg_draw.c \
	cg_drawtools.c \
	cg_effects.c \
	cg_ents.c \
	cg_event.c \
	cg_info.c \
	cg_localents.c \
	cg_main.c \
	cg_marks.c \
	cg_players.c \
	cg_playerstate.c \
	cg_predict.c \
	cg_scoreboard.c \
	cg_servercmds.c \
	cg_snapshot.c \
	cg_syscalls.c \
	cg_view.c \
	cg_weapons.c

GFILES		:=	\
	g_main.c \
	 \
	bg_misc.c \
	bg_lib.c \
	bg_pmove.c \
	bg_slidemove.c \
	q_math.c \
	q_shared.c \
	 \
	ai_vcmd.c \
	ai_dmnet.c \
	ai_dmq3.c \
	ai_main.c \
	ai_chat.c \
	ai_cmd.c \
	ai_team.c \
	 \
	g_active.c \
	g_arenas.c \
	g_bot.c \
	g_client.c \
	g_cmds.c \
	g_combat.c \
	g_items.c \
	g_mem.c \
	g_misc.c \
	g_missile.c \
	g_mover.c \
	g_session.c \
	g_spawn.c \
	g_svcmds.c \
	g_target.c \
	g_team.c \
	g_trigger.c \
	g_utils.c \
	g_weapon.c \
	g_syscalls.c

SPLINEFILES	:= \
	math_angles.cpp \
	math_matrix.cpp \
	math_quaternion.cpp \
	math_vector.cpp \
	q_parse.cpp \
	q_shared.cpp \
	splines.cpp \
	util_str.cpp

Q3UIFILES	:= \
	ui_addbots.o \
	ui_atoms.o \
	ui_cdkey.o \
	ui_cinematics.o \
	ui_confirm.o \
	ui_connect.o \
	ui_controls2.o \
	ui_credits.o \
	ui_demo2.o \
	ui_display.o \
	ui_gameinfo.o \
	ui_ingame.o \
	ui_loadconfig.o \
	ui_main.o \
	ui_menu.o \
	ui_mfield.o \
	ui_mods.o \
	ui_network.o \
	ui_options.o \
	ui_playermodel.o \
	ui_players.o \
	ui_playersettings.o \
	ui_preferences.o \
	ui_qmenu.o \
	ui_removebots.o \
	ui_saveconfig.o \
	ui_serverinfo.o \
	ui_servers2.o \
	ui_setup.o \
	ui_sound.o \
	ui_sparena.o \
	ui_specifyserver.o \
	ui_splevel.o \
	ui_sppostgame.o \
	ui_spskill.o \
	ui_startserver.o \
	ui_team.o \
	ui_teamorders.o \
	ui_video.o

UIFILES	:= \
	ui_syscalls.o

CFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.c))) \
				$(QCOMMONFILES)\
				$(SERVERFILES) \
				$(CLFILES) \
				$(SNDFILES) \
				$(BOTFILES) \
				$(RFILES) \
				$(UIFILES) \
				$(Q3UIFILES) \
				$(GFILES) \
				$(CGFILES)

CPPFILES	:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.cpp)))
SFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.s)))
PICAFILES	:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.pica)))
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

export OFILES	:=	$(addsuffix .o,$(BINFILES)) $(PICAFILES:.pica=.shbin.o) \
			$(CPPFILES:.cpp=.o) $(CFILES:.c=.o) $(SFILES:.s=.o)

export INCLUDE	:=	$(foreach dir,$(INCLUDES),-I$(CURDIR)/$(dir)) \
			$(foreach dir,$(LIBDIRS),-I$(dir)/include) \
			-I$(CURDIR)/$(BUILD)

export LIBPATHS	:=	$(foreach dir,$(LIBDIRS),-L$(dir)/lib)

ifeq ($(strip $(ICON)),)
	icons := $(wildcard *.png)
	ifneq (,$(findstring $(TARGET).png,$(icons)))
		export APP_ICON := $(TOPDIR)/$(TARGET).png
	else
		ifneq (,$(findstring icon.png,$(icons)))
			export APP_ICON := $(TOPDIR)/icon.png
		endif
	endif
else
	export APP_ICON := $(TOPDIR)/$(ICON)
endif

ifeq ($(strip $(NO_SMDH)),)
	export _3DSXFLAGS += --smdh=$(CURDIR)/$(TARGET).smdh
endif

.PHONY: $(BUILD) clean all

#---------------------------------------------------------------------------------
all: $(BUILD)

$(BUILD):
	@echo Building...
	@echo $(CFILES)
	@[ -d $@ ] || mkdir -p $@
	@$(MAKE) --no-print-directory -C $(BUILD) -f $(CURDIR)/Makefile

#---------------------------------------------------------------------------------
clean:
	@echo clean ...
	@rm -fr $(BUILD) $(TARGET).3dsx $(OUTPUT).smdh $(TARGET).elf


#---------------------------------------------------------------------------------
else

DEPENDS	:=	$(OFILES:.o=.d)

#---------------------------------------------------------------------------------
# main targets
#---------------------------------------------------------------------------------
ifeq ($(strip $(NO_SMDH)),)
$(OUTPUT).3dsx	:	$(OUTPUT).elf $(OUTPUT).smdh
else
$(OUTPUT).3dsx	:	$(OUTPUT).elf
endif

$(OUTPUT).elf	:	$(OFILES)

#---------------------------------------------------------------------------------
# rule for assembling GPU shaders
#---------------------------------------------------------------------------------
%.shbin.o: %.pica
	@echo $(notdir $<)
	$(eval CURBIN := $(patsubst %.pica,%.shbin,$(notdir $<)))
	$(eval CURH := $(patsubst %.pica,%.psh.h,$(notdir $<)))
	@picasso $(CURBIN) $< $(CURH)
	@bin2s $(CURBIN) | $(AS) -o $@
	@echo "extern const u8" `(echo $(CURBIN) | sed -e 's/^\([0-9]\)/_\1/' | tr . _)`"_end[];" > `(echo $(CURBIN) | tr . _)`.h
	@echo "extern const u8" `(echo $(CURBIN) | sed -e 's/^\([0-9]\)/_\1/' | tr . _)`"[];" >> `(echo $(CURBIN) | tr . _)`.h
	@echo "extern const u32" `(echo $(CURBIN) | sed -e 's/^\([0-9]\)/_\1/' | tr . _)`_size";" >> `(echo $(CURBIN) | tr . _)`.h

#---------------------------------------------------------------------------------
%.vsh.o	:	%.vsh
#---------------------------------------------------------------------------------
	@echo $(notdir $<)
	$(NIHSTRO)/nihstro-assemble.exe --input $(shell echo "$<" | sed -e 's/^\///' -e 's/\//\\/g' -e 's/^./\0:/') --output ../$(notdir $<).shbin
	@bin2s ../$(notdir $<).shbin | $(PREFIX)as -o $@
	@echo "extern const u8" `(echo $(notdir $<).shbin | sed -e 's/^\([0-9]\)/_\1/' | tr . _)`"_end[];" > `(echo $(notdir $<).shbin | tr . _)`.h
	@echo "extern const u8" `(echo $(notdir $<).shbin | sed -e 's/^\([0-9]\)/_\1/' | tr . _)`"[];" >> `(echo $(notdir $<).shbin | tr . _)`.h
	@echo "extern const u32" `(echo $(notdir $<).shbin | sed -e 's/^\([0-9]\)/_\1/' | tr . _)`_size";" >> `(echo $(notdir $<).shbin | tr . _)`.h
	@rm ../$(notdir $<).shbin

#---------------------------------------------------------------------------------
%.bin.o	:	%.bin
#---------------------------------------------------------------------------------
	@echo $(notdir $<)
	@$(bin2o)

-include $(DEPENDS)

#---------------------------------------------------------------------------------------
endif
#---------------------------------------------------------------------------------------
