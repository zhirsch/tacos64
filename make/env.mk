AS = $(CC)
CC = x86_64-elf-gcc
LD = $(CC)

ASFLAGS = -DASM_FILE

CFLAGS = \
	-Wall \
	-Werror \
	-Wextra \
	-ffreestanding \
	-mcmodel=large \
	-mno-mmx \
	-mno-red-zone \
	-mno-sse \
	-mno-sse2 \
	-std=c11

CPPFLAGS = -Iinclude

LDFLAGS = \
	-Wl,--fatal-warnings \
	-Wl,--gc-sections \
	-Wl,--print-gc-sections \
	-Wl,-z,defs \
	-Wl,-z,max-page-size=4096 \
	-ffreestanding \
	-nostdlib

LDLIBS = -lgcc
