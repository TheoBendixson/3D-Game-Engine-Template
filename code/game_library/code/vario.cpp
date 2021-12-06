#include "vario.h"
#include "game_startup_config.cpp"
#include "game_math.cpp"
#include "cube_map.cpp"
#include "model_loader.cpp"

extern "C"
GAME_UPDATE_AND_RENDER(GameUpdateAndRender)
{
    Assert(sizeof(game_state) <= Memory->PermanentStorageSize);
    game_state *GameState = (game_state *)Memory->PermanentStorage;

    if(!Memory->IsInitialized) 
    {
        cube_map *CubeMap = &GameState->CubeMap;
        CubeMap->CountX = CUBE_COUNT_X;
        CubeMap->CountY = CUBE_COUNT_Y;
        CubeMap->CountZ = CUBE_COUNT_Z;

        for (u32 Layer = 0;
             Layer < CubeMap->CountZ;
             Layer++)
        {
            for (u32 Row = 0;
                 Row < CubeMap->CountY;
                 Row++)
            {
                for (u32 Column = 0;
                     Column < CubeMap->CountX;
                     Column++)
                {
                    u32 Value = (Layer + Row + Column)%3;
                    CubeMap->Cubes[Layer*CubeMap->CountY*CubeMap->CountX + Row*CubeMap->CountX + Column] = Value;
                }
            }
        }

        game_vertex_buffer *VertexBuffer = &RenderCommands->VertexBuffer;
        VertexBuffer->VertexCount = 0;
        VertexBuffer->ModelCount = 0;

        r32 Red[3];
        Red[0] = 1;
        Red[1] = 0;
        Red[2] = 0;

        LoadColoredCubeVertices(RenderCommands, Red);

        r32 Green[3];
        Green[0] = 0;
        Green[1] = 1;
        Green[2] = 0;

        LoadColoredCubeVertices(RenderCommands, Green);

        r32 Blue[3];
        Blue[0] = 0;
        Blue[1] = 0;
        Blue[2] = 1;

        LoadColoredCubeVertices(RenderCommands, Blue);

        Memory->IsInitialized = true;
    }

    local_persist vector_float_3 ModelRotation = { 0.0f, 0.0f, 0.0f };

    r32 CubeSideInMeters = 200.0f;
    vector_float_3 ModelScale = { CubeSideInMeters, CubeSideInMeters, CubeSideInMeters };

    vector_float_3 *ModelTranslations = GameState->ModelTranslations;
    u32 TranslationIndex = 0;

    // TODO: (Ted)  Create the notion of a push buffer here.
    //              
    //              1. Read the value of the cube map.
    //              2. If the value is some no-draw number, don't put it in the push buffer.
    //              3. Otherwise, put it in the push buffer with a translation to that point.
    cube_map *CubeMap = &GameState->CubeMap;

    for (u32 Layer = 0;
         Layer < CubeMap->CountZ;
         Layer++)
    {
        for (u32 Row = 0;
             Row < CubeMap->CountY;
             Row++)
        {
            for (u32 Column = 0;
                 Column < CubeMap->CountX;
                 Column++)
            {
                cube_map_position Pos = {};
                Pos.X = Column;
                Pos.Y = Row;
                Pos.Z = Layer;

                ModelTranslations[TranslationIndex] = ConvertCubeMapPositionToModelTranslation(Pos);
                TranslationIndex++;
            }
        }
    }

    matrix RotateX = { 1, 0,                            0,                              0,
                       0, (r32)(cos(ModelRotation.X)),  -(r32)(sin(ModelRotation.X)),   0,
                       0, (r32)(sin(ModelRotation.X)),  (r32)(cos(ModelRotation.X)),    0,
                       0, 0,                            0,                              1 };

    matrix RotateY = { (r32)(cos(ModelRotation.Y)),     0,  (r32)(sin(ModelRotation.Y)),    0,
                       0,                               1,  0,                              0,                           
                       -(r32)(sin(ModelRotation.Y)),    0,  (r32)(cos(ModelRotation.Y)),    0,
                       0,                               0,  0,                              1 };

    matrix RotateZ = { (r32)(cos(ModelRotation.Z)), -(r32)(sin(ModelRotation.Z)),   0,  0,
                       (r32)(sin(ModelRotation.Z)), (r32)(cos(ModelRotation.Z)),    0,  0,
                       0,                           0,                              1,  0,
                       0,                           0,                              0,  1 };

    matrix Scale = { ModelScale.X,  0,              0,              0,
                     0,             ModelScale.Y,   0,              0,
                     0,             0,              ModelScale.Z,   0,
                     0,             0,              0,              1 };

    u32 ViewportWidth = RenderCommands->ViewportWidth;
    u32 ViewportHeight = RenderCommands->ViewportHeight;

    r32 MiddleOfTheWorld = CubeMap->CountX*CubeSideInMeters/2.0f;

    r32 EyeX = MiddleOfTheWorld;

    vector_float_3 Eye = { EyeX,  -MiddleOfTheWorld, MiddleOfTheWorld };
    vector_float_3 At = {  EyeX,  MiddleOfTheWorld,  0.0f };
    vector_float_3 Up = {  0.0f,  0.0f,  1.0f };

    r32 Near = 1000.0f;
    r32 Far = 10000.0f;

    vector_float_3 ZAxis = Normalize(SubtractVector3(At, Eye)); 
    vector_float_3 XAxis = Normalize(CrossProduct(Up, ZAxis));
    vector_float_3 YAxis = CrossProduct(ZAxis, XAxis);

    matrix LookAt = { XAxis.X,                  YAxis.X,                    ZAxis.X,                    0,
                      XAxis.Y,                  YAxis.Y,                    ZAxis.Y,                    0,
                      XAxis.Z,                  YAxis.Z,                    ZAxis.Z,                    0,
                      -DotProduct(XAxis, Eye),  -DotProduct(YAxis, Eye),    -DotProduct(ZAxis, Eye),    1 };

    // TODO: (Ted)  This should just read the push buffer instead.
    u32 DrawCount = CUBE_MAP_SIZE;

    for (u32 InstanceIndex = 0;
         InstanceIndex < DrawCount;
         InstanceIndex++)
    {
        vector_float_3 ModelTranslation = ModelTranslations[InstanceIndex];

        matrix Translate = { 1,                  0,                  0,                  0,
                             0,                  1,                  0,                  0,
                             0,                  0,                  1,                  0,
                             ModelTranslation.X, ModelTranslation.Y, ModelTranslation.Z, 1 };

        game_constants *Constants = &RenderCommands->Constants[InstanceIndex];
        Constants->Transform = RotateX * RotateY * RotateZ * Scale * Translate;
        Constants->View = LookAt;

        Constants->Projection = { 2 * Near / ViewportWidth, 0,                         0,                           0,
                                  0,                        2 * Near / ViewportHeight, 0,                           0,
                                  0,                        0,                         Far / (Far - Near),          1,
                                  0,                        0,                         Near*Far / (Near - Far),     0 };

        Constants->LightVector = { 1.0f, -1.0f, -1.0f };

        u32 Value = GameState->CubeMap.Cubes[InstanceIndex];
        RenderCommands->InstanceModelIndices[InstanceIndex] = Value;
    }

    RenderCommands->InstancedMeshCount = DrawCount;

    clear_color *ClearColor = &RenderCommands->ClearColor;
    ClearColor->RGBA[0] = 0.183f;
    ClearColor->RGBA[1] = 0.211f;
    ClearColor->RGBA[2] = 0.249f;
    ClearColor->RGBA[3] = 1.0f;
}

