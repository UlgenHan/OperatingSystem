# Define variables
AS = nasm
ASFLAGS = -f bin
ASFLAGS_ELF = -f elf
CC = gcc
CFLAGS = -m32 -ffreestanding -fno-pie -fno-stack-protector  # Updated to disable PIC
LD = ld
LDFLAGS = -T linker.ld -m elf_i386 -e start  # Specify entry point as 'start'
OBJCOPY = objcopy
QEMU = qemu-system-i386

# Define file paths
BUILD_DIR = build
BOOTLOADER_DIR = bootloader
KERNEL_DIR = kernel

BOOTSECT_BIN = $(BUILD_DIR)/bootsect.bin
ENTRY_BIN = $(BUILD_DIR)/Entry.bin
IDT_BIN = $(BUILD_DIR)/IDT.bin
KERNEL_O = $(BUILD_DIR)/kernel.o
KERNEL_IMG = $(BUILD_DIR)/Kernel.img
KERNEL_BIN = $(BUILD_DIR)/kernel.bin
OS_IMAGE = $(BUILD_DIR)/os-image

# Define source files
KERNEL_C = $(KERNEL_DIR)/kernel.c
KERNEL_H = $(KERNEL_DIR)/kernel.h  # Added kernel.h as a dependency

# Targets and rules
all: $(OS_IMAGE)

# Ensure the build directory exists
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Assemble the boot sector
$(BOOTSECT_BIN): $(BUILD_DIR) $(BOOTLOADER_DIR)/Boot.asm
	$(AS) $(BOOTLOADER_DIR)/Boot.asm $(ASFLAGS) -o $@

# Assemble the kernel entry
$(ENTRY_BIN): $(BUILD_DIR) $(BOOTLOADER_DIR)/Kernel_Entry.asm
	$(AS) $(BOOTLOADER_DIR)/Kernel_Entry.asm $(ASFLAGS_ELF) -o $@

# Assemble the IDT
$(IDT_BIN): $(BUILD_DIR) $(BOOTLOADER_DIR)/IDT.asm
	$(AS) $(BOOTLOADER_DIR)/IDT.asm $(ASFLAGS_ELF) -o $@

# Compile the kernel (added dependency on kernel.h)
$(KERNEL_O): $(KERNEL_C) $(KERNEL_H) $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $(KERNEL_C) -o $@

# Link the kernel and other files, with custom entry point 'start'
$(KERNEL_IMG): $(ENTRY_BIN) $(KERNEL_O) $(IDT_BIN)
	$(LD) $(LDFLAGS) -o $@ $(ENTRY_BIN) $(KERNEL_O) $(IDT_BIN)

# Convert kernel image to binary format
$(KERNEL_BIN): $(KERNEL_IMG)
	$(OBJCOPY) -O binary -j .text $< $@

# Combine boot sector and kernel binary into OS image
$(OS_IMAGE): $(BOOTSECT_BIN) $(KERNEL_BIN)
	cat $(BOOTSECT_BIN) $(KERNEL_BIN) > $(OS_IMAGE)

# Run the OS image with QEMU
run: $(OS_IMAGE)
	$(QEMU) -drive format=raw,file=$(OS_IMAGE)

# Clean up build artifacts
clean:
	rm -f $(BUILD_DIR)/*.bin $(BUILD_DIR)/*.o $(BUILD_DIR)/*.img $(BUILD_DIR)/os-image

.PHONY: all clean run
