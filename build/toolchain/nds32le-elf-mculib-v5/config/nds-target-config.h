/* CPU Family Names  */
#define NDS_CPU andes-25-series
#define NDS_LIST_CPU "N22 N25 N25F N45 A25 A27 A45 D25 D25F D45 D23"
#define NDS_CORE "andes-25-series"
#define NDS_ARCH rv32imcxandes

/* Toolchain Target Names  */
#define NDS_TARGET riscv32-elf
#define NDS_ENDIAN little

/* Toolchain Target: nds32{be|le}-elf  */
#define NDS_LIB_C_DEFAULT mculib_static
#define NDS_LIB_CPP_DEFAULT mculib_static

/* Toolchain Option: -Os1, -Os2, -Os3  */
#define NDS_SUPPORT_mOs 3

/* Toolchain Option: -Og  */
#define NDS_SUPPORT_mOg 1

/* Toolchain Support CPU Type */
#define NDS_SUPPORT_mcpu_TYPE 1

/* Toolchain Option: -mcmodel  */
#define NDS_SUPPORT_CMODEL 1

/* Toolchain Support: LTO  */
#define NDS_SUPPORT_LTO 1

/* LTO Option Support: Need to duplicate compiler options to Linker when LTO option was enabled. */
#define NDS_SUPPORT_LTO_COMPILER_OPTIONS_TO_LINKER ["-g.*", "-f.*", "-m.*", "-O.*"]
#define NDS_SUPPORT_COMPILER {"items": [{"name": "gcc", "flag": "", "isdefault": "y"}, {"name": "llvm", "flag": ""}]}
