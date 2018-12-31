AS = $(CC)
CC = gcc
LD = $(CC)

ASFLAGS = -DASM_FILE

CFLAGS = \
	-Wall \
	-Werror \
	-Wextra \
	-std=c11 \
	-m64 \
	-ffreestanding \
	-fno-pie \
	-ffunction-sections \
	-fdata-sections \
	-mcmodel=kernel \
	-mno-red-zone

CPPFLAGS = -Iinclude

LDFLAGS = \
	-static \
	-ffreestanding \
	-nostdlib \
	-Wl,--build-id=none \
	-Wl,--fatal-warnings \
	-Wl,--gc-sections \
	-Wl,--print-gc-sections \
	-Wl,-z,defs \
	-Wl,-z,max-page-size=4096

LDLIBS = -lgcc
