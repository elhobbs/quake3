#---------------------------------------------------------------------------------
.SUFFIXES:
#---------------------------------------------------------------------------------

ifeq ($(strip $(DEVKITARM)),)
$(error "Please set DEVKITARM in your environment. export DEVKITARM=<path to>devkitARM")
endif

include $(DEVKITARM)/3ds_rules

NIHSTRO	:= /c/devl/nihstro/build/Debug

#---------------------------------------------------------------------------------
# TARGET is the name of the output
# BUILD is the directory where object files & intermediate files will be placed
# SOURCES is a list of directories containing source code
# DATA is a list of directories containing data files
# INCLUDES is a list of directories containing header files
#---------------------------------------------------------------------------------
GDIR		:=	../../../game
RDIR		:=	../../../renderer
TARGET		:=	$(shell basename $(CURDIR))
BUILD		:=	build
SOURCES		:=	source
DATA		:=	data
INCLUDES	:=	include include/gl $(GDIR)

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
LIBDIRS	:=	$(CTRULIB) \
			$(PORTLIBS)

#---------------------------------------------------------------------------------
# no real need to edit anything past this point unless you need to add additional
# rules for different file extensions
#---------------------------------------------------------------------------------
ifneq ($(BUILD),$(notdir $(CURDIR)))
#---------------------------------------------------------------------------------

export OUTPUT	:=	$(CURDIR)/lib/lib$(TARGET).a

export VPATH	:=	$(foreach dir,$(SOURCES),$(CURDIR)/$(dir)) \
			$(foreach dir,$(DATA),$(CURDIR)/$(dir)) \
			$(RDIR)

export DEPSDIR	:=	$(CURDIR)/$(BUILD)

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


CFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.c))) \
				$(RFILES)
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

export OFILES	:=	$(addsuffix .o,$(BINFILES)) $(PICAFILES:.pica=.shbin.o)\
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
