
#if WINDOWS
matrix GenerateLookAtMatrix(vector_float_3 At, vector_float_3 Eye, vector_float_3 Up)
{
    vector_float_3 ZAxis = Normalize(SubtractVector3(At, Eye)); 
    vector_float_3 XAxis = Normalize(CrossProduct(Up, ZAxis));
    vector_float_3 YAxis = CrossProduct(ZAxis, XAxis);

    matrix LookAt = { XAxis.X,                  YAxis.X,                    ZAxis.X,                    0,
                      XAxis.Y,                  YAxis.Y,                    ZAxis.Y,                    0,
                      XAxis.Z,                  YAxis.Z,                    ZAxis.Z,                    0,
                      -DotProduct(XAxis, Eye),  -DotProduct(YAxis, Eye),    -DotProduct(ZAxis, Eye),    1 };

    return LookAt;
}

#elif MACOS
matrix_float4x4 GenerateLookAtMatrix(vector_float_3 At, vector_float_3 Eye, vector_float_3 Up)
{
    vector_float_3 ZAxis = Normalize(SubtractVector3(At, Eye)); 
    vector_float_3 XAxis = Normalize(CrossProduct(Up, ZAxis));
    vector_float_3 YAxis = CrossProduct(ZAxis, XAxis);

    matrix_float4x4 LookAt = (matrix_float4x4) {{
        { XAxis.X,                  YAxis.X,                    ZAxis.X,                    0 },
        { XAxis.Y,                  YAxis.Y,                    ZAxis.Y,                    0 },
        { XAxis.Z,                  YAxis.Z,                    ZAxis.Z,                    0 },
        { -DotProduct(XAxis, Eye),  -DotProduct(YAxis, Eye),    -DotProduct(ZAxis, Eye),    1 },
    }};

    return LookAt;
}

#endif
