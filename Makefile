B = bld
C = cfg
S = src

OBJS = \
	$(B)/boot.o \
	$(B)/kernel.o \
	$(B)/multiboot_header.o

include make/env.mk
include make/iso.mk

all: $(B)/tacos64 $(B)/tacos64.iso

clean:
	rm -rf $(B)

$(B)/tacos64: $(OBJS) $(C)/link.ld
	$(LD) $(LDFLAGS) -T $(C)/link.ld $(OBJS) -o $@ $(LDLIBS)

$(B)/%.o: $(S)/%.c | $(B)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

$(B)/%.o: $(S)/%.S | $(B)
	$(AS) $(CPPFLAGS) $(ASFLAGS) -c -o $@ $<

$(B):
	mkdir -p $(B)

.PHONY: all clean