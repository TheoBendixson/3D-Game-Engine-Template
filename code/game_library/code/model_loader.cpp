
#define CUBE_VERTEX_COUNT   36 

#define NEGATIVE_X_FACE_NORMAL { -1.0f,  0.0f,  0.0f }
#define POSITIVE_X_FACE_NORMAL { 1.0f,  0.0f,  0.0f }

#define NEGATIVE_Y_FACE_NORMAL { 0.0f, -1.0f,  0.0f }
#define POSITIVE_Y_FACE_NORMAL { 0.0f, 1.0f,  0.0f }

#define NEGATIVE_Z_FACE_NORMAL { 0.0f, 0.0f, -1.0f }
#define POSITIVE_Z_FACE_NORMAL { 0.0f, 0.0f, 1.0f }

#define RED     { 1, 0, 0 }
#define GREEN   { 0, 1, 0 }
#define BLUE    { 0, 0, 1 }

internal r32
RealDigitFromTextCharacter(char Character)
{
    r32 Digit = 0.0f;

    if (Character == '1')
    {
        Digit = 1.0f;
    } 
    else if (Character == '2')
    {
        Digit = 2.0f;
    }
    else if (Character == '3')
    {
        Digit = 3.0f;
    }
    else if (Character == '4')
    {
        Digit = 4.0f;
    }
    else if (Character == '5')
    {
        Digit = 5.0f;
    }
    else if (Character == '6')
    {
        Digit = 6.0f;
    }
    else if (Character == '7')
    {
        Digit = 7.0f;
    }
    else if (Character == '8')
    {
        Digit = 8.0f;
    }
    else if (Character == '9')
    {
        Digit = 9.0f;
    }

    return Digit;
}

internal u32
UnsignedDigitFromTextCharacter(char Character)
{
    u32 Digit = 0;

    if(Character == '1')
    {
         Digit = 1;
    } 
    else if (Character == '2')
    {
         Digit = 2;
    }
    else if (Character == '3')
    {
         Digit = 3;
    }
    else if (Character == '4')
    {
         Digit = 4;
    }
    else if (Character == '5')
    {
         Digit = 5;
    }
    else if (Character == '6')
    {
         Digit = 6;
    }
    else if (Character == '7')
    {
         Digit = 7;
    }
    else if (Character == '8')
    {
         Digit = 8;
    }
    else if (Character == '9')
    {
         Digit = 9;
    }

    return Digit;
}

void LoadColoredCubeVertices(game_vertex_buffer *VertexBuffer, r32 *RGBColor)
{
    // XYZ Vertices, RGB Color
    // NOTE: (Ted) Counter-Clockwise Winding
    game_flat_color_vertex ColoredCube[CUBE_VERTEX_COUNT] = 
    {
        // Index 0, 2, 1
        { { -0.5f,-0.5f,-0.5f }, NEGATIVE_X_FACE_NORMAL, { RGBColor[0], RGBColor[1], RGBColor[2] } },
        { { -0.5f, 0.5f,-0.5f }, NEGATIVE_X_FACE_NORMAL, { RGBColor[0], RGBColor[1], RGBColor[2] } },
        { { -0.5f,-0.5f, 0.5f }, NEGATIVE_X_FACE_NORMAL, { RGBColor[0], RGBColor[1], RGBColor[2] } },

        // Index 1, 2, 3
        { { -0.5f,-0.5f, 0.5f }, NEGATIVE_X_FACE_NORMAL, { RGBColor[0], RGBColor[1], RGBColor[2] } },
        { { -0.5f, 0.5f,-0.5f }, NEGATIVE_X_FACE_NORMAL, { RGBColor[0], RGBColor[1], RGBColor[2] } },
        { { -0.5f, 0.5f, 0.5f }, NEGATIVE_X_FACE_NORMAL, { RGBColor[0], RGBColor[1], RGBColor[2] } },

        // Index 4, 5, 6
        { { 0.5f, -0.5f, -0.5f }, POSITIVE_X_FACE_NORMAL, { RGBColor[0], RGBColor[1], RGBColor[2] } },
        { { 0.5f, -0.5f, 0.5f },  POSITIVE_X_FACE_NORMAL, { RGBColor[0], RGBColor[1], RGBColor[2] } },
        { { 0.5f, 0.5f, -0.5f },  POSITIVE_X_FACE_NORMAL, { RGBColor[0], RGBColor[1], RGBColor[2] } },

        // Index 5, 7, 6
        { { 0.5f, -0.5f, 0.5f },  POSITIVE_X_FACE_NORMAL, { RGBColor[0], RGBColor[1], RGBColor[2] } },
        { { 0.5f, 0.5f, 0.5f },   POSITIVE_X_FACE_NORMAL, { RGBColor[0], RGBColor[1], RGBColor[2] } },
        { { 0.5f, 0.5f, -0.5f },  POSITIVE_X_FACE_NORMAL, { RGBColor[0], RGBColor[1], RGBColor[2] } },

        // Index 0, 1, 5
        { { -0.5f,-0.5f,-0.5f },  NEGATIVE_Y_FACE_NORMAL, { RGBColor[0], RGBColor[1], RGBColor[2] } },
        { { -0.5f,-0.5f, 0.5f },  NEGATIVE_Y_FACE_NORMAL, { RGBColor[0], RGBColor[1], RGBColor[2] } },
        { { 0.5f, -0.5f, 0.5f },  NEGATIVE_Y_FACE_NORMAL, { RGBColor[0], RGBColor[1], RGBColor[2] } },

        // Index 0, 5, 4
        { { -0.5f,-0.5f,-0.5f },  NEGATIVE_Y_FACE_NORMAL, { RGBColor[0], RGBColor[1], RGBColor[2] } },
        { { 0.5f, -0.5f, 0.5f },  NEGATIVE_Y_FACE_NORMAL, { RGBColor[0], RGBColor[1], RGBColor[2] } },
        { { 0.5f, -0.5f, -0.5f }, NEGATIVE_Y_FACE_NORMAL, { RGBColor[0], RGBColor[1], RGBColor[2] } },

        // Index 2, 6, 7
        { { -0.5f, 0.5f,-0.5f },  POSITIVE_Y_FACE_NORMAL, { RGBColor[0], RGBColor[1], RGBColor[2] } },
        { { 0.5f, 0.5f, -0.5f },  POSITIVE_Y_FACE_NORMAL, { RGBColor[0], RGBColor[1], RGBColor[2] } },
        { { 0.5f, 0.5f, 0.5f },   POSITIVE_Y_FACE_NORMAL, { RGBColor[0], RGBColor[1], RGBColor[2] } },

        // Index 2, 7, 3
        { { -0.5f, 0.5f,-0.5f },  POSITIVE_Y_FACE_NORMAL, { RGBColor[0], RGBColor[1], RGBColor[2] } },
        { { 0.5f, 0.5f, 0.5f },   POSITIVE_Y_FACE_NORMAL, { RGBColor[0], RGBColor[1], RGBColor[2] } },
        { { -0.5f, 0.5f, 0.5f },  POSITIVE_Y_FACE_NORMAL, { RGBColor[0], RGBColor[1], RGBColor[2] } },

        // Index 0, 4, 6
        { { -0.5f,-0.5f,-0.5f },  NEGATIVE_Z_FACE_NORMAL, { RGBColor[0], RGBColor[1], RGBColor[2] } },
        { { 0.5f, -0.5f, -0.5f }, NEGATIVE_Z_FACE_NORMAL, { RGBColor[0], RGBColor[1], RGBColor[2] } },
        { { 0.5f, 0.5f, -0.5f },  NEGATIVE_Z_FACE_NORMAL, { RGBColor[0], RGBColor[1], RGBColor[2] } },

        // Index 0, 6, 2
        { { -0.5f,-0.5f,-0.5f },  NEGATIVE_Z_FACE_NORMAL, { RGBColor[0], RGBColor[1], RGBColor[2] } },
        { { 0.5f, 0.5f, -0.5f },  NEGATIVE_Z_FACE_NORMAL, { RGBColor[0], RGBColor[1], RGBColor[2] } },
        { { -0.5f, 0.5f,-0.5f },  NEGATIVE_Z_FACE_NORMAL, { RGBColor[0], RGBColor[1], RGBColor[2] } },

        // Index 1, 3, 7
        { { -0.5f,-0.5f, 0.5f }, POSITIVE_Z_FACE_NORMAL, { RGBColor[0], RGBColor[1], RGBColor[2] } },
        { { -0.5f, 0.5f, 0.5f }, POSITIVE_Z_FACE_NORMAL, { RGBColor[0], RGBColor[1], RGBColor[2] } },
        { { 0.5f, 0.5f, 0.5f },  POSITIVE_Z_FACE_NORMAL, { RGBColor[0], RGBColor[1], RGBColor[2] } },

        // Index 1, 7, 5
        { { -0.5f,-0.5f, 0.5f }, POSITIVE_Z_FACE_NORMAL, { RGBColor[0], RGBColor[1], RGBColor[2] } },
        { { 0.5f, 0.5f, 0.5f },  POSITIVE_Z_FACE_NORMAL, { RGBColor[0], RGBColor[1], RGBColor[2] } },
        { { 0.5f, -0.5f, 0.5f }, POSITIVE_Z_FACE_NORMAL, { RGBColor[0], RGBColor[1], RGBColor[2] } }
    };

    game_flat_color_vertex *Vertices = (game_flat_color_vertex *)VertexBuffer->Vertices;

    model_range Range = {};
    Range.StartVertex = VertexBuffer->VertexCount;
    Range.VertexCount = CUBE_VERTEX_COUNT;
    VertexBuffer->ModelRanges[VertexBuffer->ModelCount] = Range;

    for (u32 Index = 0;
         Index < CUBE_VERTEX_COUNT;
         Index++)
    {
        Vertices[Index + Range.StartVertex] = ColoredCube[Index];
    }

    VertexBuffer->VertexCount += CUBE_VERTEX_COUNT;
    VertexBuffer->ModelCount += 1;
}

struct obj_scan_result
{
    r32 Value;
    char *AdvancedScan;
};

internal obj_scan_result
ConstructFloatFromScan(char *Scan, u32 SignificantDigits)
{
    obj_scan_result Result = {};
    r32 VertexValue = 0.0f;

    char First = *Scan++;
    b32 IsNegative = false;

    r32 TensDigit = 0.0f;

    if (First == '-')
    {
        IsNegative = true;

        char TensPlace = *Scan++;
        TensDigit = RealDigitFromTextCharacter(TensPlace);
    } else
    {
        TensDigit = RealDigitFromTextCharacter(First);
    }

    VertexValue += TensDigit;

    Scan++;
    char TenthsPlace = *Scan++;
    r32 TenthsDigit = RealDigitFromTextCharacter(TenthsPlace);
    VertexValue += TenthsDigit*0.1f;

    char HundredthsPlace = *Scan++;
    r32 HundredthsDigit = RealDigitFromTextCharacter(HundredthsPlace);
    VertexValue += HundredthsDigit*0.01f;

    char ThousandthsPlace = *Scan++;
    r32 ThousandthsDigit = RealDigitFromTextCharacter(ThousandthsPlace);
    VertexValue += ThousandthsDigit*0.001f;

    char TenThousandthsPlace = *Scan++;
    r32 TenThousandthsDigit = RealDigitFromTextCharacter(TenThousandthsPlace);
    VertexValue += TenThousandthsDigit*0.0001f;

    if (SignificantDigits > 4)
    {
        char HundredThousandthsPlace = *Scan++;
        r32 HundredThousandthsDigit = RealDigitFromTextCharacter(HundredThousandthsPlace);
        VertexValue += HundredThousandthsDigit*0.00001f;

        char MillionthsPlace = *Scan++;
        r32 MillionthsDigit = RealDigitFromTextCharacter(MillionthsPlace);
        VertexValue += MillionthsDigit*0.000001f;
    }

    if (IsNegative)
    {
        VertexValue *= -1;
    }

    // Do this to get to the next value.
    Scan++;

    Result.Value = VertexValue;
    Result.AdvancedScan = Scan;

    return (Result);
}

struct temp_vertex_data
{
    vector_float3 Positions[1000];
    vector_float2 UVs[1000];
    vector_float3 Normals[1000];
};

internal char*
ScanToLineStartingWithCharacter(char StartCharacter, char *Scan, char *Line)
{
    b32 Scanning = true;

    while(Scanning)
    {
        if (*Line == StartCharacter)
        {
            Scanning = false;
        } else
        {
            // NOTE: (Ted)  Skip Lines
            while(Scan++)
            {
                if(*Scan == '\n')
                {
                    break;
                }
            }

            Scan++;
            Line = Scan;
            continue;
        }
    }

    return Scan;
}

internal u32
GetFaceLookupIndexFromCharacters(char *Characters, u32 CharacterCount)
{
    u32 FaceLookupIndex = 0;
    u32 OnesDigit = 0;
    u32 TensDigit = 0;
    u32 HundredsDigit = 0;

    if(CharacterCount == 1)
    {
        char DigitCharacter = Characters[0];
        OnesDigit = UnsignedDigitFromTextCharacter(DigitCharacter);
    } else if (CharacterCount == 2)
    {
        char TensDigitCharacter = Characters[0];
        char OnesDigitCharacter = Characters[1];
        TensDigit = UnsignedDigitFromTextCharacter(TensDigitCharacter);
        OnesDigit = UnsignedDigitFromTextCharacter(OnesDigitCharacter);
    } else if (CharacterCount == 3)
    {
        char HundredsDigitCharacter = Characters[0];
        char TensDigitCharacter = Characters[1];
        char OnesDigitCharacter = Characters[2];
        HundredsDigit = UnsignedDigitFromTextCharacter(HundredsDigitCharacter);
        TensDigit = UnsignedDigitFromTextCharacter(TensDigitCharacter);
        OnesDigit = UnsignedDigitFromTextCharacter(OnesDigitCharacter);
    }
     
    FaceLookupIndex = OnesDigit + 10*TensDigit + 100*HundredsDigit;

    return (FaceLookupIndex);
}

struct obj_face_scan
{
    u32 CharacterCount;
    char Characters[3];
    char *AdvancedScan;
};

internal obj_face_scan
GetFaceCharactersUpToToken(char *Scan, char Token)
{
    obj_face_scan Result = {};
    u32 CharacterCount = 0;

    while(*Scan != Token)
    {
        Result.Characters[CharacterCount] = *Scan;
        CharacterCount++;
        Scan++;
    }

    Scan++;

    Result.CharacterCount = CharacterCount;
    Result.AdvancedScan = Scan;
    return (Result);
}

extern "C"
GAME_LOAD_3D_MODELS(GameLoad3DModels)
{
    thread_context Thread = {};
    read_file_result Result = Memory->PlatformReadEntireFile(&Thread, "testmodel.obj");

    if (Result.ContentsSize > 0)
    {
        temp_vertex_data *VertexData = (temp_vertex_data *)Memory->TransientStoragePartition.SecondaryPartition;

        char *Scan = (char *)Result.Contents;
        char *Line = Scan;

        Scan = ScanToLineStartingWithCharacter('v', Scan, Line);

        u32 PositionIndex = 0;
        b32 LoadingPositions = true;

        while(LoadingPositions)
        {
            Scan +=2;

            u32 PositionDigits = 6;
            obj_scan_result ObjScan = ConstructFloatFromScan(Scan, PositionDigits);
            r32 X = ObjScan.Value; 
            Scan = ObjScan.AdvancedScan;

            ObjScan = ConstructFloatFromScan(Scan, PositionDigits);
            r32 Y = ObjScan.Value;
            Scan = ObjScan.AdvancedScan;

            ObjScan = ConstructFloatFromScan(Scan, PositionDigits);
            r32 Z = ObjScan.Value;
            Scan = ObjScan.AdvancedScan;

            vector_float3 Position = { X, Y, Z };
            VertexData->Positions[PositionIndex] = Position; 

            char OneAhead = *(Scan + 1);

            if (*Scan == 'v' && 
                OneAhead != 't')
            {
                PositionIndex++;
            } else
            {
                LoadingPositions = false;
            }
        }

        Scan +=3;

        u32 UVIndex = 0;
        b32 LoadingUVs = true;

        while(LoadingUVs)
        {
            u32 UVDigits = 6;
            obj_scan_result ObjScan = ConstructFloatFromScan(Scan, UVDigits);
            r32 U = ObjScan.Value; 
            Scan = ObjScan.AdvancedScan;

            ObjScan = ConstructFloatFromScan(Scan, UVDigits);
            r32 V = ObjScan.Value;
            Scan = ObjScan.AdvancedScan;

            vector_float2 UV = { U, V };
            VertexData->UVs[UVIndex] = UV; 

            char OneAhead = *(Scan + 1);

            if (*Scan == 'v' && 
                OneAhead == 't')
            {
                UVIndex++;
                Scan +=3;
            } else
            {
                LoadingUVs = false;
            }
        }

        Scan +=3;

        u32 NormalIndex = 0;
        b32 LoadingNormals = true;

        while(LoadingNormals)
        {
            u32 NormalDigits = 4;
            obj_scan_result ObjScan = ConstructFloatFromScan(Scan, NormalDigits);
            r32 X = ObjScan.Value; 
            Scan = ObjScan.AdvancedScan;

            ObjScan = ConstructFloatFromScan(Scan, NormalDigits);
            r32 Y = ObjScan.Value;
            Scan = ObjScan.AdvancedScan;

            ObjScan = ConstructFloatFromScan(Scan, NormalDigits);
            r32 Z = ObjScan.Value;
            Scan = ObjScan.AdvancedScan;

            vector_float3 Normal = { X, Y, Z };
            VertexData->Normals[NormalIndex] = Normal; 

            char OneAhead = *(Scan + 1);

            if (*Scan == 'v' && 
                OneAhead == 'n')
            {
                NormalIndex++;
                Scan +=3;
            } else
            {
                LoadingNormals = false;
            }
        }
       
        Scan = ScanToLineStartingWithCharacter('f', Scan, Line);
        Scan += 2;

        b32 LoadingFaces = true;
       
        while(LoadingFaces)
        {
            obj_face_scan FaceScan = GetFaceCharactersUpToToken(Scan, '/');
            u32 PositionLookupIndex = GetFaceLookupIndexFromCharacters(FaceScan.Characters, FaceScan.CharacterCount);
            Scan = FaceScan.AdvancedScan;

            FaceScan = GetFaceCharactersUpToToken(Scan, '/');
            u32 UVLookupIndex = GetFaceLookupIndexFromCharacters(FaceScan.Characters, FaceScan.CharacterCount);
            Scan = FaceScan.AdvancedScan;

            // TODO: (Ted)  The last one needs to go up to a space instead.
            FaceScan = GetFaceCharactersUpToToken(Scan, ' ');
            u32 NormalLookupIndex = GetFaceLookupIndexFromCharacters(FaceScan.Characters, FaceScan.CharacterCount);
            Scan = FaceScan.AdvancedScan;

        }
    }

    game_vertex_buffer *FlatColorVertexBuffer = &RenderCommands->FlatColorVertexBuffer;
    FlatColorVertexBuffer->VertexCount = 0;
    FlatColorVertexBuffer->ModelCount = 0;

    r32 Red[3];
    Red[0] = 1;
    Red[1] = 0;
    Red[2] = 0;

    LoadColoredCubeVertices(FlatColorVertexBuffer, Red);

    r32 Green[3];
    Green[0] = 0;
    Green[1] = 1;
    Green[2] = 0;

    LoadColoredCubeVertices(FlatColorVertexBuffer, Green);

    r32 Blue[3];
    Blue[0] = 0;
    Blue[1] = 0;
    Blue[2] = 1;

    LoadColoredCubeVertices(FlatColorVertexBuffer, Blue);

    // NOTE: (Ted) Counter-Clockwise Winding
    game_texture_vertex TexturedCube[CUBE_VERTEX_COUNT] = 
    {
        // Index 0, 2, 1
        { { -0.5f,-0.5f,-0.5f }, NEGATIVE_X_FACE_NORMAL, { 0.0f, 0.0f } },
        { { -0.5f, 0.5f,-0.5f }, NEGATIVE_X_FACE_NORMAL, { 0.0f, 1.0f } },
        { { -0.5f,-0.5f, 0.5f }, NEGATIVE_X_FACE_NORMAL, { 1.0f, 0.0f } },

        // Index 1, 2, 3
        { { -0.5f,-0.5f, 0.5f }, NEGATIVE_X_FACE_NORMAL, { 1.0f, 0.0f } },
        { { -0.5f, 0.5f,-0.5f }, NEGATIVE_X_FACE_NORMAL, { 0.0f, 1.0f } },
        { { -0.5f, 0.5f, 0.5f }, NEGATIVE_X_FACE_NORMAL, { 1.0f, 1.0f } },

        // Index 4, 5, 6
        { { 0.5f, -0.5f, -0.5f }, POSITIVE_X_FACE_NORMAL, { 0.0f, 0.0f } },
        { { 0.5f, -0.5f, 0.5f },  POSITIVE_X_FACE_NORMAL, { 1.0f, 0.0f } },
        { { 0.5f, 0.5f, -0.5f },  POSITIVE_X_FACE_NORMAL, { 0.0f, 1.0f } },

        // Index 5, 7, 6
        { { 0.5f, -0.5f, 0.5f },  POSITIVE_X_FACE_NORMAL, { 1.0f, 0.0f } },
        { { 0.5f, 0.5f, 0.5f },   POSITIVE_X_FACE_NORMAL, { 1.0f, 1.0f } },
        { { 0.5f, 0.5f, -0.5f },  POSITIVE_X_FACE_NORMAL, { 0.0f, 1.0f } },

        // Index 0, 1, 5
        { { -0.5f,-0.5f,-0.5f },  NEGATIVE_Y_FACE_NORMAL, { 0.0f, 0.0f } },
        { { -0.5f,-0.5f, 0.5f },  NEGATIVE_Y_FACE_NORMAL, { 0.0f, 1.0f } },
        { { 0.5f, -0.5f, 0.5f },  NEGATIVE_Y_FACE_NORMAL, { 1.0f, 1.0f } },

        // Index 0, 5, 4
        { { -0.5f,-0.5f,-0.5f },  NEGATIVE_Y_FACE_NORMAL, { 0.0f, 0.0f } },
        { { 0.5f, -0.5f, 0.5f },  NEGATIVE_Y_FACE_NORMAL, { 1.0f, 1.0f } },
        { { 0.5f, -0.5f, -0.5f }, NEGATIVE_Y_FACE_NORMAL, { 1.0f, 0.0f } },

        // Index 2, 6, 7
        { { -0.5f, 0.5f,-0.5f },  POSITIVE_Y_FACE_NORMAL, { 0.0f, 0.0f } },
        { { 0.5f, 0.5f, -0.5f },  POSITIVE_Y_FACE_NORMAL, { 1.0f, 0.0f } },
        { { 0.5f, 0.5f, 0.5f },   POSITIVE_Y_FACE_NORMAL, { 1.0f, 1.0f } },

        // Index 2, 7, 3
        { { -0.5f, 0.5f,-0.5f },  POSITIVE_Y_FACE_NORMAL, { 0.0f, 0.0f } },
        { { 0.5f, 0.5f, 0.5f },   POSITIVE_Y_FACE_NORMAL, { 1.0f, 1.0f } },
        { { -0.5f, 0.5f, 0.5f },  POSITIVE_Y_FACE_NORMAL, { 0.0f, 1.0f } },

        // Index 0, 4, 6
        { { -0.5f,-0.5f,-0.5f },  NEGATIVE_Z_FACE_NORMAL, { 0.0f, 0.0f } },
        { { 0.5f, -0.5f, -0.5f }, NEGATIVE_Z_FACE_NORMAL, { 1.0f, 0.0f } },
        { { 0.5f, 0.5f, -0.5f },  NEGATIVE_Z_FACE_NORMAL, { 1.0f, 1.0f } },

        // Index 0, 6, 2
        { { -0.5f,-0.5f,-0.5f },  NEGATIVE_Z_FACE_NORMAL, { 0.0f, 0.0f } },
        { { 0.5f, 0.5f, -0.5f },  NEGATIVE_Z_FACE_NORMAL, { 1.0f, 1.0f } },
        { { -0.5f, 0.5f,-0.5f },  NEGATIVE_Z_FACE_NORMAL, { 0.0f, 1.0f } },

        // Index 1, 3, 7
        { { -0.5f,-0.5f, 0.5f }, POSITIVE_Z_FACE_NORMAL, { 0.0f, 0.0f } },
        { { -0.5f, 0.5f, 0.5f }, POSITIVE_Z_FACE_NORMAL, { 0.0f, 1.0f } },
        { { 0.5f, 0.5f, 0.5f },  POSITIVE_Z_FACE_NORMAL, { 1.0f, 1.0f } },

        // Index 1, 7, 5
        { { -0.5f,-0.5f, 0.5f }, POSITIVE_Z_FACE_NORMAL, { 0.0f, 0.0f } },
        { { 0.5f, 0.5f, 0.5f },  POSITIVE_Z_FACE_NORMAL, { 1.0f, 1.0f } },
        { { 0.5f, -0.5f, 0.5f }, POSITIVE_Z_FACE_NORMAL, { 0.0f, 1.0f } }
    };

    game_vertex_buffer *TextureVertexBuffer = &RenderCommands->TextureVertexBuffer;
    TextureVertexBuffer->VertexCount = 0;
    TextureVertexBuffer->ModelCount = 0;

    game_texture_vertex *Vertices = (game_texture_vertex *)TextureVertexBuffer->Vertices;

    model_range Range = {};
    Range.StartVertex = TextureVertexBuffer->VertexCount;
    Range.VertexCount = CUBE_VERTEX_COUNT;
    TextureVertexBuffer->ModelRanges[TextureVertexBuffer->ModelCount] = Range;

    for (u32 Index = 0;
         Index < CUBE_VERTEX_COUNT;
         Index++)
    {
        Vertices[Index + Range.StartVertex] = TexturedCube[Index];
    }

    TextureVertexBuffer->VertexCount += CUBE_VERTEX_COUNT;
    TextureVertexBuffer->ModelCount += 1;
}
