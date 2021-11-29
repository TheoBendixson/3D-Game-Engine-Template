
// NOTE: (Ted)  A cube map position is defined by having bounds with -0.5, 0.5 on all three axes.
struct cube_map_position
{
    u32 X, Y, Z;
    vector_float_3 Offset;
};
