#include "vario.h"
#include "game_startup_config.cpp"
#include "game_math.cpp"
#include "cube_map.cpp"
#include "model_loader.cpp"
#include "camera.cpp"

#define RED_CUBE            1
#define GREEN_CUBE          2
#define BLUE_CUBE           3

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
                                Value = 0;
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

    vector_float_3 *ModelTranslations = GameState->ModelTranslations;

    u32 PushBufferIndex = 0;

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
                    ModelTranslations[PushBufferIndex] = ConvertCubeMapPositionToModelTranslation(Pos, CubeSideInMeters);
                    PushBufferIndex++;
                }
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

    vector_float_3 Eye = { EyeX,  -2*MiddleOfTheWorld, 2*MiddleOfTheWorld };
    vector_float_3 Up = {  0.0f,  0.0f,  1.0f };

    r32 Near = 1000.0f;
    r32 Far = 10000.0f;

    matrix View = {};

    b32 RotateCamera = true;

    if (RotateCamera)
    {
        local_persist r32 CameraRotation = 0.0f;
        CameraRotation += 0.010f;

        matrix CameraRotateZ = { (r32)(cos(CameraRotation)), -(r32)(sin(CameraRotation)),   0,  0,
                                 (r32)(sin(CameraRotation)), (r32)(cos(CameraRotation)),    0,  0,
                                 0,                           0,                            1,  0,
                                 0,                           0,                            0,  1 };

        r32 CameraRotationAxisOrigin = MiddleOfTheWorld;

        matrix CameraTranslate = { 1,                           0,                          0,                  0,
                                   0,                           1,                          0,                  0,
                                   0,                           0,                          1,                  0,
                                   -CameraRotationAxisOrigin,   -CameraRotationAxisOrigin,  0,                  1 };

        // NOTE: (Ted)  Any time you translate the camera, you also have to translate the look at point for the look at matrix.
        vector_float_3 At = {  (EyeX -CameraRotationAxisOrigin), (MiddleOfTheWorld - CameraRotationAxisOrigin),  0.0f };
        matrix LookAt = GenerateLookAtMatrix(At, Eye, Up);
        View = CameraTranslate*CameraRotateZ*LookAt;
    } else
    {
        vector_float_3 At = { EyeX, MiddleOfTheWorld, 0.0f };
        View = GenerateLookAtMatrix(At, Eye, Up);
    }

    matrix Projection = { 2 * Near / ViewportWidth, 0,                         0,                           0,
                          0,                        2 * Near / ViewportHeight, 0,                           0,
                          0,                        0,                         Far / (Far - Near),          1,
                          0,                        0,                         Near*Far / (Near - Far),     0 };

    u32 DrawCount = PushBufferIndex;

    mesh_instance_buffer *MeshInstanceBuffer = &RenderCommands->FlatColorMeshInstances;

    for (u32 InstanceIndex = 0;
         InstanceIndex < DrawCount;
         InstanceIndex++)
    {
        vector_float_3 ModelTranslation = ModelTranslations[InstanceIndex];

        matrix Translate = { 1,                  0,                  0,                  0,
                             0,                  1,                  0,                  0,
                             0,                  0,                  1,                  0,
                             ModelTranslation.X, ModelTranslation.Y, ModelTranslation.Z, 1 };

        mesh_instance *MeshInstance = &MeshInstanceBuffer->Meshes[InstanceIndex];
        game_constants *Constants = &MeshInstance->Constants;
        Constants->Transform = RotateX * RotateY * RotateZ * Scale * Translate;
        Constants->View = View;
        Constants->Projection = Projection;
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

    MeshInstanceBuffer->MeshCount = DrawCount;

    clear_color *ClearColor = &RenderCommands->ClearColor;
    ClearColor->RGBA[0] = 0.183f;
    ClearColor->RGBA[1] = 0.211f;
    ClearColor->RGBA[2] = 0.249f;
    ClearColor->RGBA[3] = 1.0f;
}

