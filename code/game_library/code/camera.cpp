
matrix GenerateLookAtMatrix(vector_float_3 At, vector_float_3 Eye, vector_float_3 Up)
{
    vector_float_3 ZAxis = Normalize(SubtractVector3(At, Eye)); 
    vector_float_3 XAxis = Normalize(CrossProduct(Up, ZAxis));
    vector_float_3 YAxis = CrossProduct(ZAxis, XAxis);

#if WINDOWS
    matrix LookAt = { XAxis.X,                  YAxis.X,                    ZAxis.X,                    0,
                      XAxis.Y,                  YAxis.Y,                    ZAxis.Y,                    0,
                      XAxis.Z,                  YAxis.Z,                    ZAxis.Z,                    0,
                      -DotProduct(XAxis, Eye),  -DotProduct(YAxis, Eye),    -DotProduct(ZAxis, Eye),    1 };
#elif MACOS
    matrix LookAt = { XAxis.X,                  XAxis.Y,                    XAxis.Z, -DotProduct(XAxis, Eye),
                      YAxis.X,                  YAxis.Y,                    YAxis.Z, -DotProduct(YAxis, Eye),
                      ZAxis.X,                  ZAxis.Y,                    ZAxis.Z, -DotProduct(ZAxis, Eye),
                      0,                        0,                          0,       1 };

#endif

    return LookAt;
}
