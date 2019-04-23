ROOT=..
PROJ_NAME = FYDE
include ${ROOT}/makedefs
SRCS += $(wildcard *.c)

$(PROJ_NAME).elf: $(OBJS)
		$(LD) $(LDFLAGS) -o $@ $^ $(LIBS)
