
#include "obj_file_reader.cpp"

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

#define VERTEX_LOOKUP_HASH_COUNT 10000

struct vertex_lookup
{
    u32 Hash;
    u32 VertexIndex;
};

struct temp_vertex_data
{
#if WINDOWS
    vector_float_3 Positions[4000];
    vector_float_2 UVs[4000];
    vector_float_3 Normals[6000];
#elif MACOS
    // TODO: (Ted)  Check if this is even necessary.
    vector_float3 Positions[4000];
    vector_float2 UVs[4000];
    vector_float3 Normals[6000];
#endif

    vertex_lookup VertexIndexHashmap[VERTEX_LOOKUP_HASH_COUNT];
};

extern "C"
GAME_LOAD_3D_MODELS(GameLoad3DModels)
{
    game_indexed_vertex_buffer *LoadedModelVertexBuffer = &RenderCommands->LoadedModelVertexBuffer;

    thread_context Thread = {};
    read_file_result Result = Memory->PlatformReadEntireFile(&Thread, "cartoon.obj");

    if (Result.ContentsSize > 0)
    {
        game_texture_vertex *Vertices = (game_texture_vertex *)LoadedModelVertexBuffer->Vertices;

        u32 *Indices = (u32 *)LoadedModelVertexBuffer->Indices;
        u32 IndexCount = 0;

        temp_vertex_data *TempVertexData = (temp_vertex_data *)Memory->TransientPartition.SecondaryGeneric.Data;

        char *Scan = (char *)Result.Contents;
        char *Line = Scan;

        Scan = ScanToLineStartingWithCharacter('v', Scan, Line);

        u32 PositionIndex = 0;
        b32 LoadingPositions = true;

#if WINDOWS
        
        // NOTE: (Ted)  On Windows, a newline is represented as \r\n, thus 
        //              two characters.
        u32 NewLineCharacterCount = 2;

#elif MACOS
        
        // NOTE: (Ted)  On Mac OS, a newline is represented as \n, thus 
        //              one character.
        u32 NewLineCharacterCount = 1;
#endif

        while(LoadingPositions)
        {
            // NOTE: (Ted)  At this point, the character scan pointer should be at a v
            //              at the start of a new line. This distance is from the v to
            //              the start of an actual floating point number.
            Scan += 2;

            u32 PositionDigits = 6;
            obj_scan_result ObjScan = ConstructFloatFromScan(Scan, PositionDigits);
            r32 X = ObjScan.Value; 
            Scan = ObjScan.AdvancedScan;

            Scan++;

            ObjScan = ConstructFloatFromScan(Scan, PositionDigits);
            r32 Y = ObjScan.Value;
            Scan = ObjScan.AdvancedScan;

            Scan++;

            ObjScan = ConstructFloatFromScan(Scan, PositionDigits);
            r32 Z = ObjScan.Value;
            Scan = ObjScan.AdvancedScan;

            Scan += NewLineCharacterCount;

#if WINDOWS
            vector_float_3 Position = {};
            Position.X = X;
            Position.Y = Y;
            Position.Z = Z;
#elif MACOS
            vector_float3 Position = { X, Y, Z };
#endif
            TempVertexData->Positions[PositionIndex] = Position; 

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

        u32 UVIndex = 0;
        b32 LoadingUVs = true;

        while(LoadingUVs)
        {
            // NOTE: (Ted)  At this point, the scan pointer should be at a 'vt ' with a space in front
            //              of it. The three spaces takes us to the next floating point value, which is
            //              meant to be a UV.
            Scan +=3;

            u32 UVDigits = 6;
            obj_scan_result ObjScan = ConstructFloatFromScan(Scan, UVDigits);
            r32 U = ObjScan.Value; 
            Scan = ObjScan.AdvancedScan;

            Scan++;

            ObjScan = ConstructFloatFromScan(Scan, UVDigits);
            r32 V = ObjScan.Value;
            Scan = ObjScan.AdvancedScan;

            Scan += NewLineCharacterCount;

#if WINDOWS
            vector_float_2 UV = {};
            UV.X = U;
            UV.Y = V;
#elif MACOS
            vector_float2 UV = { U, V };
#endif
            TempVertexData->UVs[UVIndex] = UV; 

            char OneAhead = *(Scan + 1);

            if (*Scan == 'v' && 
                OneAhead == 't')
            {
                UVIndex++;
            } else
            {
                LoadingUVs = false;
            }
        }

        u32 NormalIndex = 0;
        b32 LoadingNormals = true;

        while(LoadingNormals)
        {
            Scan +=3;

            u32 NormalDigits = 4;
            obj_scan_result ObjScan = ConstructFloatFromScan(Scan, NormalDigits);
            r32 X = ObjScan.Value; 
            Scan = ObjScan.AdvancedScan;

            Scan++;

            ObjScan = ConstructFloatFromScan(Scan, NormalDigits);
            r32 Y = ObjScan.Value;
            Scan = ObjScan.AdvancedScan;

            Scan++;

            ObjScan = ConstructFloatFromScan(Scan, NormalDigits);
            r32 Z = ObjScan.Value;
            Scan = ObjScan.AdvancedScan;

            Scan += NewLineCharacterCount;
            
#if WINDOWS
            vector_float_3 Normal = {};
            Normal.X = X;
            Normal.Y = Y;
            Normal.Z = Z;
#elif MACOS
            vector_float3 Normal = { X, Y, Z };
#endif
            TempVertexData->Normals[NormalIndex] = Normal; 

            char OneAhead = *(Scan + 1);

            if (*Scan == 'v' && 
                OneAhead == 'n')
            {
                NormalIndex++;
            } else
            {
                LoadingNormals = false;
            }
        }
       
        Scan = ScanToLineStartingWithCharacter('f', Scan, Line);

        // TODO: (Ted)  This starts at an 'f ' with a space to the next floating point value.
        //              I surmise that this can be used at the top of the loop just once to get to
        //              the first face value triplet.
        Scan += 2;

        u32 PositionCount = PositionIndex + 1;
        u32 UVCount = UVIndex + 1;
        u32 NormalCount = NormalIndex + 1;

        u32 VertexIndex = 0;
        b32 LoadingFaces = true;
     
        u32 VertexParseCount = 0;

        while(LoadingFaces)
        {
            obj_face_scan FaceScan = GetFaceCharactersUpToToken(Scan, '/');
            u32 PositionLookupIndex = GetFaceLookupIndexFromCharacters(FaceScan.Characters, FaceScan.CharacterCount);
            Scan = FaceScan.AdvancedScan;

            FaceScan = GetFaceCharactersUpToToken(Scan, '/');
            u32 UVLookupIndex = GetFaceLookupIndexFromCharacters(FaceScan.Characters, FaceScan.CharacterCount);
            Scan = FaceScan.AdvancedScan;

            char NormalScanToToken = ' ';

            if (VertexParseCount == 2)
            {
                NormalScanToToken = '\n';
            }

            FaceScan = GetFaceCharactersUpToToken(Scan, NormalScanToToken);
            u32 NormalLookupIndex = GetFaceLookupIndexFromCharacters(FaceScan.Characters, FaceScan.CharacterCount);
            Scan = FaceScan.AdvancedScan;

            VertexParseCount++;

            u64 HashValue = NormalCount*UVCount*NormalLookupIndex + UVCount*UVLookupIndex + PositionLookupIndex + 1;
            u32 HashIndex = (u32)(HashValue%VERTEX_LOOKUP_HASH_COUNT);

            b32 FoundOrCreatedVertexIndex = false;
            u32 AttemptCount = 0;

            while (!FoundOrCreatedVertexIndex)
            {
                vertex_lookup Retrieved = TempVertexData->VertexIndexHashmap[HashIndex];

                if (Retrieved.Hash == 0)
                {
                    // NOTE: (Ted)  A zero hash means the hash map is not occupied.
                    //              Assign the index and increment it.
                    vertex_lookup Replaced = {};
                    Replaced.Hash = HashValue;
                    Replaced.VertexIndex = VertexIndex;
                    TempVertexData->VertexIndexHashmap[HashIndex] = Replaced;
    
                    game_texture_vertex Vertex = {};

#if WINDOWS
                    vector_float_3 Position = TempVertexData->Positions[PositionLookupIndex];
                    Vertex.Position[0] = Position.X;
                    Vertex.Position[1] = Position.Y;
                    Vertex.Position[2] = Position.Z;

                    vector_float_2 UV = TempVertexData->UVs[UVLookupIndex];
                    Vertex.UV[0] = UV.X;
                    Vertex.UV[1] = UV.Y;

                    vector_float_3 Normal = TempVertexData->Normals[NormalLookupIndex];
                    Vertex.Normal[0] = Normal.X;
                    Vertex.Normal[1] = Normal.Y;
                    Vertex.Normal[2] = Normal.Z;
#elif MACOS
                    Vertex.Position = TempVertexData->Positions[PositionLookupIndex];
                    Vertex.UV = TempVertexData->UVs[UVLookupIndex];
                    Vertex.Normal = TempVertexData->Normals[NormalLookupIndex];
#endif
                    Vertices[VertexIndex] = Vertex;

                    Indices[IndexCount] = VertexIndex;
                    IndexCount++;

                    VertexIndex++;
                    FoundOrCreatedVertexIndex = true;
                } 
                else if (Retrieved.Hash == HashValue)
                {
                    // NOTE: (Ted)  There is already a vertex index with this hash value.
                    //              Use it.
                    Indices[IndexCount] = Retrieved.VertexIndex;
                    IndexCount++;

                    FoundOrCreatedVertexIndex = true;
                } 
               
                AttemptCount++;
                HashIndex = (HashValue + AttemptCount)%VERTEX_LOOKUP_HASH_COUNT;
            }

            if (*Scan == 'e')
            {
                LoadingFaces = false;
            } else if (VertexParseCount == 3)
            {
                if (*Scan != 'f')
                {
                    Scan = ScanToLineStartingWithCharacter('f', Scan, Line);
                }

                // TODO: (Ted)  Investigate potential carriage return
                Scan += 2;
                VertexParseCount = 0;
            } 
        }

        LoadedModelVertexBuffer->IndexCount = IndexCount;
        LoadedModelVertexBuffer->VertexCount = VertexIndex;
    }

    Memory->PlatformFreeFileMemory(&Thread, Result.Contents);
    ClearMemoryPartition(&Memory->TransientPartition.SecondaryGeneric);

    r32 yMin = 0.0f;
    r32 yMax = 0.0f;

    for (u32 Index = 0;
         Index < LoadedModelVertexBuffer->VertexCount;
         Index++)
    {
        game_texture_vertex *Vertices = (game_texture_vertex *)LoadedModelVertexBuffer->Vertices;
        game_texture_vertex *Vertex = &Vertices[Index];

#if WINDOWS
        Vertex->Position[0] *= -1;
        r32 yPosition = Vertex->Position[1];
#elif MACOS
        Vertex->Position.x *= -1;
        r32 yPosition = Vertex->Position.y;
#endif
    
        if (yPosition < yMin)
        {
            yMin = yPosition;
        }

        if (yPosition > yMax)
        {
            yMax = yPosition;
        }
    }

    r32 ModelHeight = yMax - yMin;
    LoadedModelVertexBuffer->ModelHeight = ModelHeight;

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
