project('pico-core-test', 'cpp', 'c',
  version : '1.0.0',
  default_options : ['warning_level=0', 'cpp_std=gnu++20', 'optimization=s'])

# ===================================================================================
# Raspberry Pi Pico (RP2040) Build System
# ===================================================================================
# This Meson build configuration mimics the Arduino IDE build process for RP2040 boards
# but provides more transparency and control over the build process.
#
# EXTENDING THIS BUILD SYSTEM:
# 1. For multiple source files:
#    - Create a 'src' directory and place your .cpp/.h files there
#    - Use 'subdir' to include nested meson.build files from subdirectories
#    - Replace the single sketch.cpp with a file_array of source files
#
# 2. For external dependencies:
#    - Use Meson's dependency() function to locate system libraries
#    - For custom libraries, create a subproject or specify include paths manually
#    - Consider using Meson's wrap system for third-party libraries
#
# 3. For configurable builds:
#    - Add build options using the meson_options.txt file
#    - Access them in this file via get_option('option_name')
#    - Create build variants with different configurations

# ===============================
# 1. BUILD CONFIGURATION
# ===============================
# Board configuration
# For multi-board support:
# 1. Move these configurations to meson_options.txt as build options
# 2. Create a board_config dictionary for multiple boards
# 3. Select the appropriate config based on the chosen board
board = 'generic'              # Generic RP2040 board
flash_size = '2MB'             # Flash memory size
cpu_speed = '133MHz'           # CPU clock speed
optimize = '-Os'               # Optimization level: -Os = optimize for size
arduino_version = 10607        # Arduino API version (corresponds to IDE version)
pico_flash_size_bytes = 2097152  # 2MB = 2 * 1024 * 1024 bytes

# ===============================
# 2. PATH CONFIGURATION
# ===============================
# Project paths
# For larger projects:
# 1. Define source/include directories separately
# 2. Use file globbing or explicit file lists for multiple source files
# 3. Consider organizing code into libraries with their own meson.build files
workspace_dir = meson.current_source_dir()
sketch_file = workspace_dir / 'sketch.cpp'
build_dir = meson.current_build_dir()

# Arduino toolchain paths
# For custom toolchains:
# 1. Make these configurable via meson_options.txt
# 2. Add support for environment variables to override paths
# 3. Add fallbacks for different development environments
arduino15_dir = '/home/w1n5t0n/.arduino15'
rp2040_package_dir = arduino15_dir / 'packages/rp2040'
hardware_dir = rp2040_package_dir / 'hardware/rp2040/4.4.4'
tools_dir = rp2040_package_dir / 'tools'

# Tool paths
# For CI/CD environments:
# 1. Check for tools in system PATH as fallback
# 2. Add version checks for critical tools
# 3. Support downloading missing tools automatically
gcc_path = tools_dir / 'pqt-gcc/4.0.1-8ec9d6f/bin'
python3_path = tools_dir / 'pqt-python3/1.0.1-base-3a57aed-1/python3'
picotool_path = tools_dir / 'pqt-picotool/4.0.1-8ec9d6f/picotool'
ctags_path = arduino15_dir / 'packages/builtin/tools/ctags/5.8-arduino11/ctags'

# Core paths
# When extending for custom frameworks:
# 1. Support multiple core selections
# 2. Add ability to use custom cores/variants
# 3. Create configuration for framework-specific include paths
rp2040_core_path = hardware_dir / 'cores/rp2040'
rp2040_variant_path = hardware_dir / 'variants/generic'
pico_sdk_dir = hardware_dir / 'pico-sdk'

# Find Python interpreter
# If your build requires specific Python modules:
# 1. Add a requirements.txt file for Python dependencies
# 2. Create a custom target to install dependencies if needed
# 3. Check for minimum Python version
python = find_program('python3', required : true)

# ===============================
# 3. COMPILER FLAGS
# ===============================
# Common compiler flags used for all compilation steps
# For complex projects:
# 1. Separate flags by purpose (optimization, warnings, features)
# 2. Create target-specific flag collections for different build types
# 3. Support conditional flags based on build options
common_flags = [
  # Warning flags
  '-Werror=return-type',
  '-Wno-psabi',
  
  # USB configuration
  '-DUSBD_PID=0xf00a',
  '-DUSBD_VID=0x2e8a',
  '-DUSBD_MAX_POWER_MA=250',
  '-DUSB_MANUFACTURER="Generic"',
  '-DUSB_PRODUCT="RP2040"',
  
  # Network stack configuration
  '-DLWIP_IPV6=0',
  '-DLWIP_IPV4=1',
  '-DLWIP_IGMP=1',
  '-DLWIP_CHECKSUM_CTRL_PER_NETIF=1',
  
  # Filesystem and file handling configuration
  '-DFILE_COPY_CONSTRUCTOR_SELECT=FILE_COPY_CONSTRUCTOR_PUBLIC',
  '-DUSE_UTF8_LONG_NAMES=1',
  '-DDISABLE_FS_H_WARNING=1',
  
  # Board configuration
  '-DARDUINO_VARIANT="generic"',
  '-DPICO_FLASH_SIZE_BYTES=' + pico_flash_size_bytes.to_string(),
  
  # Architecture-specific flags
  '-march=armv6-m',
  '-mcpu=cortex-m0plus',
  '-mthumb',
  
  # Code size optimization
  '-ffunction-sections',
  '-fdata-sections',
  '-fno-exceptions'
]

# CPU-specific flags
# For multi-target support:
# 1. Create a dictionary of flags for different CPU configurations
# 2. Select appropriate flags based on build options
cpu_flags = [
  '-DF_CPU=133000000L',
  '-DARDUINO=' + arduino_version.to_string(),
  '-DARDUINO_GENERIC_RP2040',
  '-DBOARD_NAME="GENERIC_RP2040"',
  '-DARDUINO_ARCH_RP2040',
  optimize,
  '-DWIFICC=CYW43_COUNTRY_WORLDWIDE'
]

# Linker flags for undefined references 
# These force inclusion of important initialization functions
# For custom frameworks or non-Arduino projects:
# 1. Make these configurable based on the target platform
# 2. Create separate collections for different chip variants
# 3. Support conditional linking based on enabled features
undefined_flags = [
  '-Wl,--undefined=runtime_init_install_ram_vector_table',
  '-Wl,--undefined=__pre_init_runtime_init_clocks',
  '-Wl,--undefined=__pre_init_runtime_init_bootrom_reset',
  '-Wl,--undefined=__pre_init_runtime_init_early_resets',
  '-Wl,--undefined=__pre_init_runtime_init_usb_power_down',
  '-Wl,--undefined=__pre_init_runtime_init_clocks',
  '-Wl,--undefined=__pre_init_runtime_init_post_clock_resets',
  '-Wl,--undefined=__pre_init_runtime_init_spin_locks_reset',
  '-Wl,--undefined=__pre_init_runtime_init_boot_locks_reset',
  '-Wl,--undefined=__pre_init_runtime_init_bootrom_locking_enable',
  '-Wl,--undefined=__pre_init_runtime_init_mutex',
  '-Wl,--undefined=__pre_init_runtime_init_default_alarm_pool',
  '-Wl,--undefined=__pre_init_first_per_core_initializer',
  '-Wl,--undefined=__pre_init_runtime_init_per_core_bootrom_reset',
  '-Wl,--undefined=__pre_init_runtime_init_per_core_h3_irq_registers',
  '-Wl,--undefined=__pre_init_runtime_init_per_core_irq_priorities',
]

# ===============================
# 4. INCLUDE PATHS & FILES
# ===============================
# Basic include paths for headers
# For complex projects:
# 1. Organize includes by component or module
# 2. Support both system and project-specific include paths
# 3. Create include path collections for different build configurations
rp2040_includes = [
  '-I' + rp2040_core_path,
  '-I' + rp2040_variant_path,
  '-I' + hardware_dir / 'include'
]

# Additional include files that may be needed
# To support custom modules:
# 1. Detect additional include files dynamically
# 2. Allow project-specific include definitions
# 3. Support conditional inclusion based on build options
platform_inc_file = hardware_dir / 'lib/rp2040/platform_inc.txt'
core_inc_file = hardware_dir / 'lib/core_inc.txt'
platform_def_file = hardware_dir / 'lib/rp2040/platform_def.txt'
platform_wrap_file = hardware_dir / 'lib/rp2040/platform_wrap.txt'
core_wrap_file = hardware_dir / 'lib/core_wrap.txt'

# Check if these files exist (will be used during the build)
# For robust build systems:
# 1. Add more meaningful error messages for missing files
# 2. Make certain files optional with fallbacks
# 3. Support generating these files if they don't exist
file_check = custom_target(
  'file_check',
  command : [python, '-c', '''
import os

files_to_check = {
    "platform_inc_file": "''' + platform_inc_file + '''",
    "core_inc_file": "''' + core_inc_file + '''",
    "platform_def_file": "''' + platform_def_file + '''",
    "platform_wrap_file": "''' + platform_wrap_file + '''",
    "core_wrap_file": "''' + core_wrap_file + '''"
}

results = {}
build_dir = "''' + build_dir + '''"

for name, path in files_to_check.items():
    exists = os.path.exists(path)
    results[name] = exists
    with open(f"{build_dir}/{name}_exists", "w") as f:
        f.write("1" if exists else "0")
    
    print(f"{name}: {'Found' if exists else 'Not found'} at {path}")
  '''],
  output : 'file_check_output',
  build_by_default : true,
)

# ===============================
# 5. ARDUINO CORE LIBRARY
# ===============================
# Find and copy precompiled core.a if available
# For custom cores or libraries:
# 1. Support building the core from source when precompiled isn't available
# 2. Allow specifying multiple libraries to include
# 3. Add support for version checking of libraries
tmp_cores_dir = '/tmp/arduino/cores'
core_setup = custom_target(
  'core_setup',
  command : [python, '-c', '''
import os
import shutil

tmp_cores_dir = "''' + tmp_cores_dir + '''"
build_dir = "''' + build_dir + '''"
hardware_dir = "''' + hardware_dir + '''"

# Try to find core.a
core_a_path = ""
if os.path.exists(tmp_cores_dir):
    for root, dirs, files in os.walk(tmp_cores_dir):
        if "core.a" in files:
            core_a_path = os.path.join(root, "core.a")
            break

# If found, copy to build dir
if core_a_path:
    print(f"Found precompiled core.a at: {core_a_path}")
    dst = os.path.join(build_dir, "core.a")
    shutil.copy2(core_a_path, dst)
    print(f"Copied to: {dst}")
else:
    print("WARNING: Could not find precompiled core.a")
    hardware_core = os.path.join(hardware_dir, "lib/rp2040/libpico.a")
    if os.path.exists(hardware_core):
        print(f"Using alternate: {hardware_core}")
        dst = os.path.join(build_dir, "core.a")
        shutil.copy2(hardware_core, dst)
    else:
        print("ERROR: Cannot find required library files")
  '''],
  output : 'core_setup_output',
  build_by_default : true,
)

# ===============================
# 6. BUILD PROCESS STEPS
# ===============================

# STEP 1: Prepare the sketch
# For multi-file projects:
# 1. Replace this with a foreach loop over all source files
# 2. Support different source directories
# 3. Add file type detection and appropriate compiler flags
sketch_combined = custom_target(
  'prepare_sketch',
  output : 'sketch_combined.cpp',
  command : ['cp', sketch_file, '@OUTPUT@'],
  build_by_default : true,
)

# STEP 2: Process sketch to generate function prototypes
# For larger projects with multiple source files:
# 1. Create a dedicated target for preprocessing each source file
# 2. Use Meson's 'foreach' to iterate through an array of source files:
#    - src_files = ['src/main.cpp', 'src/module1.cpp', 'src/module2.cpp']
#    - merged_files = []
#    - foreach src : src_files
#        merged_files += custom_target(...)
# 3. Combine preprocessed files into a single unit if needed for prototype generation
#    - Use Python/shell script to concatenate preprocessed files
# 4. Organize file dependencies to track changes and trigger rebuilds efficiently
sketch_merged = custom_target(
  'sketch_merged',
  input : sketch_combined,
  output : 'sketch_merged.cpp',
  command : [
    gcc_path / 'arm-none-eabi-g++',
    '-c', common_flags, '@' + platform_def_file, '-iprefix' + hardware_dir / '',
    rp2040_includes, '@' + platform_inc_file, '@' + core_inc_file,
    '-fno-rtti', '-std=gnu++20', '-g', '-pipe', '-w', '-x', 'c++', '-E', '-CC',
    cpu_flags, sketch_combined, '-o', '@OUTPUT@'
  ],
  build_by_default : true,
  depends : file_check,
)

# Generate function prototypes with ctags
# For complex multi-file projects:
# 1. Run ctags on all source files to generate comprehensive prototype information
# 2. Parse the ctags output to create organized header files by module/component
# 3. Support selective inclusion based on build configuration
# 4. Consider using custom generators for API documentation from prototypes
ctags_gen = custom_target(
  'ctags_gen',
  input : sketch_merged,
  output : 'ctags_output',
  command : [
    ctags_path,
    '-u', '--language-force=c++', '-f', '-', '--c++-kinds=svpf', 
    '--fields=KSTtzns', '--line-directives', sketch_merged
  ],
  build_by_default : true,
  capture : true,
)

# Generate signing keys header (optional)
# For secure firmware distribution:
# 1. Integrate with secure key storage systems for production environments
# 2. Support different key types/lengths for various security levels
# 3. Add version signing and verification capabilities
# 4. Consider implementing secure boot chain verification
signing_header = custom_target(
  'signing_header',
  output : 'Updater_Signing.h',
  command : [
    python3_path, '-I', hardware_dir / 'tools/signing.py',
    '--mode', 'header', '--publickey', build_dir / 'public.key',
    '--out', '@OUTPUT@'
  ],
  build_by_default : false,  # Optional, so not required by default
)

# STEP 3: Compile the sketch
# For larger projects with multiple source files:
# 1. Replace with a foreach loop over source file array:
#    - obj_files = []
#    - foreach src : src_files
#        obj_files += custom_target('compile_' + src.underscorify(), ...)
# 2. Add support for different compilation flags per file/directory:
#    - Create flag collections for different source types (core, drivers, app)
#    - Apply appropriate flags based on file path or explicit mappings
# 3. Implement parallel compilation with proper dependency tracking
# 4. Support precompiled headers for faster builds
# 5. Add build configurations (debug, release, test) with different flag sets
sketch_obj = custom_target(
  'sketch_obj',
  input : sketch_combined,
  output : 'sketch_combined.cpp.o',
  command : [
    gcc_path / 'arm-none-eabi-g++',
    '-c', common_flags, '@' + platform_def_file, '-MMD',
    '-iprefix' + hardware_dir / '',
    rp2040_includes, '@' + platform_inc_file, '@' + core_inc_file,
    '-fno-rtti', '-std=gnu++20', '-g', '-pipe',
    cpu_flags, '@INPUT@', '-o', '@OUTPUT@'
  ],
  build_by_default : true,
  depends : [file_check, ctags_gen]
)

# Compile boot2 stage
# For different bootloaders or multi-stage boot processes:
# 1. Make bootloader selection configurable based on board/memory requirements
# 2. Support custom bootloaders with different features (encryption, verification)
# 3. Add bootloader configuration options (e.g., boot timeouts, fallback options)
# 4. Support specialized boot processes for different deployment scenarios
boot2_obj = custom_target(
  'boot2_obj',
  output : 'boot2.o',
  command : [
    gcc_path / 'arm-none-eabi-gcc',
    common_flags, '@' + platform_def_file, optimize,
    '-u', '_printf_float', '-u', '_scanf_float', '-c',
    hardware_dir / 'boot2/rp2040/boot2_generic_03h_2_padded_checksum.S',
    '-I' + pico_sdk_dir / 'src/rp2040/hardware_regs/include/',
    '-I' + pico_sdk_dir / 'src/common/pico_binary_info/include',
    '-o', '@OUTPUT@'
  ],
  build_by_default : true,
  depends : file_check,
)

# STEP 4: Create memory map
# For advanced memory management:
# 1. Support different memory layouts based on feature sets and available RAM/flash
# 2. Create dedicated sections for different types of code/data:
#    - Add separate sections for critical/real-time code
#    - Create protected memory regions for secure operations
# 3. Support dynamic memory allocation schemes for different application profiles
# 4. Add memory protection configurations for safety-critical applications
# 5. Define memory regions for DMA operations and special peripheral access
sketch_flash_size = 2093056  # 2MB - some overhead

memmap_default = custom_target(
  'memmap_default',
  output : 'memmap_default.ld',
  command : [
    python3_path, '-I', hardware_dir / 'tools/simplesub.py',
    '--input', hardware_dir / 'lib/rp2040/memmap_default.ld',
    '--out', '@OUTPUT@',
    '--sub', '__FLASH_LENGTH__', sketch_flash_size.to_string(),
    '--sub', '__EEPROM_START__', '270528512',
    '--sub', '__FS_START__', '270528512',
    '--sub', '__FS_END__', '270528512',
    '--sub', '__RAM_LENGTH__', '256k',
    '--sub', '__PSRAM_LENGTH__', '0'
  ],
  build_by_default : true,
)

# STEP 5: Link everything together
# For complex projects with multiple components:
# 1. Support modular linking with multiple object files:
#    - Replace sketch_obj with an array of object files collected from compilation
#    - Use a joining pattern like: ' '.join([obj.full_path() for obj in obj_files])
# 2. Add support for static and dynamic libraries:
#    - Create dedicated targets for building libraries
#    - Support third-party library integration with dependency() function
# 3. Implement link-time optimization for performance-critical applications
# 4. Create specialized linker scripts for different memory configurations
# 5. Support conditional linking based on feature flags:
#    - Only link in modules that are needed for the current configuration
# 6. Add profiling and debugging information as needed
sketch_elf = custom_target(
  'sketch_elf',
  depends : [core_setup, file_check],
  input : [sketch_obj, boot2_obj, memmap_default],
  output : 'sketch.ino.elf',
  command : [
    gcc_path / 'arm-none-eabi-g++',
    '-L' + build_dir, common_flags, '@' + platform_def_file,
    optimize, '-u', '_printf_float', '-u', '_scanf_float',
    '@' + platform_wrap_file, '@' + core_wrap_file,
    '-Wl,--cref', '-Wl,--check-sections', '-Wl,--gc-sections',
    '-Wl,--unresolved-symbols=report-all', '-Wl,--warn-common',
    undefined_flags, '-Wl,--script=' + build_dir / 'memmap_default.ld',
    '-Wl,-Map,' + build_dir / 'sketch.ino.map',
    '-o', '@OUTPUT@', '-Wl,--no-warn-rwx-segments',
    '-Wl,--start-group',
    sketch_obj, build_dir / 'core.a', boot2_obj,
    hardware_dir / 'lib/rp2040/ota.o',
    hardware_dir / 'lib/rp2040/libpico.a',
    hardware_dir / 'lib/rp2040/libipv4.a',
    hardware_dir / 'lib/rp2040/libbearssl.a',
    '-lm', '-lc', '-lstdc++', '-lc',
    '-Wl,--end-group'
  ],
  build_by_default : true,
)

# STEP 6: Create binary output formats
# For deployment packaging and distribution:
# 1. Add support for additional output formats based on deployment needs:
#    - Create hex files for certain programmers
#    - Generate encrypted firmware packages for secure deployment
# 2. Add firmware versioning and metadata:
#    - Embed version info, build date, and configuration details in binary
#    - Generate checksums/hashes for integrity verification
# 3. Support firmware packaging for different deployment methods:
#    - OTA update packages with delta compression
#    - Bootloader-compatible formats for various update mechanisms
# 4. Create deployment packages with documentation and support files
sketch_bin = custom_target(
  'sketch_bin',
  input : sketch_elf,
  output : 'sketch.ino.bin',
  command : [
    gcc_path / 'arm-none-eabi-objcopy',
    '-Obinary', '@INPUT@', '@OUTPUT@'
  ],
  build_by_default : true,
)

sketch_uf2 = custom_target(
  'sketch_uf2',
  input : sketch_elf,
  output : 'sketch.ino.uf2',
  command : [
    picotool_path,
    'uf2', 'convert', '@INPUT@', '@OUTPUT@', '--family', 'rp2040'
  ],
  build_by_default : true,
)

# ===============================
# 7. BUILD SUMMARY
# ===============================
# For comprehensive build reporting:
# 1. Add detailed memory usage analysis by component/module
# 2. Generate reports comparing current build with previous builds
# 3. Perform static analysis checks and include results in summary
# 4. Add performance estimates based on compile-time metrics
# 5. Generate documentation from build artifacts (API docs, memory maps)
# 6. Include test coverage information when tests are part of the build
build_summary = custom_target(
  'build_summary',
  command : [python, '-c', '''
import os
import subprocess

build_dir = "''' + build_dir + '''"
gcc_path = "''' + gcc_path + '''"
elf_file = os.path.join(build_dir, "sketch.ino.elf")

# Print header
print("=======================================================")
print("Raspberry Pi Pico (RP2040) Build Summary")
print("=======================================================")
print("- Board: Generic RP2040")
print("- Flash Size: 2MB")
print("- CPU Speed: 133MHz")
print("- Optimize: -Os")
print("- Arduino Version: 10607")
print("-------------------------------------------------------")

# Print size information if the ELF file exists
if os.path.exists(elf_file):
    size_cmd = os.path.join(gcc_path, "arm-none-eabi-size") + " -A " + elf_file
    try:
        size_output = subprocess.check_output(size_cmd, shell=True, text=True)
        # Filter for relevant sections
        for line in size_output.split("\\n"):
            if any(section in line for section in ["Total", ".text", ".data", ".bss", ".rodata"]):
                print(line)
    except subprocess.CalledProcessError as e:
        print(f"Error getting size information: {e}")
else:
    print("ELF file not found - build may have failed")

# Print output files
print("-------------------------------------------------------")
print("Build complete!")
print("Output files:")
print(f"- ELF: {elf_file}")
print(f"- BIN: {os.path.join(build_dir, 'sketch.ino.bin')}")
print(f"- UF2: {os.path.join(build_dir, 'sketch.ino.uf2')}")

# Print upload instructions
print("-------------------------------------------------------")
print("Upload instructions:")
print("1. Press and hold the BOOTSEL button on the Pico")
print("2. Connect the Pico to your computer via USB")
print("3. Release the BOOTSEL button")
print("4. The Pico should appear as a USB drive")
print("5. Copy the UF2 file to the Pico drive")
  '''],
  input : sketch_uf2,
  output : 'build_info.txt',
  build_by_default : true,
  capture : true,
)

# Default target - ensures everything is built
# For complex project management:
# 1. Add specialized targets for different build goals:
#    - Create 'test', 'docs', 'analyze' targets for specific workflows
#    - Add deployment targets for different environments (dev, staging, prod)
# 2. Support incremental and partial builds for large projects:
#    - Allow building specific components or modules
#    - Create dependency-aware rebuilds for efficient development
# 3. Add continuous integration hooks for automated building and testing
default_target = custom_target(
  'default',
  command : ['echo', 'Build completed successfully!'],
  output : 'build_complete',
  build_by_default : true,
  depends : [sketch_bin, sketch_uf2, build_summary],
)