
inline b32
WideStringsAreEqual(wchar_t *A, wchar_t *B)
{
    b32 Result = A==B;
    if (A && B)
    {
        while(*A && *B && (*A == *B))
        {
            A++;
            B++;
        }
        Result = (*A == 0) && (*B == 0);
    }
    return Result;
}

v2
WindowsGetWindowDimension(HWND Window)
{
    RECT CR;
    GetClientRect(Window, &CR);
    v2 Result = {(r32)(CR.right - CR.left), (r32)(CR.bottom - CR.top)};
    return Result;
}
