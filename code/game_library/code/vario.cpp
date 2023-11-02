#if MACOS
#include "vario.h"
#endif

#include "game_startup_config.cpp"
#include "game_math.cpp"
#include "cube_map.cpp"
#include "model_loader.cpp"
#include "camera.cpp"
#include "game_transforms.cpp"
#include "game_texture_loading.cpp"
#include "game_memory.cpp"

#define RED_CUBE            1
#define GREEN_CUBE          2
#define BLUE_CUBE           3
#define TEXTURED_CUBE       4

// TODO: (Ted)  1. Basic tile-based collision detection.
//              2. Either invert the controls or look at the world from
//                 a different orientation.

#if WINDOWS
internal void
SetupMeshConstants(game_constants *Constants, matrix RotateX, matrix RotateY, matrix RotateZ,
                   matrix Scale, matrix Translate, matrix View, matrix Projection, vector_float_3 LightVector)
{
    Constants->Transform = RotateX * RotateY * RotateZ * Scale * Translate;
    Constants->View = View;
    Constants->Projection = Projection;
    Constants->LightVector = LightVector;
}
#endif

extern "C"
GAME_UPDATE_AND_RENDER(GameUpdateAndRender)
{
    Assert(sizeof(game_state) <= Memory->PermanentStorageSize);
    game_state *GameState = (game_state *)Memory->PermanentStorage;

    if(!Memory->IsInitialized) 
    {
        GameState->DemoModes[0] = GameDemoModeStandard;
        GameState->DemoModes[1] = GameDemoModeRotateWorld;
        GameState->DemoModes[2] = GameDemoModeScalePlayer;
        GameState->DemoModes[3] = GameDemoModeRotatePlayer;

        GameState->PersonScaleMultiplier = 1.0f;

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
                        Value = TEXTURED_CUBE;
                    } else if (Layer == 1)
                    {
                        if (Row%2 == 0 && Column%3 == 0)
                        {
                            Value = (Row + Column)%4;

                            if (Value == GREEN_CUBE)
                            {
                                Value = 0;
                            }
                        }
                    }

                    CubeMap->Cubes[Layer*CubeMap->CountY*CubeMap->CountX + Row*CubeMap->CountX + Column] = Value;
                }
            }
        }

        cube_map_position P = {};
        P.X = 1;
        P.Y = 1;
        P.Z = 1;
        GameState->PlayerP = P;

        GameState->ActionSlopFrames = 0;

        Memory->IsInitialized = true;
    }

    cube_map *CubeMap = &GameState->CubeMap;
    
    game_controller_input *Input1 = &Input->Controller;

    if (Input1->Down.EndedDown &&
        GameState->ActionSlopFrames == 0)
    {
        s32 NextY = GameState->PlayerP.Y - 1;

        if (NextY >= 0 && NextY < CubeMap->CountY)
        {
            GameState->PlayerP.Y = NextY;
        }

        GameState->ActionSlopFrames = 10;
    }

    if (Input1->Up.EndedDown &&
        GameState->ActionSlopFrames == 0)
    {
        s32 NextY = GameState->PlayerP.Y + 1;

        if (NextY >= 0 && NextY < CubeMap->CountY)
        {
            GameState->PlayerP.Y = NextY;
        }

        GameState->ActionSlopFrames = 10;
    }

    if (Input1->Left.EndedDown &&
        GameState->ActionSlopFrames == 0)
    {
        s32 NextX = GameState->PlayerP.X + 1;

        if (NextX >= 0 && NextX < CubeMap->CountX)
        {
            GameState->PlayerP.X = NextX;
        }

        GameState->ActionSlopFrames = 10;
    }

    if (Input1->Right.EndedDown &&
        GameState->ActionSlopFrames == 0)
    {
        s32 NextX = GameState->PlayerP.X - 1;

        if (NextX >= 0 && NextX < CubeMap->CountX)
        {
            GameState->PlayerP.X = NextX;
        }

        GameState->ActionSlopFrames = 10;
    }

    if (Input1->A.EndedDown &&
        GameState->ActionSlopFrames == 0)
    {
        GameState->DemoModeIndex += 1;

        if (GameState->DemoModeIndex > 3)
        {
            GameState->DemoModeIndex = 0;
        }

        GameState->ActionSlopFrames = 30;
    }

    if (GameState->ActionSlopFrames > 0)
    {
        GameState->ActionSlopFrames--;
    }

    game_demo_mode DemoMode = GameState->DemoModes[GameState->DemoModeIndex];

    local_persist vector_float_3 ModelRotation = { 0.0f, 0.0f, 0.0f };

    r32 CubeSideInMeters = 200.0f;


    vector_float_3 ModelScale = { CubeSideInMeters, CubeSideInMeters, CubeSideInMeters };

    push_buffer *ColoredCubePushBuffer = &RenderCommands->ColoredCubePushBuffer;
    ColoredCubePushBuffer->DrawCount = 0;

    push_buffer *TexturedCubePushBuffer = &RenderCommands->TexturedCubePushBuffer;
    TexturedCubePushBuffer->DrawCount = 0;

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

    matrix Scale = { ModelScale.X,  0,              0,              0,
                     0,             ModelScale.Y,   0,              0,
                     0,             0,              ModelScale.Z,   0,
                     0,             0,              0,              1 };

    matrix View = {};
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

    matrix_float4x4 Scale = (matrix_float4x4) {{
        { ModelScale.X, 0,            0,             0 },
        { 0,            ModelScale.Y, 0,             0 },
        { 0,            0,            ModelScale.Z,  0 },
        { 0,            0,            0,             1 } 
    }};

    matrix_float4x4 View = {};
#endif

    u32 ViewportWidth = RenderCommands->ViewportWidth;
    u32 ViewportHeight = RenderCommands->ViewportHeight;

    r32 MiddleOfTheWorld = CubeMap->CountX*CubeSideInMeters/2.0f;

    r32 EyeX = MiddleOfTheWorld;

    // TODO: (Ted)  Those who build on both Mac OS and Windows will notice that this isn't
    //              the same viewing distance on both platforms. This will need to change.
    //
    //              I think it depends more on the Viewport Width / Viewport Height than
    //              I previously thought.
    vector_float_3 Eye = { EyeX,  (r32)(-0.5*MiddleOfTheWorld), (r32)(1.5*MiddleOfTheWorld) };
    vector_float_3 Up = {  0.0f,  0.0f,  1.0f };

    r32 Near = 1000.0f;
    r32 Far = 10000.0f;


    b32 RotateCamera = false;

    if (DemoMode == GameDemoModeRotateWorld)
    {
        RotateCamera = true;
    }

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
        matrix_float4x4 LookAtCamera = matrix_multiply(LookAt, CameraRotateZ);
        View = matrix_multiply(LookAtCamera, CameraTranslate);
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
    vector_float_3 LightVector = { 1.0f, -0.5f, -0.5f };
#elif MACOS
    matrix_float4x4 Projection = (matrix_float4x4) {{
        { 2 * Near / ViewportWidth, 0,                         0,                           0 },
        { 0,                        2 * Near / ViewportHeight, 0,                           0 },
        { 0,                        0,                         Far / (Far - Near),          1 },
        { 0,                        0,                         Near*Far / (Near - Far),     0 },
    }};
    vector_float3 LightVector = { 1.0f, -0.5f, -0.5f };
#endif

    mesh_instance_buffer *FlatColorMeshInstanceBuffer = &RenderCommands->FlatColorMeshInstances;

    for (u32 InstanceIndex = 0;
         InstanceIndex < ColoredCubePushBuffer->DrawCount;
         InstanceIndex++)
    {
        vector_float_3 ModelTranslation = ColoredCubePushBuffer->Translations[InstanceIndex];
        mesh_instance *MeshInstance = &FlatColorMeshInstanceBuffer->Meshes[InstanceIndex];

#if WINDOWS
        matrix Translate = GenerateTranslationMatrix(ModelTranslation);
        SetupMeshConstants(&MeshInstance->Constants, RotateX, RotateY, RotateZ,
                           Scale, Translate, View, Projection, LightVector);
#elif MACOS
        matrix_float4x4 Translate = GenerateTranslationMatrix(ModelTranslation);
        instance_uniforms *Uniforms = &MeshInstance->Uniforms;
        Uniforms->Transform = matrix_multiply(Translate, Scale);
#endif

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

    mesh_instance_buffer *TexturedMeshInstanceBuffer = &RenderCommands->TexturedMeshInstances;

    for (u32 InstanceIndex = 0;
         InstanceIndex < TexturedCubePushBuffer->DrawCount;
         InstanceIndex++)
    {
        vector_float_3 ModelTranslation = TexturedCubePushBuffer->Translations[InstanceIndex];
        mesh_instance *MeshInstance = &TexturedMeshInstanceBuffer->Meshes[InstanceIndex];

#if WINDOWS
        matrix Translate = GenerateTranslationMatrix(ModelTranslation);
        SetupMeshConstants(&MeshInstance->Constants, RotateX, RotateY, RotateZ,
                           Scale, Translate, View, Projection, LightVector);

#elif MACOS
        matrix_float4x4 Translate = GenerateTranslationMatrix(ModelTranslation);
        instance_uniforms *Uniforms = &MeshInstance->Uniforms;
        Uniforms->Transform = matrix_multiply(Translate, Scale);
#endif
        MeshInstance->ModelIndex = 0;
    }

    TexturedMeshInstanceBuffer->MeshCount = TexturedCubePushBuffer->DrawCount;

    {
        mesh_instance_buffer *LoadedModelMeshInstanceBuffer = &RenderCommands->LoadedModelMeshInstances;

        if (DemoMode == GameDemoModeScalePlayer)
        {
            if (GameState->ReverseScale)
            {
                GameState->PersonScaleMultiplier -= 0.01f;

                if (GameState->PersonScaleMultiplier < 1.0f)
                {
                    GameState->ReverseScale = false;
                }
            } else
            {
                GameState->PersonScaleMultiplier += 0.01f;

                if (GameState->PersonScaleMultiplier > 3.0f)
                {
                    GameState->ReverseScale = true;
                }
            }


        } else
        {
            GameState->ReverseScale = false;
            GameState->PersonScaleMultiplier = 1.0f;
        }

        r32 PersonSide = 300.0f*GameState->PersonScaleMultiplier;

        vector_float_3 PersonScale = { PersonSide, PersonSide, PersonSide };
        GameState->PlayerP.Offset.Z = -0.40f;
        vector_float_3 Translation = ConvertCubeMapPositionToModelTranslation(GameState->PlayerP, CubeSideInMeters);
        mesh_instance *MeshInstance = &LoadedModelMeshInstanceBuffer->Meshes[0];
#if WINDOWS
        matrix Translate = GenerateTranslationMatrix(Translation);
        matrix RotateX = GenerateXRotationMatrix((r32)(M_PI*1.5));
        matrix LoadedModelScale = GenerateScaleMatrix(PersonScale);

        SetupMeshConstants(&MeshInstance->Constants, RotateX, RotateY, RotateZ,
                           LoadedModelScale, Translate, View, Projection, LightVector);
#elif MACOS
        matrix_float4x4 Translate = GenerateTranslationMatrix(Translation);
        matrix_float4x4 Rotate = GenerateXRotationMatrix(M_PI*1.5);
        instance_uniforms *Uniforms = &MeshInstance->Uniforms;
        matrix_float4x4 LoadedModelScale = GenerateScaleMatrix(PersonScale);
        Uniforms->Transform = matrix_multiply(Translate, matrix_multiply(Rotate, LoadedModelScale));
#endif

        MeshInstance->ModelIndex = 0;
        LoadedModelMeshInstanceBuffer->MeshCount = 1;
    }

#if MACOS
    game_constants *Constants = &RenderCommands->Constants;
    Constants->View = View;
    Constants->Projection = Projection;
    Constants->LightVector = { 1.0f, -0.5f, -0.5f };
#endif

    clear_color *ClearColor = &RenderCommands->ClearColor;
    ClearColor->RGBA[0] = 0.183f;
    ClearColor->RGBA[1] = 0.211f;
    ClearColor->RGBA[2] = 0.249f;
    ClearColor->RGBA[3] = 1.0f;
}

