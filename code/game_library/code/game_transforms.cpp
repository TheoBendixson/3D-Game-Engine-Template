
#if WINDOWS
internal 
matrix GenerateTranslationMatrix(vector_float_3 T)
{

    matrix Translate = { 1,   0,   0,   0,
                         0,   1,   0,   0,
                         0,   0,   1,   0,
                         T.X, T.Y, T.Z, 1 };

    return Translate;
}

internal 
matrix GenerateScaleMatrix(vector_float_3 S)
{
    matrix Scale = { S.X, 0,   0,    0,
                     0,   S.Y, 0,    0, 
                     0,   0,   S.Z,  0, 
                     0,   0,   0,    1 };

    return Scale;
}

internal
matrix GenerateXRotationMatrix(r32 R)
{
    matrix RotateX = { 1, 0,       0,       0,
                       0, cos(R),  -sin(R), 0, 
                       0, sin(R),  cos(R),  0,
                       0, 0,       0,       1 };

    return RotateX;
}

#elif MACOS
internal 
matrix_float4x4 GenerateTranslationMatrix(vector_float_3 T)
{
    matrix_float4x4 Translate = (matrix_float4x4) {{
        { 1,   0,   0,    0 },
        { 0,   1,   0,    0 },
        { 0,   0,   1,    0 },
        { T.X, T.Y, T.Z,  1 } 
    }};

    return Translate;
}

internal 
matrix_float4x4 GenerateScaleMatrix(vector_float_3 S)
{
    matrix_float4x4 Scale = (matrix_float4x4) {{
        { S.X, 0,   0,    0 },
        { 0,   S.Y, 0,    0 },
        { 0,   0,   S.Z,  0 },
        { 0,   0,   0,    1 } 
    }};

    return Scale;
}

internal
matrix_float4x4 GenerateXRotationMatrix(r32 R)
{
    matrix_float4x4 RotateX = (matrix_float4x4) {{
        { 1, 0,       0,       0 },
        { 0, cos(R),  -sin(R), 0 },
        { 0, sin(R),  cos(R),  0 },
        { 0, 0,       0,       1 }
    }};

    return RotateX;
}

#endif
