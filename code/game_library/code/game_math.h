
struct vector_float_3
{
    r32 X, Y, Z;
};

struct vector_float_2
{
    r32 X, Y;
};

struct matrix
{
    r32 m[4][4];
};

struct vector_4
{
    r32 X;
    r32 Y;
    r32 Z;
    r32 W;
};

union v2
{
    struct { r32 X, Y; };
    r32 E[2];
};

inline v2 V2(r32 X, r32 Y) { v2 R; R.X = X; R.Y = Y; return R; }
inline v2 operator+(v2 A, v2 B) { return V2(A.X + B.X, A.Y + B.Y); }
inline v2 operator-(v2 A, v2 B) { return V2(A.X - B.X, A.Y - B.Y); }
inline v2 operator*(r32 A, v2 B) { return V2(A * B.X, A * B.Y); }
inline v2 operator*(v2 A, r32 B) { return V2(A.X * B, A.Y * B); }
