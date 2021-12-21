#include "vario.h"
#include "game_startup_config.cpp"
#include "game_math.cpp"
#include "cube_map.cpp"
#include "model_loader.cpp"
#include "camera.cpp"

#define RED_CUBE            1
#define GREEN_CUBE          2
#define BLUE_CUBE           3
#define TEXTURED_CUBE       4

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

#elif MACOS
internal 
matrix_float4x4 GenerateTranslationMatrix(vector_float_3 T)
{
    matrix_float4x4 Translate = (matrix_float4x4) {{
        { 1,   0,   0,    0 },     // Column 0.
        { 0,   1,   0,    0 },     // Column 1.
        { 0,   0,   1,    0 },     // Column 2.
        { T.X, T.Y, T.Z,  1 }      // Column 3.
    }};

    return Translate;
}
#endif

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
                    u32 Value = 0; 
                    
                    if (Layer == 0)
                    {
                        Value = GREEN_CUBE;
                    } else if (Layer == 1)
                    {
                        if (Row%2 == 0 && Column%3 == 0)
                        {
                            Value = (Row + Column)%4;

                            if (Value == GREEN_CUBE)
                            {
                                Value = TEXTURED_CUBE;
                            }
                        }
                    }

                    CubeMap->Cubes[Layer*CubeMap->CountY*CubeMap->CountX + Row*CubeMap->CountX + Column] = Value;
                }
            }
        }

        Memory->IsInitialized = true;
    }

    local_persist vector_float_3 ModelRotation = { 0.0f, 0.0f, 0.0f };

    r32 CubeSideInMeters = 200.0f;
    vector_float_3 ModelScale = { CubeSideInMeters, CubeSideInMeters, CubeSideInMeters };

    push_buffer *ColoredCubePushBuffer = &GameState->ColoredCubePushBuffer;
    ColoredCubePushBuffer->DrawCount = 0;

    push_buffer *TexturedCubePushBuffer = &GameState->TexturedCubePushBuffer;
    TexturedCubePushBuffer->DrawCount = 0;

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

                u32 CubeValue = CubeMap->Cubes[Layer*CubeMap->CountY*CubeMap->CountX + Row*CubeMap->CountX + Column];

                if (CubeValue > 0)
                {
                    vector_float_3 Translation = ConvertCubeMapPositionToModelTranslation(Pos, CubeSideInMeters);

                    if (CubeValue == TEXTURED_CUBE)
                    {
                        TexturedCubePushBuffer->Translations[TexturedCubePushBuffer->DrawCount] = Translation;
                        TexturedCubePushBuffer->DrawCount++;
                    } else
                    {
                        ColoredCubePushBuffer->Translations[ColoredCubePushBuffer->DrawCount] = Translation;
                        ColoredCubePushBuffer->DrawCount++;
                    }
                } 
            }
        }
    }

// NOTE: (Ted)  Windows is row-major. Metal is column-major. Ugh.
//
// TODO: (Ted)  Unify this between Windows and Metal if possible.
#if WINDOWS
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

#elif MACOS
    matrix_float4x4 
        RotateX = (matrix_float4x4) {{
            { 1, 0,                            0,                            0 },
            { 0, (r32)(cos(ModelRotation.X)),  -(r32)(sin(ModelRotation.X)), 0 },
            { 0, (r32)(sin(ModelRotation.X)),  (r32)(cos(ModelRotation.X)),  0 },
            { 0, 0,                            0,                            1 }
        }};

    matrix_float4x4 
        RotateY = (matrix_float4x4) {{ 
            { (r32)(cos(ModelRotation.Y)),     0,  (r32)(sin(ModelRotation.Y)),    0 },
            { 0,                               1,  0,                              0 },
            { -(r32)(sin(ModelRotation.Y)),    0,  (r32)(cos(ModelRotation.Y)),    0 },
            { 0,                               0,  0,                              1 },
        }};

    matrix_float4x4 
        RotateZ = (matrix_float4x4) {{
            { (r32)(cos(ModelRotation.Z)), -(r32)(sin(ModelRotation.Z)),   0,  0 },
            { (r32)(sin(ModelRotation.Z)), (r32)(cos(ModelRotation.Z)),    0,  0 },
            { 0,                           0,                              1,  0 },
            { 0,                           0,                              0,  1 },
        }};
#endif

#if WINDOWS
    matrix Scale = { ModelScale.X,  0,              0,              0,
                     0,             ModelScale.Y,   0,              0,
                     0,             0,              ModelScale.Z,   0,
                     0,             0,              0,              1 };
#elif MACOS
    matrix_float4x4 Scale = (matrix_float4x4) {{
        { ModelScale.X, 0,            0,             0 },
        { 0,            ModelScale.Y, 0,             0 },
        { 0,            0,            ModelScale.Z,  0 },
        { 0,            0,            0,             1 } 
    }};
#endif

    u32 ViewportWidth = RenderCommands->ViewportWidth;
    u32 ViewportHeight = RenderCommands->ViewportHeight;

    r32 MiddleOfTheWorld = CubeMap->CountX*CubeSideInMeters/2.0f;

    r32 EyeX = MiddleOfTheWorld;

    vector_float_3 Eye = { EyeX,  -2*MiddleOfTheWorld, 2*MiddleOfTheWorld };
    vector_float_3 Up = {  0.0f,  0.0f,  1.0f };

    r32 Near = 1000.0f;
    r32 Far = 10000.0f;

#if WINDOWS
    matrix View = {};
#elif MACOS
    matrix_float4x4 View = {};
#endif

    b32 RotateCamera = false;

    if (RotateCamera)
    {
        local_persist r32 CameraRotation = 0.0f;
        CameraRotation += 0.010f;

#if WINDOWS
        matrix CameraRotateZ = { (r32)(cos(CameraRotation)), -(r32)(sin(CameraRotation)),   0,  0,
                                 (r32)(sin(CameraRotation)), (r32)(cos(CameraRotation)),    0,  0,
                                 0,                           0,                            1,  0,
                                 0,                           0,                            0,  1 };
#elif MACOS
        matrix_float4x4 CameraRotateZ = (matrix_float4x4) {{
            { (r32)(cos(CameraRotation)), -(r32)(sin(CameraRotation)),   0,  0 },
            { (r32)(sin(CameraRotation)), (r32)(cos(CameraRotation)),    0,  0 },
            { 0,                           0,                            1,  0 },
            { 0,                           0,                            0,  1 },
        }};
#endif

        r32 CameraRotationAxisOrigin = MiddleOfTheWorld;

#if WINDOWS
        matrix CameraTranslate = { 1,                           0,                          0,                  0,
                                   0,                           1,                          0,                  0,
                                   0,                           0,                          1,                  0,
                                   -CameraRotationAxisOrigin,   -CameraRotationAxisOrigin,  0,                  1 };
#elif MACOS
        matrix_float4x4 CameraTranslate = (matrix_float4x4) {{
            { 1,                           0,                          0,                  0 },
            { 0,                           1,                          0,                  0 },
            { 0,                           0,                          1,                  0 },
            { -CameraRotationAxisOrigin,   -CameraRotationAxisOrigin,  0,                  1 },
        }};
#endif

        // NOTE: (Ted)  Any time you translate the camera, you also have to translate the look at point for the look at 
        //              matrix.
        vector_float_3 At = {  (EyeX -CameraRotationAxisOrigin), (MiddleOfTheWorld - CameraRotationAxisOrigin),  0.0f };
#if WINDOWS
        matrix LookAt = GenerateLookAtMatrix(At, Eye, Up);
        View = CameraTranslate*CameraRotateZ*LookAt;
#elif MACOS
        matrix_float4x4 LookAt = GenerateLookAtMatrix(At, Eye, Up);
        // TODO: (Ted)  Calculate the view matrix here with a series of multiplies 
#endif

    } else
    {
        vector_float_3 At = { EyeX, MiddleOfTheWorld, 0.0f };
        View = GenerateLookAtMatrix(At, Eye, Up);
    }

#if WINDOWS
    matrix Projection = { 2 * Near / ViewportWidth, 0,                         0,                           0,
                          0,                        2 * Near / ViewportHeight, 0,                           0,
                          0,                        0,                         Far / (Far - Near),          1,
                          0,                        0,                         Near*Far / (Near - Far),     0 };
#elif MACOS
    matrix_float4x4 Projection = (matrix_float4x4) {{
        { 2 * Near / ViewportWidth, 0,                         0,                           0 },
        { 0,                        2 * Near / ViewportHeight, 0,                           0 },
        { 0,                        0,                         Far / (Far - Near),          1 },
        { 0,                        0,                         Near*Far / (Near - Far),     0 },
    }};

#endif

    mesh_instance_buffer *FlatColorMeshInstanceBuffer = &RenderCommands->FlatColorMeshInstances;

    for (u32 InstanceIndex = 0;
         InstanceIndex < ColoredCubePushBuffer->DrawCount;
         InstanceIndex++)
    {
        vector_float_3 ModelTranslation = ColoredCubePushBuffer->Translations[InstanceIndex];
#if WINDOWS
        matrix Translate = GenerateTranslationMatrix(ModelTranslation);
#elif MACOS
        matrix_float4x4 Translate = GenerateTranslationMatrix(ModelTranslation);
#endif
        mesh_instance *MeshInstance = &FlatColorMeshInstanceBuffer->Meshes[InstanceIndex];
        game_constants *Constants = &MeshInstance->Constants;
#if WINDOWS
        Constants->Transform = RotateX * RotateY * RotateZ * Scale * Translate;
#elif MACOS
        Constants->Transform = matrix_multiply(Translate, Scale);
#endif

        Constants->View = View;
        Constants->Projection = Projection;

#if WINDOWS
        //Constants->LightVector = { 1.0f, -0.5f, -0.5f };
#endif
        Constants->LightVector = { 1.0f, -0.5f, -0.5f };

        u32 CubeValue = GameState->CubeMap.Cubes[InstanceIndex];

        u32 ModelIndex = 0;

        if (CubeValue == 2)
        {
            ModelIndex = 1;
        } else if (CubeValue == 3)
        {
            ModelIndex = 2;
        }

        MeshInstance->ModelIndex = ModelIndex;
    }

    FlatColorMeshInstanceBuffer->MeshCount = ColoredCubePushBuffer->DrawCount;

// TODO: (Ted)  Port this to Apple/Metal
#if WINDOWS
    mesh_instance_buffer *TexturedMeshInstanceBuffer = &RenderCommands->TexturedMeshInstances;

    for (u32 InstanceIndex = 0;
         InstanceIndex < TexturedCubePushBuffer->DrawCount;
         InstanceIndex++)
    {
        vector_float_3 ModelTranslation = TexturedCubePushBuffer->Translations[InstanceIndex];
        matrix Translate = GenerateTranslationMatrix(ModelTranslation);
        mesh_instance *MeshInstance = &TexturedMeshInstanceBuffer->Meshes[InstanceIndex];
        game_constants *Constants = &MeshInstance->Constants;
        Constants->Transform = RotateX * RotateY * RotateZ * Scale * Translate;
        Constants->View = View;
        Constants->Projection = Projection;

        Constants->LightVector = { 1.0f, -0.5f, -0.5f };

        MeshInstance->ModelIndex = 0;
    }

    TexturedMeshInstanceBuffer->MeshCount = TexturedCubePushBuffer->DrawCount;
#endif

    clear_color *ClearColor = &RenderCommands->ClearColor;
    ClearColor->RGBA[0] = 0.183f;
    ClearColor->RGBA[1] = 0.211f;
    ClearColor->RGBA[2] = 0.249f;
    ClearColor->RGBA[3] = 1.0f;
}

