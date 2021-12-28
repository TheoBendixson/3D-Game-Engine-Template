
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

#include "obj_file_reader.cpp"

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

struct temp_vertex_data
{
    vector_float3 Positions[1000];
    vector_float2 UVs[1000];
    vector_float3 Normals[1000];
};

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
