#if 0
//
// Generated by Microsoft (R) D3D Shader Disassembler
//
//
// Input signature:
//
// Name                 Index   Mask Register SysValue  Format   Used
// -------------------- ----- ------ -------- -------- ------- ------
// POSITION                 0   xyz         0     NONE   float   xyz 
// COLOR                    0   xyz         1     NONE   float   xyz 
//
//
// Output signature:
//
// Name                 Index   Mask Register SysValue  Format   Used
// -------------------- ----- ------ -------- -------- ------- ------
// SV_POSITION              0   xyzw        0      POS   float   xyzw
// COLOR                    0   xyzw        1     NONE   float   xyzw
//
vs_5_0
dcl_globalFlags refactoringAllowed
dcl_constantbuffer CB0[8], immediateIndexed
dcl_input v0.xyz
dcl_input v1.xyz
dcl_output_siv o0.xyzw, position
dcl_output o1.xyzw
dcl_temps 2
mul r0.xyzw, cb0[1].yyyy, cb0[5].xyzw
mad r0.xyzw, cb0[1].xxxx, cb0[4].xyzw, r0.xyzw
mad r0.xyzw, cb0[1].zzzz, cb0[6].xyzw, r0.xyzw
mad r0.xyzw, cb0[1].wwww, cb0[7].xyzw, r0.xyzw
mul r0.xyzw, r0.xyzw, v0.yyyy
mul r1.xyzw, cb0[0].yyyy, cb0[5].xyzw
mad r1.xyzw, cb0[0].xxxx, cb0[4].xyzw, r1.xyzw
mad r1.xyzw, cb0[0].zzzz, cb0[6].xyzw, r1.xyzw
mad r1.xyzw, cb0[0].wwww, cb0[7].xyzw, r1.xyzw
mad r0.xyzw, v0.xxxx, r1.xyzw, r0.xyzw
mul r1.xyzw, cb0[2].yyyy, cb0[5].xyzw
mad r1.xyzw, cb0[2].xxxx, cb0[4].xyzw, r1.xyzw
mad r1.xyzw, cb0[2].zzzz, cb0[6].xyzw, r1.xyzw
mad r1.xyzw, cb0[2].wwww, cb0[7].xyzw, r1.xyzw
mad r0.xyzw, v0.zzzz, r1.xyzw, r0.xyzw
mul r1.xyzw, cb0[3].yyyy, cb0[5].xyzw
mad r1.xyzw, cb0[3].xxxx, cb0[4].xyzw, r1.xyzw
mad r1.xyzw, cb0[3].zzzz, cb0[6].xyzw, r1.xyzw
mad r1.xyzw, cb0[3].wwww, cb0[7].xyzw, r1.xyzw
add o0.xyzw, r0.xyzw, r1.xyzw
mov o1.xyz, v1.xyzx
mov o1.w, l(1.000000)
ret 
// Approximately 0 instruction slots used
#endif

const BYTE d3d11_vshader[] =
{
     68,  88,  66,  67, 145,  28, 
     78,   3,  83,  80,  97, 142, 
    212, 215,  43, 222, 254, 116, 
     36,  11,   1,   0,   0,   0, 
    124,   4,   0,   0,   3,   0, 
      0,   0,  44,   0,   0,   0, 
    124,   0,   0,   0, 208,   0, 
      0,   0,  73,  83,  71,  78, 
     72,   0,   0,   0,   2,   0, 
      0,   0,   8,   0,   0,   0, 
     56,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      3,   0,   0,   0,   0,   0, 
      0,   0,   7,   7,   0,   0, 
     65,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      3,   0,   0,   0,   1,   0, 
      0,   0,   7,   7,   0,   0, 
     80,  79,  83,  73,  84,  73, 
     79,  78,   0,  67,  79,  76, 
     79,  82,   0, 171,  79,  83, 
     71,  78,  76,   0,   0,   0, 
      2,   0,   0,   0,   8,   0, 
      0,   0,  56,   0,   0,   0, 
      0,   0,   0,   0,   1,   0, 
      0,   0,   3,   0,   0,   0, 
      0,   0,   0,   0,  15,   0, 
      0,   0,  68,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   3,   0,   0,   0, 
      1,   0,   0,   0,  15,   0, 
      0,   0,  83,  86,  95,  80, 
     79,  83,  73,  84,  73,  79, 
     78,   0,  67,  79,  76,  79, 
     82,   0, 171, 171,  83,  72, 
     69,  88, 164,   3,   0,   0, 
     80,   0,   1,   0, 233,   0, 
      0,   0, 106,   8,   0,   1, 
     89,   0,   0,   4,  70, 142, 
     32,   0,   0,   0,   0,   0, 
      8,   0,   0,   0,  95,   0, 
      0,   3, 114,  16,  16,   0, 
      0,   0,   0,   0,  95,   0, 
      0,   3, 114,  16,  16,   0, 
      1,   0,   0,   0, 103,   0, 
      0,   4, 242,  32,  16,   0, 
      0,   0,   0,   0,   1,   0, 
      0,   0, 101,   0,   0,   3, 
    242,  32,  16,   0,   1,   0, 
      0,   0, 104,   0,   0,   2, 
      2,   0,   0,   0,  56,   0, 
      0,   9, 242,   0,  16,   0, 
      0,   0,   0,   0,  86, 133, 
     32,   0,   0,   0,   0,   0, 
      1,   0,   0,   0,  70, 142, 
     32,   0,   0,   0,   0,   0, 
      5,   0,   0,   0,  50,   0, 
      0,  11, 242,   0,  16,   0, 
      0,   0,   0,   0,   6, 128, 
     32,   0,   0,   0,   0,   0, 
      1,   0,   0,   0,  70, 142, 
     32,   0,   0,   0,   0,   0, 
      4,   0,   0,   0,  70,  14, 
     16,   0,   0,   0,   0,   0, 
     50,   0,   0,  11, 242,   0, 
     16,   0,   0,   0,   0,   0, 
    166, 138,  32,   0,   0,   0, 
      0,   0,   1,   0,   0,   0, 
     70, 142,  32,   0,   0,   0, 
      0,   0,   6,   0,   0,   0, 
     70,  14,  16,   0,   0,   0, 
      0,   0,  50,   0,   0,  11, 
    242,   0,  16,   0,   0,   0, 
      0,   0, 246, 143,  32,   0, 
      0,   0,   0,   0,   1,   0, 
      0,   0,  70, 142,  32,   0, 
      0,   0,   0,   0,   7,   0, 
      0,   0,  70,  14,  16,   0, 
      0,   0,   0,   0,  56,   0, 
      0,   7, 242,   0,  16,   0, 
      0,   0,   0,   0,  70,  14, 
     16,   0,   0,   0,   0,   0, 
     86,  21,  16,   0,   0,   0, 
      0,   0,  56,   0,   0,   9, 
    242,   0,  16,   0,   1,   0, 
      0,   0,  86, 133,  32,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,  70, 142,  32,   0, 
      0,   0,   0,   0,   5,   0, 
      0,   0,  50,   0,   0,  11, 
    242,   0,  16,   0,   1,   0, 
      0,   0,   6, 128,  32,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,  70, 142,  32,   0, 
      0,   0,   0,   0,   4,   0, 
      0,   0,  70,  14,  16,   0, 
      1,   0,   0,   0,  50,   0, 
      0,  11, 242,   0,  16,   0, 
      1,   0,   0,   0, 166, 138, 
     32,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,  70, 142, 
     32,   0,   0,   0,   0,   0, 
      6,   0,   0,   0,  70,  14, 
     16,   0,   1,   0,   0,   0, 
     50,   0,   0,  11, 242,   0, 
     16,   0,   1,   0,   0,   0, 
    246, 143,  32,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
     70, 142,  32,   0,   0,   0, 
      0,   0,   7,   0,   0,   0, 
     70,  14,  16,   0,   1,   0, 
      0,   0,  50,   0,   0,   9, 
    242,   0,  16,   0,   0,   0, 
      0,   0,   6,  16,  16,   0, 
      0,   0,   0,   0,  70,  14, 
     16,   0,   1,   0,   0,   0, 
     70,  14,  16,   0,   0,   0, 
      0,   0,  56,   0,   0,   9, 
    242,   0,  16,   0,   1,   0, 
      0,   0,  86, 133,  32,   0, 
      0,   0,   0,   0,   2,   0, 
      0,   0,  70, 142,  32,   0, 
      0,   0,   0,   0,   5,   0, 
      0,   0,  50,   0,   0,  11, 
    242,   0,  16,   0,   1,   0, 
      0,   0,   6, 128,  32,   0, 
      0,   0,   0,   0,   2,   0, 
      0,   0,  70, 142,  32,   0, 
      0,   0,   0,   0,   4,   0, 
      0,   0,  70,  14,  16,   0, 
      1,   0,   0,   0,  50,   0, 
      0,  11, 242,   0,  16,   0, 
      1,   0,   0,   0, 166, 138, 
     32,   0,   0,   0,   0,   0, 
      2,   0,   0,   0,  70, 142, 
     32,   0,   0,   0,   0,   0, 
      6,   0,   0,   0,  70,  14, 
     16,   0,   1,   0,   0,   0, 
     50,   0,   0,  11, 242,   0, 
     16,   0,   1,   0,   0,   0, 
    246, 143,  32,   0,   0,   0, 
      0,   0,   2,   0,   0,   0, 
     70, 142,  32,   0,   0,   0, 
      0,   0,   7,   0,   0,   0, 
     70,  14,  16,   0,   1,   0, 
      0,   0,  50,   0,   0,   9, 
    242,   0,  16,   0,   0,   0, 
      0,   0, 166,  26,  16,   0, 
      0,   0,   0,   0,  70,  14, 
     16,   0,   1,   0,   0,   0, 
     70,  14,  16,   0,   0,   0, 
      0,   0,  56,   0,   0,   9, 
    242,   0,  16,   0,   1,   0, 
      0,   0,  86, 133,  32,   0, 
      0,   0,   0,   0,   3,   0, 
      0,   0,  70, 142,  32,   0, 
      0,   0,   0,   0,   5,   0, 
      0,   0,  50,   0,   0,  11, 
    242,   0,  16,   0,   1,   0, 
      0,   0,   6, 128,  32,   0, 
      0,   0,   0,   0,   3,   0, 
      0,   0,  70, 142,  32,   0, 
      0,   0,   0,   0,   4,   0, 
      0,   0,  70,  14,  16,   0, 
      1,   0,   0,   0,  50,   0, 
      0,  11, 242,   0,  16,   0, 
      1,   0,   0,   0, 166, 138, 
     32,   0,   0,   0,   0,   0, 
      3,   0,   0,   0,  70, 142, 
     32,   0,   0,   0,   0,   0, 
      6,   0,   0,   0,  70,  14, 
     16,   0,   1,   0,   0,   0, 
     50,   0,   0,  11, 242,   0, 
     16,   0,   1,   0,   0,   0, 
    246, 143,  32,   0,   0,   0, 
      0,   0,   3,   0,   0,   0, 
     70, 142,  32,   0,   0,   0, 
      0,   0,   7,   0,   0,   0, 
     70,  14,  16,   0,   1,   0, 
      0,   0,   0,   0,   0,   7, 
    242,  32,  16,   0,   0,   0, 
      0,   0,  70,  14,  16,   0, 
      0,   0,   0,   0,  70,  14, 
     16,   0,   1,   0,   0,   0, 
     54,   0,   0,   5, 114,  32, 
     16,   0,   1,   0,   0,   0, 
     70,  18,  16,   0,   1,   0, 
      0,   0,  54,   0,   0,   5, 
    130,  32,  16,   0,   1,   0, 
      0,   0,   1,  64,   0,   0, 
      0,   0, 128,  63,  62,   0, 
      0,   1
};
