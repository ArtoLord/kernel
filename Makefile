#
## Kernel make file
#

IDIR=./include
BUILD_DIR=./build
CC=gcc
AS=nasm
LD=ld
DD=dd
CC_FLAGS=-g -m32 -isystem $(IDIR) -I include -DKERNEL=1 -DTEST=1 -fno-stack-protector -Werror -fno-pie
AS_FLAGS= -felf
LD_FLAGS=-m elf_i386

all: build start

#
# Build kernel
#
build: build-kernel build-lib build-kernel-arch-asm build-kernel-arch-c \
	build-mem
	$(LD) $(LD_FLAGS) -T ./link.ld -o $(BUILD_DIR)/kernel.elf \
		$(BUILD_DIR)/*


build-kernel: ./src/kernel.c ./src/writer.c ./src/rb_tree.c
	$(CC) $(CC_FLAGS) -c ./src/kernel.c -o $(BUILD_DIR)/kernel.c.o
	$(CC) $(CC_FLAGS) -c ./src/rb_tree.c -o $(BUILD_DIR)/rb_tree.c.o
	$(CC) $(CC_FLAGS) -c ./src/writer.c -o $(BUILD_DIR)/writer.c.o

build-mem: ./src/mem.c ./src/mem/cash.c ./src/data/heap.c
	$(CC) $(CC_FLAGS) -c ./src/mem.c -o $(BUILD_DIR)/mem.c.o
	$(CC) $(CC_FLAGS) -c ./src/data/heap.c -o $(BUILD_DIR)/heap.c.o
	$(CC) $(CC_FLAGS) -c ./src/mem/cash.c -o $(BUILD_DIR)/cash.c.o

build-kernel-arch-asm: ./src/arch/*.s 
	$(AS) $(AS_FLAGS) ./src/arch/gdt.s -o $(BUILD_DIR)/gdt.s.o
	$(AS) $(AS_FLAGS) ./src/arch/entry.s -o $(BUILD_DIR)/entry.s.o
	$(AS) $(AS_FLAGS) ./src/arch/interrupt.s -o $(BUILD_DIR)/interrupt.s.o

build-kernel-arch-c: ./src/arch/*.c
	$(CC) $(CC_FLAGS) -c ./src/arch/common.c -o $(BUILD_DIR)/common.c.o
	$(CC) $(CC_FLAGS) -c ./src/arch/descriptor_tables.c -o $(BUILD_DIR)/dt.c.o
	$(CC) $(CC_FLAGS) -c ./src/arch/timer.c -o $(BUILD_DIR)/timer.c.o
	$(CC) $(CC_FLAGS) -c ./src/arch/isr.c -o $(BUILD_DIR)/isr.c.o

build-lib: ./stdlib/string.c  ./stdlib/deque.c
	$(CC) $(CC_FLAGS) -c ./stdlib/string.c -o $(BUILD_DIR)/string.c.o
	$(CC) $(CC_FLAGS) -c ./stdlib/deque.c -o $(BUILD_DIR)/deque.c.o

