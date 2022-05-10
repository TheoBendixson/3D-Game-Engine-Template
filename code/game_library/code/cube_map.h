
// NOTE: (Ted)  A cube map position is defined by having bounds with -0.5, 0.5 on all three axes.
struct cube_map_position
{
    s32 X, Y, Z;
    vector_float_3 Offset;
};

#define CUBE_COUNT_X    10
#define CUBE_COUNT_Y    10
#define CUBE_COUNT_Z    2

#define CUBE_MAP_SIZE   CUBE_COUNT_X*CUBE_COUNT_Y*CUBE_COUNT_Z

struct cube_map
{
    u32 Cubes[CUBE_MAP_SIZE];
    
    u32 CountX;
    u32 CountY;
    u32 CountZ;
};
