// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.

/*****************************************************************************
 *  Arm64 instructions for JIT compiler
 *
 *          id      -- the enum name for the instruction
 *          nm      -- textual name (for assembly dipslay)
 *          info    -- miscellaneous instruction info (load/store/compare/ASIMD right shift)
 *          fmt     -- encoding format used by this instruction
 *          e1      -- encoding 1
 *          e2      -- encoding 2
 *          e3      -- encoding 3
 *          e4      -- encoding 4
 *          e5      -- encoding 5
 *          e6      -- encoding 6
 *          e7      -- encoding 7
 *          e8      -- encoding 8
 *          e9      -- encoding 9
 *
 ******************************************************************************/

#if !defined(TARGET_ARM64)
#error Unexpected target type
#endif

#ifndef INST1
#error INST1 must be defined before including this file.
#endif
#ifndef INST2
#error INST2 must be defined before including this file.
#endif
#ifndef INST3
#error INST3 must be defined before including this file.
#endif
#ifndef INST4
#error INST4 must be defined before including this file.
#endif
#ifndef INST5
#error INST5 must be defined before including this file.
#endif
#ifndef INST6
#error INST6 must be defined before including this file.
#endif
#ifndef INST9
#error INST9 must be defined before including this file.
#endif

/*****************************************************************************/
/*               The following is ARM64-specific                             */
/*****************************************************************************/

// If you're adding a new instruction:
// You need not only to fill in one of these macros describing the instruction, but also:
//   * If the instruction writes to more than one destination register, update the function
//     emitInsMayWriteMultipleRegs in emitArm64.cpp.

// clang-format off
INST9(invalid,     "INVALID",      0,      IF_NONE,   BAD_CODE,    BAD_CODE,    BAD_CODE,    BAD_CODE,    BAD_CODE,    BAD_CODE,    BAD_CODE,    BAD_CODE,    BAD_CODE)

//    enum         name            info               DR_2E        DR_2G        DI_1B        DI_1D        DV_3C        DV_2B        DV_2C        DV_2E        DV_2F
INST9(mov,         "mov",          0,      IF_EN9,    0x2A0003E0,  0x11000000,  0x52800000,  0x320003E0,  0x0EA01C00,  0x0E003C00,  0x4E001C00,  0x5E000400,  0x6E000400)
                                   //  mov     Rd,Rm                DR_2E  X0101010000mmmmm 00000011111ddddd   2A00 03E0
                                   //  mov     Rd,Rn                DR_2G  X001000100000000 000000nnnnnddddd   1100 0000   mov to/from SP only
                                   //  mov     Rd,imm(i16,hw)       DI_1B  X10100101hwiiiii iiiiiiiiiiiddddd   5280 0000   imm(i16,hw)
                                   //  mov     Rd,imm(N,r,s)        DI_1D  X01100100Nrrrrrr ssssss11111ddddd   3200 03E0   imm(N,r,s)
                                   //  mov     Vd,Vn                DV_3C  0Q001110101nnnnn 000111nnnnnddddd   0EA0 1C00   Vd,Vn
                                   //  mov     Rd,Vn[0]             DV_2B  0Q001110000iiiii 001111nnnnnddddd   0E00 3C00   Rd,Vn[]   (to general)
                                   //  mov     Vd[],Rn              DV_2C  01001110000iiiii 000111nnnnnddddd   4E00 1C00   Vd[],Rn   (from general)
                                   //  mov     Vd,Vn[]              DV_2E  01011110000iiiii 000001nnnnnddddd   5E00 0400   Vd,Vn[]   (scalar by element)
                                   //  mov     Vd[],Vn[]            DV_2F  01101110000iiiii 0jjjj1nnnnnddddd   6E00 0400   Vd[],Vn[] (from/to elem)

//    enum         name            info               DR_3A        DR_3B        DR_3C        DI_2A        DV_3A        DV_3E
INST6(add,         "add",          0,      IF_EN6A,   0x0B000000,  0x0B000000,  0x0B200000,  0x11000000,  0x0E208400,  0x5E208400)
                                   //  add     Rd,Rn,Rm             DR_3A  X0001011000mmmmm 000000nnnnnddddd   0B00 0000   Rd,Rn,Rm
                                   //  add     Rd,Rn,(Rm,shk,imm)   DR_3B  X0001011sh0mmmmm ssssssnnnnnddddd   0B00 0000   Rm {LSL,LSR,ASR} imm(0-63)
                                   //  add     Rd,Rn,(Rm,ext,shl)   DR_3C  X0001011001mmmmm ooosssnnnnnddddd   0B20 0000   ext(Rm) LSL imm(0-4)
                                   //  add     Rd,Rn,i12            DI_2A  X0010001shiiiiii iiiiiinnnnnddddd   1100 0000   imm(i12,sh)
                                   //  add     Vd,Vn,Vm             DV_3A  0Q001110XX1mmmmm 100001nnnnnddddd   0E20 8400   Vd,Vn,Vm  (vector)
                                   //  add     Vd,Vn,Vm             DV_3E  01011110XX1mmmmm 100001nnnnnddddd   5E20 8400   Vd,Vn,Vm  (scalar)

INST6(sub,         "sub",          0,      IF_EN6A,   0x4B000000,  0x4B000000,  0x4B200000,  0x51000000,  0x2E208400,  0x7E208400)
                                   //  sub     Rd,Rn,Rm             DR_3A  X1001011000mmmmm 000000nnnnnddddd   4B00 0000   Rd,Rn,Rm
                                   //  sub     Rd,Rn,(Rm,shk,imm)   DR_3B  X1001011sh0mmmmm ssssssnnnnnddddd   4B00 0000   Rm {LSL,LSR,ASR} imm(0-63)
                                   //  sub     Rd,Rn,(Rm,ext,shl)   DR_3C  X1001011001mmmmm ooosssnnnnnddddd   4B20 0000   ext(Rm) LSL imm(0-4)
                                   //  sub     Rd,Rn,i12            DI_2A  X1010001shiiiiii iiiiiinnnnnddddd   5100 0000   imm(i12,sh)
                                   //  sub     Vd,Vn,Vm             DV_3A  0Q101110XX1mmmmm 100001nnnnnddddd   2E20 8400   Vd,Vn,Vm  (vector)
                                   //  sub     Vd,Vn,Vm             DV_3E  01111110XX1mmmmm 100001nnnnnddddd   7E20 8400   Vd,Vn,Vm  (scalar)

//    enum         name            info               LS_2D        LS_3F        LS_2E        LS_2F        LS_3G        LS_2G
INST6(ld1,         "ld1",          LD,     IF_EN6B,   0x0C407000,  0x0CC07000,  0x0CDF7000,  0x0D400000,  0x0DC00000,  0x0DDF0000)
                                   // LD1 (multiple structures, one register variant)
                                   //  ld1     {Vt},[Xn]            LS_2D  0Q00110001000000 0111ssnnnnnttttt   0C40 7000   base register
                                   //  ld1     {Vt},[Xn],Xm         LS_3F  0Q001100110mmmmm 0111ssnnnnnttttt   0CC0 7000   post-indexed by a register
                                   //  ld1     {Vt},[Xn],#imm       LS_2E  0Q00110011011111 0111ssnnnnnttttt   0CDF 7000   post-indexed by an immediate
                                   // LD1 (single structure)
                                   //  ld1     {Vt}[],[Xn]          LS_2F  0Q00110101000000 xx0Sssnnnnnttttt   0D40 0000   base register
                                   //  ld1     {Vt}[],[Xn],Xm       LS_3G  0Q001101110mmmmm xx0Sssnnnnnttttt   0DC0 0000   post-indexed by a register
                                   //  ld1     {Vt}[],[Xn],#imm     LS_2G  0Q00110111011111 xx0Sssnnnnnttttt   0DDF 0000   post-indexed by an immediate

INST6(ld2,         "ld2",          LD,     IF_EN6B,   0x0C408000,  0x0CC08000,  0x0CDF8000,  0x0D600000,  0x0DE00000,  0x0DFF0000)
                                   // LD2 (multiple structures)
                                   //  ld2     {Vt,Vt2},[Xn]        LS_2D  0Q00110001000000 1000ssnnnnnttttt   0C40 8000   base register
                                   //  ld2     {Vt,Vt2},[Xn],Xm     LS_3F  0Q001100110mmmmm 1000ssnnnnnttttt   0CC0 8000   post-indexed by a register
                                   //  ld2     {Vt,Vt2},[Xn],#imm   LS_2E  0Q001100110mmmmm 1000ssnnnnnttttt   0CDF 8000   post-indexed by an immediate
                                   // LD2 (single structure)
                                   //  ld2     {Vt,Vt2}[],[Xn]      LS_2F  0Q00110101100000 xx0Sssnnnnnttttt   0D60 0000   base register
                                   //  ld2     {Vt,Vt2}[],[Xn],Xm   LS_3G  0Q001101111mmmmm xx0Sssnnnnnttttt   0DE0 0000   post-indexed by a register
                                   //  ld2     {Vt,Vt2}[],[Xn],#imm LS_2G  0Q00110111111111 xx0Sssnnnnnttttt   0DFF 0000   post-indexed by an immediate

INST6(ld3,         "ld3",          LD,     IF_EN6B,   0x0C404000,  0x0CC04000,  0x0CDF4000,  0x0D402000,  0x0DC02000,  0x0DDF2000)
                                   // LD3 (multiple structures)
                                   //  ld3     {Vt-Vt3},[Xn]        LS_2D  0Q00110001000000 0100ssnnnnnttttt   0C40 4000   base register
                                   //  ld3     {Vt-Vt3},[Xn],Xm     LS_3F  0Q001100110mmmmm 0100ssnnnnnttttt   0CC0 4000   post-indexed by a register
                                   //  ld3     {Vt-Vt3},[Xn],#imm   LS_2E  0Q001100110mmmmm 0100ssnnnnnttttt   0CDF 4000   post-indexed by an immediate
                                   // LD3 (single structure)
                                   //  ld3     {Vt-Vt3}[],[Xn]      LS_2F  0Q00110101000000 xx1Sssnnnnnttttt   0D40 2000   base register
                                   //  ld3     {Vt-Vt3}[],[Xn],Xm   LS_3G  0Q001101110mmmmm xx1Sssnnnnnttttt   0DC0 2000   post-indexed by a register
                                   //  ld3     {Vt-Vt3}[],[Xn],#imm LS_2G  0Q00110111011111 xx1Sssnnnnnttttt   0DDF 2000   post-indexed by an immediate

INST6(ld4,         "ld4",          LD,     IF_EN6B,   0x0C400000,  0x0CC00000,  0x0CDF0000,  0x0D602000,  0x0DE02000,  0x0DFF2000)
                                   // LD4 (multiple structures)
                                   //  ld4     {Vt-Vt4},[Xn]        LS_2D  0Q00110001000000 0000ssnnnnnttttt   0C40 0000   base register
                                   //  ld4     {Vt-Vt4},[Xn],Xm     LS_3F  0Q001100110mmmmm 0000ssnnnnnttttt   0CC0 0000   post-indexed by a register
                                   //  ld4     {Vt-Vt4},[Xn],#imm   LS_2E  0Q00110011011111 0000ssnnnnnttttt   0CDF 0000   post-indexed by an immediate
                                   // LD4 (single structure)
                                   //  ld4     {Vt-Vt4}[],[Xn]      LS_2F  0Q00110101100000 xx1Sssnnnnnttttt   0D60 2000   base register
                                   //  ld4     {Vt-Vt4}[],[Xn],Xm   LS_3G  0Q001101111mmmmm xx1Sssnnnnnttttt   0DE0 2000   post-indexed by a register
                                   //  ld4     {Vt-Vt4}[],[Xn],#imm LS_2G  0Q00110111111111 xx1Sssnnnnnttttt   0DFF 2000   post-indexed by an immediate

INST6(st1,         "st1",          LD,     IF_EN6B,   0x0C007000,  0x0C807000,  0x0C9F7000,  0x0D000000,  0x0D800000,  0x0D9F0000)
                                   // ST1 (multiple structures, one register variant)
                                   //  st1     {Vt},[Xn]            LS_2D  0Q00110000000000 0111ssnnnnnttttt   0C00 7000   base register
                                   //  st1     {Vt},[Xn],Xm         LS_3F  0Q001100100mmmmm 0111ssnnnnnttttt   0C80 7000   post-indexed by a register
                                   //  st1     {Vt},[Xn],#imm       LS_2E  0Q00110010011111 0111ssnnnnnttttt   0C9F 7000   post-indexed by an immediate
                                   // ST1 (single structure)
                                   //  st1     {Vt}[],[Xn]          LS_2F  0Q00110100000000 xx0Sssnnnnnttttt   0D00 0000   base register
                                   //  st1     {Vt}[],[Xn],Xm       LS_3G  0Q001101100mmmmm xx0Sssnnnnnttttt   0D80 0000   post-indexed by a register
                                   //  st1     {Vt}[],[Xn],#imm     LS_2G  0Q00110110011111 xx0Sssnnnnnttttt   0D9F 0000   post-indexed by an immediate

INST6(st2,         "st2",          ST,     IF_EN6B,   0x0C008000,  0x0C808000,  0x0C9F8000,  0x0D200000,  0x0DA00000,  0x0DBF0000)
                                   // ST2 (multiple structures)
                                   //  st2     {Vt,Vt2},[Xn]        LS_2D  0Q00110000000000 1000ssnnnnnttttt   0C00 8000   base register
                                   //  st2     {Vt,Vt2},[Xn],Xm     LS_3F  0Q001100100mmmmm 1000ssnnnnnttttt   0C80 8000   post-indexed by a register
                                   //  st2     {Vt,Vt2},[Xn],#imm   LS_2E  0Q00110010011111 1000ssnnnnnttttt   0C9F 8000   post-indexed by an immediate
                                   // ST2 (single structure)
                                   //  st2     {Vt,Vt2}[],[Xn]      LS_2F  0Q00110100100000 xx0Sssnnnnnttttt   0D20 0000   base register
                                   //  st2     {Vt,Vt2}[],[Xn],Xm   LS_3G  0Q001101101mmmmm xx0Sssnnnnnttttt   0DA0 0000   post-indexed by a register
                                   //  st2     {Vt,Vt2}[],[Xn],#imm LS_2G  0Q00110110111111 xx0Sssnnnnnttttt   0DBF 0000   post-indexed by an immediate

INST6(st3,         "st3",          ST,     IF_EN6B,   0x0C004000,  0x0C804000,  0x0C9F4000,  0x0D002000,  0x0D802000,  0x0D9F2000)
                                   // ST3 (multiple structures)
                                   //  st3     {Vt-Vt3},[Xn]        LS_2D  0Q00110000000000 0100ssnnnnnttttt   0C00 4000   base register
                                   //  st3     {Vt-Vt3},[Xn],Xm     LS_3F  0Q001100100mmmmm 0100ssnnnnnttttt   0C80 4000   post-indexed by a register
                                   //  st3     {Vt-Vt3},[Xn],#imm   LS_2E  0Q00110010011111 0100ssnnnnnttttt   0C9F 4000   post-indexed by an immediate
                                   // ST3 (single structure)
                                   //  st3     {Vt-Vt3}[],[Xn]      LS_2F  0Q00110100000000 xx1Sssnnnnnttttt   0D00 2000   base register
                                   //  st3     {Vt-Vt3}[],[Xn],Xm   LS_3G  0Q001101100mmmmm xx1Sssnnnnnttttt   0D80 2000   post-indexed by a register
                                   //  st3     {Vt-Vt3}[],[Xn],#imm LS_2G  0Q00110110011111 xx1Sssnnnnnttttt   0D9F 2000   post-indexed by an immediate

INST6(st4,         "st4",          ST,     IF_EN6B,   0x0C000000,  0x0C800000,  0x0C9F0000,  0x0D202000,  0x0DA02000,  0x0DBF2000)
                                   // ST4 (multiple structures)
                                   //  st4     {Vt-Vt4},[Xn]        LS_2D  0Q00110000000000 0000ssnnnnnttttt   0C00 0000   base register
                                   //  st4     {Vt-Vt4},[Xn],Xm     LS_3F  0Q001100100mmmmm 0000ssnnnnnttttt   0C80 0000   post-indexed by a register
                                   //  st4     {Vt-Vt4},[Xn],#imm   LS_2E  0Q00110010011111 0000ssnnnnnttttt   0C9F 0000   post-indexed by an immediate
                                   // ST4 (single structure)
                                   //  st4     {Vt-Vt4}[],[Xn]      LS_2F  0Q00110100100000 xx1Sssnnnnnttttt   0D20 2000   base register
                                   //  st4     {Vt-Vt4}[],[Xn],Xm   LS_3G  0Q001101101mmmmm xx1Sssnnnnnttttt   0DA0 2000   post-indexed by a register
                                   //  st4     {Vt-Vt4}[],[Xn],#imm LS_2G  0Q00110110111111 xx1Sssnnnnnttttt   0DBF 2000   post-indexed by an immediate

//    enum         name            info               LS_2A        LS_2B        LS_2C        LS_3A        LS_1A
INST5(ldr,         "ldr",          LD,     IF_EN5A,   0xB9400000,  0xB9400000,  0xB8400000,  0xB8600800,  0x18000000)
                                   //  ldr     Rt,[Xn]              LS_2A  1X11100101000000 000000nnnnnttttt   B940 0000
                                   //  ldr     Rt,[Xn+pimm12]       LS_2B  1X11100101iiiiii iiiiiinnnnnttttt   B940 0000   imm(0-4095<<{2,3})
                                   //  ldr     Rt,[Xn+simm9]        LS_2C  1X111000010iiiii iiiiPPnnnnnttttt   B840 0000   [Xn imm(-256..+255) pre/post/no inc]
                                   //  ldr     Rt,[Xn,(Rm,ext,shl)] LS_3A  1X111000011mmmmm oooS10nnnnnttttt   B860 0800   [Xn, ext(Rm) LSL {0,2,3}]
                                   //  ldr     Vt/Rt,[PC+simm19<<2] LS_1A  XX011V00iiiiiiii iiiiiiiiiiittttt   1800 0000   [PC +- imm(1MB)]

INST5(ldrsw,       "ldrsw",        LD,     IF_EN5A,   0xB9800000,  0xB9800000,  0xB8800000,  0xB8A00800,  0x98000000)
                                   //  ldrsw   Rt,[Xn]              LS_2A  1011100110000000 000000nnnnnttttt   B980 0000
                                   //  ldrsw   Rt,[Xn+pimm12]       LS_2B  1011100110iiiiii iiiiiinnnnnttttt   B980 0000   imm(0-4095<<2)
                                   //  ldrsw   Rt,[Xn+simm9]        LS_2C  10111000100iiiii iiiiPPnnnnnttttt   B880 0000   [Xn imm(-256..+255) pre/post/no inc]
                                   //  ldrsw   Rt,[Xn,(Rm,ext,shl)] LS_3A  10111000101mmmmm oooS10nnnnnttttt   B8A0 0800   [Xn, ext(Rm) LSL {0,2}]
                                   //  ldrsw   Rt,[PC+simm19<<2]    LS_1A  10011000iiiiiiii iiiiiiiiiiittttt   9800 0000   [PC +- imm(1MB)]

//    enum         name            info               DV_2G        DV_2H        DV_2I        DV_1A        DV_1B
INST5(fmov,        "fmov",         0,      IF_EN5B,   0x1E204000,  0x1E260000,  0x1E270000,  0x1E201000,  0x0F00F400)
                                   //  fmov    Vd,Vn                DV_2G  000111100X100000 010000nnnnnddddd   1E20 4000   Vd,Vn    (scalar)
                                   //  fmov    Rd,Vn                DV_2H  X00111100X100110 000000nnnnnddddd   1E26 0000   Rd,Vn    (scalar, to general)
                                   //  fmov    Vd,Rn                DV_2I  X00111100X100111 000000nnnnnddddd   1E27 0000   Vd,Rn    (scalar, from general)
                                   //  fmov    Vd,immfp             DV_1A  000111100X1iiiii iii10000000ddddd   1E20 1000   Vd,immfp (scalar)
                                   //  fmov    Vd,immfp             DV_1B  0QX0111100000iii 111101iiiiiddddd   0F00 F400   Vd,immfp (immediate vector)

//    enum         name            info               DR_3A        DR_3B        DI_2C        DV_3C        DV_1B
INST5(orr,         "orr",          0,      IF_EN5C,   0x2A000000,  0x2A000000,  0x32000000,  0x0EA01C00,  0x0F001400)
                                   //  orr     Rd,Rn,Rm             DR_3A  X0101010000mmmmm 000000nnnnnddddd   2A00 0000
                                   //  orr     Rd,Rn,(Rm,shk,imm)   DR_3B  X0101010sh0mmmmm iiiiiinnnnnddddd   2A00 0000   Rm {LSL,LSR,ASR,ROR} imm(0-63)
                                   //  orr     Rd,Rn,imm(N,r,s)     DI_2C  X01100100Nrrrrrr ssssssnnnnnddddd   3200 0000   imm(N,r,s)
                                   //  orr     Vd,Vn,Vm             DV_3C  0Q001110101mmmmm 000111nnnnnddddd   0EA0 1C00   Vd,Vn,Vm
                                   //  orr     Vd,imm8              DV_1B  0Q00111100000iii ---101iiiiiddddd   0F00 1400   Vd imm8  (immediate vector)

//    enum         name            info               LS_2A        LS_2B        LS_2C        LS_3A
INST4(ldrb,        "ldrb",         LD,     IF_EN4A,   0x39400000,  0x39400000,  0x38400000,  0x38600800)
                                   //  ldrb    Rt,[Xn]              LS_2A  0011100101000000 000000nnnnnttttt   3940 0000
                                   //  ldrb    Rt,[Xn+pimm12]       LS_2B  0011100101iiiiii iiiiiinnnnnttttt   3940 0000   imm(0-4095)
                                   //  ldrb    Rt,[Xn+simm9]        LS_2C  00111000010iiiii iiiiPPnnnnnttttt   3840 0000   [Xn imm(-256..+255) pre/post/no inc]
                                   //  ldrb    Rt,[Xn,(Rm,ext,shl)] LS_3A  00111000011mmmmm oooS10nnnnnttttt   3860 0800   [Xn, ext(Rm)]

INST4(ldrh,        "ldrh",         LD,     IF_EN4A,   0x79400000,  0x79400000,  0x78400000,  0x78600800)
                                   //  ldrh    Rt,[Xn]              LS_2A  0111100101000000 000000nnnnnttttt   7940 0000
                                   //  ldrh    Rt,[Xn+pimm12]       LS_2B  0111100101iiiiii iiiiiinnnnnttttt   7940 0000   imm(0-4095<<1)
                                   //  ldrh    Rt,[Xn+simm9]        LS_2C  01111000010iiiii iiiiPPnnnnnttttt   7840 0000   [Xn imm(-256..+255) pre/post/no inc]
                                   //  ldrh    Rt,[Xn,(Rm,ext,shl)] LS_3A  01111000011mmmmm oooS10nnnnnttttt   7860 0800   [Xn, ext(Rm) LSL {0,1}]

INST4(ldrsb,       "ldrsb",        LD,     IF_EN4A,   0x39800000,  0x39800000,  0x38800000,  0x38A00800)
                                   //  ldrsb   Rt,[Xn]              LS_2A  001110011X000000 000000nnnnnttttt   3980 0000
                                   //  ldrsb   Rt,[Xn+pimm12]       LS_2B  001110011Xiiiiii iiiiiinnnnnttttt   3980 0000   imm(0-4095)
                                   //  ldrsb   Rt,[Xn+simm9]        LS_2C  001110001X0iiiii iiii01nnnnnttttt   3880 0000   [Xn imm(-256..+255) pre/post/no inc]
                                   //  ldrsb   Rt,[Xn,(Rm,ext,shl)] LS_3A  001110001X1mmmmm oooS10nnnnnttttt   38A0 0800   [Xn, ext(Rm)]

INST4(ldrsh,       "ldrsh",        LD,     IF_EN4A,   0x79800000,  0x79800000,  0x78800000,  0x78A00800)
                                   //  ldrsh   Rt,[Xn]              LS_2A  011110011X000000 000000nnnnnttttt   7980 0000
                                   //  ldrsh   Rt,[Xn+pimm12]       LS_2B  011110011Xiiiiii iiiiiinnnnnttttt   7980 0000   imm(0-4095<<1)
                                   //  ldrsh   Rt,[Xn+simm9]        LS_2C  011110001X0iiiii iiiiPPnnnnnttttt   7880 0000   [Xn imm(-256..+255) pre/post/no inc]
                                   //  ldrsh   Rt,[Xn,(Rm,ext,shl)] LS_3A  011110001X1mmmmm oooS10nnnnnttttt   78A0 0800   [Xn, ext(Rm) LSL {0,1}]

INST4(str,         "str",          ST,     IF_EN4A,   0xB9000000,  0xB9000000,  0xB8000000,  0xB8200800)
                                   //  str     Rt,[Xn]              LS_2A  1X11100100000000 000000nnnnnttttt   B900 0000
                                   //  str     Rt,[Xn+pimm12]       LS_2B  1X11100100iiiiii iiiiiinnnnnttttt   B900 0000   imm(0-4095<<{2,3})
                                   //  str     Rt,[Xn+simm9]        LS_2C  1X111000000iiiii iiiiPPnnnnnttttt   B800 0000   [Xn imm(-256..+255) pre/post/no inc]
                                   //  str     Rt,[Xn,(Rm,ext,shl)] LS_3A  1X111000001mmmmm oooS10nnnnnttttt   B820 0800   [Xn, ext(Rm)]

INST4(strb,        "strb",         ST,     IF_EN4A,   0x39000000,  0x39000000,  0x38000000,  0x38200800)
                                   //  strb    Rt,[Xn]              LS_2A  0011100100000000 000000nnnnnttttt   3900 0000
                                   //  strb    Rt,[Xn+pimm12]       LS_2B  0011100100iiiiii iiiiiinnnnnttttt   3900 0000   imm(0-4095)
                                   //  strb    Rt,[Xn+simm9]        LS_2C  00111000000iiiii iiiiPPnnnnnttttt   3800 0000   [Xn imm(-256..+255) pre/post/no inc]
                                   //  strb    Rt,[Xn,(Rm,ext,shl)] LS_3A  00111000001mmmmm oooS10nnnnnttttt   3820 0800   [Xn, ext(Rm)]

INST4(strh,        "strh",         ST,     IF_EN4A,   0x79000000,  0x79000000,  0x78000000,  0x78200800)
                                   //  strh    Rt,[Xn]              LS_2A  0111100100000000 000000nnnnnttttt   7900 0000
                                   //  strh    Rt,[Xn+pimm12]       LS_2B  0111100100iiiiii iiiiiinnnnnttttt   7900 0000   imm(0-4095<<1)
                                   //  strh    Rt,[Xn+simm9]        LS_2C  01111000000iiiii iiiiPPnnnnnttttt   7800 0000   [Xn imm(-256..+255) pre/post/no inc]
                                   //  strh    Rt,[Xn,(Rm,ext,shl)] LS_3A  01111000001mmmmm oooS10nnnnnttttt   7820 0800   [Xn, ext(Rm)]

//    enum         name            info               DR_3A        DR_3B        DR_3C        DI_2A
INST4(adds,        "adds",         0,      IF_EN4B,   0x2B000000,  0x2B000000,  0x2B200000,  0x31000000)
                                   //  adds    Rd,Rn,Rm             DR_3A  X0101011000mmmmm 000000nnnnnddddd   2B00 0000
                                   //  adds    Rd,Rn,(Rm,shk,imm)   DR_3B  X0101011sh0mmmmm ssssssnnnnnddddd   2B00 0000   Rm {LSL,LSR,ASR} imm(0-63)
                                   //  adds    Rd,Rn,(Rm,ext,shl)   DR_3C  X0101011001mmmmm ooosssnnnnnddddd   2B20 0000   ext(Rm) LSL imm(0-4)
                                   //  adds    Rd,Rn,i12            DI_2A  X0110001shiiiiii iiiiiinnnnnddddd   3100 0000   imm(i12,sh)

INST4(subs,        "subs",         0,      IF_EN4B,   0x6B000000,  0x6B000000,  0x6B200000,  0x71000000)
                                   //  subs    Rd,Rn,Rm             DR_3A  X1101011000mmmmm 000000nnnnnddddd   6B00 0000
                                   //  subs    Rd,Rn,(Rm,shk,imm)   DR_3B  X1101011sh0mmmmm ssssssnnnnnddddd   6B00 0000   Rm {LSL,LSR,ASR} imm(0-63)
                                   //  subs    Rd,Rn,(Rm,ext,shl)   DR_3C  X1101011001mmmmm ooosssnnnnnddddd   6B20 0000   ext(Rm) LSL imm(0-4)
                                   //  subs    Rd,Rn,i12            DI_2A  X1110001shiiiiii iiiiiinnnnnddddd   7100 0000   imm(i12,sh)

//    enum         name            info               DR_2A        DR_2B        DR_2C        DI_1A
INST4(cmp,         "cmp",          CMP,    IF_EN4C,   0x6B00001F,  0x6B00001F,  0x6B20001F,  0x7100001F)
                                   //  cmp     Rn,Rm                DR_2A  X1101011000mmmmm 000000nnnnn11111   6B00 001F
                                   //  cmp     Rn,(Rm,shk,imm)      DR_2B  X1101011sh0mmmmm ssssssnnnnn11111   6B00 001F   Rm {LSL,LSR,ASR} imm(0-63)
                                   //  cmp     Rn,(Rm,ext,shl)      DR_2C  X1101011001mmmmm ooosssnnnnn11111   6B20 001F   ext(Rm) LSL imm(0-4)
                                   //  cmp     Rn,i12               DI_1A  X111000100iiiiii iiiiiinnnnn11111   7100 001F   imm(i12,sh)

INST4(cmn,         "cmn",          CMP,    IF_EN4C,   0x2B00001F,  0x2B00001F,  0x2B20001F,  0x3100001F)
                                   //  cmn     Rn,Rm                DR_2A  X0101011000mmmmm 000000nnnnn11111   2B00 001F
                                   //  cmn     Rn,(Rm,shk,imm)      DR_2B  X0101011sh0mmmmm ssssssnnnnn11111   2B00 001F   Rm {LSL,LSR,ASR} imm(0-63)
                                   //  cmn     Rn,(Rm,ext,shl)      DR_2C  X0101011001mmmmm ooosssnnnnn11111   2B20 001F   ext(Rm) LSL imm(0-4)
                                   //  cmn     Rn,i12               DI_1A  X0110001shiiiiii iiiiiinnnnn11111   3100 001F   imm(0-4095)

//    enum         name            info               DV_3B        DV_3D        DV_3BI       DV_3DI
INST4(fmul,        "fmul",         0,      IF_EN4D,   0x2E20DC00,  0x1E200800,  0x0F809000,  0x5F809000)
                                   //  fmul    Vd,Vn,Vm             DV_3B  0Q1011100X1mmmmm 110111nnnnnddddd   2E20 DC00   Vd,Vn,Vm   (vector)
                                   //  fmul    Vd,Vn,Vm             DV_3D  000111100X1mmmmm 000010nnnnnddddd   1E20 0800   Vd,Vn,Vm   (scalar)
                                   //  fmul    Vd,Vn,Vm[]           DV_3BI 0Q0011111XLmmmmm 1001H0nnnnnddddd   0F80 9000   Vd,Vn,Vm[] (vector by element)
                                   //  fmul    Vd,Vn,Vm[]           DV_3DI 010111111XLmmmmm 1001H0nnnnnddddd   5F80 9000   Vd,Vn,Vm[] (scalar by element)

INST4(fmulx,       "fmulx",        0,      IF_EN4D,   0x0E20DC00,  0x5E20DC00,  0x2F809000,  0x7F809000)
                                   //  fmulx   Vd,Vn,Vm             DV_3B  0Q0011100X1mmmmm 110111nnnnnddddd   0E20 DC00   Vd,Vn,Vm   (vector)
                                   //  fmulx   Vd,Vn,Vm             DV_3D  010111100X1mmmmm 110111nnnnnddddd   5E20 DC00   Vd,Vn,Vm   (scalar)
                                   //  fmulx   Vd,Vn,Vm[]           DV_3BI 0Q1011111XLmmmmm 1001H0nnnnnddddd   2F80 9000   Vd,Vn,Vm[] (vector by element)
                                   //  fmulx   Vd,Vn,Vm[]           DV_3DI 011111111XLmmmmm 1001H0nnnnnddddd   7F80 9000   Vd,Vn,Vm[] (scalar by element)

//    enum         name            info               DR_3A        DR_3B        DI_2C        DV_3C
INST4(and,         "and",          0,      IF_EN4E,   0x0A000000,  0x0A000000,  0x12000000,  0x0E201C00)
                                   //  and     Rd,Rn,Rm             DR_3A  X0001010000mmmmm 000000nnnnnddddd   0A00 0000
                                   //  and     Rd,Rn,(Rm,shk,imm)   DR_3B  X0001010sh0mmmmm iiiiiinnnnnddddd   0A00 0000   Rm {LSL,LSR,ASR,ROR} imm(0-63)
                                   //  and     Rd,Rn,imm(N,r,s)     DI_2C  X00100100Nrrrrrr ssssssnnnnnddddd   1200 0000   imm(N,r,s)
                                   //  and     Vd,Vn,Vm             DV_3C  0Q001110001mmmmm 000111nnnnnddddd   0E20 1C00   Vd,Vn,Vm

INST4(eor,         "eor",          0,      IF_EN4E,   0x4A000000,  0x4A000000,  0x52000000,  0x2E201C00)
                                   //  eor     Rd,Rn,Rm             DR_3A  X1001010000mmmmm 000000nnnnnddddd   4A00 0000
                                   //  eor     Rd,Rn,(Rm,shk,imm)   DR_3B  X1001010sh0mmmmm iiiiiinnnnnddddd   4A00 0000   Rm {LSL,LSR,ASR,ROR} imm(0-63)
                                   //  eor     Rd,Rn,imm(N,r,s)     DI_2C  X10100100Nrrrrrr ssssssnnnnnddddd   5200 0000   imm(N,r,s)
                                   //  eor     Vd,Vn,Vm             DV_3C  0Q101110001mmmmm 000111nnnnnddddd   2E20 1C00   Vd,Vn,Vm

//    enum         name            info               DR_3A        DR_3B        DV_3C        DV_1B
INST4(bic,         "bic",          0,      IF_EN4F,   0x0A200000,  0x0A200000,  0x0E601C00,  0x2F001400)
                                   //  bic     Rd,Rn,Rm             DR_3A  X0001010001mmmmm 000000nnnnnddddd   0A20 0000
                                   //  bic     Rd,Rn,(Rm,shk,imm)   DR_3B  X0001010sh1mmmmm iiiiiinnnnnddddd   0A20 0000   Rm {LSL,LSR,ASR,ROR} imm(0-63)
                                   //  bic     Vd,Vn,Vm             DV_3C  0Q001110011mmmmm 000111nnnnnddddd   0E60 1C00   Vd,Vn,Vm
                                   //  bic     Vd,imm8              DV_1B  0Q10111100000iii ---101iiiiiddddd   2F00 1400   Vd imm8  (immediate vector)

//    enum         name            info               DR_2E        DR_2F        DV_2M        DV_2L
INST4(neg,         "neg",          0,      IF_EN4G,   0x4B0003E0,  0x4B0003E0,  0x2E20B800,  0x7E20B800)
                                   //  neg     Rd,Rm                DR_2E  X1001011000mmmmm 00000011111ddddd   4B00 03E0
                                   //  neg     Rd,(Rm,shk,imm)      DR_2F  X1001011sh0mmmmm ssssss11111ddddd   4B00 03E0   Rm {LSL,LSR,ASR} imm(0-63)
                                   //  neg     Vd,Vn                DV_2M  0Q101110XX100000 101110nnnnnddddd   2E20 B800   Vd,Vn    (vector)
                                   //  neg     Vd,Vn                DV_2L  01111110XX100000 101110nnnnnddddd   7E20 B800   Vd,Vn    (scalar)

//    enum         name            info               DV_3E        DV_3A      DV_2L        DV_2M
INST4(cmeq,        "cmeq",         0,      IF_EN4H,   0x7EE08C00,  0x2E208C00,  0x5E209800,  0x0E209800)
                                   //  cmeq    Vd,Vn,Vm             DV_3E  01111110001mmmmm 100011nnnnnddddd   7E20 8C00   Vd,Vn,Vm   (scalar)
                                   //  cmeq    Vd,Vn,Vm             DV_3A  0Q101110XX1mmmmm 100011nnnnnddddd   2E20 8C00   Vd,Vn,Vm   (vector)
                                   //  cmeq    Vd,Vn,#0             DV_2L  01011110XX100000 100110nnnnnddddd   5E20 9800   Vd,Vn,#0   (scalar - with zero)
                                   //  cmeq    Vd,Vn,#0             DV_2M  0Q001110XX100000 100110nnnnnddddd   0E20 9800   Vd,Vn,#0   (vector - with zero)

INST4(cmge,        "cmge",         0,      IF_EN4H,   0x5EE03C00,  0x0E203C00,  0x7E208800,  0x2E208800)
                                   //  cmge    Vd,Vn,Vm             DV_3E  01011110001mmmmm 001111nnnnnddddd   5E20 3C00   Vd,Vn,Vm   (scalar)
                                   //  cmge    Vd,Vn,Vm             DV_3A  0Q001110XX1mmmmm 001111nnnnnddddd   0E20 3C00   Vd,Vn,Vm   (vector)
                                   //  cmge    Vd,Vn,#0             DV_2L  01111110XX100000 100010nnnnnddddd   7E20 8800   Vd,Vn,#0   (scalar - with zero)
                                   //  cmge    Vd,Vn,#0             DV_2M  0Q101110XX100000 100010nnnnnddddd   2E20 8800   Vd,Vn,#0   (vector - with zero)

INST4(cmgt,        "cmgt",         0,      IF_EN4H,   0x5EE03400,  0x0E203400,  0x5E208800,  0x0E208800)
                                   //  cmgt    Vd,Vn,Vm             DV_3E  01011110001mmmmm 001101nnnnnddddd   5E20 3400   Vd,Vn,Vm   (scalar)
                                   //  cmgt    Vd,Vn,Vm             DV_3A  0Q001110XX1mmmmm 001101nnnnnddddd   0E20 3400   Vd,Vn,Vm   (vector)
                                   //  cmgt    Vd,Vn,#0             DV_2L  01011110XX100000 100010nnnnnddddd   5E20 8800   Vd,Vn,#0   (scalar - with zero)
                                   //  cmgt    Vd,Vn,#0             DV_2M  0Q001110XX100000 101110nnnnnddddd   0E20 8800   Vd,Vn,#0   (vector - with zero)

//    enum         name            info               DV_3D        DV_3B        DV_2G        DV_2A
INST4(fcmeq,       "fcmeq",        0,      IF_EN4I,   0x5E20E400,  0x0E20E400,  0x5EA0D800,  0x0EA0D800)
                                   //  fcmeq   Vd,Vn,Vm             DV_3D  010111100X1mmmmm 111001nnnnnddddd   5E20 E400   Vd,Vn,Vm   (scalar)
                                   //  fcmeq   Vd,Vn,Vm             DV_3B  0Q0011100X1mmmmm 111001nnnnnddddd   0E20 E400   Vd,Vn,Vm   (vector)
                                   //  fcmeq   Vd,Vn,#0             DV_2G  010111101X100000 110110nnnnnddddd   5EA0 D800   Vd,Vn,#0   (scalar - with zero)
                                   //  fcmeq   Vd,Vn,#0             DV_2A  0Q0011101X100000 110110nnnnnddddd   0EA0 D800   Vd,Vn,#0   (vector - with zero)

INST4(fcmge,       "fcmge",        0,      IF_EN4I,   0x7E20E400,  0x2E20E400,  0x7EA0C800,  0x2EA0C800)
                                   //  fcmge   Vd,Vn,Vm             DV_3D  011111100X1mmmmm 111001nnnnnddddd   7E20 E400   Vd,Vn,Vm   (scalar)
                                   //  fcmge   Vd,Vn,Vm             DV_3B  0Q1011100X1mmmmm 111001nnnnnddddd   2E20 E400   Vd,Vn,Vm   (vector)
                                   //  fcmge   Vd,Vn,#0             DV_2G  011111101X100000 110010nnnnnddddd   7EA0 C800   Vd,Vn,#0   (scalar - with zero)
                                   //  fcmge   Vd,Vn,#0             DV_2A  0Q1011101X100000 110010nnnnnddddd   2EA0 C800   Vd,Vn,#0   (vector - with zero)

INST4(fcmgt,       "fcmgt",        0,      IF_EN4I,   0x7EA0E400,  0x2EA0E400,  0x5EA0C800,  0x0EA0C800)
                                   //  fcmgt   Vd,Vn,Vm             DV_3D  011111101X1mmmmm 111001nnnnnddddd   7EA0 E400   Vd,Vn,Vm   (scalar)
                                   //  fcmgt   Vd,Vn,Vm             DV_3B  0Q1011101X1mmmmm 111001nnnnnddddd   2EA0 E400   Vd,Vn,Vm   (vector)
                                   //  fcmgt   Vd,Vn,#0             DV_2G  010111101X100000 110010nnnnnddddd   5EA0 C800   Vd,Vn,#0   (scalar - with zero)
                                   //  fcmgt   Vd,Vn,#0             DV_2A  0Q0011101X100000 110010nnnnnddddd   0EA0 C800   Vd,Vn,#0   (vector - with zero)

//    enum         name            info               DV_2N        DV_2O        DV_3E        DV_3A
INST4(sqshl,       "sqshl",        0,      IF_EN4J,   0x5F007400,  0x0F007400,  0x5E204C00,  0x0E204C00)
                                   //  sqshl   Vd,Vn,imm            DV_2N  010111110iiiiiii 011101nnnnnddddd   5F00 7400   Vd Vn imm  (left shift - scalar)
                                   //  sqshl   Vd,Vn,imm            DV_2O  0Q0011110iiiiiii 011101nnnnnddddd   0F00 7400   Vd Vn imm  (left shift - vector)
                                   //  sqshl   Vd,Vn,Vm             DV_3E  01011110XX1mmmmm 010011nnnnnddddd   5E20 4C00   Vd Vn Vm   (scalar)
                                   //  sqshl   Vd,Vn,Vm             DV_3A  0Q001110XX1mmmmm 010011nnnnnddddd   0E20 4C00   Vd Vn Vm   (vector)

INST4(uqshl,       "uqshl",        0,      IF_EN4J,   0x7F007400,  0x2F007400,  0x7E204C00,  0x2E204C00)
                                   //  uqshl   Vd,Vn,imm            DV_2N  011111110iiiiiii 011101nnnnnddddd   7F00 7400   Vd Vn imm  (left shift - scalar)
                                   //  uqshl   Vd,Vn,imm            DV_2O  0Q1011110iiiiiii 011101nnnnnddddd   2F00 7400   Vd Vn imm  (left shift - vector)
                                   //  uqshl   Vd,Vn,Vm             DV_3E  01111110XX1mmmmm 010011nnnnnddddd   7E20 4C00   Vd Vn Vm   (scalar)
                                   //  uqshl   Vd,Vn,Vm             DV_3A  0Q101110XX1mmmmm 010011nnnnnddddd   2E20 4C00   Vd Vn Vm   (vector)

//    enum         name            info               DV_3E        DV_3A        DV_3EI       DV_3AI
INST4(sqdmlal,     "sqdmlal",      LNG,    IF_EN4K,   0x5E209000,  0x0E209000,  0x5F003000,  0x0F003000)
                                   //  sqdmlal Vd,Vn,Vm             DV_3E  01011110XX1mmmmm 100100nnnnnddddd   5E20 9000   Vd,Vn,Vm   (scalar)
                                   //  sqdmlal Vd,Vn,Vm             DV_3A  00001110XX1mmmmm 100100nnnnnddddd   0E20 9000   Vd,Vn,Vm   (vector)
                                   //  sqdmlal Vd,Vn,Vm[]           DV_3EI 01011111XXLMmmmm 0011H0nnnnnddddd   5F00 3000   Vd,Vn,Vm[] (scalar by element)
                                   //  sqdmlal Vd,Vn,Vm[]           DV_3AI 00001111XXLMmmmm 0011H0nnnnnddddd   0F00 3000   Vd,Vn,Vm[] (vector by element)

INST4(sqdmlsl,     "sqdmlsl",      LNG,    IF_EN4K,   0x5E20B000,  0x0E20B000,  0x5F007000,  0x0F007000)
                                   //  sqdmlsl Vd,Vn,Vm             DV_3E  01011110XX1mmmmm 101100nnnnnddddd   5E20 B000   Vd,Vn,Vm   (scalar)
                                   //  sqdmlsl Vd,Vn,Vm             DV_3A  00001110XX1mmmmm 101100nnnnnddddd   0E20 B000   Vd,Vn,Vm   (vector)
                                   //  sqdmlsl Vd,Vn,Vm[]           DV_3EI 01011111XXLMmmmm 0111H0nnnnnddddd   5F00 7000   Vd,Vn,Vm[] (scalar by element)
                                   //  sqdmlsl Vd,Vn,Vm[]           DV_3AI 00001111XXLMmmmm 0111H0nnnnnddddd   0F00 7000   Vd,Vn,Vm[] (vector by element)

INST4(sqdmulh,     "sqdmulh",      0,      IF_EN4K,   0x5E20B400,  0x0E20B400,  0x5F00C000,  0x0F00C000)
                                   //  sqdmulh Vd,Vn,Vm             DV_3E  01011110XX1mmmmm 101101nnnnnddddd   5E20 B400   Vd,Vn,Vm   (scalar)
                                   //  sqdmulh Vd,Vn,Vm             DV_3A  0Q001110XX1mmmmm 101101nnnnnddddd   0E20 B400   Vd,Vn,Vm   (vector)
                                   //  sqdmulh Vd,Vn,Vm[]           DV_3EI 01011111XXLMmmmm 1100H0nnnnnddddd   5F00 C000   Vd,Vn,Vm[] (scalar by element)
                                   //  sqdmulh Vd,Vn,Vm[]           DV_3AI 0Q001111XXLMmmmm 1100H0nnnnnddddd   0F00 C000   Vd,Vn,Vm[] (vector by element)

INST4(sqdmull,     "sqdmull",      LNG,    IF_EN4K,   0x5E20D000,  0x0E20D000,  0x5F00B000,  0x0F00B000)
                                   //  sqdmull Vd,Vn,Vm             DV_3E  01011110XX1mmmmm 110100nnnnnddddd   5E20 D000   Vd,Vn,Vm   (scalar)
                                   //  sqdmull Vd,Vn,Vm             DV_3A  00001110XX1mmmmm 110100nnnnnddddd   0E20 D000   Vd,Vn,Vm   (vector)
                                   //  sqdmull Vd,Vn,Vm[]           DV_3EI 01011111XXLMmmmm 1011H0nnnnnddddd   5F00 B000   Vd,Vn,Vm[] (scalar by element)
                                   //  sqdmull Vd,Vn,Vm[]           DV_3AI 00001111XXLMmmmm 1011H0nnnnnddddd   0F00 B000   Vd,Vn,Vm[] (vector by element)

INST4(sqrdmlah,    "sqrdmlah",     0,      IF_EN4K,   0x7E008400,  0x2E008400,  0x7F00D000,  0x2F00D000)
                                   //  sqrdmlah Vd,Vn,Vm            DV_3E  01111110XX0mmmmm 100001nnnnnddddd   7E00 8400   Vd,Vn,Vm   (scalar)
                                   //  sqrdmlah Vd,Vn,Vm            DV_3A  0Q101110XX0mmmmm 100001nnnnnddddd   2E00 8400   Vd,Vn,Vm   (vector)
                                   //  sqrdmlah Vd,Vn,Vm[]          DV_3EI 01111111XXLMmmmm 1101H0nnnnnddddd   7F00 D000   Vd,Vn,Vm[] (scalar by element)
                                   //  sqrdmlah Vd,Vn,Vm[]          DV_3AI 0Q101111XXLMmmmm 1101H0nnnnnddddd   2F00 D000   Vd,Vn,Vm[] (vector by element)

INST4(sqrdmlsh,    "sqrdmlsh",     0,      IF_EN4K,   0x7E008C00,  0x2E008C00,  0x7F00F000,  0x2F00F000)
                                   //  sqrdmlsh Vd,Vn,Vm            DV_3E  01111110XX0mmmmm 100011nnnnnddddd   7E00 8C00   Vd,Vn,Vm   (scalar)
                                   //  sqrdmlsh Vd,Vn,Vm            DV_3A  0Q101110XX0mmmmm 100011nnnnnddddd   2E00 8C00   Vd,Vn,Vm   (vector)
                                   //  sqrdmlsh Vd,Vn,Vm[]          DV_3EI 01111111XXLMmmmm 1111H0nnnnnddddd   7F00 F000   Vd,Vn,Vm[] (scalar by element)
                                   //  sqrdmlsh Vd,Vn,Vm[]          DV_3AI 0Q101111XXLMmmmm 1111H0nnnnnddddd   2F00 F000   Vd,Vn,Vm[] (vector by element)

INST4(sqrdmulh,    "sqrdmulh",     0,      IF_EN4K,   0x7E20B400,  0x2E20B400,  0x5F00D000,  0x0F00D000)
                                   //  sqrdmulh Vd,Vn,Vm            DV_3E  01111110XX1mmmmm 101101nnnnnddddd   7E20 B400   Vd,Vn,Vm   (scalar)
                                   //  sqrdmulh Vd,Vn,Vm            DV_3A  0Q101110XX1mmmmm 101101nnnnnddddd   2E20 B400   Vd,Vn,Vm   (vector)
                                   //  sqrdmulh Vd,Vn,Vm[]          DV_3EI 01011111XXLMmmmm 1101H0nnnnnddddd   5F00 D000   Vd,Vn,Vm[] (scalar by element)
                                   //  sqrdmulh Vd,Vn,Vm[]          DV_3AI 0Q001111XXLMmmmm 1101H0nnnnnddddd   0F00 D000   Vd,Vn,Vm[] (vector by element)

//    enum         name            info               DR_3A        DR_3B        DI_2C
INST3(ands,        "ands",         0,      IF_EN3A,   0x6A000000,  0x6A000000,  0x72000000)
                                   //  ands    Rd,Rn,Rm             DR_3A  X1101010000mmmmm 000000nnnnnddddd   6A00 0000
                                   //  ands    Rd,Rn,(Rm,shk,imm)   DR_3B  X1101010sh0mmmmm iiiiiinnnnnddddd   6A00 0000   Rm {LSL,LSR,ASR,ROR} imm(0-63)
                                   //  ands    Rd,Rn,imm(N,r,s)     DI_2C  X11100100Nrrrrrr ssssssnnnnnddddd   7200 0000   imm(N,r,s)

//    enum         name            info               DR_2A        DR_2B        DI_1C
INST3(tst,         "tst",          0,      IF_EN3B,   0x6A00001F,  0x6A00001F,  0x7200001F)
                                   //  tst     Rn,Rm                DR_2A  X1101010000mmmmm 000000nnnnn11111   6A00 001F
                                   //  tst     Rn,(Rm,shk,imm)      DR_2B  X1101010sh0mmmmm iiiiiinnnnn11111   6A00 001F   Rm {LSL,LSR,ASR,ROR} imm(0-63)
                                   //  tst     Rn,imm(N,r,s)        DI_1C  X11100100Nrrrrrr ssssssnnnnn11111   7200 001F   imm(N,r,s)

//    enum         name            info               DR_3A        DR_3B        DV_3C
INST3(orn,         "orn",          0,      IF_EN3C,   0x2A200000,  0x2A200000,  0x0EE01C00)
                                   //  orn     Rd,Rn,Rm             DR_3A  X0101010001mmmmm 000000nnnnnddddd   2A20 0000
                                   //  orn     Rd,Rn,(Rm,shk,imm)   DR_3B  X0101010sh1mmmmm iiiiiinnnnnddddd   2A20 0000   Rm {LSL,LSR,ASR,ROR} imm(0-63)
                                   //  orn     Vd,Vn,Vm             DV_3C  0Q001110111mmmmm 000111nnnnnddddd   0EE0 1C00   Vd,Vn,Vm

//    enum         name            info               DV_2C        DV_2D       DV_2E
INST3(dup,         "dup",          0,      IF_EN3D,   0x0E000C00,  0x0E000400,  0x5E000400)
                                   //  dup     Vd,Rn                DV_2C  0Q001110000iiiii 000011nnnnnddddd   0E00 0C00   Vd,Rn   (vector from general)
                                   //  dup     Vd,Vn[]              DV_2D  0Q001110000iiiii 000001nnnnnddddd   0E00 0400   Vd,Vn[] (vector by element)
                                   //  dup     Vd,Vn[]              DV_2E  01011110000iiiii 000001nnnnnddddd   5E00 0400   Vd,Vn[] (scalar by element)

//    enum         name            info               DV_3B        DV_3BI       DV_3DI
INST3(fmla,        "fmla",         0,      IF_EN3E,   0x0E20CC00,  0x0F801000,  0x5F801000)
                                   //  fmla    Vd,Vn,Vm             DV_3B  0Q0011100X1mmmmm 110011nnnnnddddd   0E20 CC00   Vd,Vn,Vm   (vector)
                                   //  fmla    Vd,Vn,Vm[]           DV_3BI 0Q0011111XLmmmmm 0001H0nnnnnddddd   0F80 1000   Vd,Vn,Vm[] (vector by element)
                                   //  fmla    Vd,Vn,Vm[]           DV_3DI 010111111XLmmmmm 0001H0nnnnnddddd   5F80 1000   Vd,Vn,Vm[] (scalar by element)

INST3(fmls,        "fmls",         0,      IF_EN3E,   0x0EA0CC00,  0x0F805000,  0x5F805000)
                                   //  fmls    Vd,Vn,Vm             DV_3B  0Q0011101X1mmmmm 110011nnnnnddddd   0EA0 CC00   Vd,Vn,Vm   (vector)
                                   //  fmls    Vd,Vn,Vm[]           DV_3BI 0Q0011111XLmmmmm 0101H0nnnnnddddd   0F80 5000   Vd,Vn,Vm[] (vector by element)
                                   //  fmls    Vd,Vn,Vm[]           DV_3DI 010111111XLmmmmm 0101H0nnnnnddddd   5F80 5000   Vd,Vn,Vm[] (scalar by element)

//    enum         name            info               DV_2A        DV_2G        DV_2H
INST3(fcvtas,      "fcvtas",       0,      IF_EN3F,   0x0E21C800,  0x5E21C800,  0x1E240000)
                                   //  fcvtas  Vd,Vn                DV_2A  0Q0011100X100001 110010nnnnnddddd   0E21 C800   Vd,Vn    (vector)
                                   //  fcvtas  Vd,Vn                DV_2G  010111100X100001 110010nnnnnddddd   5E21 C800   Vd,Vn    (scalar)
                                   //  fcvtas  Rd,Vn                DV_2H  X00111100X100100 000000nnnnnddddd   1E24 0000   Rd,Vn    (scalar, to general)

INST3(fcvtau,      "fcvtau",       0,      IF_EN3F,   0x2E21C800,  0x7E21C800,  0x1E250000)
                                   //  fcvtau  Vd,Vn                DV_2A  0Q1011100X100001 111010nnnnnddddd   2E21 C800   Vd,Vn    (vector)
                                   //  fcvtau  Vd,Vn                DV_2G  011111100X100001 111010nnnnnddddd   7E21 C800   Vd,Vn    (scalar)
                                   //  fcvtau  Rd,Vn                DV_2H  X00111100X100101 000000nnnnnddddd   1E25 0000   Rd,Vn    (scalar, to general)

INST3(fcvtms,      "fcvtms",       0,      IF_EN3F,   0x0E21B800,  0x5E21B800,  0x1E300000)
                                   //  fcvtms  Vd,Vn                DV_2A  0Q0011100X100001 101110nnnnnddddd   0E21 B800   Vd,Vn    (vector)
                                   //  fcvtms  Vd,Vn                DV_2G  010111100X100001 101110nnnnnddddd   5E21 B800   Vd,Vn    (scalar)
                                   //  fcvtms  Rd,Vn                DV_2H  X00111100X110000 000000nnnnnddddd   1E30 0000   Rd,Vn    (scalar, to general)

INST3(fcvtmu,      "fcvtmu",       0,      IF_EN3F,   0x2E21B800,  0x7E21B800,  0x1E310000)
                                   //  fcvtmu  Vd,Vn                DV_2A  0Q1011100X100001 101110nnnnnddddd   2E21 B800   Vd,Vn    (vector)
                                   //  fcvtmu  Vd,Vn                DV_2G  011111100X100001 101110nnnnnddddd   7E21 B800   Vd,Vn    (scalar)
                                   //  fcvtmu  Rd,Vn                DV_2H  X00111100X110001 000000nnnnnddddd   1E31 0000   Rd,Vn    (scalar, to general)

INST3(fcvtns,      "fcvtns",       0,      IF_EN3F,   0x0E21A800,  0x5E21A800,  0x1E200000)
                                   //  fcvtns  Vd,Vn                DV_2A  0Q0011100X100001 101010nnnnnddddd   0E21 A800   Vd,Vn    (vector)
                                   //  fcvtns  Vd,Vn                DV_2G  010111100X100001 101010nnnnnddddd   5E21 A800   Vd,Vn    (scalar)
                                   //  fcvtns  Rd,Vn                DV_2H  X00111100X100000 000000nnnnnddddd   1E20 0000   Rd,Vn    (scalar, to general)

INST3(fcvtnu,      "fcvtnu",       0,      IF_EN3F,   0x2E21A800,  0x7E21A800,  0x1E210000)
                                   //  fcvtnu  Vd,Vn                DV_2A  0Q1011100X100001 101010nnnnnddddd   2E21 A800   Vd,Vn    (vector)
                                   //  fcvtnu  Vd,Vn                DV_2G  011111100X100001 101010nnnnnddddd   7E21 A800   Vd,Vn    (scalar)
                                   //  fcvtnu  Rd,Vn                DV_2H  X00111100X100001 000000nnnnnddddd   1E21 0000   Rd,Vn    (scalar, to general)

INST3(fcvtps,      "fcvtps",       0,      IF_EN3F,   0x0EA1A800,  0x5EA1A800,  0x1E280000)
                                   //  fcvtps  Vd,Vn                DV_2A  0Q0011101X100001 101010nnnnnddddd   0EA1 A800   Vd,Vn    (vector)
                                   //  fcvtps  Vd,Vn                DV_2G  010111101X100001 101010nnnnnddddd   5EA1 A800   Vd,Vn    (scalar)
                                   //  fcvtps  Rd,Vn                DV_2H  X00111100X101000 000000nnnnnddddd   1E28 0000   Rd,Vn    (scalar, to general)

INST3(fcvtpu,      "fcvtpu",       0,      IF_EN3F,   0x2EA1A800,  0x7EA1A800,  0x1E290000)
                                   //  fcvtpu  Vd,Vn                DV_2A  0Q1011101X100001 101010nnnnnddddd   2EA1 A800   Vd,Vn    (vector)
                                   //  fcvtpu  Vd,Vn                DV_2G  011111101X100001 101010nnnnnddddd   7EA1 A800   Vd,Vn    (scalar)
                                   //  fcvtpu  Rd,Vn                DV_2H  X00111100X101001 000000nnnnnddddd   1E29 0000   Rd,Vn    (scalar, to general)

INST3(fcvtzs,      "fcvtzs",       0,      IF_EN3F,   0x0EA1B800,  0x5EA1B800,  0x1E380000)
                                   //  fcvtzs  Vd,Vn                DV_2A  0Q0011101X100001 101110nnnnnddddd   0EA1 B800   Vd,Vn    (vector)
                                   //  fcvtzs  Vd,Vn                DV_2G  010111101X100001 101110nnnnnddddd   5EA1 B800   Vd,Vn    (scalar)
                                   //  fcvtzs  Rd,Vn                DV_2H  X00111100X111000 000000nnnnnddddd   1E38 0000   Rd,Vn    (scalar, to general)

INST3(fcvtzu,      "fcvtzu",       0,      IF_EN3F,   0x2EA1B800,  0x7EA1B800,  0x1E390000)
                                   //  fcvtzu  Vd,Vn                DV_2A  0Q1011101X100001 101110nnnnnddddd   2EA1 B800   Vd,Vn    (vector)
                                   //  fcvtzu  Vd,Vn                DV_2G  011111101X100001 101110nnnnnddddd   7EA1 B800   Vd,Vn    (scalar)
                                   //  fcvtzu  Rd,Vn                DV_2H  X00111100X111001 000000nnnnnddddd   1E39 0000   Rd,Vn    (scalar, to general)

//    enum         name            info               DV_2A        DV_2G        DV_2I
INST3(scvtf,       "scvtf",        0,      IF_EN3G,   0x0E21D800,  0x5E21D800,  0x1E220000)
                                   //  scvtf   Vd,Vn                DV_2A  0Q0011100X100001 110110nnnnnddddd   0E21 D800   Vd,Vn    (vector)
                                   //  scvtf   Vd,Vn                DV_2G  010111100X100001 110110nnnnnddddd   5E21 D800   Vd,Vn    (scalar)
                                   //  scvtf   Rd,Vn                DV_2I  X00111100X100010 000000nnnnnddddd   1E22 0000   Vd,Rn    (scalar, from general)

INST3(ucvtf,       "ucvtf",        0,      IF_EN3G,   0x2E21D800,  0x7E21D800,  0x1E230000)
                                   //  ucvtf   Vd,Vn                DV_2A  0Q1011100X100001 110110nnnnnddddd   2E21 D800   Vd,Vn    (vector)
                                   //  ucvtf   Vd,Vn                DV_2G  011111100X100001 110110nnnnnddddd   7E21 D800   Vd,Vn    (scalar)
                                   //  ucvtf   Rd,Vn                DV_2I  X00111100X100011 000000nnnnnddddd   1E23 0000   Vd,Rn    (scalar, from general)

INST3(mul,         "mul",          0,      IF_EN3H,   0x1B007C00,  0x0E209C00,  0x0F008000)
                                   //  mul     Rd,Rn,Rm             DR_3A  X0011011000mmmmm 011111nnnnnddddd   1B00 7C00
                                   //  mul     Vd,Vn,Vm             DV_3A  0Q001110XX1mmmmm 100111nnnnnddddd   0E20 9C00   Vd,Vn,Vm   (vector)
                                   //  mul     Vd,Vn,Vm[]           DV_3AI 0Q001111XXLMmmmm 1000H0nnnnnddddd   0F00 8000   Vd,Vn,Vm[] (vector by element)

INST3(smull,       "smull",        LNG,    IF_EN3H,   0x9B207C00,  0x0E20C000,  0x0F00A000)
                                   //  smull   Rd,Rn,Rm             DR_3A  10011011001mmmmm 011111nnnnnddddd   9B20 7C00
                                   //  smull   Vd,Vn,Vm             DV_3A  0000111000100000 1100000000000000   0E20 C000   Vd,Vn,Vm   (vector)
                                   //  smull   Vd,Vn,Vm[]           DV_3AI 00001111XXLMmmmm 1010H0nnnnnddddd   0F00 A000   Vd,Vn,Vm[] (vector by element)

INST3(umull,       "umull",        LNG,    IF_EN3H,   0x9BA07C00,  0x2E20C000,  0x2F00A000)
                                   //  umull   Rd,Rn,Rm             DR_3A  10011011101mmmmm 011111nnnnnddddd   9BA0 7C00
                                   //  umull   Vd,Vn,Vm             DV_3A  00101110XX1mmmmm 110000nnnnnddddd   2E20 C000   Vd,Vn,Vm   (vector)
                                   //  umull   Vd,Vn,Vm[]           DV_3AI 00101111XXLMmmmm 1010H0nnnnnddddd   2F00 A000   Vd,Vn,Vm[] (vector by element)

//    enum         name            info               DR_2E        DR_2F        DV_2M
INST3(mvn,         "mvn",          0,      IF_EN3I,   0x2A2003E0,  0x2A2003E0,  0x2E205800)
                                   //  mvn     Rd,Rm                DR_2E  X0101010001mmmmm 00000011111ddddd   2A20 03E0
                                   //  mvn     Rd,(Rm,shk,imm)      DR_2F  X0101010sh1mmmmm iiiiii11111ddddd   2A20 03E0   Rm {LSL,LSR,ASR} imm(0-63)
                                   //  mvn     Vd,Vn                DV_2M  0Q10111000100000 010110nnnnnddddd   2E20 5800   Vd,Vn    (vector)

//    enum         name            info               LS_2D        LS_3F        LS_2E
INST3(ld1_2regs,   "ld1",          LD,     IF_EN3J,   0x0C40A000,  0x0CC0A000,  0x0CDFA000)
                                   // LD1 (multiple structures, two registers variant)
                                   //  ld1     {Vt,Vt2},[Xn]        LS_2D  0Q00110001000000 1010ssnnnnnttttt   0C40 A000   base register
                                   //  ld1     {Vt,Vt2},[Xn],Xm     LS_3F  0Q001100110mmmmm 1010ssnnnnnttttt   0CC0 A000   post-indexed by a register
                                   //  ld1     {Vt,Vt2},[Xn],#imm   LS_2E  0Q00110011011111 1010ssnnnnnttttt   0CDF A000   post-indexed by an immediate

INST3(ld1_3regs,   "ld1",          LD,     IF_EN3J,   0x0C406000,  0x0CC06000,  0x0CDF6000)
                                   // LD1 (multiple structures, three registers variant)
                                   //  ld1     {Vt-Vt3},[Xn]        LS_2D  0Q00110001000000 0110ssnnnnnttttt   0C40 6000   base register
                                   //  ld1     {Vt-Vt3},[Xn],Xm     LS_3F  0Q001100110mmmmm 0110ssnnnnnttttt   0CC0 6000   post-indexed by a register
                                   //  ld1     {Vt-Vt3},[Xn],#imm   LS_2E  0Q00110011011111 0110ssnnnnnttttt   0CDF 6000   post-indexed by an immediate

INST3(ld1_4regs,   "ld1",          LD,     IF_EN3J,   0x0C402000,  0x0CC02000,  0x0CDF2000)
                                   // LD1 (multiple structures, four registers variant)
                                   //  ld1     {Vt-Vt4},[Xn]        LS_2D  0Q00110001000000 0010ssnnnnnttttt   0C40 2000   base register
                                   //  ld1     {Vt-Vt4},[Xn],Xm     LS_3F  0Q001100110mmmmm 0010ssnnnnnttttt   0CC0 2000   post-indexed by a register
                                   //  ld1     {Vt-Vt4},[Xn],#imm   LS_2E  0Q00110011011111 0010ssnnnnnttttt   0CDF 2000   post-indexed by an immediate

INST3(st1_2regs,   "st1",          ST,     IF_EN3J,   0x0C00A000,  0x0C80A000,  0x0C9FA000)
                                   // ST1 (multiple structures, two registers variant)
                                   //  st1     {Vt,Vt2},[Xn]        LS_2D  0Q00110000000000 1010ssnnnnnttttt   0C00 A000   base register
                                   //  st1     {Vt,Vt2},[Xn],Xm     LS_3F  0Q001100100mmmmm 1010ssnnnnnttttt   0C80 A000   post-indexed by a register
                                   //  st1     {Vt,Vt2},[Xn],#imm   LS_2E  0Q00110010011111 1010ssnnnnnttttt   0C9F A000   post-indexed by an immediate

INST3(st1_3regs,   "st1",          ST,     IF_EN3J,   0x0C006000,  0x0C806000,  0x0C9F6000)
                                   // ST1 (multiple structures, three registers variant)
                                   //  st1     {Vt-Vt3},[Xn]        LS_2D  0Q00110000000000 0110ssnnnnnttttt   0C00 6000   base register
                                   //  st1     {Vt-Vt3},[Xn],Xm     LS_3F  0Q001100100mmmmm 0110XXnnnnnttttt   0C80 6000   post-indexed by a register
                                   //  st1     {Vt-Vt3},[Xn],#imm   LS_2E  0Q00110010011111 0110XXnnnnnttttt   0C9F 6000   post-indexed by an immediate

INST3(st1_4regs,   "st1",          ST,     IF_EN3J,   0x0C002000,  0x0C802000,  0x0C9F2000)
                                   // ST1 (multiple structures, four registers variant)
                                   //  st1     {Vt-Vt4},[Xn]        LS_2D  0Q00110000000000 0010XXnnnnnttttt   0C00 2000   base register
                                   //  st1     {Vt-Vt4},[Xn],Xm     LS_3F  0Q001100100mmmmm 0010XXnnnnnttttt   0C80 2000   post-indexed by a register
                                   //  st1     {Vt-Vt4},[Xn],#imm   LS_2E  0Q00110010011111 0010XXnnnnnttttt   0C9F 2000   post-indexed by an immediate

INST3(ld1r,        "ld1r",         LD,     IF_EN3J,   0x0D40C000,  0x0DC0C000,  0x0DDFC000)
                                   //  ld1r    {Vt},[Xn]            LS_2D  0Q00110101000000 1100ssnnnnnttttt   0D40 C000   base register
                                   //  ld1r    {Vt},[Xn],Xm         LS_3F  0Q001101110mmmmm 1100ssnnnnnttttt   0DC0 C000   post-indexed by a register
                                   //  ld1r    {Vt},[Xn],#1         LS_2E  0Q00110111011111 1100ssnnnnnttttt   0DDF C000   post-indexed by an immediate

INST3(ld2r,        "ld2r",         LD,     IF_EN3J,   0x0D60C000,  0x0DE0C000,  0x0DFFC000)
                                   //  ld2r    {Vt,Vt2},[Xn]        LS_2D  0Q00110101100000 1100ssnnnnnttttt   0D60 C000   base register
                                   //  ld2r    {Vt,Vt2},[Xn],Xm     LS_3F  0Q001101111mmmmm 1100ssnnnnnttttt   0DE0 C000   post-indexed by a register
                                   //  ld2r    {Vt,Vt2},[Xn],#2     LS_2E  0Q00110111111111 1100ssnnnnnttttt   0DFF C000   post-indexed by an immediate

INST3(ld3r,        "ld3r",         LD,     IF_EN3J,   0x0D40E000,  0x0DC0E000,  0x0DDFE000)
                                   //  ld3r    {Vt-Vt3},[Xn]        LS_2D  0Q00110101000000 1110ssnnnnnttttt   0D40 E000   base register
                                   //  ld3r    {Vt-Vt3},[Xn],Xm     LS_3F  0Q001101110mmmmm 1110ssnnnnnttttt   0DC0 E000   post-indexed by a register
                                   //  ld3r    {Vt-Vt3},[Xn],#4     LS_2E  0Q00110111011111 1110ssnnnnnttttt   0DDF E000   post-indexed by an immediate

INST3(ld4r,        "ld4r",         LD,     IF_EN3J,   0x0D60E000,  0x0DE0E000,  0x0DFFE000)
                                   //  ld4r    {Vt-Vt4},[Xn]        LS_2D  0Q00110101100000 1110ssnnnnnttttt   0D60 E000   base register
                                   //  ld4r    {Vt-Vt4},[Xn],Xm     LS_3F  0Q001101111mmmmm 1110ssnnnnnttttt   0DE0 E000   post-indexed by a register
                                   //  ld4r    {Vt-Vt4},[Xn],#8     LS_2E  0Q00110111111111 1110ssnnnnnttttt   0DFF E000   post-indexed by an immediate

//    enum         name            info               DR_2E        DR_2F
INST2(negs,        "negs",         0,      IF_EN2A,   0x6B0003E0,  0x6B0003E0)
                                   //  negs    Rd,Rm                DR_2E  X1101011000mmmmm 00000011111ddddd   6B00 03E0
                                   //  negs    Rd,(Rm,shk,imm)      DR_2F  X1101011sh0mmmmm ssssss11111ddddd   6B00 03E0   Rm {LSL,LSR,ASR} imm(0-63)

//    enum         name            info             DR_3A        DR_3B
INST2(bics,        "bics",         0,      IF_EN2B,   0x6A200000,  0x6A200000)
                                   //  bics    Rd,Rn,Rm             DR_3A  X1101010001mmmmm 000000nnnnnddddd   6A20 0000
                                   //  bics    Rd,Rn,(Rm,shk,imm)   DR_3B  X1101010sh1mmmmm iiiiiinnnnnddddd   6A20 0000   Rm {LSL,LSR,ASR,ROR} imm(0-63)

INST2(eon,         "eon",          0,      IF_EN2B,   0x4A200000,  0x4A200000)
                                   //  eon     Rd,Rn,Rm             DR_3A  X1001010001mmmmm 000000nnnnnddddd   4A20 0000
                                   //  eon     Rd,Rn,(Rm,shk,imm)   DR_3B  X1001010sh1mmmmm iiiiiinnnnnddddd   4A20 0000   Rm {LSL,LSR,ASR,ROR} imm(0-63)

//    enum         name            info               DR_3A         DI_2C
INST2(lsl,         "lsl",          0,      IF_EN2C,   0x1AC02000,  0x53000000)
                                   //  lsl     Rd,Rn,Rm             DR_3A  X0011010110mmmmm 001000nnnnnddddd   1AC0 2000
                                   //  lsl     Rd,Rn,imm6           DI_2D  X10100110Xrrrrrr ssssssnnnnnddddd   5300 0000   imm(N,r,s)

INST2(lsr,         "lsr",          0,      IF_EN2C,   0x1AC02400,  0x53007C00)
                                   //  lsr     Rd,Rn,Rm             DR_3A  X0011010110mmmmm 001001nnnnnddddd   1AC0 2400
                                   //  lsr     Rd,Rn,imm6           DI_2D  X10100110Xrrrrrr 011111nnnnnddddd   5300 7C00   imm(N,r,s)

INST2(asr,         "asr",          0,      IF_EN2C,   0x1AC02800,  0x13007C00)
                                   //  asr     Rd,Rn,Rm             DR_3A  X0011010110mmmmm 001010nnnnnddddd   1AC0 2800
                                   //  asr     Rd,Rn,imm6           DI_2D  X00100110Xrrrrrr 011111nnnnnddddd   1300 7C00   imm(N,r,s)

//    enum         name            info               DR_3A        DI_2B
INST2(ror,         "ror",          0,      IF_EN2D,   0x1AC02C00,  0x13800000)
                                   //  ror     Rd,Rn,Rm             DR_3A  X0011010110mmmmm 001011nnnnnddddd   1AC0 2C00
                                   //  ror     Rd,Rn,imm6           DI_2B  X00100111X0nnnnn ssssssnnnnnddddd   1380 0000   imm(0-63)

//    enum         name            info               LS_3B        LS_3C
INST2(ldp,         "ldp",          LD,     IF_EN2E,   0x29400000,  0x28400000)
                                   //  ldp     Rt,Ra,[Xn]           LS_3B  X010100101000000 0aaaaannnnnttttt   2940 0000   [Xn imm7]
                                   //  ldp     Rt,Ra,[Xn+simm7]     LS_3C  X010100PP1iiiiii iaaaaannnnnttttt   2840 0000   [Xn imm7 LSL {} pre/post/no inc]

INST2(ldpsw,       "ldpsw",        LD,     IF_EN2E,   0x69400000,  0x68400000)
                                   //  ldpsw   Rt,Ra,[Xn]           LS_3B  0110100101000000 0aaaaannnnnttttt   6940 0000   [Xn imm7]
                                   //  ldpsw   Rt,Ra,[Xn+simm7]     LS_3C  0110100PP1iiiiii iaaaaannnnnttttt   6840 0000   [Xn imm7 LSL {} pre/post/no inc]

INST2(stp,         "stp",          ST,     IF_EN2E,   0x29000000,  0x28000000)
                                   //  stp     Rt,Ra,[Xn]           LS_3B  X010100100000000 0aaaaannnnnttttt   2900 0000   [Xn imm7]
                                   //  stp     Rt,Ra,[Xn+simm7]     LS_3C  X010100PP0iiiiii iaaaaannnnnttttt   2800 0000   [Xn imm7 LSL {} pre/post/no inc]

INST2(ldnp,        "ldnp",         LD,     IF_EN2E,   0x28400000,  0x28400000)
                                   //  ldnp    Rt,Ra,[Xn]           LS_3B  X010100001000000 0aaaaannnnnttttt   2840 0000   [Xn imm7]
                                   //  ldnp    Rt,Ra,[Xn+simm7]     LS_3C  X010100001iiiiii iaaaaannnnnttttt   2840 0000   [Xn imm7 LSL {}]

INST2(stnp,        "stnp",         ST,     IF_EN2E,   0x28000000,  0x28000000)
                                   //  stnp    Rt,Ra,[Xn]           LS_3B  X010100000000000 0aaaaannnnnttttt   2800 0000   [Xn imm7]
                                   //  stnp    Rt,Ra,[Xn+simm7]     LS_3C  X010100000iiiiii iaaaaannnnnttttt   2800 0000   [Xn imm7 LSL {}]

INST2(ccmp,        "ccmp",         CMP,    IF_EN2F,   0x7A400000,  0x7A400800)
                                   //  ccmp    Rn,Rm,  nzcv,cond    DR_2I  X1111010010mmmmm cccc00nnnnn0nzcv   7A40 0000         nzcv, cond
                                   //  ccmp    Rn,imm5,nzcv,cond    DI_1F  X1111010010iiiii cccc10nnnnn0nzcv   7A40 0800   imm5, nzcv, cond

INST2(ccmn,        "ccmn",         CMP,    IF_EN2F,   0x3A400000,  0x3A400800)
                                   //  ccmn    Rn,Rm,  nzcv,cond    DR_2I  X0111010010mmmmm cccc00nnnnn0nzcv   3A40 0000         nzcv, cond
                                   //  ccmn    Rn,imm5,nzcv,cond    DI_1F  X0111010910iiiii cccc10nnnnn0nzcv   3A40 0800   imm5, nzcv, cond

//    enum         name            info               DV_2C        DV_2F
INST2(ins,         "ins",          0,      IF_EN2H,   0x4E001C00,  0x6E000400)
                                   //  ins     Vd[],Rn              DV_2C  01001110000iiiii 000111nnnnnddddd   4E00 1C00   Vd[],Rn   (from general)
                                   //  ins     Vd[],Vn[]            DV_2F  01101110000iiiii 0jjjj1nnnnnddddd   6E00 0400   Vd[],Vn[] (from/to elem)

//    enum         name            info               DV_3B        DV_3D
INST2(fadd,        "fadd",         0,      IF_EN2G,   0x0E20D400,  0x1E202800)
                                   //  fadd    Vd,Vn,Vm             DV_3B  0Q0011100X1mmmmm 110101nnnnnddddd   0E20 D400   Vd,Vn,Vm  (vector)
                                   //  fadd    Vd,Vn,Vm             DV_3D  000111100X1mmmmm 001010nnnnnddddd   1E20 2800   Vd,Vn,Vm  (scalar)

INST2(fsub,        "fsub",         0,      IF_EN2G,   0x0EA0D400,  0x1E203800)
                                   //  fsub    Vd,Vn,Vm             DV_3B  0Q0011101X1mmmmm 110101nnnnnddddd   0EA0 D400   Vd,Vn,Vm  (vector)
                                   //  fsub    Vd,Vn,Vm             DV_3D  000111100X1mmmmm 001110nnnnnddddd   1E20 3800   Vd,Vn,Vm  (scalar)

INST2(fdiv,        "fdiv",         0,      IF_EN2G,   0x2E20FC00,  0x1E201800)
                                   //  fdiv    Vd,Vn,Vm             DV_3B  0Q1011100X1mmmmm 111111nnnnnddddd   2E20 FC00   Vd,Vn,Vm  (vector)
                                   //  fdiv    Vd,Vn,Vm             DV_3D  000111100X1mmmmm 000110nnnnnddddd   1E20 1800   Vd,Vn,Vm  (scalar)

INST2(fmax,        "fmax",         0,      IF_EN2G,   0x0E20F400,  0x1E204800)
                                   //  fmax    Vd,Vn,Vm             DV_3B  0Q0011100X1mmmmm 111101nnnnnddddd   0E20 F400   Vd,Vn,Vm  (vector)
                                   //  fmax    Vd,Vn,Vm             DV_3D  000111100X1mmmmm 010010nnnnnddddd   1E20 4800   Vd,Vn,Vm  (scalar)

INST2(fmaxnm,      "fmaxnm",       0,      IF_EN2G,   0x0E20C400,  0x1E206800)
                                   //  fmaxnm  Vd,Vn,Vm             DV_3B  0Q0011100X1mmmmm 110001nnnnnddddd   0E20 C400   Vd,Vn,Vm  (vector)
                                   //  fmaxnm  Vd,Vn,Vm             DV_3D  000111100X1mmmmm 011010nnnnnddddd   1E20 6800   Vd,Vn,Vm  (scalar)

INST2(fmin,        "fmin",         0,      IF_EN2G,   0x0EA0F400,  0x1E205800)
                                   //  fmin    Vd,Vn,Vm             DV_3B  0Q0011101X1mmmmm 111101nnnnnddddd   0EA0 F400   Vd,Vn,Vm  (vector)
                                   //  fmin    Vd,Vn,Vm             DV_3D  000111100X1mmmmm 010110nnnnnddddd   1E20 5800   Vd,Vn,Vm  (scalar)

INST2(fminnm,      "fminnm",       0,      IF_EN2G,   0x0EA0C400,  0x1E207800)
                                   //  fminnm  Vd,Vn,Vm             DV_3B  0Q0011101X1mmmmm 110001nnnnnddddd   0EA0 C400   Vd,Vn,Vm  (vector)
                                   //  fminnm  Vd,Vn,Vm             DV_3D  000111100X1mmmmm 011110nnnnnddddd   1E20 7800   Vd,Vn,Vm  (scalar)

INST2(fabd,        "fabd",         0,      IF_EN2G,   0x2EA0D400,  0x7EA0D400)
                                   //  fabd    Vd,Vn,Vm             DV_3B  0Q1011101X1mmmmm 110101nnnnnddddd   2EA0 D400   Vd,Vn,Vm  (vector)
                                   //  fabd    Vd,Vn,Vm             DV_3D  011111101X1mmmmm 110101nnnnnddddd   7EA0 D400   Vd,Vn,Vm  (scalar)

INST2(facge,       "facge",        0,      IF_EN2G,   0x2E20EC00,  0x7E20EC00)
                                   //  facge   Vd,Vn,Vm             DV_3B  0Q1011100X1mmmmm 111011nnnnnddddd   2E20 EC00   Vd,Vn,Vm  (vector)
                                   //  facge   Vd,Vn,Vm             DV_3D  011111100X1mmmmm 111011nnnnnddddd   7E20 EC00   Vd,Vn,Vm  (scalar)

INST2(facgt,       "facgt",        0,      IF_EN2G,   0x2EA0EC00,  0x7EA0EC00)
                                   //  facgt   Vd,Vn,Vm             DV_3B  0Q1011101X1mmmmm 111011nnnnnddddd   2EA0 EC00   Vd,Vn,Vm  (vector)
                                   //  facgt   Vd,Vn,Vm             DV_3D  011111101X1mmmmm 111011nnnnnddddd   7EA0 EC00   Vd,Vn,Vm  (scalar)

INST2(frecps,      "frecps",       0,      IF_EN2G,   0x0E20FC00,  0x5E20FC00)
                                   //  frecps  Vd,Vn,Vm            DV_3B  0Q0011100X1mmmmm 111111nnnnnddddd    0E20 FC00   Vd,Vn,Vm  (vector)
                                   //  frecps  Vd,Vn,Vm            DV_3D  010111100X1mmmmm 111111nnnnnddddd    5E20 FC00   Vd,Vn,Vm  (scalar)

INST2(frsqrts,     "frsqrts",      0,      IF_EN2G,   0x0EA0FC00,  0x5EA0FC00)
                                   //  frsqrts Vd,Vn,Vm            DV_3B  0Q0011101X1mmmmm 111111nnnnnddddd    0EA0 FC00   Vd,Vn,Vm  (vector)
                                   //  frsqrts Vd,Vn,Vm            DV_3D  010111101X1mmmmm 111111nnnnnddddd    5EA0 FC00   Vd,Vn,Vm  (scalar)

//    enum         name            info               DV_2K        DV_1C
INST2(fcmp,        "fcmp",         0,      IF_EN2I,   0x1E202000,  0x1E202008)
                                   //  fcmp    Vn,Vm                DV_2K  000111100X1mmmmm 001000nnnnn00000   1E20 2000   Vn Vm
                                   //  fcmp    Vn,#0.0              DV_1C  000111100X100000 001000nnnnn01000   1E20 2008   Vn #0.0

INST2(fcmpe,       "fcmpe",        0,      IF_EN2I,   0x1E202010,  0x1E202018)
                                   //  fcmpe   Vn,Vm                DV_2K  000111100X1mmmmm 001000nnnnn10000   1E20 2010   Vn Vm
                                   //  fcmpe   Vn,#0.0              DV_1C  000111100X100000 001000nnnnn11000   1E20 2018   Vn #0.0

//    enum         name            info               DV_2A        DV_2G
INST2(fabs,        "fabs",         0,      IF_EN2J,   0x0EA0F800,  0x1E20C000)
                                   //  fabs    Vd,Vn                DV_2A  0Q0011101X100000 111110nnnnnddddd   0EA0 F800   Vd,Vn    (vector)
                                   //  fabs    Vd,Vn                DV_2G  000111100X100000 110000nnnnnddddd   1E20 C000   Vd,Vn    (scalar)

INST2(fcmle,       "fcmle",        0,      IF_EN2J,   0x2EA0D800,  0x7EA0D800)
                                   //  fcmle   Vd,Vn,#0             DV_2A  0Q1011101X100000 111110nnnnnddddd   2EA0 D800   Vd,Vn,#0 (vector - with zero)
                                   //  fcmle   Vd,Vn,#0             DV_2G  011111101X100000 110110nnnnnddddd   7EA0 D800   Vd,Vn,#0 (scalar - with zero)

INST2(fcmlt,       "fcmlt",        0,      IF_EN2J,   0x0EA0E800,  0x5EA0E800)
                                   //  fcmlt   Vd,Vn,#0             DV_2A  0Q0011101X100000 111110nnnnnddddd   0EA0 E800   Vd,Vn,#0 (vector - with zero)
                                   //  fcmlt   Vd,Vn,#0             DV_2G  010111101X100000 111010nnnnnddddd   5EA0 E800   Vd,Vn,#0 (scalar - with zero)

INST2(fcvtxn,      "fcvtxn",       NRW,    IF_EN2J,   0x2E616800,  0x7E616800)
                                   //  fcvtxn  Vd,Vn                DV_2A  0010111001100001 011010nnnnnddddd   2E61 6800   Vd,Vn    (vector)
                                   //  fcvtxn  Vd,Vn                DV_2G  0111111001100001 011010nnnnnddddd   7E61 6800   Vd,Vn    (scalar)

INST2(fneg,        "fneg",         0,      IF_EN2J,   0x2EA0F800,  0x1E214000)
                                   //  fneg    Vd,Vn                DV_2A  0Q1011101X100000 111110nnnnnddddd   2EA0 F800   Vd,Vn    (vector)
                                   //  fneg    Vd,Vn                DV_2G  000111100X100001 010000nnnnnddddd   1E21 4000   Vd,Vn    (scalar)

INST2(frecpe,      "frecpe",       0,      IF_EN2J,   0x0EA1D800,  0x5EA1D800)
                                   //  frecpe  Vd,Vn                DV_2A  0Q0011101X100001 110110nnnnnddddd   0EA1 D800   Vd,Vn    (vector)
                                   //  frecpe  Vd,Vn                DV_2G  010111101X100001 110110nnnnnddddd   5EA1 D800   Vd,Vn    (scalar)

INST2(frintn,      "frintn",       0,      IF_EN2J,   0x0E218800,  0x1E244000)
                                   //  frintn  Vd,Vn                DV_2A  0Q0011100X100001 100010nnnnnddddd   0E21 8800   Vd,Vn    (vector)
                                   //  frintn  Vd,Vn                DV_2G  000111100X100100 010000nnnnnddddd   1E24 4000   Vd,Vn    (scalar)

INST2(frintp,      "frintp",       0,      IF_EN2J,   0x0EA18800,  0x1E24C000)
                                   //  frintp  Vd,Vn                DV_2A  0Q0011101X100001 100010nnnnnddddd   0EA1 8800   Vd,Vn    (vector)
                                   //  frintp  Vd,Vn                DV_2G  000111100X100100 110000nnnnnddddd   1E24 C000   Vd,Vn    (scalar)

INST2(frintm,      "frintm",       0,      IF_EN2J,   0x0E219800,  0x1E254000)
                                   //  frintm  Vd,Vn                DV_2A  0Q0011100X100001 100110nnnnnddddd   0E21 9800   Vd,Vn    (vector)
                                   //  frintm  Vd,Vn                DV_2G  000111100X100101 010000nnnnnddddd   1E25 4000   Vd,Vn    (scalar)

INST2(frintz,      "frintz",       0,      IF_EN2J,   0x0EA19800,  0x1E25C000)
                                   //  frintz  Vd,Vn                DV_2A  0Q0011101X100001 100110nnnnnddddd   0EA1 9800   Vd,Vn    (vector)
                                   //  frintz  Vd,Vn                DV_2G  000111100X100101 110000nnnnnddddd   1E25 C000   Vd,Vn    (scalar)

INST2(frinta,      "frinta",       0,      IF_EN2J,   0x2E218800,  0x1E264000)
                                   //  frinta  Vd,Vn                DV_2A  0Q1011100X100001 100010nnnnnddddd   2E21 8800   Vd,Vn    (vector)
                                   //  frinta  Vd,Vn                DV_2G  000111100X100110 010000nnnnnddddd   1E26 4000   Vd,Vn    (scalar)

INST2(frintx,      "frintx",       0,      IF_EN2J,   0x2E219800,  0x1E274000)
                                   //  frintx  Vd,Vn                DV_2A  0Q1011100X100001 100110nnnnnddddd   2E21 9800   Vd,Vn    (vector)
                                   //  frintx  Vd,Vn                DV_2G  000111100X100111 010000nnnnnddddd   1E27 4000   Vd,Vn    (scalar)

INST2(frinti,      "frinti",       0,      IF_EN2J,   0x2EA19800,  0x1E27C000)
                                   //  frinti  Vd,Vn                DV_2A  0Q1011101X100001 100110nnnnnddddd   2EA1 9800   Vd,Vn    (vector)
                                   //  frinti  Vd,Vn                DV_2G  000111100X100111 110000nnnnnddddd   1E27 C000   Vd,Vn    (scalar)

INST2(frsqrte,     "frsqrte",      0,      IF_EN2J,   0x2EA1D800,  0x7EA1D800)
                                   //  frsqrte Vd,Vn                DV_2A  0Q1011101X100001 110110nnnnnddddd   2EA1 D800   Vd,Vn    (vector)
                                   //  frsqrte Vd,Vn                DV_2G  011111101X100001 110110nnnnnddddd   7EA1 D800   Vd,Vn    (scalar)

INST2(fsqrt,       "fsqrt",        0,      IF_EN2J,   0x2EA1F800,  0x1E21C000)
                                   //  fsqrt   Vd,Vn                DV_2A  0Q1011101X100001 111110nnnnnddddd   2EA1 F800   Vd,Vn    (vector)
                                   //  fsqrt   Vd,Vn                DV_2G  000111100X100001 110000nnnnnddddd   1E21 C000   Vd,Vn    (scalar)

//    enum         name            info               DV_2M        DV_2L
INST2(abs,         "abs",          0,      IF_EN2K,   0x0E20B800,  0x5E20B800)
                                   //  abs     Vd,Vn                DV_2M  0Q001110XX100000 101110nnnnnddddd   0E20 B800   Vd,Vn    (vector)
                                   //  abs     Vd,Vn                DV_2L  01011110XX100000 101110nnnnnddddd   5E20 B800   Vd,Vn    (scalar)

INST2(cmle,        "cmle",         0,      IF_EN2K,   0x2E209800,  0x7E209800)
                                   //  cmle    Vd,Vn,#0             DV_2M  0Q101110XX100000 100110nnnnnddddd   2E20 9800   Vd,Vn,#0 (vector - with zero)
                                   //  cmle    Vd,Vn,#0             DV_2L  01111110XX100000 100110nnnnnddddd   7E20 9800   Vd,Vn,#0 (scalar - with zero)

INST2(cmlt,        "cmlt",         0,      IF_EN2K,   0x0E20A800,  0x5E20A800)
                                   //  cmlt    Vd,Vn,#0             DV_2M  0Q101110XX100000 101010nnnnnddddd   0E20 A800   Vd,Vn,#0 (vector - with zero)
                                   //  cmlt    Vd,Vn,#0             DV_2L  01011110XX100000 101010nnnnnddddd   5E20 A800   Vd,Vn,#0 (scalar - with zero)

INST2(sqabs,       "sqabs",        0,      IF_EN2K,   0x0E207800,  0x5E207800)
                                   //  sqabs   Vd,Vn                DV_2M  0Q001110XX100000 011110nnnnnddddd   0E20 7800   Vd,Vn    (vector)
                                   //  sqabs   Vd,Vn                DV_2L  01011110XX100000 011110nnnnnddddd   5E20 7800   Vd,Vn    (scalar)

INST2(sqneg,       "sqneg",        0,      IF_EN2K,   0x2E207800,  0x7E207800)
                                   //  sqneg   Vd,Vn                DV_2M  0Q101110XX100000 011110nnnnnddddd   2E20 7800   Vd,Vn    (vector)
                                   //  sqneg   Vd,Vn                DV_2L  01111110XX100000 011110nnnnnddddd   7E20 7800   Vd,Vn    (scalar)

INST2(sqxtn,       "sqxtn",        NRW,    IF_EN2K,   0x0E214800,  0x5E214800)
                                   //  sqxtn   Vd,Vn                DV_2M  0Q001110XX100001 010010nnnnnddddd   0E21 4800   Vd,Vn    (vector)
                                   //  sqxtn   Vd,Vn                DV_2L  01011110XX100001 010010nnnnnddddd   5E21 4800   Vd,Vn    (scalar)

INST2(sqxtun,      "sqxtun",       NRW,    IF_EN2K,   0x2E212800,  0x7E212800)
                                   //  sqxtun  Vd,Vn                DV_2M  0Q101110XX100001 001010nnnnnddddd   2E21 2800   Vd,Vn    (vector)
                                   //  sqxtun  Vd,Vn                DV_2L  01111110XX100001 001010nnnnnddddd   7E21 2800   Vd,Vn    (scalar)

INST2(suqadd,      "suqadd",       0,      IF_EN2K,   0x0E203800,  0x5E203800)
                                   //  suqadd  Vd,Vn                DV_2M  0Q001110XX100000 001110nnnnnddddd   0E20 3800   Vd,Vn    (vector)
                                   //  suqadd  Vd,Vn                DV_2L  01011110XX100000 001110nnnnnddddd   5E20 3800   Vd,Vn    (scalar)

INST2(usqadd,      "usqadd",       0,      IF_EN2K,   0x2E203800,  0x7E203800)
                                   //  usqadd  Vd,Vn                DV_2M  0Q101110XX100000 001110nnnnnddddd   2E20 3800   Vd,Vn    (vector)
                                   //  usqadd  Vd,Vn                DV_2L  01111110XX100000 001110nnnnnddddd   7E20 3800   Vd,Vn    (scalar)

INST2(uqxtn,       "uqxtn",        NRW,    IF_EN2K,   0x2E214800,  0x7E214800)
                                   //  uqxtn   Vd,Vn                DV_2M  0Q101110XX100001 010010nnnnnddddd   2E21 4800   Vd,Vn    (vector)
                                   //  uqxtn   Vd,Vn                DV_2L  01111110XX100001 010010nnnnnddddd   7E21 4800   Vd,Vn    (scalar)

//    enum         name            info               DR_2G        DV_2M
INST2(cls,         "cls",          0,      IF_EN2L,   0x5AC01400,  0x0E204800)
                                   //  cls     Rd,Rm                DR_2G  X101101011000000 000101nnnnnddddd   5AC0 1400   Rd Rn    (general)
                                   //  cls     Vd,Vn                DV_2M  0Q00111000100000 010010nnnnnddddd   0E20 4800   Vd,Vn    (vector)

INST2(clz,         "clz",          0,      IF_EN2L,   0x5AC01000,  0x2E204800)
                                   //  clz     Rd,Rm                DR_2G  X101101011000000 000100nnnnnddddd   5AC0 1000   Rd Rn    (general)
                                   //  clz     Vd,Vn                DV_2M  0Q10111000100000 010010nnnnnddddd   2E20 4800   Vd,Vn    (vector)

INST2(rbit,        "rbit",         0,      IF_EN2L,   0x5AC00000,  0x2E605800)
                                   //  rbit    Rd,Rm                DR_2G  X101101011000000 000000nnnnnddddd   5AC0 0000   Rd Rn    (general)
                                   //  rbit    Vd,Vn                DV_2M  0Q10111001100000 010110nnnnnddddd   2E60 5800   Vd,Vn    (vector)

INST2(rev16,       "rev16",        0,      IF_EN2L,   0x5AC00400,  0x0E201800)
                                   //  rev16   Rd,Rm                DR_2G  X101101011000000 000001nnnnnddddd   5AC0 0400   Rd Rn    (general)
                                   //  rev16   Vd,Vn                DV_2M  0Q001110XX100000 000110nnnnnddddd   0E20 1800   Vd,Vn    (vector)

INST2(rev32,       "rev32",        0,      IF_EN2L,   0x5AC00800,  0x2E200800)
                                   //  rev32   Rd,Rm                DR_2G  X101101011000000 000010nnnnnddddd   5AC0 0800   Rd Rn    (general)
                                   //  rev32   Vd,Vn                DV_2M  0Q101110XX100000 000010nnnnnddddd   2E20 0800   Vd,Vn    (vector)

//    enum         name            info               DV_3A        DV_3AI
INST2(mla,         "mla",          0,      IF_EN2M,   0x0E209400,  0x2F000000)
                                   //  mla     Vd,Vn,Vm             DV_3A  0Q001110XX1mmmmm 100101nnnnnddddd   0E20 9400   Vd,Vn,Vm   (vector)
                                   //  mla     Vd,Vn,Vm[]           DV_3AI 0Q101111XXLMmmmm 0000H0nnnnnddddd   2F00 0000   Vd,Vn,Vm[] (vector by element)

INST2(mls,         "mls",          0,      IF_EN2M,   0x2E209400,  0x2F004000)
                                   //  mls     Vd,Vn,Vm             DV_3A  0Q101110XX1mmmmm 100101nnnnnddddd   2E20 9400   Vd,Vn,Vm   (vector)
                                   //  mls     Vd,Vn,Vm[]           DV_3AI 0Q101111XXLMmmmm 0100H0nnnnnddddd   2F00 4000   Vd,Vn,Vm[] (vector by element)

INST2(smlal,       "smlal",        LNG,    IF_EN2M,   0x0E208000,  0x0F002000)
                                   //  smlal     Vd,Vn,Vm           DV_3A  00001110XX1mmmmm 100000nnnnnddddd   0E20 8000   Vd,Vn,Vm   (vector)
                                   //  smlal     Vd,Vn,Vm[]         DV_3AI 00001111XXLMmmmm 0010H0nnnnnddddd   0F00 2000   Vd,Vn,Vm[] (vector by element)

INST2(smlal2,      "smlal2",       LNG,    IF_EN2M,   0x4E208000,  0x4F002000)
                                   //  smlal2     Vd,Vn,Vm          DV_3A  01001110XX1mmmmm 100000nnnnnddddd   4E20 8000   Vd,Vn,Vm   (vector)
                                   //  smlal2     Vd,Vn,Vm[]        DV_3AI 01001111XXLMmmmm 0010H0nnnnnddddd   4F00 2000   Vd,Vn,Vm[] (vector by element)

INST2(smlsl,       "smlsl",        LNG,    IF_EN2M,   0x0E20A000,  0x0F006000)
                                   //  smlsl     Vd,Vn,Vm           DV_3A  00001110XX1mmmmm 101000nnnnnddddd   0E20 A000   Vd,Vn,Vm   (vector)
                                   //  smlsl     Vd,Vn,Vm[]         DV_3AI 00001111XXLMmmmm 0110H0nnnnnddddd   0F00 6000   Vd,Vn,Vm[] (vector by element)

INST2(smlsl2,      "smlsl2",       LNG,    IF_EN2M,   0x4E20A000,  0x4F006000)
                                   //  smlsl2     Vd,Vn,Vm          DV_3A  01001110XX1mmmmm 101000nnnnnddddd   4E20 A000   Vd,Vn,Vm   (vector)
                                   //  smlsl2     Vd,Vn,Vm[]        DV_3AI 01001111XXLMmmmm 0110H0nnnnnddddd   4F00 6000   Vd,Vn,Vm[] (vector by element)

INST2(smull2,      "smull2",       LNG,    IF_EN2M,   0x4E20C000,  0x4F00A000)
                                   //  smull2     Vd,Vn,Vm          DV_3A  01001110XX1mmmmm 110000nnnnnddddd   4E20 C000   Vd,Vn,Vm   (vector)
                                   //  smull2     Vd,Vn,Vm[]        DV_3AI 01001111XXLMmmmm 1010H0nnnnnddddd   4F00 A000   Vd,Vn,Vm[] (vector by element)

INST2(sqdmlal2,    "sqdmlal2",     LNG,    IF_EN2M,   0x4E209000,  0x4F003000)
                                   // sqdmlal2    Vd,Vn,Vm          DV_3A  01001110XX1mmmmm 100100nnnnnddddd   4E20 9000   Vd,Vn,Vm   (vector)
                                   // sqdmlal2    Vd,Vn,Vm[]        DV_3AI 01001111XXLMmmmm 0011H0nnnnnddddd   4F00 3000   Vd,Vn,Vm[] (vector by element)

INST2(sqdmlsl2,    "sqdmlsl2",     LNG,    IF_EN2M,   0x4E20B000,  0x4F007000)
                                   // sqdmlsl2    Vd,Vn,Vm          DV_3A  01001110XX1mmmmm 101100nnnnnddddd   4E20 B000   Vd,Vn,Vm   (vector)
                                   // sqdmlsl2    Vd,Vn,Vm[]        DV_3AI 01001111XXLMmmmm 0111H0nnnnnddddd   4F00 7000   Vd,Vn,Vm[] (vector by element)

INST2(sqdmull2,    "sqdmull2",     LNG,    IF_EN2M,   0x4E20D000,  0x4F00B000)
                                   // sqdmull2    Vd,Vn,Vm          DV_3A  01001110XX1mmmmm 110100nnnnnddddd   4E20 D000   Vd,Vn,Vm   (vector)
                                   // sqdmull2    Vd,Vn,Vm[]        DV_3AI 01001111XXLMmmmm 1011H0nnnnnddddd   4F00 B000   Vd,Vn,Vm[] (vector by element)

INST2(sdot,        "sdot",         0,      IF_EN2M,   0x0E009400,  0x0F00E000)
                                   //  sdot       Vd,Vn,Vm          DV_3A  0Q001110XX0mmmmm 100101nnnnnddddd   0E00 9400   Vd,Vn,Vm   (vector)
                                   //  sdot       Vd,Vn,Vm[]        DV_3AI 0Q001111XXLMmmmm 1110H0nnnnnddddd   0F00 E000   Vd,Vn,Vm[] (vector by element)

INST2(udot,        "udot",         0,      IF_EN2M,   0x2E009400,  0x2F00E000)
                                   //  udot       Vd,Vn,Vm          DV_3A  0Q101110XX0mmmmm 100101nnnnnddddd   2E00 9400   Vd,Vn,Vm   (vector)
                                   //  udot       Vd,Vn,Vm[]        DV_3AI 0Q101111XXLMmmmm 1110H0nnnnnddddd   2F00 E000   Vd,Vn,Vm[] (vector by element)

INST2(umlal,       "umlal",        LNG,    IF_EN2M,   0x2E208000,  0x2F002000)
                                   //  umlal     Vd,Vn,Vm           DV_3A  00101110XX1mmmmm 100000nnnnnddddd   2E20 8000   Vd,Vn,Vm   (vector)
                                   //  umlal     Vd,Vn,Vm[]         DV_3AI 00101111XXLMmmmm 0010H0nnnnnddddd   2F00 2000   Vd,Vn,Vm[] (vector by element)

INST2(umlal2,      "umlal2",       LNG,    IF_EN2M,   0x6E208000,  0x6F002000)
                                   //  umlal2     Vd,Vn,Vm          DV_3A  01101110XX1mmmmm 100000nnnnnddddd   6E20 8000   Vd,Vn,Vm   (vector)
                                   //  umlal2     Vd,Vn,Vm[]        DV_3AI 01101111XXLMmmmm 0010H0nnnnnddddd   6F00 2000   Vd,Vn,Vm[] (vector by element)

INST2(umlsl,       "umlsl",        LNG,    IF_EN2M,   0x2E20A000,  0x2F006000)
                                   //  umlsl     Vd,Vn,Vm           DV_3A  00101110XX1mmmmm 101000nnnnnddddd   2E20 A000   Vd,Vn,Vm   (vector)
                                   //  umlsl     Vd,Vn,Vm[]         DV_3AI 00101111XXLMmmmm 0110H0nnnnnddddd   2F00 6000   Vd,Vn,Vm[] (vector by element)

INST2(umlsl2,      "umlsl2",       LNG,    IF_EN2M,   0x6E20A000,  0x6F006000)
                                   //  umlsl2     Vd,Vn,Vm          DV_3A  01101110XX1mmmmm 101000nnnnnddddd   6E20 A000   Vd,Vn,Vm   (vector)
                                   //  umlsl2     Vd,Vn,Vm[]        DV_3AI 01101111XXLMmmmm 0110H0nnnnnddddd   6F00 6000   Vd,Vn,Vm[] (vector by element)

INST2(umull2,      "umull2",       LNG,    IF_EN2M,   0x6E20C000,  0x6F00A000)
                                   //  umull2     Vd,Vn,Vm          DV_3A  01101110XX1mmmmm 110000nnnnnddddd   6E20 C000   Vd,Vn,Vm   (vector)
                                   //  umull2     Vd,Vn,Vm[]        DV_3AI 01101111XXLMmmmm 1010H0nnnnnddddd   6F00 A000   Vd,Vn,Vm[] (vector by element)

//    enum         name            info               DV_2N        DV_2O
INST2(sshr,        "sshr",         RSH,    IF_EN2N,   0x5F000400,  0x0F000400)
                                   //  sshr    Vd,Vn,imm            DV_2N  010111110iiiiiii 000001nnnnnddddd   5F00 0400   Vd Vn imm  (right shift - scalar)
                                   //  sshr    Vd,Vn,imm            DV_2O  0Q0011110iiiiiii 000001nnnnnddddd   0F00 0400   Vd,Vn imm  (right shift - vector)

INST2(ssra,        "ssra",         RSH,    IF_EN2N,   0x5F001400,  0x0F001400)
                                   //  ssra    Vd,Vn,imm            DV_2N  010111110iiiiiii 000101nnnnnddddd   5F00 1400   Vd Vn imm  (right shift - scalar)
                                   //  ssra    Vd,Vn,imm            DV_2O  0Q0011110iiiiiii 000101nnnnnddddd   0F00 1400   Vd,Vn imm  (right shift - vector)

INST2(srshr,       "srshr",        RSH,    IF_EN2N,   0x5F002400,  0x0F002400)
                                   //  srshr   Vd,Vn,imm            DV_2N  010111110iiiiiii 001001nnnnnddddd   5F00 2400   Vd Vn imm  (right shift - scalar)
                                   //  srshr   Vd,Vn,imm            DV_2O  0Q0011110iiiiiii 001001nnnnnddddd   0F00 2400   Vd,Vn imm  (right shift - vector)

INST2(srsra,       "srsra",        RSH,    IF_EN2N,   0x5F003400,  0x0F003400)
                                   //  srsra   Vd,Vn,imm            DV_2N  010111110iiiiiii 001101nnnnnddddd   5F00 3400   Vd Vn imm  (right shift - scalar)
                                   //  srsra   Vd,Vn,imm            DV_2O  0Q0011110iiiiiii 001101nnnnnddddd   0F00 3400   Vd,Vn imm  (right shift - vector)

INST2(shl,         "shl",          0,      IF_EN2N,   0x5F005400,  0x0F005400)
                                   //  shl     Vd,Vn,imm            DV_2N  010111110iiiiiii 010101nnnnnddddd   5F00 5400   Vd Vn imm  (left shift - scalar)
                                   //  shl     Vd,Vn,imm            DV_2O  0Q0011110iiiiiii 010101nnnnnddddd   0F00 5400   Vd,Vn imm  (left shift - vector)

INST2(ushr,        "ushr",         RSH,    IF_EN2N,   0x7F000400,  0x2F000400)
                                   //  ushr    Vd,Vn,imm            DV_2N  011111110iiiiiii 000001nnnnnddddd   7F00 0400   Vd Vn imm  (right shift - scalar)
                                   //  ushr    Vd,Vn,imm            DV_2O  0Q1011110iiiiiii 000001nnnnnddddd   2F00 0400   Vd,Vn imm  (right shift - vector)

INST2(usra,        "usra",         RSH,    IF_EN2N,   0x7F001400,  0x2F001400)
                                   //  usra    Vd,Vn,imm            DV_2N  011111110iiiiiii 000101nnnnnddddd   7F00 1400   Vd Vn imm  (right shift - scalar)
                                   //  usra    Vd,Vn,imm            DV_2O  0Q1011110iiiiiii 000101nnnnnddddd   2F00 1400   Vd,Vn imm  (right shift - vector)

INST2(urshr,       "urshr",        RSH,    IF_EN2N,   0x7F002400,  0x2F002400)
                                   //  urshr   Vd,Vn,imm            DV_2N  011111110iiiiiii 001001nnnnnddddd   7F00 2400   Vd Vn imm  (right shift - scalar)
                                   //  urshr   Vd,Vn,imm            DV_2O  0Q1011110iiiiiii 001001nnnnnddddd   2F00 2400   Vd,Vn imm  (right shift - vector)

INST2(ursra,       "ursra",        RSH,    IF_EN2N,   0x7F003400,  0x2F003400)
                                   //  ursra   Vd,Vn,imm            DV_2N  011111110iiiiiii 001101nnnnnddddd   7F00 3400   Vd Vn imm  (right shift - scalar)
                                   //  ursra   Vd,Vn,imm            DV_2O  0Q1011110iiiiiii 001101nnnnnddddd   2F00 3400   Vd,Vn imm  (right shift - vector)

INST2(sri,         "sri",          RSH,    IF_EN2N,   0x7F004400,  0x2F004400)
                                   //  sri     Vd,Vn,imm            DV_2N  011111110iiiiiii 010001nnnnnddddd   7F00 4400   Vd Vn imm  (right shift - scalar)
                                   //  sri     Vd,Vn,imm            DV_2O  0Q1011110iiiiiii 010001nnnnnddddd   2F00 4400   Vd,Vn imm  (right shift - vector)

INST2(sli,         "sli",          0,      IF_EN2N,   0x7F005400,  0x2F005400)
                                   //  sli     Vd,Vn,imm            DV_2N  011111110iiiiiii 010101nnnnnddddd   7F00 5400   Vd Vn imm  (left shift - scalar)
                                   //  sli     Vd,Vn,imm            DV_2O  0Q1011110iiiiiii 010101nnnnnddddd   2F00 5400   Vd,Vn imm  (left shift - vector)

INST2(sqshlu,      "sqshlu",       0,      IF_EN2N,   0x7F006400,  0x2F006400)
                                   //  sqshlu  Vd,Vn,imm            DV_2N  011111110iiiiiii 011001nnnnnddddd   7F00 6400   Vd Vn imm  (left shift - scalar)
                                   //  sqshlu  Vd,Vn,imm            DV_2O  0Q1011110iiiiiii 011001nnnnnddddd   2F00 6400   Vd Vn imm  (left shift - vector)

INST2(sqrshrn,     "sqrshrn",      RSH|NRW,IF_EN2N,   0x5F009C00,  0x0F009C00)
                                   //  sqrshrn Vd,Vn,imm            DV_2N  010111110iiiiiii 100111nnnnnddddd   5F00 9C00   Vd Vn imm  (right shift - scalar)
                                   //  sqrshrn Vd,Vn,imm            DV_2O  0Q0011110iiiiiii 100111nnnnnddddd   0F00 9C00   Vd Vn imm  (right shift - vector)

INST2(sqrshrun,    "sqrshrun",     RSH|NRW,IF_EN2N,   0x7F008C00,  0x2F008C00)
                                   //  sqrshrun Vd,Vn,imm           DV_2N  011111110iiiiiii 100011nnnnnddddd   7F00 8C00   Vd Vn imm  (right shift - scalar)
                                   //  sqrshrun Vd,Vn,imm           DV_2O  0Q1011110iiiiiii 100011nnnnnddddd   2F00 8C00   Vd Vn imm  (right shift - vector)

INST2(sqshrn,      "sqshrn",       RSH|NRW,IF_EN2N,   0x5F009400,  0x0F009400)
                                   //  sqshrn  Vd,Vn,imm            DV_2N  010111110iiiiiii 100101nnnnnddddd   5F00 9400   Vd Vn imm  (right shift - scalar)
                                   //  sqshrn  Vd,Vn,imm            DV_2O  0Q0011110iiiiiii 100101nnnnnddddd   0F00 9400   Vd Vn imm  (right shift - vector)

INST2(sqshrun,     "sqshrun",      RSH|NRW,IF_EN2N,   0x7F008400,  0x2F008400)
                                   //  sqshrun  Vd,Vn,imm           DV_2N  011111110iiiiiii 100001nnnnnddddd   7F00 8400   Vd Vn imm  (right shift - scalar)
                                   //  sqshrun  Vd,Vn,imm           DV_2O  0Q1011110iiiiiii 100001nnnnnddddd   2F00 8400   Vd Vn imm  (right shift - vector)

INST2(uqrshrn,     "uqrshrn",      RSH|NRW,IF_EN2N,   0x7F009C00,  0x2F009C00)
                                   //  uqrshrn Vd,Vn,imm            DV_2N  011111110iiiiiii 100111nnnnnddddd   7F00 9C00   Vd Vn imm  (right shift - scalar)
                                   //  uqrshrn Vd,Vn,imm            DV_2O  0Q1011110iiiiiii 100111nnnnnddddd   2F00 9C00   Vd Vn imm  (right shift - vector)

INST2(uqshrn,      "uqshrn",       RSH|NRW,IF_EN2N,   0x7F009400,  0x2F009400)
                                   //  usqhrn  Vd,Vn,imm            DV_2N  011111110iiiiiii 100101nnnnnddddd   7F00 9400   Vd Vn imm  (right shift - scalar)
                                   //  usqhrn  Vd,Vn,imm            DV_2O  0Q1011110iiiiiii 100101nnnnnddddd   2F00 9400   Vd Vn imm  (right shift - vector)

//    enum         name            info               DV_3E        DV_3A
INST2(cmhi,        "cmhi",         0,      IF_EN2O,   0x7E203400,  0x2E203400)
                                   //  cmhi    Vd,Vn,Vm             DV_3E  01111110001mmmmm 001101nnnnnddddd   7E20 3400   Vd,Vn,Vm   (scalar)
                                   //  cmhi    Vd,Vn,Vm             DV_3A  0Q101110XX1mmmmm 001101nnnnnddddd   2E20 3400   Vd,Vn,Vm   (vector)

INST2(cmhs,        "cmhs",         0,      IF_EN2O,   0x7E203C00,  0x2E203C00)
                                   //  cmhs    Vd,Vn,Vm             DV_3E  01111110001mmmmm 001111nnnnnddddd   7E20 3C00   Vd,Vn,Vm   (scalar)
                                   //  cmhs    Vd,Vn,Vm             DV_3A  0Q101110XX1mmmmm 001111nnnnnddddd   2E20 3C00   Vd,Vn,Vm   (vector)

INST2(cmtst,       "cmtst",        0,      IF_EN2O,   0x5E208C00,  0x0E208C00)
                                   //  cmtst   Vd,Vn,Vm             DV_3E  01011110001mmmmm 100011nnnnnddddd   5E20 8C00   Vd,Vn,Vm   (scalar)
                                   //  cmtst   Vd,Vn,Vm             DV_3A  0Q001110XX1mmmmm 100011nnnnnddddd   0E20 8C00   Vd,Vn,Vm   (vector)

INST2(sqadd,       "sqadd",        0,      IF_EN2O,   0x5E200C00,  0x0E200C00)
                                   //  sqadd   Vd,Vn,Vm             DV_3E  01011110XX1mmmmm 000011nnnnnddddd   5E20 0C00   Vd,Vn,Vm   (scalar)
                                   //  sqadd   Vd,Vn,Vm             DV_3A  0Q001110XX1mmmmm 000011nnnnnddddd   0E20 0C00   Vd,Vn,Vm   (vector)

INST2(sqrshl,      "sqrshl",       0,      IF_EN2O,   0x5E205C00,  0x0E205C00)
                                   //  sqrshl  Vd,Vn,Vm             DV_3E  01011110XX1mmmmm 010111nnnnnddddd   5E20 5C00   Vd,Vn,Vm   (scalar)
                                   //  sqrshl  Vd,Vn,Vm             DV_3A  0Q001110XX1mmmmm 010111nnnnnddddd   0E20 5C00   Vd,Vn,Vm   (vector)

INST2(sqsub,       "sqsub",        0,      IF_EN2O,   0x5E202C00,  0x0E202C00)
                                   //  sqsub   Vd,Vn,Vm             DV_3E  01011110XX1mmmmm 001011nnnnnddddd   5E20 2C00   Vd,Vn,Vm   (scalar)
                                   //  sqsub   Vd,Vn,Vm             DV_3A  0Q001110XX1mmmmm 001011nnnnnddddd   0E20 2C00   Vd,Vn,Vm   (vector)

INST2(srshl,       "srshl",        0,      IF_EN2O,   0x5E205400,  0x0E205400)
                                   //  srshl    Vd,Vn,Vm            DV_3E  01011110XX1mmmmm 010101nnnnnddddd   5E20 5400   Vd,Vn,Vm   (scalar)
                                   //  srshl    Vd,Vn,Vm            DV_3A  0Q001110XX1mmmmm 010101nnnnnddddd   0E20 5400   Vd,Vn,Vm   (vector)

INST2(sshl,        "sshl",         0,      IF_EN2O,   0x5E204400,  0x0E204400)
                                   //  sshl    Vd,Vn,Vm             DV_3E  01011110XX1mmmmm 010001nnnnnddddd   5E20 4400   Vd,Vn,Vm   (scalar)
                                   //  sshl    Vd,Vn,Vm             DV_3A  0Q001110XX1mmmmm 010001nnnnnddddd   0E20 4400   Vd,Vn,Vm   (vector)

INST2(uqadd,       "uqadd",        0,      IF_EN2O,   0x7E200C00,  0x2E200C00)
                                   //  uqadd   Vd,Vn,Vm             DV_3E  01111110XX1mmmmm 000011nnnnnddddd   7E20 0C00   Vd,Vn,Vm   (scalar)
                                   //  uqadd   Vd,Vn,Vm             DV_3A  0Q101110XX1mmmmm 000011nnnnnddddd   2E20 0C00   Vd,Vn,Vm   (vector)

INST2(uqrshl,      "uqrshl",       0,      IF_EN2O,   0x7E205C00,  0x2E205C00)
                                   //  uqrshl  Vd,Vn,Vm             DV_3E  01111110XX1mmmmm 010111nnnnnddddd   7E20 5C00   Vd,Vn,Vm   (scalar)
                                   //  uqrshl  Vd,Vn,Vm             DV_3A  0Q101110XX1mmmmm 010111nnnnnddddd   2E20 5C00   Vd,Vn,Vm   (vector)

INST2(uqsub,       "uqsub",        0,      IF_EN2O,   0x7E202C00,  0x2E202C00)
                                   //  uqsub   Vd,Vn,Vm             DV_3E  01111110XX1mmmmm 001011nnnnnddddd   7E20 2C00   Vd,Vn,Vm   (scalar)
                                   //  uqsub   Vd,Vn,Vm             DV_3A  0Q101110XX1mmmmm 001011nnnnnddddd   2E20 2C00   Vd,Vn,Vm   (vector)

INST2(urshl,       "urshl",        0,      IF_EN2O,   0x7E205400,  0x2E205400)
                                   //  urshl    Vd,Vn,Vm            DV_3E  01111110XX1mmmmm 010101nnnnnddddd   7E20 5400   Vd,Vn,Vm   (scalar)
                                   //  urshl    Vd,Vn,Vm            DV_3A  0Q101110XX1mmmmm 010101nnnnnddddd   2E20 5400   Vd,Vn,Vm   (vector)

INST2(ushl,        "ushl",         0,      IF_EN2O,   0x7E204400,  0x2E204400)
                                   //  ushl    Vd,Vn,Vm             DV_3E  01111110XX1mmmmm 010001nnnnnddddd   7E20 4400   Vd,Vn,Vm   (scalar)
                                   //  ushl    Vd,Vn,Vm             DV_3A  0Q101110XX1mmmmm 010001nnnnnddddd   2E20 4400   Vd,Vn,Vm   (vector)

//    enum         name            info               DV_2Q        DV_3B
INST2(faddp,       "faddp",        0,      IF_EN2P,   0x7E30D800,  0x2E20D400)
                                   //  faddp   Vd,Vn                DV_2Q  011111100X110000 110110nnnnnddddd   7E30 D800   Vd,Vn      (scalar)
                                   //  faddp   Vd,Vn,Vm             DV_3B  0Q1011100X1mmmmm 110101nnnnnddddd   2E20 D400   Vd,Vn,Vm   (vector)

INST2(fmaxnmp,     "fmaxnmp",      0,      IF_EN2P,   0x7E30C800,  0x2E20C400)
                                   //  fmaxnmp Vd,Vn                DV_2Q  011111100X110000 110010nnnnnddddd   7E30 C800   Vd,Vn      (scalar)
                                   //  fmaxnmp Vd,Vn,Vm             DV_3B  0Q1011100X1mmmmm 110001nnnnnddddd   2E20 C400   Vd,Vn,Vm   (vector)

INST2(fmaxp,       "fmaxp",        0,      IF_EN2P,   0x7E30F800,  0x2E20F400)
                                   //  fmaxp Vd,Vn                  DV_2Q  011111100X110000 111110nnnnnddddd   7E30 F800   Vd,Vn      (scalar)
                                   //  fmaxp Vd,Vn,Vm               DV_3B  0Q1011100X1mmmmm 111101nnnnnddddd   2E20 F400   Vd,Vn,Vm   (vector)

INST2(fminnmp,     "fminnmp",      0,      IF_EN2P,   0x7EB0C800,  0x2EA0C400)
                                   //  fminnmp Vd,Vn                DV_2Q  011111101X110000 110010nnnnnddddd   7EB0 C800   Vd,Vn      (scalar)
                                   //  fminnmp Vd,Vn,Vm             DV_3B  0Q1011101X1mmmmm 110001nnnnnddddd   2EA0 C400   Vd,Vn,Vm   (vector)

INST2(fminp,       "fminp",        0,      IF_EN2P,   0x7EB0F800,  0x2EA0F400)
                                   //  fminp Vd,Vn                  DV_2Q  011111101X110000 111110nnnnnddddd   7EB0 F800   Vd,Vn      (scalar)
                                   //  fminp Vd,Vn,Vm               DV_3B  0Q1011101X1mmmmm 111101nnnnnddddd   2EA0 F400   Vd,Vn,Vm   (vector)

INST2(addp,        "addp",         0,      IF_EN2Q,   0x5E31B800,  0x0E20BC00)
                                   //  addp Vd,Vn                   DV_2S  01011110XX110001 101110nnnnnddddd   5E31 B800   Vd,Vn      (scalar)
                                   //  addp Vd,Vn,Vm                DV_3A  0Q001110XX1mmmmm 101111nnnnnddddd   0E20 BC00   Vd,Vn,Vm   (vector)

INST1(ldar,        "ldar",         LD,     IF_LS_2A,  0x88DFFC00)
                                   //  ldar    Rt,[Xn]              LS_2A  1X00100011011111 111111nnnnnttttt   88DF FC00

INST1(ldarb,       "ldarb",        LD,     IF_LS_2A,  0x08DFFC00)
                                   //  ldarb   Rt,[Xn]              LS_2A  0000100011011111 111111nnnnnttttt   08DF FC00

INST1(ldarh,       "ldarh",        LD,     IF_LS_2A,  0x48DFFC00)
                                   //  ldarh   Rt,[Xn]              LS_2A  0100100011011111 111111nnnnnttttt   48DF FC00

INST1(ldapr,       "ldapr",       LD,     IF_LS_2A,  0xB8BFC000)
                                   //  ldapr   Rt,[Xn]              LS_2A  1X11100010111111 110000nnnnnttttt   B8BF C000   Rm Rt Rn ARMv8.3 LRCPC

INST1(ldaprb,      "ldaprb",      LD,     IF_LS_2A,  0x38BFC000)
                                   //  ldaprb  Rt,[Xn]              LS_2A  0011100010111111 110000nnnnnttttt   38BF C000   Rm Rt Rn ARMv8.3 LRCPC

INST1(ldaprh,      "ldaprh",      LD,     IF_LS_2A,  0x78BFC000)
                                   //  ldaprh  Rt,[Xn]              LS_2A  0111100010111111 110000nnnnnttttt   78BF C000   Rm Rt Rn ARMv8.3 LRCPC

INST1(ldxr,        "ldxr",         LD,     IF_LS_2A,  0x885F7C00)
                                   //  ldxr    Rt,[Xn]              LS_2A  1X00100001011111 011111nnnnnttttt   885F 7C00

INST1(ldxrb,       "ldxrb",        LD,     IF_LS_2A,  0x085F7C00)
                                   //  ldxrb   Rt,[Xn]              LS_2A  0000100001011111 011111nnnnnttttt   085F 7C00

INST1(ldxrh,       "ldxrh",        LD,     IF_LS_2A,  0x485F7C00)
                                   //  ldxrh   Rt,[Xn]              LS_2A  0100100001011111 011111nnnnnttttt   485F 7C00

INST1(ldaxr,       "ldaxr",        LD,     IF_LS_2A,  0x885FFC00)
                                   //  ldaxr   Rt,[Xn]              LS_2A  1X00100001011111 111111nnnnnttttt   885F FC00

INST1(ldaxrb,      "ldaxrb",       LD,     IF_LS_2A,  0x085FFC00)
                                   //  ldaxrb  Rt,[Xn]              LS_2A  0000100001011111 111111nnnnnttttt   085F FC00

INST1(ldaxrh,      "ldaxrh",       LD,     IF_LS_2A,  0x485FFC00)
                                   //  ldaxrh  Rt,[Xn]              LS_2A  0100100001011111 111111nnnnnttttt   485F FC00

INST1(ldur,        "ldur",         LD,     IF_LS_2C,  0xB8400000)
                                   //  ldur    Rt,[Xn+simm9]        LS_2C  1X111000010iiiii iiii00nnnnnttttt   B840 0000   [Xn imm(-256..+255)]

INST1(ldurb,       "ldurb",        LD,     IF_LS_2C,  0x38400000)
                                   //  ldurb   Rt,[Xn+simm9]        LS_2C  00111000010iiiii iiii00nnnnnttttt   3840 0000   [Xn imm(-256..+255)]

INST1(ldurh,       "ldurh",        LD,     IF_LS_2C,  0x78400000)
                                   //  ldurh   Rt,[Xn+simm9]        LS_2C  01111000010iiiii iiii00nnnnnttttt   7840 0000   [Xn imm(-256..+255)]

INST1(ldursb,      "ldursb",       LD,     IF_LS_2C,  0x38800000)
                                   //  ldursb  Rt,[Xn+simm9]        LS_2C  001110001X0iiiii iiii00nnnnnttttt   3880 0000   [Xn imm(-256..+255)]

INST1(ldursh,      "ldursh",       LD,     IF_LS_2C,  0x78800000)
                                   //  ldursh  Rt,[Xn+simm9]        LS_2C  011110001X0iiiii iiii00nnnnnttttt   7880 0000   [Xn imm(-256..+255)]

INST1(ldursw,      "ldursw",       LD,     IF_LS_2C,  0xB8800000)
                                   //  ldursw  Rt,[Xn+simm9]        LS_2C  10111000100iiiii iiii00nnnnnttttt   B880 0000   [Xn imm(-256..+255)]

INST1(ldapur,      "ldapur",       LD,     IF_LS_2C,  0x99400000)
                                   //  ldapur  Rt,[Xn+simm9]        LS_2C  1X011001010iiiii iiii00nnnnnttttt   9940 0000   [Xn imm(-256..+255)] ARMv8.4 RCPC2

INST1(ldapurb,     "ldapurb",      LD,     IF_LS_2C,  0x19400000)
                                   //  ldapurb Rt,[Xn+simm9]        LS_2C  00011001010iiiii iiii00nnnnnttttt   1940 0000   [Xn imm(-256..+255)] ARMv8.4 RCPC2

INST1(ldapurh,     "ldapurh",      LD,     IF_LS_2C,  0x59400000)
                                   //  ldapurh Rt,[Xn+simm9]        LS_2C  01011001010iiiii iiii00nnnnnttttt   5940 0000   [Xn imm(-256..+255)] ARMv8.4 RCPC2

INST1(stlr,        "stlr",         ST,     IF_LS_2A,  0x889FFC00)
                                   //  stlr    Rt,[Xn]              LS_2A  1X00100010011111 111111nnnnnttttt   889F FC00

INST1(stlrb,       "stlrb",        ST,     IF_LS_2A,  0x089FFC00)
                                   //  stlrb   Rt,[Xn]              LS_2A  0000100010011111 111111nnnnnttttt   089F FC00

INST1(stlrh,       "stlrh",        ST,     IF_LS_2A,  0x489FFC00)
                                   //  stlrh   Rt,[Xn]              LS_2A  0100100010011111 111111nnnnnttttt   489F FC00

INST1(stxr,        "stxr",         ST,     IF_LS_3D,  0x88007C00)
                                   //  stxr    Ws, Rt,[Xn]          LS_3D  1X001000000sssss 011111nnnnnttttt   8800 7C00

INST1(stxrb,       "stxrb",        ST,     IF_LS_3D,  0x08007C00)
                                   //  stxrb   Ws, Rt,[Xn]          LS_3D  00001000000sssss 011111nnnnnttttt   0800 7C00

INST1(stxrh,       "stxrh",        ST,     IF_LS_3D,  0x48007C00)
                                   //  stxrh   Ws, Rt,[Xn]          LS_3D  01001000000sssss 011111nnnnnttttt   4800 7C00

INST1(stlxr,       "stlxr",        ST,     IF_LS_3D,  0x8800FC00)
                                   //  stlxr   Ws, Rt,[Xn]          LS_3D  1X001000000sssss 111111nnnnnttttt   8800 FC00

INST1(stlxrb,      "stlxrb",       ST,     IF_LS_3D,  0x0800FC00)
                                   //  stlxrb  Ws, Rt,[Xn]          LS_3D  00001000000sssss 111111nnnnnttttt   0800 FC00

INST1(stlxrh,      "stlxrh",       ST,     IF_LS_3D,  0x4800FC00)
                                   //  stlxrh  Ws, Rt,[Xn]          LS_3D  01001000000sssss 111111nnnnnttttt   4800 FC00

INST1(stur,        "stur",         ST,     IF_LS_2C,  0xB8000000)
                                   //  stur    Rt,[Xn+simm9]        LS_2C  1X111000000iiiii iiii00nnnnnttttt   B800 0000   [Xn imm(-256..+255)]

INST1(sturb,       "sturb",        ST,     IF_LS_2C,  0x38000000)
                                   //  sturb   Rt,[Xn+simm9]        LS_2C  00111000000iiiii iiii00nnnnnttttt   3800 0000   [Xn imm(-256..+255)]

INST1(sturh,       "sturh",        ST,     IF_LS_2C,  0x78000000)
                                   //  sturh   Rt,[Xn+simm9]        LS_2C  01111000000iiiii iiii00nnnnnttttt   7800 0000   [Xn imm(-256..+255)]

INST1(stlur,       "stlur",        ST,     IF_LS_2C,  0x99000000)
                                   //  stlur    Rt,[Xn+simm9]        LS_2C  1X011001000iiiii iiii00nnnnnttttt  9900 0000   [Xn imm(-256..+255)] ARMv8.4 RCPC2

INST1(stlurb,      "stlurb",       ST,     IF_LS_2C,  0x19000000)
                                   //  stlurb   Rt,[Xn+simm9]        LS_2C  00011001000iiiii iiii00nnnnnttttt  1900 0000   [Xn imm(-256..+255)] ARMv8.4 RCPC2

INST1(stlurh,      "stlurh",       ST,     IF_LS_2C,  0x59000000)
                                   //  stlurh   Rt,[Xn+simm9]        LS_2C  01011001000iiiii iiii00nnnnnttttt  5900 0000   [Xn imm(-256..+255)] ARMv8.4 RCPC2

INST1(casb,        "casb",         LD|ST,  IF_LS_3E,  0x08A07C00)
                                   //  casb    Wm, Wt, [Xn]         LS_3E  00001000101mmmmm 011111nnnnnttttt   08A0 7C00   Rm Rt Rn ARMv8.1 LSE Atomics

INST1(casab,       "casab",        LD|ST,  IF_LS_3E,  0x08E07C00)
                                   //  casab   Wm, Wt, [Xn]         LS_3E  00001000111mmmmm 011111nnnnnttttt   08E0 7C00   Rm Rt Rn ARMv8.1 LSE Atomics

INST1(casalb,      "casalb",       LD|ST,  IF_LS_3E,  0x08E0FC00)
                                   //  casalb  Wm, Wt, [Xn]         LS_3E  00001000111mmmmm 111111nnnnnttttt   08E0 FC00   Rm Rt Rn ARMv8.1 LSE Atomics

INST1(caslb,       "caslb",        LD|ST,  IF_LS_3E,  0x08A0FC00)
                                   //  caslb   Wm, Wt, [Xn]         LS_3E  00001000101mmmmm 111111nnnnnttttt   08A0 FC00   Rm Rt Rn ARMv8.1 LSE Atomics

INST1(cash,        "cash",         LD|ST,  IF_LS_3E,  0x48A07C00)
                                   //  cash    Wm, Wt, [Xn]         LS_3E  01001000101mmmmm 011111nnnnnttttt   48A0 7C00   Rm Rt Rn ARMv8.1 LSE Atomics

INST1(casah,       "casah",        LD|ST,  IF_LS_3E,  0x48E07C00)
                                   //  casah   Wm, Wt, [Xn]         LS_3E  01001000111mmmmm 011111nnnnnttttt   48E0 7C00   Rm Rt Rn ARMv8.1 LSE Atomics

INST1(casalh,      "casalh",       LD|ST,  IF_LS_3E,  0x48E0FC00)
                                   //  casalh  Wm, Wt, [Xn]         LS_3E  01001000111mmmmm 111111nnnnnttttt   48E0 FC00   Rm Rt Rn ARMv8.1 LSE Atomics

INST1(caslh,       "caslh",        LD|ST,  IF_LS_3E,  0x48A0FC00)
                                   //  caslh   Wm, Wt, [Xn]         LS_3E  01001000101mmmmm 111111nnnnnttttt   48A0 FC00   Rm Rt Rn ARMv8.1 LSE Atomics

INST1(cas,         "cas",          LD|ST,  IF_LS_3E,  0x88A07C00)
                                   //  cas     Rm, Rt, [Xn]         LS_3E  1X001000101mmmmm 011111nnnnnttttt   88A0 7C00   Rm Rt Rn ARMv8.1 LSE Atomics

INST1(casa,        "casa",         LD|ST,  IF_LS_3E,  0x88E07C00)
                                   //  casa    Rm, Rt, [Xn]         LS_3E  1X001000111mmmmm 011111nnnnnttttt   88E0 7C00   Rm Rt Rn ARMv8.1 LSE Atomics

INST1(casal,       "casal",        LD|ST,  IF_LS_3E,  0x88E0FC00)
                                   //  casal   Rm, Rt, [Xn]         LS_3E  1X001000111mmmmm 111111nnnnnttttt   88E0 FC00   Rm Rt Rn ARMv8.1 LSE Atomics

INST1(casl,        "casl",         LD|ST,  IF_LS_3E,  0x88A0FC00)
                                   //  casl    Rm, Rt, [Xn]         LS_3E  1X001000101mmmmm 111111nnnnnttttt   88A0 FC00   Rm Rt Rn ARMv8.1 LSE Atomics

INST1(ldaddb,      "ldaddb",   WR2|LD|ST,  IF_LS_3E,  0x38200000)
                                   //  ldaddb   Wm, Wt, [Xn]        LS_3E  00111000001mmmmm 000000nnnnnttttt   3820 0000   Rm Rt Rn ARMv8.1 LSE Atomics

INST1(ldaddab,     "ldaddab",  WR2|LD|ST,  IF_LS_3E,  0x38A00000)
                                   //  ldaddab  Wm, Wt, [Xn]        LS_3E  00111000101mmmmm 000000nnnnnttttt   38A0 0000   Rm Rt Rn ARMv8.1 LSE Atomics

INST1(ldaddalb,    "ldaddalb", WR2|LD|ST,  IF_LS_3E,  0x38E00000)
                                   //  ldaddalb Wm, Wt, [Xn]        LS_3E  00111000111mmmmm 000000nnnnnttttt   38E0 0000   Rm Rt Rn ARMv8.1 LSE Atomics

INST1(ldaddlb,     "ldaddlb",  WR2|LD|ST,  IF_LS_3E,  0x38600000)
                                   //  ldaddlb  Wm, Wt, [Xn]        LS_3E  00111000011mmmmm 000000nnnnnttttt   3860 0000   Rm Rt Rn ARMv8.1 LSE Atomics

INST1(ldaddh,      "ldaddh",   WR2|LD|ST,  IF_LS_3E,  0x78200000)
                                   //  ldaddh   Wm, Wt, [Xn]        LS_3E  01111000001mmmmm 000000nnnnnttttt   7820 0000   Rm Rt Rn ARMv8.1 LSE Atomics

INST1(ldaddah,     "ldaddah",  WR2|LD|ST,  IF_LS_3E,  0x78A00000)
                                   //  ldaddah  Wm, Wt, [Xn]        LS_3E  01111000101mmmmm 000000nnnnnttttt   78A0 0000   Rm Rt Rn ARMv8.1 LSE Atomics

INST1(ldaddalh,    "ldaddalh", WR2|LD|ST,  IF_LS_3E,  0x78E00000)
                                   //  ldaddalh Wm, Wt, [Xn]        LS_3E  01111000111mmmmm 000000nnnnnttttt   78E0 0000   Rm Rt Rn ARMv8.1 LSE Atomics

INST1(ldaddlh,     "ldaddlh",  WR2|LD|ST,  IF_LS_3E,  0x78600000)
                                   //  ldaddlh  Wm, Wt, [Xn]        LS_3E  01111000011mmmmm 000000nnnnnttttt   7860 0000   Rm Rt Rn ARMv8.1 LSE Atomics

INST1(ldadd,       "ldadd",    WR2|LD|ST,  IF_LS_3E,  0xB8200000)
                                   //  ldadd    Rm, Rt, [Xn]        LS_3E  1X111000001mmmmm 000000nnnnnttttt   B820 0000   Rm Rt Rn ARMv8.1 LSE Atomics

INST1(ldadda,      "ldadda",   WR2|LD|ST,  IF_LS_3E,  0xB8A00000)
                                   //  ldadda   Rm, Rt, [Xn]        LS_3E  1X111000101mmmmm 000000nnnnnttttt   B8A0 0000   Rm Rt Rn ARMv8.1 LSE Atomics

INST1(ldaddal,     "ldaddal",  WR2|LD|ST,  IF_LS_3E,  0xB8E00000)
                                   //  ldaddal  Rm, Rt, [Xn]        LS_3E  1X111000111mmmmm 000000nnnnnttttt   B8E0 0000   Rm Rt Rn ARMv8.1 LSE Atomics

INST1(ldclral,     "ldclral",  WR2|LD|ST,  IF_LS_3E,  0xB8E01000)
                                   //  ldclral  Rm, Rt, [Xn]        LS_3E  1X111000111mmmmm 000100nnnnnttttt   B8E0 1000   Rm Rt Rn ARMv8.1 LSE Atomics

INST1(ldsetal,     "ldsetal",  WR2|LD|ST,  IF_LS_3E,  0xB8E03000)
                                   //  ldsetal  Rm, Rt, [Xn]        LS_3E  1X111000111mmmmm 001100nnnnnttttt   B8E0 3000   Rm Rt Rn ARMv8.1 LSE Atomics

INST1(ldaddl,      "ldaddl",   WR2|LD|ST,  IF_LS_3E,  0xB8600000)
                                   //  ldaddl   Rm, Rt, [Xn]        LS_3E  1X111000011mmmmm 000000nnnnnttttt   B860 0000   Rm Rt Rn ARMv8.1 LSE Atomics

INST1(staddb,      "staddb",   WR2|ST,     IF_LS_3E,  0x3820001F)
                                   //  staddb   Wm, [Xn]            LS_3E  00111000001mmmmm 000000nnnnn11111   3820 001F   Rm Rt Rn ARMv8.1 LSE Atomics

INST1(staddlb,     "staddlb",  WR2|ST,     IF_LS_3E,  0x3860001F)
                                   //  staddlb  Wm, [Xn]            LS_3E  00111000011mmmmm 000000nnnnn11111   3860 001F   Rm Rt Rn ARMv8.1 LSE Atomics

INST1(staddh,      "staddh",   WR2|ST,     IF_LS_3E,  0x7820001F)
                                   //  staddh   Wm, [Xn]            LS_3E  01111000001mmmmm 000000nnnnn11111   7820 001F   Rm Rt Rn ARMv8.1 LSE Atomics

INST1(staddlh,     "staddlh",  WR2|ST,     IF_LS_3E,  0x7860001F)
                                   //  staddlh  Wm, [Xn]            LS_3E  01111000011mmmmm 000000nnnnn11111   7860 001F   Rm Rt Rn ARMv8.1 LSE Atomics

INST1(stadd,       "stadd",    WR2|ST,     IF_LS_3E,  0xB820001F)
                                   //  stadd    Rm, [Xn]            LS_3E  1X111000001mmmmm 000000nnnnn11111   B820 001F   Rm Rt Rn ARMv8.1 LSE Atomics

INST1(staddl,      "staddl",   WR2|ST,     IF_LS_3E,  0xB860001F)
                                   //  staddl   Rm, [Xn]            LS_3E  1X111000011mmmmm 000000nnnnn11111   B860 001F   Rm Rt Rn ARMv8.1 LSE Atomics

INST1(swpb,        "swpb",     WR2|LD|ST,  IF_LS_3E,  0x38208000)
                                   //  swpb    Wm, Wt, [Xn]         LS_3E  00111000001mmmmm 100000nnnnnttttt   3820 8000   Rm Rt Rn ARMv8.1 LSE Atomics

INST1(swpab,       "swpab",    WR2|LD|ST,  IF_LS_3E,  0x38A08000)
                                   //  swpab   Wm, Wt, [Xn]         LS_3E  00111000101mmmmm 100000nnnnnttttt   38A0 8000   Rm Rt Rn ARMv8.1 LSE Atomics

INST1(swpalb,      "swpalb",   WR2|LD|ST,  IF_LS_3E,  0x38E08000)
                                   //  swpalb  Wm, Wt, [Xn]         LS_3E  00111000111mmmmm 100000nnnnnttttt   38E0 8000   Rm Rt Rn ARMv8.1 LSE Atomics

INST1(swplb,       "swplb",    WR2|LD|ST,  IF_LS_3E,  0x38608000)
                                   //  swplb   Wm, Wt, [Xn]         LS_3E  00111000011mmmmm 100000nnnnnttttt   3860 8000   Rm Rt Rn ARMv8.1 LSE Atomics

INST1(swph,        "swph",     WR2|LD|ST,  IF_LS_3E,  0x78208000)
                                   //  swph    Wm, Wt, [Xn]         LS_3E  01111000001mmmmm 100000nnnnnttttt   7820 8000   Rm Rt Rn ARMv8.1 LSE Atomics

INST1(swpah,       "swpah",    WR2|LD|ST,  IF_LS_3E,  0x78A08000)
                                   //  swpah   Wm, Wt, [Xn]         LS_3E  01111000101mmmmm 100000nnnnnttttt   78A0 8000   Rm Rt Rn ARMv8.1 LSE Atomics

INST1(swpalh,      "swpalh",   WR2|LD|ST,  IF_LS_3E,  0x78E08000)
                                   //  swpalh  Wm, Wt, [Xn]         LS_3E  01111000111mmmmm 100000nnnnnttttt   78E0 8000   Rm Rt Rn ARMv8.1 LSE Atomics

INST1(swplh,       "swplh",    WR2|LD|ST,  IF_LS_3E,  0x78608000)
                                   //  swplh   Wm, Wt, [Xn]         LS_3E  01111000011mmmmm 100000nnnnnttttt   7860 8000   Rm Rt Rn ARMv8.1 LSE Atomics

INST1(swp,         "swp",      WR2|LD|ST,  IF_LS_3E,  0xB8208000)
                                   //  swp     Rm, Rt, [Xn]         LS_3E  1X111000001mmmmm 100000nnnnnttttt   B820 8000   Rm Rt Rn ARMv8.1 LSE Atomics

INST1(swpa,        "swpa",     WR2|LD|ST,  IF_LS_3E,  0xB8A08000)
                                   //  swpa    Rm, Rt, [Xn]         LS_3E  1X111000101mmmmm 100000nnnnnttttt   B8A0 8000   Rm Rt Rn ARMv8.1 LSE Atomics

INST1(swpal,       "swpal",    WR2|LD|ST,  IF_LS_3E,  0xB8E08000)
                                   //  swpal   Rm, Rt, [Xn]         LS_3E  1X111000111mmmmm 100000nnnnnttttt   B8E0 8000   Rm Rt Rn ARMv8.1 LSE Atomics

INST1(swpl,        "swpl",     WR2|LD|ST,  IF_LS_3E,  0xB8608000)
                                   //  swpl    Rm, Rt, [Xn]         LS_3E  1X111000011mmmmm 100000nnnnnttttt   B860 8000   Rm Rt Rn ARMv8.1 LSE Atomics

INST1(adr,         "adr",          0,      IF_DI_1E,  0x10000000)
                                   //  adr     Rd, simm21           DI_1E  0ii10000iiiiiiii iiiiiiiiiiiddddd   1000 0000   Rd simm21

INST1(adrp,        "adrp",         0,      IF_DI_1E,  0x90000000)
                                   //  adrp    Rd, simm21           DI_1E  1ii10000iiiiiiii iiiiiiiiiiiddddd   9000 0000   Rd simm21

INST1(b,           "b",            0,      IF_BI_0A,  0x14000000)
                                   //  b       simm26               BI_0A  000101iiiiiiiiii iiiiiiiiiiiiiiii   1400 0000   simm26:00

INST1(b_tail,      "b",            0,      IF_BI_0C,  0x14000000)
                                   //  b       simm26               BI_0A  000101iiiiiiiiii iiiiiiiiiiiiiiii   1400 0000   simm26:00, same as b representing a tail call of bl.

INST1(bl_local,    "bl",           0,      IF_BI_0A,  0x94000000)
                                   //  bl      simm26               BI_0A  100101iiiiiiiiii iiiiiiiiiiiiiiii   9400 0000   simm26:00, same as bl, but with a BasicBlock target.

INST1(bl,          "bl",           0,      IF_BI_0C,  0x94000000)
                                   //  bl      simm26               BI_0C  100101iiiiiiiiii iiiiiiiiiiiiiiii   9400 0000   simm26:00

INST1(br,          "br",           0,      IF_BR_1A,  0xD61F0000)
                                   //  br      Rn                   BR_1A  1101011000011111 000000nnnnn00000   D61F 0000, an indirect branch like switch expansion

INST1(br_tail,     "br",           0,      IF_BR_1B,  0xD61F0000)
                                   //  br      Rn                   BR_1B  1101011000011111 000000nnnnn00000   D61F 0000, same as br representing a tail call of blr. Encode target with Reg3.

INST1(blr,         "blr",          0,      IF_BR_1B,  0xD63F0000)
                                   //  blr     Rn                   BR_1B  1101011000111111 000000nnnnn00000   D63F 0000, Encode target with Reg3.

INST1(ret,         "ret",          0,      IF_BR_1A,  0xD65F0000)
                                   //  ret     Rn                   BR_1A  1101011001011111 000000nnnnn00000   D65F 0000

INST1(beq,         "beq",          0,      IF_BI_0B,  0x54000000)
                                   //  beq     simm19               BI_0B  01010100iiiiiiii iiiiiiiiiii00000   5400 0000   simm19:00

INST1(bne,         "bne",          0,      IF_BI_0B,  0x54000001)
                                   //  bne     simm19               BI_0B  01010100iiiiiiii iiiiiiiiiii00001   5400 0001   simm19:00

INST1(bhs,         "bhs",          0,      IF_BI_0B,  0x54000002)
                                   //  bhs     simm19               BI_0B  01010100iiiiiiii iiiiiiiiiii00010   5400 0002   simm19:00

INST1(blo,         "blo",          0,      IF_BI_0B,  0x54000003)
                                   //  blo     simm19               BI_0B  01010100iiiiiiii iiiiiiiiiii00011   5400 0003   simm19:00

INST1(bmi,         "bmi",          0,      IF_BI_0B,  0x54000004)
                                   //  bmi     simm19               BI_0B  01010100iiiiiiii iiiiiiiiiii00100   5400 0004   simm19:00

INST1(bpl,         "bpl",          0,      IF_BI_0B,  0x54000005)
                                   //  bpl     simm19               BI_0B  01010100iiiiiiii iiiiiiiiiii00101   5400 0005   simm19:00

INST1(bvs,         "bvs",          0,      IF_BI_0B,  0x54000006)
                                   //  bvs     simm19               BI_0B  01010100iiiiiiii iiiiiiiiiii00110   5400 0006   simm19:00

INST1(bvc,         "bvc",          0,      IF_BI_0B,  0x54000007)
                                   //  bvc     simm19               BI_0B  01010100iiiiiiii iiiiiiiiiii00111   5400 0007   simm19:00

INST1(bhi,         "bhi",          0,      IF_BI_0B,  0x54000008)
                                   //  bhi     simm19               BI_0B  01010100iiiiiiii iiiiiiiiiii01000   5400 0008   simm19:00

INST1(bls,         "bls",          0,      IF_BI_0B,  0x54000009)
                                   //  bls     simm19               BI_0B  01010100iiiiiiii iiiiiiiiiii01001   5400 0009   simm19:00

INST1(bge,         "bge",          0,      IF_BI_0B,  0x5400000A)
                                   //  bge     simm19               BI_0B  01010100iiiiiiii iiiiiiiiiii01010   5400 000A   simm19:00

INST1(blt,         "blt",          0,      IF_BI_0B,  0x5400000B)
                                   //  blt     simm19               BI_0B  01010100iiiiiiii iiiiiiiiiii01011   5400 000B   simm19:00

INST1(bgt,         "bgt",          0,      IF_BI_0B,  0x5400000C)
                                   //  bgt     simm19               BI_0B  01010100iiiiiiii iiiiiiiiiii01100   5400 000C   simm19:00

INST1(ble,         "ble",          0,      IF_BI_0B,  0x5400000D)
                                   //  ble     simm19               BI_0B  01010100iiiiiiii iiiiiiiiiii01101   5400 000D   simm19:00

INST1(cbz,         "cbz",          0,      IF_BI_1A,  0x34000000)
                                   //  cbz     Rt, simm19           BI_1A  X0110100iiiiiiii iiiiiiiiiiittttt   3400 0000   Rt simm19:00

INST1(cbnz,        "cbnz",         0,      IF_BI_1A,  0x35000000)
                                   //  cbnz    Rt, simm19           BI_1A  X0110101iiiiiiii iiiiiiiiiiittttt   3500 0000   Rt simm19:00

INST1(tbz,         "tbz",          0,      IF_BI_1B,  0x36000000)
                                   //  tbz     Rt, imm6, simm14     BI_1B  B0110110bbbbbiii iiiiiiiiiiittttt   3600 0000   Rt imm6, simm14:00

INST1(tbnz,        "tbnz",         0,      IF_BI_1B,  0x37000000)
                                   //  tbnz    Rt, imm6, simm14     BI_1B  B0110111bbbbbiii iiiiiiiiiiittttt   3700 0000   Rt imm6, simm14:00

INST1(movk,        "movk",         0,      IF_DI_1B,  0x72800000)
                                   //  movk    Rd,imm(i16,hw)       DI_1B  X11100101hwiiiii iiiiiiiiiiiddddd   7280 0000   imm(i16,hw)

INST1(movn,        "movn",         0,      IF_DI_1B,  0x12800000)
                                   //  movn    Rd,imm(i16,hw)       DI_1B  X00100101hwiiiii iiiiiiiiiiiddddd   1280 0000   imm(i16,hw)

INST1(movz,        "movz",         0,      IF_DI_1B,  0x52800000)
                                   //  movz    Rd,imm(i16,hw)       DI_1B  X10100101hwiiiii iiiiiiiiiiiddddd   5280 0000   imm(i16,hw)

INST1(csel,        "csel",         0,      IF_DR_3D,  0x1A800000)
                                   //  csel    Rd,Rn,Rm,cond        DR_3D  X0011010100mmmmm cccc00nnnnnddddd   1A80 0000   cond

INST1(csinc,       "csinc",        0,      IF_DR_3D,  0x1A800400)
                                   //  csinc   Rd,Rn,Rm,cond        DR_3D  X0011010100mmmmm cccc01nnnnnddddd   1A80 0400   cond

INST1(csinv,       "csinv",        0,      IF_DR_3D,  0x5A800000)
                                   //  csinv   Rd,Rn,Rm,cond        DR_3D  X1011010100mmmmm cccc00nnnnnddddd   5A80 0000   cond

INST1(csneg,       "csneg",        0,      IF_DR_3D,  0x5A800400)
                                   //  csneg   Rd,Rn,Rm,cond        DR_3D  X1011010100mmmmm cccc01nnnnnddddd   5A80 0400   cond

INST1(cinc,        "cinc",         0,      IF_DR_2D,  0x1A800400)
                                   //  cinc    Rd,Rn,cond           DR_2D  X0011010100nnnnn cccc01nnnnnddddd   1A80 0400   cond

INST1(cinv,        "cinv",         0,      IF_DR_2D,  0x5A800000)
                                   //  cinv    Rd,Rn,cond           DR_2D  X1011010100nnnnn cccc00nnnnnddddd   5A80 0000   cond

INST1(cneg,        "cneg",         0,      IF_DR_2D,  0x5A800400)
                                   //  cneg    Rd,Rn,cond           DR_2D  X1011010100nnnnn cccc01nnnnnddddd   5A80 0400   cond

INST1(cset,        "cset",         0,      IF_DR_1D,  0x1A9F07E0)
                                   //  cset    Rd,cond              DR_1D  X001101010011111 cccc0111111ddddd   1A9F 07E0   Rd cond

INST1(csetm,       "csetm",        0,      IF_DR_1D,  0x5A9F03E0)
                                   //  csetm   Rd,cond              DR_1D  X101101010011111 cccc0011111ddddd   5A9F 03E0   Rd cond

INST1(aese,        "aese",         0,      IF_DV_2P,  0x4E284800)
                                   //  aese   Vd.16B,Vn.16B         DV_2P  0100111000101000 010010nnnnnddddd   4E28 4800   Vd.16B Vn.16B  (vector)

INST1(aesd,        "aesd",         0,      IF_DV_2P,  0x4E285800)
                                   //  aesd   Vd.16B,Vn.16B         DV_2P  0100111000101000 010110nnnnnddddd   4E28 5800   Vd.16B Vn.16B  (vector)

INST1(aesmc,       "aesmc",        0,      IF_DV_2P,  0x4E286800)
                                   //  aesmc  Vd.16B,Vn.16B         DV_2P  0100111000101000 011010nnnnnddddd   4E28 6800   Vd.16B Vn.16B  (vector)

INST1(aesimc,      "aesimc",       0,      IF_DV_2P,  0x4E287800)
                                   //  aesimc Vd.16B,Vn.16B         DV_2P  0100111000101000 011110nnnnnddddd   4E28 7800   Vd.16B Vn.16B  (vector)

INST1(rev,         "rev",          0,      IF_DR_2G,  0x5AC00800)
                                   //  rev     Rd,Rm                DR_2G  X101101011000000 00001Xnnnnnddddd   5AC0 0800   Rd Rn

INST1(rev64,       "rev64",        0,      IF_DV_2M,  0x0E200800)
                                   //  rev64   Vd,Vn                DV_2M  0Q001110XX100000 000010nnnnnddddd   0E20 0800   Vd,Vn    (vector)

INST1(adc,         "adc",          0,      IF_DR_3A,  0x1A000000)
                                   //  adc     Rd,Rn,Rm             DR_3A  X0011010000mmmmm 000000nnnnnddddd   1A00 0000

INST1(adcs,        "adcs",         0,      IF_DR_3A,  0x3A000000)
                                   //  adcs    Rd,Rn,Rm             DR_3A  X0111010000mmmmm 000000nnnnnddddd   3A00 0000

INST1(sbc,         "sbc",          0,      IF_DR_3A,  0x5A000000)
                                   //  sdc     Rd,Rn,Rm             DR_3A  X1011010000mmmmm 000000nnnnnddddd   5A00 0000

INST1(sbcs,        "sbcs",         0,      IF_DR_3A,  0x7A000000)
                                   //  sdcs    Rd,Rn,Rm             DR_3A  X1111010000mmmmm 000000nnnnnddddd   7A00 0000

INST1(udiv,        "udiv",         0,      IF_DR_3A,  0x1AC00800)
                                   //  udiv    Rd,Rn,Rm             DR_3A  X0011010110mmmmm 000010nnnnnddddd   1AC0 0800

INST1(sdiv,        "sdiv",         0,      IF_DR_3A,  0x1AC00C00)
                                   //  sdiv    Rd,Rn,Rm             DR_3A  X0011010110mmmmm 000011nnnnnddddd   1AC0 0C00

INST1(mneg,        "mneg",         0,      IF_DR_3A,  0x1B00FC00)
                                   //  mneg    Rd,Rn,Rm             DR_3A  X0011011000mmmmm 111111nnnnnddddd   1B00 FC00

INST1(madd,        "madd",         0,      IF_DR_4A,  0x1B000000)
                                   //  madd    Rd,Rn,Rm,Ra          DR_4A  X0011011000mmmmm 0aaaaannnnnddddd   1B00 0000

INST1(msub,        "msub",         0,      IF_DR_4A,  0x1B008000)
                                   //  msub    Rd,Rn,Rm,Ra          DR_4A  X0011011000mmmmm 1aaaaannnnnddddd   1B00 8000

INST1(smaddl,      "smaddl",       0,      IF_DR_4A,  0x1B200000)
                                   //  smaddl  Rd,Rn,Rm,Ra          DR_4A  X0011011001mmmmm 0aaaaannnnnddddd   1B20 0000

INST1(smnegl,      "smnegl",       0,      IF_DR_3A,  0x1B20FC00)
                                   //  smnegl  Rd,Rn,Rm             DR_3A  X0011011001mmmmm 111111nnnnnddddd   1B20 FC00

INST1(smsubl,      "smsubl",       0,      IF_DR_4A,  0x1B208000)
                                   //  smsubl  Rd,Rn,Rm,Ra          DR_4A  X0011011001mmmmm 1aaaaannnnnddddd   1B20 8000

INST1(smulh,       "smulh",        0,      IF_DR_3A,  0x1B407C00)
                                   //  smulh   Rd,Rn,Rm             DR_3A  X0011011010mmmmm 011111nnnnnddddd   1B40 7C00

INST1(umaddl,      "umaddl",       0,      IF_DR_4A,  0x1BA00000)
                                   //  umaddl  Rd,Rn,Rm,Ra          DR_4A  X0011011101mmmmm 0aaaaannnnnddddd   1BA0 0000

INST1(umnegl,      "umnegl",       0,      IF_DR_3A,  0x1BA0FC00)
                                   //  umnegl  Rd,Rn,Rm             DR_3A  X0011011101mmmmm 111111nnnnnddddd   1BA0 FC00

INST1(umsubl,      "umsubl",       0,      IF_DR_4A,  0x1BA08000)
                                   //  umsubl  Rd,Rn,Rm,Ra          DR_4A  X0011011101mmmmm 1aaaaannnnnddddd   1BA0 8000

INST1(umulh,       "umulh",        0,      IF_DR_3A,  0x1BC07C00)
                                   //  umulh   Rd,Rn,Rm             DR_3A  X0011011110mmmmm 011111nnnnnddddd   1BC0 7C00

INST1(extr,        "extr",         0,      IF_DR_3E,  0x13800000)
                                   //  extr    Rd,Rn,Rm,imm6        DR_3E  X00100111X0mmmmm ssssssnnnnnddddd   1380 0000   imm(0-63)

INST1(lslv,        "lslv",         0,      IF_DR_3A,  0x1AC02000)
                                   //  lslv    Rd,Rn,Rm             DR_3A  X0011010110mmmmm 001000nnnnnddddd   1AC0 2000

INST1(lsrv,        "lsrv",         0,      IF_DR_3A,  0x1AC02400)
                                   //  lsrv    Rd,Rn,Rm             DR_3A  X0011010110mmmmm 001001nnnnnddddd   1AC0 2400

INST1(asrv,        "asrv",         0,      IF_DR_3A,  0x1AC02800)
                                   //  asrv    Rd,Rn,Rm             DR_3A  X0011010110mmmmm 001010nnnnnddddd   1AC0 2800

INST1(rorv,        "rorv",         0,      IF_DR_3A,  0x1AC02C00)
                                   //  rorv    Rd,Rn,Rm             DR_3A  X0011010110mmmmm 001011nnnnnddddd   1AC0 2C00

INST1(crc32b,      "crc32b",       0,      IF_DR_3A,  0x1AC04000)
                                   //  crc32b  Rd,Rn,Rm             DR_3A  00011010110mmmmm 010000nnnnnddddd   1AC0 4000

INST1(crc32h,      "crc32h",       0,      IF_DR_3A,  0x1AC04400)
                                   //  crc32h  Rd,Rn,Rm             DR_3A  00011010110mmmmm 010001nnnnnddddd   1AC0 4400

INST1(crc32w,      "crc32w",       0,      IF_DR_3A,  0x1AC04800)
                                   //  crc32w  Rd,Rn,Rm             DR_3A  00011010110mmmmm 010010nnnnnddddd   1AC0 4800

INST1(crc32x,      "crc32x",       0,      IF_DR_3A,  0x1AC04C00)
                                   //  crc32x  Rd,Rn,Xm             DR_3A  10011010110mmmmm 010011nnnnnddddd   1AC0 4C00

INST1(crc32cb,     "crc32cb",      0,      IF_DR_3A,  0x1AC05000)
                                   //  crc32cb Rd,Rn,Rm             DR_3A  00011010110mmmmm 010100nnnnnddddd   1AC0 5000

INST1(crc32ch,     "crc32ch",      0,      IF_DR_3A,  0x1AC05400)
                                   //  crc32ch Rd,Rn,Rm             DR_3A  00011010110mmmmm 010101nnnnnddddd   1AC0 5400

INST1(crc32cw,     "crc32cw",      0,      IF_DR_3A,  0x1AC05800)
                                   //  crc32cw Rd,Rn,Rm             DR_3A  00011010110mmmmm 010110nnnnnddddd   1AC0 5800

INST1(crc32cx,     "crc32cx",      0,      IF_DR_3A,  0x1AC05C00)
                                   //  crc32cx Rd,Rn,Xm             DR_3A  10011010110mmmmm 010111nnnnnddddd   1AC0 5C00

INST1(sha1c,       "sha1c",        0,      IF_DV_3F,  0x5E000000)
                                   //  sha1c   Qd, Sn Vm.4S         DV_3F  01011110000mmmmm 000000nnnnnddddd   5E00 0000   Qd Sn Vm.4S   (vector)

INST1(sha1m,       "sha1m",        0,      IF_DV_3F,  0x5E002000)
                                   //  sha1m   Qd, Sn Vm.4S         DV_3F  01011110000mmmmm 001000nnnnnddddd   5E00 2000   Qd Sn Vm.4S   (vector)

INST1(sha1p,       "sha1p",        0,      IF_DV_3F,  0x5E001000)
                                   //  sha1m   Qd, Sn Vm.4S         DV_3F  01011110000mmmmm 000100nnnnnddddd   5E00 1000   Qd Sn Vm.4S   (vector)

INST1(sha1h,       "sha1h",        0,      IF_DV_2U,  0x5E280800)
                                   //  sha1h   Sd, Sn               DV_2U  0101111000101000 000010nnnnnddddd   5E28 0800   Sn Sn

INST1(sha1su0,     "sha1su0",      0,      IF_DV_3F,  0x5E003000)
                                   //  sha1su0 Vd.4S,Vn.4S,Vm.4S    DV_3F  01011110000mmmmm 001100nnnnnddddd   5E00 3000   Vd.4S Vn.4S Vm.4S  (vector)

INST1(sha1su1,     "sha1su1",      0,      IF_DV_2P,  0x5E281800)
                                   //  sha1su1 Vd.4S, Vn.4S         DV_2P  0101111000101000 000110nnnnnddddd   5E28 1800   Vd.4S Vn.4S   (vector)

INST1(sha256h,     "sha256h",      0,      IF_DV_3F,  0x5E004000)
                                   //  sha256h  Qd,Qn,Vm.4S         DV_3F  01011110000mmmmm 010000nnnnnddddd   5E00 4000   Qd Qn Vm.4S   (vector)

INST1(sha256h2,    "sha256h2",     0,      IF_DV_3F,  0x5E005000)
                                   //  sha256h  Qd,Qn,Vm.4S         DV_3F  01011110000mmmmm 010100nnnnnddddd   5E00 5000   Qd Qn Vm.4S   (vector)

INST1(sha256su0,   "sha256su0",    0,      IF_DV_2P,  0x5E282800)
                                   // sha256su0  Vd.4S,Vn.4S        DV_2P  0101111000101000 001010nnnnnddddd   5E28 2800   Vd.4S Vn.4S   (vector)

INST1(sha256su1,   "sha256su1",    0,      IF_DV_3F,  0x5E006000)
                                   // sha256su1  Vd.4S,Vn.4S,Vm.4S  DV_3F  01011110000mmmmm 011000nnnnnddddd   5E00 6000   Vd.4S Vn.4S Vm.4S  (vector)

INST1(ext,         "ext",          0,      IF_DV_3G,  0x2E000000)
                                   //  ext     Vd,Vn,Vm,index       DV_3G  0Q101110000mmmmm 0iiii0nnnnnddddd   2E00 0000   Vd Vn Vm index  (vector)

INST1(sbfm,        "sbfm",         0,      IF_DI_2D,  0x13000000)
                                   //  sbfm    Rd,Rn,imr,ims        DI_2D  X00100110Nrrrrrr ssssssnnnnnddddd   1300 0000   imr, ims

INST1(bfm,         "bfm",          0,      IF_DI_2D,  0x33000000)
                                   //  bfm     Rd,Rn,imr,ims        DI_2D  X01100110Nrrrrrr ssssssnnnnnddddd   3300 0000   imr, ims

INST1(ubfm,        "ubfm",         0,      IF_DI_2D,  0x53000000)
                                   //  ubfm    Rd,Rn,imr,ims        DI_2D  X10100110Nrrrrrr ssssssnnnnnddddd   5300 0000   imr, ims

INST1(sbfiz,       "sbfiz",        0,      IF_DI_2D,  0x13000000)
                                   //  sbfiz   Rd,Rn,lsb,width      DI_2D  X00100110Nrrrrrr ssssssnnnnnddddd   1300 0000   imr, ims

INST1(bfi,         "bfi",          0,      IF_DI_2D,  0x33000000)
                                   //  bfi     Rd,Rn,lsb,width      DI_2D  X01100110Nrrrrrr ssssssnnnnnddddd   3300 0000   imr, ims

INST1(ubfiz,       "ubfiz",        0,      IF_DI_2D,  0x53000000)
                                   //  ubfiz   Rd,Rn,lsb,width      DI_2D  X10100110Nrrrrrr ssssssnnnnnddddd   5300 0000   imr, ims

INST1(sbfx,        "sbfx",         0,      IF_DI_2D,  0x13000000)
                                   //  sbfx    Rd,Rn,lsb,width      DI_2D  X00100110Nrrrrrr ssssssnnnnnddddd   1300 0000   imr, ims

INST1(bfxil,       "bfxil",        0,      IF_DI_2D,  0x33000000)
                                   //  bfxil   Rd,Rn,lsb,width      DI_2D  X01100110Nrrrrrr ssssssnnnnnddddd   3300 0000   imr, ims

INST1(ubfx,        "ubfx",         0,      IF_DI_2D,  0x53000000)
                                   //  ubfx    Rd,Rn,lsb,width      DI_2D  X10100110Nrrrrrr ssssssnnnnnddddd   5300 0000   imr, ims

INST1(sxtb,        "sxtb",         0,      IF_DR_2H,  0x13001C00)
                                   //  sxtb    Rd,Rn                DR_2H  X00100110X000000 000111nnnnnddddd   1300 1C00

INST1(sxth,        "sxth",         0,      IF_DR_2H,  0x13003C00)
                                   //  sxth    Rd,Rn                DR_2H  X00100110X000000 001111nnnnnddddd   1300 3C00

INST1(sxtw,        "sxtw",         0,      IF_DR_2H,  0x13007C00)
                                   //  sxtw    Rd,Rn                DR_2H  X00100110X000000 011111nnnnnddddd   1300 7C00

INST1(uxtb,        "uxtb",         0,      IF_DR_2H,  0x53001C00)
                                   //  uxtb    Rd,Rn                DR_2H  0101001100000000 000111nnnnnddddd   5300 1C00

INST1(uxth,        "uxth",         0,      IF_DR_2H,  0x53003C00)
                                   //  uxth    Rd,Rn                DR_2H  0101001100000000 001111nnnnnddddd   5300 3C00

INST1(autia1716,   "autia1716",    0,      IF_PC_0A,  0xD503219F)
                                   //  autia1716                    PC_0A  1101010100000011 0010000110011111   D503 219F

INST1(autiasp,     "autiasp",      0,      IF_PC_0A,  0xD50323BF)
                                   //  autiasp                      PC_0A  1101010100000011 0010001110111111   D503 23BF

INST1(autiaz,      "autiaz",       0,      IF_PC_0A,  0xD503239F)
                                   //  autiaz                       PC_0A  1101010100000011 0010001110011111   D503 239F

INST1(pacia1716,   "pacia1716 ",   0,      IF_PC_0A,  0xD503211F)
                                   //  pacia1716                    PC_0A  1101010100000011 0010000100011111   D503 211F

INST1(paciasp,     "paciasp",      0,      IF_PC_0A,  0xD503233F)
                                   //  paciasp                      PC_0A  1101010100000011 0010001100111111   D503 233F

INST1(paciaz,      "paciaz",       0,      IF_PC_0A,  0xD503231F)
                                   //  paciaz                       PC_0A  1101010100000011 0010001100011111   D503 231F

INST1(xpaclri,     "xpaclri",      0,      IF_PC_0A,  0xD50320FF)
                                   //  xpaclri Rd                   PC_0A  1101010100000011 0010000011111111   D503 20FF

INST1(autiza,      "autiza",       0,      IF_PC_1A,  0xDAC133E0)
                                   //  autiza  Rd                   PC_1A  1101101011000001 0001000000000000   DAC1 33E0

INST1(paciza,      "paciza",       0,      IF_PC_1A,  0xDAC123E0)
                                   //  paciza  Rd                   PC_1A  1101101011000001 00000011111ddddd   DAC1 23E0

INST1(xpacd,       "xpacd",        0,      IF_PC_1A,  0xDAC147E0)
                                   //  xpacd   Rd                   PC_0A  1101101011000001 0100011111100000   DAC1 47E0

INST1(xpaci,       "xpaci",        0,      IF_PC_1A,  0xDAC143E0)
                                   //  xpaci   Rd                   PC_0A  1101101011000001 0100001111100000   DAC1 43E0

INST1(autia,       "autia",        0,      IF_PC_2A,  0xDAC11000)
                                   //  autia   Rd,Rn                PC_2A  1101101011000001 00110011111ddddd   DAC1 1000

INST1(pacia,       "pacia",        0,      IF_PC_2A,  0xDAC10000)
                                   //  pacia   Rd,Rn                PC_2A  1101101011000001 000000nnnnnddddd   DAC1 0000

INST1(nop,         "nop",          0,      IF_SN_0A,  0xD503201F)
                                   //  nop                          SN_0A  1101010100000011 0010000000011111   D503 201F

INST1(yield,       "yield",        0,      IF_SN_0A,  0xD503203F)
                                   //  yield                        SN_0A  1101010100000011 0010000000111111   D503 203F

INST1(brk,         "brk",          0,      IF_SI_0A,  0xD43E0000)
                                   //  brk     0xf000               SI_0A  1101010000111110 0000000000000000   D43E 0000   0xF000

INST1(dsb,         "dsb",          0,      IF_SI_0B,  0xD503309F)
                                   //  dsb     barrierKind          SI_0B  1101010100000011 0011bbbb10011111   D503 309F   imm4 - barrier kind

INST1(dmb,         "dmb",          0,      IF_SI_0B,  0xD50330BF)
                                   //  dmb     barrierKind          SI_0B  1101010100000011 0011bbbb10111111   D503 30BF   imm4 - barrier kind

INST1(isb,         "isb",          0,      IF_SI_0B,  0xD50330DF)
                                   //  isb     barrierKind          SI_0B  1101010100000011 0011bbbb11011111   D503 30DF   imm4 - barrier kind

INST1(dczva,       "dczva",        0,      IF_SR_1A,  0xD50B7420)
                                   //  dc      zva,Rt               SR_1A  1101010100001011 01110100001ttttt   D50B 7420   Rt

INST1(mrs_tpid0,   "mrs",          0,      IF_SR_1A,  0xD53BD040)
                                   //  mrs     Rt,tpidr_el0         SR_1A  1101010100111011 11010000010ttttt   D53B D040   Rt, tpidr_el0

INST1(umov,        "umov",         0,      IF_DV_2B,  0x0E003C00)
                                   //  umov    Rd,Vn[]              DV_2B  0Q001110000iiiii 001111nnnnnddddd   0E00 3C00   Rd,Vn[]

INST1(smov,        "smov",         0,      IF_DV_2B,  0x0E002C00)
                                   //  smov    Rd,Vn[]              DV_2B  0Q001110000iiiii 001011nnnnnddddd   0E00 2C00   Rd,Vn[]

INST1(movi,        "movi",         0,      IF_DV_1B,  0x0F000400)
                                   //  movi    Vd,imm8              DV_1B  0QX0111100000iii cmod01iiiiiddddd   0F00 0400   Vd imm8 (immediate vector)

INST1(mvni,        "mvni",         0,      IF_DV_1B,  0x2F000400)
                                   //  mvni    Vd,imm8              DV_1B  0Q10111100000iii cmod01iiiiiddddd   2F00 0400   Vd imm8 (immediate vector)

INST1(urecpe,      "urecpe",       0,      IF_DV_2A,  0x0EA1C800)
                                   //  urecpe  Vd,Vn                DV_2A  0Q0011101X100001 110010nnnnnddddd   0EA1 C800   Vd,Vn      (vector)

INST1(ursqrte,     "ursqrte",      0,      IF_DV_2A,  0x2EA1C800)
                                   //  ursqrte Vd,Vn                DV_2A  0Q1011101X100001 110010nnnnnddddd   2EA1 C800   Vd,Vn      (vector)

INST1(bsl,         "bsl",          0,      IF_DV_3C,  0x2E601C00)
                                   //  bsl     Vd,Vn,Vm             DV_3C  0Q101110011mmmmm 000111nnnnnddddd   2E60 1C00   Vd,Vn,Vm

INST1(bit,         "bit",          0,      IF_DV_3C,  0x2EA01C00)
                                   //  bit     Vd,Vn,Vm             DV_3C  0Q101110101mmmmm 000111nnnnnddddd   2EA0 1C00   Vd,Vn,Vm

INST1(bif,         "bif",          0,      IF_DV_3C,  0x2EE01C00)
                                   //  bif     Vd,Vn,Vm             DV_3C  0Q101110111mmmmm 000111nnnnnddddd   2EE0 1C00   Vd,Vn,Vm

INST1(addv,        "addv",         0,      IF_DV_2T,  0x0E31B800)
                                   //  addv    Vd,Vn                DV_2T  0Q001110XX110001 101110nnnnnddddd   0E31 B800   Vd,Vn      (vector)

INST1(cnt,         "cnt",          0,      IF_DV_2M,  0x0E205800)
                                   //  cnt     Vd,Vn                DV_2M  0Q00111000100000 010110nnnnnddddd   0E20 5800   Vd,Vn      (vector)

INST1(not,         "not",          0,      IF_DV_2M,  0x2E205800)
                                   //  not     Vd,Vn                DV_2M  0Q10111000100000 010110nnnnnddddd   2E20 5800   Vd,Vn      (vector)

INST1(saddlv,      "saddlv",       LNG,    IF_DV_2T,  0x0E303800)
                                   //  saddlv  Vd,Vn                DV_2T  0Q001110XX110000 001110nnnnnddddd   0E30 3800   Vd,Vn      (vector)

INST1(smaxv,       "smaxv",        0,      IF_DV_2T,  0x0E30A800)
                                   //  smaxv   Vd,Vn                DV_2T  0Q001110XX110000 101010nnnnnddddd   0E30 A800   Vd,Vn      (vector)

INST1(sminv,       "sminv",        0,      IF_DV_2T,  0x0E31A800)
                                   //  sminv   Vd,Vn                DV_2T  0Q001110XX110001 101010nnnnnddddd   0E31 A800   Vd,Vn      (vector)

INST1(uaddlv,      "uaddlv",       0,      IF_DV_2T,  0x2E303800)
                                   //  uaddlv  Vd,Vn                DV_2T  0Q101110XX110000 001110nnnnnddddd   2E30 3800   Vd,Vn      (vector)

INST1(umaxv,       "umaxv",        0,      IF_DV_2T,  0x2E30A800)
                                   //  umaxv   Vd,Vn                DV_2T  0Q101110XX110000 101010nnnnnddddd   2E30 A800   Vd,Vn      (vector)

INST1(uminv,       "uminv",        0,      IF_DV_2T,  0x2E31A800)
                                   //  uminv   Vd,Vn                DV_2T  0Q101110XX110001 101010nnnnnddddd   2E31 A800   Vd,Vn      (vector)

INST1(fmaxnmv,     "fmaxnmv",      0,      IF_DV_2R,  0x2E30C800)
                                   //  fmaxnmv Vd,Vn                DV_2R  0Q1011100X110000 110010nnnnnddddd   2E30 C800   Vd,Vn      (vector)

INST1(fmaxv,       "fmaxv",        0,      IF_DV_2R,  0x2E30F800)
                                   //  fmaxv   Vd,Vn                DV_2R  0Q1011100X110000 111110nnnnnddddd   2E30 F800   Vd,Vn      (vector)

INST1(fminnmv,     "fminnmv",      0,      IF_DV_2R,  0x2EB0C800)
                                   //  fminnmv Vd,Vn                DV_2R  0Q1011101X110000 110010nnnnnddddd   2EB0 C800   Vd,Vn      (vector)

INST1(fminv,       "fminv",        0,      IF_DV_2R,  0x2EB0F800)
                                   //  fminv   Vd,Vn                DV_2R  0Q1011101X110000 111110nnnnnddddd   2EB0 F800   Vd,Vn      (vector)

INST1(uzp1,        "uzp1",         0,      IF_DV_3A,  0x0E001800)
                                   //  uzp1    Vd,Vn,Vm             DV_3A  0Q001110XX0mmmmm 000110nnnnnddddd   0E00 1800   Vd,Vn,Vm  (vector)

INST1(uzp2,        "uzp2",         0,      IF_DV_3A,  0x0E005800)
                                   //  upz2    Vd,Vn,Vm             DV_3A  0Q001110XX0mmmmm 010110nnnnnddddd   0E00 5800   Vd,Vn,Vm  (vector)

INST1(zip1,        "zip1",         0,      IF_DV_3A,  0x0E003800)
                                   //  zip1    Vd,Vn,Vm             DV_3A  0Q001110XX0mmmmm 011110nnnnnddddd   0E00 3800   Vd,Vn,Vm  (vector)

INST1(zip2,        "zip2",         0,      IF_DV_3A,  0x0E007800)
                                   //  zip2    Vd,Vn,Vm             DV_3A  0Q001110XX0mmmmm 001110nnnnnddddd   0E00 7800   Vd,Vn,Vm  (vector)

INST1(trn1,        "trn1",         0,      IF_DV_3A,  0x0E002800)
                                   //  trn1    Vd,Vn,Vm             DV_3A  0Q001110XX0mmmmm 001010nnnnnddddd   0E00 2800   Vd,Vn,Vm  (vector)

INST1(trn2,        "trn2",         0,      IF_DV_3A,  0x0E006800)
                                   //  trn2    Vd,Vn,Vm             DV_3A  0Q001110XX0mmmmm 011010nnnnnddddd   0E00 6800   Vd,Vn,Vm  (vector)

INST1(sqxtn2,      "sqxtn2",       NRW,    IF_DV_2M,  0x4E214800)
                                   //  sqxtn2  Vd,Vn                DV_2M  01001110XX100001 010010nnnnnddddd   4E21 4800   Vd,Vn      (vector)

INST1(sqxtun2,     "sqxtun2",      NRW,    IF_DV_2M,  0x6E212800)
                                   //  sqxtnu2 Vd,Vn                DV_2M  01101110XX100001 001010nnnnnddddd   6E21 2800   Vd,Vn      (vector)

INST1(uqxtn2,      "uqxtn2",       NRW,    IF_DV_2M,  0x2E214800)
                                   //  uqxtn2  Vd,Vn                DV_2M  01101110XX100001 010010nnnnnddddd   6E21 4800   Vd,Vn      (vector)

INST1(xtn,         "xtn",          NRW,    IF_DV_2M,  0x0E212800)
                                   //  xtn     Vd,Vn                DV_2M  00101110XX110000 001110nnnnnddddd   0E21 2800   Vd,Vn      (vector)

INST1(xtn2,        "xtn2",         NRW,    IF_DV_2M,  0x4E212800)
                                   //  xtn2    Vd,Vn                DV_2M  01101110XX110000 001110nnnnnddddd   4E21 2800   Vd,Vn      (vector)

INST1(fnmul,       "fnmul",        0,      IF_DV_3D,  0x1E208800)
                                   //  fnmul   Vd,Vn,Vm             DV_3D  000111100X1mmmmm 100010nnnnnddddd   1E20 8800   Vd,Vn,Vm   (scalar)

INST1(fmadd,       "fmadd",        0,      IF_DV_4A,  0x1F000000)
                                   //  fmadd   Vd,Va,Vn,Vm          DV_4A  000111110X0mmmmm 0aaaaannnnnddddd   1F00 0000   Vd Vn Vm Va (scalar)

INST1(fmsub,       "fmsub",        0,      IF_DV_4A,  0x1F008000)
                                   //  fmsub   Vd,Va,Vn,Vm          DV_4A  000111110X0mmmmm 1aaaaannnnnddddd   1F00 8000   Vd Vn Vm Va (scalar)

INST1(fnmadd,      "fnmadd",       0,      IF_DV_4A,  0x1F200000)
                                   //  fnmadd  Vd,Va,Vn,Vm          DV_4A  000111110X1mmmmm 0aaaaannnnnddddd   1F20 0000   Vd Vn Vm Va (scalar)

INST1(fnmsub,      "fnmsub",       0,      IF_DV_4A,  0x1F208000)
                                   //  fnmsub  Vd,Va,Vn,Vm          DV_4A  000111110X1mmmmm 1aaaaannnnnddddd   1F20 8000   Vd Vn Vm Va (scalar)

INST1(fcvt,        "fcvt",         0,      IF_DV_2J,  0x1E224000)
                                   //  fcvt    Vd,Vn                DV_2J  00011110SS10001D D10000nnnnnddddd   1E22 4000   Vd,Vn

INST1(pmul,        "pmul",         0,      IF_DV_3A,  0x2E209C00)
                                   //  pmul    Vd,Vn,Vm             DV_3A  0Q101110XX1mmmmm 100111nnnnnddddd   2E20 9C00   Vd,Vn,Vm  (vector)

INST1(saba,        "saba",         0,      IF_DV_3A,  0x0E207C00)
                                   //  saba    Vd,Vn,Vm             DV_3A  0Q001110XX1mmmmm 011111nnnnnddddd   0E20 7C00   Vd,Vn,Vm  (vector)

INST1(sabd,        "sabd",         0,      IF_DV_3A,  0x0E207400)
                                   //  sabd    Vd,Vn,Vm             DV_3A  0Q001110XX1mmmmm 011101nnnnnddddd   0E20 7400   Vd,Vn,Vm  (vector)

INST1(smax,        "smax",         0,      IF_DV_3A,  0x0E206400)
                                   //  smax    Vd,Vn,Vm             DV_3A  0Q001110XX1mmmmm 011001nnnnnddddd   0E20 6400   Vd,Vn,Vm  (vector)

INST1(smaxp,       "smaxp",        0,      IF_DV_3A,  0x0E20A400)
                                   //  smaxp   Vd,Vn,Vm             DV_3A  0Q001110XX1mmmmm 101001nnnnnddddd   0E20 A400   Vd,Vn,Vm  (vector)

INST1(smin,        "smin",         0,      IF_DV_3A,  0x0E206C00)
                                   //  smax    Vd,Vn,Vm             DV_3A  0Q001110XX1mmmmm 011011nnnnnddddd   0E20 6C00   Vd,Vn,Vm  (vector)

INST1(sminp,       "sminp",        0,      IF_DV_3A,  0x0E20AC00)
                                   //  smax    Vd,Vn,Vm             DV_3A  0Q001110XX1mmmmm 101011nnnnnddddd   0E20 AC00   Vd,Vn,Vm  (vector)

INST1(uaba,        "uaba",         0,      IF_DV_3A,  0x2E207C00)
                                   //  uaba    Vd,Vn,Vm             DV_3A  0Q101110XX1mmmmm 011111nnnnnddddd   2E20 7C00   Vd,Vn,Vm  (vector)

INST1(uabd,        "uabd",         0,      IF_DV_3A,  0x2E207400)
                                   //  uabd    Vd,Vn,Vm             DV_3A  0Q101110XX1mmmmm 011101nnnnnddddd   2E20 7400   Vd,Vn,Vm  (vector)

INST1(umax,        "umax",         0,      IF_DV_3A,  0x2E206400)
                                   //  umax    Vd,Vn,Vm             DV_3A  0Q101110XX1mmmmm 011001nnnnnddddd   2E20 6400   Vd,Vn,Vm  (vector)

INST1(umaxp,       "umaxp",        0,      IF_DV_3A,  0x2E20A400)
                                   //  umaxp   Vd,Vn,Vm             DV_3A  0Q101110XX1mmmmm 101001nnnnnddddd   2E20 A400   Vd,Vn,Vm  (vector)

INST1(umin,        "umin",         0,      IF_DV_3A,  0x2E206C00)
                                   //  umin    Vd,Vn,Vm             DV_3A  0Q101110XX1mmmmm 011011nnnnnddddd   2E20 6C00   Vd,Vn,Vm  (vector)

INST1(uminp,       "uminp",        0,      IF_DV_3A,  0x2E20AC00)
                                   //  umin    Vd,Vn,Vm             DV_3A  0Q101110XX1mmmmm 101011nnnnnddddd   2E20 AC00   Vd,Vn,Vm  (vector)

INST1(fcvtl,       "fcvtl",        LNG,    IF_DV_2A,  0x0E217800)
                                   //  fcvtl   Vd,Vn                DV_2A  000011100X100001 011110nnnnnddddd   0E21 7800   Vd,Vn    (vector)

INST1(fcvtl2,      "fcvtl2",       LNG,    IF_DV_2A,  0x4E217800)
                                   //  fcvtl2  Vd,Vn                DV_2A  040011100X100001 011110nnnnnddddd   4E21 7800   Vd,Vn    (vector)

INST1(fcvtn,       "fcvtn",        NRW,    IF_DV_2A,  0x0E216800)
                                   //  fcvtn   Vd,Vn                DV_2A  000011100X100001 011010nnnnnddddd   0E21 6800   Vd,Vn    (vector)

INST1(fcvtn2,      "fcvtn2",       NRW,    IF_DV_2A,  0x4E216800)
                                   //  fcvtn2  Vd,Vn                DV_2A  040011100X100001 011010nnnnnddddd   4E21 6800   Vd,Vn    (vector)

INST1(fcvtxn2,     "fcvtxn2",      NRW,    IF_DV_2A,  0x6E616800)
                                   //  fcvtxn2 Vd,Vn                DV_2A  0110111001100001 011010nnnnnddddd   6E61 6800   Vd,Vn    (vector)

INST1(frecpx,      "frecpx",       0,      IF_DV_2G,  0x5EA1F800)
                                   //  frecpx  Vd,Vn                DV_2G  010111101X100001 111110nnnnnddddd   5EA1 F800   Vd,Vn    (scalar)

INST1(addhn,       "addhn",        NRW,    IF_DV_3A,  0x0E204000)
                                   //  addhn   Vd,Vn,Vm             DV_3A  00001110XX1mmmmm 010000nnnnnddddd   0E20 4000   Vd,Vn,Vm (vector)

INST1(addhn2,      "addhn2",       NRW,    IF_DV_3A,  0x4E204000)
                                   //  addhn2  Vd,Vn,Vm             DV_3A  01001110XX1mmmmm 010000nnnnnddddd   4E20 4000   Vd,Vn,Vm (vector)

INST1(pmull,       "pmull",        LNG,    IF_DV_3A,  0x0E20E000)
                                   //  pmull   Vd,Vn,Vm             DV_3A  00001110XX1mmmmm 111000nnnnnddddd   0E20 E000   Vd,Vn,Vm (vector)

INST1(pmull2,      "pmull2",       LNG,    IF_DV_3A,  0x4E20E000)
                                   //  pmull2  Vd,Vn,Vm             DV_3A  01001110XX1mmmmm 111000nnnnnddddd   4E20 E000   Vd,Vn,Vm (vector)

INST1(raddhn,      "raddhn",       NRW,    IF_DV_3A,  0x2E204000)
                                   //  raddhn   Vd,Vn,Vm            DV_3A  00101110XX1mmmmm 010000nnnnnddddd   2E20 4000   Vd,Vn,Vm (vector)

INST1(raddhn2,     "raddhn2",      NRW,    IF_DV_3A,  0x6E204000)
                                   //  raddhn2  Vd,Vn,Vm            DV_3A  01101110XX1mmmmm 010000nnnnnddddd   6E20 4000   Vd,Vn,Vm (vector)

INST1(rsubhn,      "rsubhn",       NRW,    IF_DV_3A,  0x2E206000)
                                   //  rsubhn   Vd,Vn,Vm            DV_3A  00101110XX1mmmmm 011000nnnnnddddd   2E20 6000   Vd,Vn,Vm (vector)

INST1(rsubhn2,     "rsubhn2",      NRW,    IF_DV_3A,  0x6E206000)
                                   //  rsubhn2  Vd,Vn,Vm            DV_3A  01101110XX1mmmmm 011000nnnnnddddd   6E20 6000   Vd,Vn,Vm (vector)

INST1(sabal,       "sabal",        LNG,    IF_DV_3A,  0x0E205000)
                                   //  sabal   Vd,Vn,Vm             DV_3A  00001110XX1mmmmm 010100nnnnnddddd   0E20 5000   Vd,Vn,Vm (vector)

INST1(sabal2,      "sabal2",       LNG,    IF_DV_3A,  0x4E205000)
                                   //  sabal2  Vd,Vn,Vm             DV_3A  01001110XX1mmmmm 010100nnnnnddddd   4E20 5000   Vd,Vn,Vm (vector)

INST1(sabdl,       "sabdl",        LNG,    IF_DV_3A,  0x0E207000)
                                   //  sabdl   Vd,Vn,Vm             DV_3A  00001110XX1mmmmm 011100nnnnnddddd   0E20 7000   Vd,Vn,Vm (vector)

INST1(sabdl2,      "sabdl2",       LNG,    IF_DV_3A,  0x4E207000)
                                   //  sabdl2  Vd,Vn,Vm             DV_3A  01001110XX1mmmmm 011100nnnnnddddd   4E20 7000   Vd,Vn,Vm (vector)

INST1(sadalp,      "sadalp",       LNG,    IF_DV_2T,  0x0E206800)
                                   //  sadalp  Vd,Vn                DV_2T  0Q001110XX100000 011010nnnnnddddd   0E20 6800   Vd,Vn    (vector)

INST1(saddl,       "saddl",        LNG,    IF_DV_3A,  0x0E200000)
                                   //  saddl   Vd,Vn,Vm             DV_3A  00001110XX1mmmmm 000000nnnnnddddd   0E20 0000   Vd,Vn,Vm (vector)

INST1(saddl2,      "saddl2",       LNG,    IF_DV_3A,  0x4E200000)
                                   //  saddl2  Vd,Vn,Vm             DV_3A  01001110XX1mmmmm 000000nnnnnddddd   4E20 0000   Vd,Vn,Vm (vector)

INST1(saddlp,      "saddlp",       LNG,    IF_DV_2T,  0x0E202800)
                                   //  saddlp  Vd,Vn                DV_2T  0Q001110XX100000 001010nnnnnddddd   0E20 2800   Vd,Vn    (vector)

INST1(saddw,       "saddw",        WID,    IF_DV_3A,  0x0E201000)
                                   //  saddw   Vd,Vn,Vm             DV_3A  00001110XX1mmmmm 000100nnnnnddddd   0E20 1000   Vd,Vn,Vm (vector)

INST1(saddw2,      "saddw2",       WID,    IF_DV_3A,  0x4E201000)
                                   //  saddw2  Vd,Vn,Vm             DV_3A  01001110XX1mmmmm 000100nnnnnddddd   4E20 1000   Vd,Vn,Vm (vector)

INST1(shadd,       "shadd",        0,      IF_DV_3A,  0x0E200400)
                                   //  shadd  Vd,Vn,Vm              DV_3A  0Q001110XX1mmmmm 000001nnnnnddddd   0E20 0400   Vd,Vn,Vm (vector)

INST1(shsub,       "shsub",        0,      IF_DV_3A,  0x0E202400)
                                   //  shsub  Vd,Vn,Vm              DV_3A  0Q001110XX1mmmmm 001001nnnnnddddd   0E20 2400   Vd,Vn,Vm (vector)

INST1(srhadd,      "srhadd",       0,      IF_DV_3A,  0x0E201400)
                                   //  srhadd Vd,Vn,Vm              DV_3A  0Q001110XX1mmmmm 000101nnnnnddddd   0E20 1400   Vd,Vn,Vm (vector)

INST1(ssubl,       "ssubl",        LNG,    IF_DV_3A,  0x0E202000)
                                   //  ssubl   Vd,Vn,Vm             DV_3A  00001110XX1mmmmm 001000nnnnnddddd   0E20 2000   Vd,Vn,Vm (vector)

INST1(ssubl2,      "ssubl2",       LNG,    IF_DV_3A,  0x4E202000)
                                   //  ssubl2  Vd,Vn,Vm             DV_3A  01001110XX1mmmmm 001000nnnnnddddd   4E20 2000   Vd,Vn,Vm (vector)

INST1(ssubw,       "ssubw",        WID,    IF_DV_3A,  0x0E203000)
                                   //  ssubw   Vd,Vn,Vm             DV_3A  00001110XX1mmmmm 001100nnnnnddddd   0E20 3000   Vd,Vn,Vm (vector)

INST1(ssubw2,      "ssubw2",       WID,    IF_DV_3A,  0x4E203000)
                                   //  ssubw2  Vd,Vn,Vm             DV_3A  01001110XX1mmmmm 001100nnnnnddddd   4E20 3000   Vd,Vn,Vm (vector)

INST1(subhn,       "subhn",        NRW,    IF_DV_3A,  0x0E206000)
                                   //  subhn   Vd,Vn,Vm             DV_3A  00001110XX1mmmmm 011000nnnnnddddd   0E20 6000   Vd,Vn,Vm (vector)

INST1(subhn2,      "subhn2",       NRW,    IF_DV_3A,  0x4E206000)
                                   //  subhn2  Vd,Vn,Vm             DV_3A  01001110XX1mmmmm 011000nnnnnddddd   4E20 6000   Vd,Vn,Vm (vector)

INST1(uabal,       "uabal",        LNG,    IF_DV_3A,  0x2E205000)
                                   //  uabal   Vd,Vn,Vm             DV_3A  00101110XX1mmmmm 010100nnnnnddddd   2E20 5000   Vd,Vn,Vm (vector)

INST1(uabal2,      "uabal2",       LNG,    IF_DV_3A,  0x6E205000)
                                   //  uabal2  Vd,Vn,Vm             DV_3A  01101110XX1mmmmm 010100nnnnnddddd   6E20 5000   Vd,Vn,Vm (vector)

INST1(uabdl,       "uabdl",        LNG,    IF_DV_3A,  0x2E207000)
                                   //  uabdl   Vd,Vn,Vm             DV_3A  00101110XX1mmmmm 011100nnnnnddddd   2E20 7000   Vd,Vn,Vm (vector)

INST1(uabdl2,      "uabdl2",       LNG,    IF_DV_3A,  0x6E207000)
                                   //  uabdl2  Vd,Vn,Vm             DV_3A  01101110XX1mmmmm 011100nnnnnddddd   6E20 7000   Vd,Vn,Vm (vector)

INST1(uadalp,      "uadalp",       LNG,    IF_DV_2T,  0x2E206800)
                                   //  uadalp  Vd,Vn                DV_2T  0Q101110XX100000 011010nnnnnddddd   2E20 6800   Vd,Vn    (vector)

INST1(uaddl,       "uaddl",        LNG,    IF_DV_3A,  0x2E200000)
                                   //  uaddl   Vd,Vn,Vm             DV_3A  00101110XX1mmmmm 000000nnnnnddddd   2E20 0000   Vd,Vn,Vm (vector)

INST1(uaddl2,      "uaddl2",       LNG,    IF_DV_3A,  0x6E200000)
                                   //  uaddl2  Vd,Vn,Vm             DV_3A  01101110XX1mmmmm 000000nnnnnddddd   6E20 0000   Vd,Vn,Vm (vector)

INST1(uaddlp,      "uaddlp",       LNG,    IF_DV_2T,  0x2E202800)
                                   //  uaddlp  Vd,Vn                DV_2T  0Q101110XX100000 001010nnnnnddddd   2E20 2800   Vd,Vn    (vector)

INST1(uaddw,       "uaddw",        WID,    IF_DV_3A,  0x2E201000)
                                   //  uaddw   Vd,Vn,Vm             DV_3A  00101110XX1mmmmm 000100nnnnnddddd   2E20 1000   Vd,Vn,Vm (vector)

INST1(uaddw2,      "uaddw2",       WID,    IF_DV_3A,  0x6E201000)
                                   //  uaddw2  Vd,Vn,Vm             DV_3A  01101110XX1mmmmm 000100nnnnnddddd   6E20 1000   Vd,Vn,Vm (vector)

INST1(uhadd,       "uhadd",        0,      IF_DV_3A,  0x2E200400)
                                   //  uhadd  Vd,Vn,Vm              DV_3A  0Q101110XX1mmmmm 000001nnnnnddddd   2E20 0400   Vd,Vn,Vm (vector)

INST1(uhsub,       "uhsub",        0,      IF_DV_3A,  0x2E202400)
                                   //  uhsub  Vd,Vn,Vm              DV_3A  0Q101110XX1mmmmm 001001nnnnnddddd   2E20 2400   Vd,Vn,Vm (vector)

INST1(urhadd,      "urhadd",       0,      IF_DV_3A,  0x2E201400)
                                   //  urhadd Vd,Vn,Vm              DV_3A  0Q101110XX1mmmmm 000101nnnnnddddd   2E20 1400   Vd,Vn,Vm (vector)

INST1(usubl,       "usubl",        LNG,    IF_DV_3A,  0x2E202000)
                                   //  usubl   Vd,Vn,Vm             DV_3A  00101110XX1mmmmm 001000nnnnnddddd   2E20 2000   Vd,Vn,Vm (vector)

INST1(usubl2,      "usubl2",       LNG,    IF_DV_3A,  0x6E202000)
                                   //  usubl2  Vd,Vn,Vm             DV_3A  01101110XX1mmmmm 001000nnnnnddddd   6E20 2000   Vd,Vn,Vm (vector)

INST1(usubw,       "usubw",        WID,    IF_DV_3A,  0x2E203000)
                                   //  usubw   Vd,Vn,Vm             DV_3A  00101110XX1mmmmm 001100nnnnnddddd   2E20 3000   Vd,Vn,Vm (vector)

INST1(usubw2,      "usubw2",       WID,    IF_DV_3A,  0x6E203000)
                                   //  usubw2  Vd,Vn,Vm             DV_3A  01101110XX1mmmmm 001100nnnnnddddd   6E20 3000   Vd,Vn,Vm (vector)

INST1(shll,        "shll",         LNG,    IF_DV_2M,  0x2E213800)
                                   //  shll    Vd,Vn,imm            DV_2M  0Q101110XX100001 001110nnnnnddddd   2E21 3800   Vd,Vn, {8/16/32}

INST1(shll2,       "shll2",        LNG,    IF_DV_2M,  0x6E213800)
                                   //  shll2   Vd,Vn,imm            DV_2M  01101110XX100001 001110nnnnnddddd   6E21 3800   Vd,Vn, {8/16/32}

INST1(sshll,       "sshll",        LNG,    IF_DV_2O,  0x0F00A400)
                                   //  sshll   Vd,Vn,imm            DV_2O  000011110iiiiiii 101001nnnnnddddd   0F00 A400   Vd,Vn imm  (left shift - vector)

INST1(sshll2,      "sshll2",       LNG,    IF_DV_2O,  0x4F00A400)
                                   //  sshll2  Vd,Vn,imm            DV_2O  010011110iiiiiii 101001nnnnnddddd   4F00 A400   Vd,Vn imm  (left shift - vector)

INST1(ushll,       "ushll",        LNG,    IF_DV_2O,  0x2F00A400)
                                   //  ushll   Vd,Vn,imm            DV_2O  001011110iiiiiii 101001nnnnnddddd   2F00 A400   Vd,Vn imm  (left shift - vector)

INST1(ushll2,      "ushll2",       LNG,    IF_DV_2O,  0x6F00A400)
                                   //  ushll2  Vd,Vn,imm            DV_2O  011011110iiiiiii 101001nnnnnddddd   6F00 A400   Vd,Vn imm  (left shift - vector)

INST1(shrn,        "shrn",         RSH|NRW,IF_DV_2O,  0x0F008400)
                                   //  shrn    Vd,Vn,imm            DV_2O  000011110iiiiiii 100001nnnnnddddd   0F00 8400   Vd,Vn imm  (right shift - vector)

INST1(shrn2,       "shrn2",        RSH|NRW,IF_DV_2O,  0x4F008400)
                                   //  shrn2   Vd,Vn,imm            DV_2O  010011110iiiiiii 100001nnnnnddddd   4F00 8400   Vd,Vn imm  (right shift - vector)

INST1(rshrn,       "rshrn",        RSH|NRW,IF_DV_2O,  0x0F008C00)
                                   //  rshrn   Vd,Vn,imm            DV_2O  000011110iiiiiii 100011nnnnnddddd   0F00 8C00   Vd,Vn imm  (right shift - vector)

INST1(rshrn2,      "rshrn2",       RSH|NRW,IF_DV_2O,  0x4F008C00)
                                   //  rshrn2  Vd,Vn,imm            DV_2O  010011110iiiiiii 100011nnnnnddddd   4F00 8C00   Vd,Vn imm  (right shift - vector)

INST1(sqrshrn2,    "sqrshrn2",     RSH|NRW,IF_DV_2O,  0x4F009C00)
                                   //  sqrshrn2 Vd,Vn,imm           DV_2O  010011110iiiiiii 100111nnnnnddddd   4F00 9C00   Vd Vn imm  (right shift - vector)

INST1(sqrshrun2,   "sqrshrun2",    RSH|NRW,IF_DV_2O,  0x6F008C00)
                                   //  sqrshrun2 Vd,Vn,imm          DV_2O  011011110iiiiiii 100011nnnnnddddd   6F00 8C00   Vd Vn imm  (right shift - vector)

INST1(sqshrn2,     "sqshrn2",      RSH|NRW,IF_DV_2O,  0x4F009400)
                                   //  sqshrn2 Vd,Vn,imm            DV_2O  010011110iiiiiii 100101nnnnnddddd   4F00 9400   Vd Vn imm  (right shift - vector)

INST1(sqshrun2,    "sqshrun2",     RSH|NRW,IF_DV_2O,  0x6F008400)
                                   //  sqshrun2 Vd,Vn,imm           DV_2O  011011110iiiiiii 100001nnnnnddddd   6F00 8400   Vd Vn imm  (right shift - vector)

INST1(uqrshrn2,    "uqrshrn2",     RSH|NRW,IF_DV_2O,  0x6F009C00)
                                   //  uqrshrn2 Vd,Vn,imm           DV_2O  011011110iiiiiii 100111nnnnnddddd   6F00 9C00   Vd Vn imm  (right shift - vector)

INST1(uqshrn2,     "uqshrn2",      RSH|NRW,IF_DV_2O,  0x6F009400)
                                   //  uqshrn2 Vd,Vn,imm            DV_2O  011011110iiiiiii 100101nnnnnddddd   6F00 9400   Vd Vn imm  (right shift - vector)

INST1(sxtl,        "sxtl",         LNG,    IF_DV_2O,  0x0F00A400)
                                   //  sxtl    Vd,Vn                DV_2O  000011110iiiiiii 101001nnnnnddddd   0F00 A400   Vd,Vn      (left shift - vector)

INST1(sxtl2,       "sxtl2",        LNG,    IF_DV_2O,  0x4F00A400)
                                   //  sxtl2   Vd,Vn                DV_2O  010011110iiiiiii 101001nnnnnddddd   4F00 A400   Vd,Vn      (left shift - vector)

INST1(uxtl,        "uxtl",         LNG,    IF_DV_2O,  0x2F00A400)
                                   //  uxtl    Vd,Vn                DV_2O  001011110iiiiiii 101001nnnnnddddd   2F00 A400   Vd,Vn      (left shift - vector)

INST1(uxtl2,       "uxtl2",        LNG,    IF_DV_2O,  0x6F00A400)
                                   //  uxtl2   Vd,Vn                DV_2O  011011110iiiiiii 101001nnnnnddddd   6F00 A400   Vd,Vn      (left shift - vector)

INST1(tbl,         "tbl",          0,      IF_DV_3C,  0x0E000000)
                                   //  tbl    Vd,{Vn},Vm                DV_3C  0Q001110000mmmmm 000000nnnnnddddd   0E00 0000   Vd,Vn,Vm   (vector)

INST1(tbl_2regs,   "tbl",          0,      IF_DV_3C,  0x0E002000)
                                   //  tbl    Vd,{Vn,Vn+1},Vm           DV_3C  0Q001110000mmmmm 001000nnnnnddddd   0E00 2000   Vd,Vn,Vm   (vector)

INST1(tbl_3regs,   "tbl",          0,      IF_DV_3C,  0x0E004000)
                                   //  tbl    Vd,{Vn,Vn+1,Vn+2},Vm      DV_3C  0Q001110000mmmmm 010000nnnnnddddd   0E00 4000   Vd,Vn,Vm   (vector)

INST1(tbl_4regs,   "tbl",          0,      IF_DV_3C,  0x0E006000)
                                   //  tbl    Vd,{Vn,Vn+1,Vn+2,Vn+3},Vm DV_3C  0Q001110000mmmmm 011000nnnnnddddd   0E00 6000   Vd,Vn,Vm   (vector)

INST1(tbx,         "tbx",          0,      IF_DV_3C,  0x0E001000)
                                   //  tbx    Vd,{Vn},Vm                DV_3C  0Q001110000mmmmm 000100nnnnnddddd   0E00 1000   Vd,Vn,Vm   (vector)

INST1(tbx_2regs,   "tbx",          0,      IF_DV_3C,  0x0E003000)
                                   //  tbx    Vd,{Vn,Vn+1},Vm           DV_3C  0Q001110000mmmmm 001100nnnnnddddd   0E00 3000   Vd,Vn,Vm   (vector)

INST1(tbx_3regs,   "tbx",          0,      IF_DV_3C,  0x0E005000)
                                   //  tbx    Vd,{Vn,Vn+1,Vn+2},Vm      DV_3C  0Q001110000mmmmm 010100nnnnnddddd   0E00 5000   Vd,Vn,Vm   (vector)

INST1(tbx_4regs,   "tbx",          0,      IF_DV_3C,  0x0E007000)
                                   //  tbx    Vd,{Vn,Vn+1,Vn+2,Vn+3},Vm DV_3C  0Q001110000mmmmm 011100nnnnnddddd   0E00 7000   Vd,Vn,Vm   (vector)

#if FEATURE_LOOP_ALIGN
INST1(align,       "align",        0,      IF_SN_0A,  BAD_CODE)
                                   //  align                          SN_0A
#endif
// clang-format on

/*****************************************************************************/
#undef INST1
#undef INST2
#undef INST3
#undef INST4
#undef INST5
#undef INST6
#undef INST9
/*****************************************************************************/
