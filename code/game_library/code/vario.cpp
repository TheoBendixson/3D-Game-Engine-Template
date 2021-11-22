#include "vario.h"
#include "game_startup_config.cpp"
#include "game_math.cpp"

#define CUBE_VERTEX_COUNT   36 

#define FACE_NORMAL_1 { 0.0f, 0.0f, -1.0f }
#define FACE_NORMAL_2 { 0.0f, 0.0f, 1.0f }
#define FACE_NORMAL_3 { -1.0f,  0.0f,  0.0f }
#define FACE_NORMAL_4 { 1.0f,  0.0f,  0.0f }
#define FACE_NORMAL_5 { 0.0f, -1.0f,  0.0f }
#define FACE_NORMAL_6 { 0.0f, 1.0f,  0.0f }

#define RED { 1, 0, 0 }

internal
vector_float_3 SubtractVector3(vector_float_3 LeftHandSide, vector_float_3 RightHandSide)
{
    vector_float_3 Result = {};
    Result.X = RightHandSide.X - LeftHandSide.X;
    Result.Y = RightHandSide.Y - LeftHandSide.Y;
    Result.Z = RightHandSide.Z - LeftHandSide.Z;
    return (Result);
}

internal
vector_float_3 Normalize(vector_float_3 Vector)
{
    r32 Magnitude = sqrt((Vector.X*Vector.X) + (Vector.Y*Vector.Y) + (Vector.Z*Vector.Z));
    vector_float_3 Result = {};
    Result.X = Vector.X/Magnitude;
    Result.Y = Vector.Y/Magnitude;
    Result.Z = Vector.Z/Magnitude;
    return (Result);
}

internal
vector_float_3 CrossProduct(vector_float_3 A, vector_float_3 B)
{
    vector_float_3 Result = {};
    Result.X = (A.Y*B.Z) - (A.Z*B.Y);
    Result.Y = (A.Z*B.X) - (A.X*B.Z);
    Result.Z = (A.X*B.Y) - (A.Y*B.X);
    return (Result);
}

extern "C"
GAME_UPDATE_AND_RENDER(GameUpdateAndRender)
{
    // XYZ Vertices, RGB Color
    game_vertex ColoredCube[CUBE_VERTEX_COUNT] = 
    {
        { { -0.5f, -0.5f, -0.5f },  FACE_NORMAL_1, RED },
        { { 0.5f, -0.5f, -0.5f },   FACE_NORMAL_1, RED },
        { { 0.5f,  0.5f, -0.5f },   FACE_NORMAL_1, RED },
        { { 0.5f,  0.5f, -0.5f },   FACE_NORMAL_1, RED },
        { { -0.5f,  0.5f, -0.5f},   FACE_NORMAL_1, RED },
        { { -0.5f, -0.5f, -0.5f },  FACE_NORMAL_1, RED },

        { { -0.5f, -0.5f,  0.5f },  FACE_NORMAL_2, RED },
        { { 0.5f, -0.5f,  0.5f },   FACE_NORMAL_2, RED },
        { { 0.5f,  0.5f,  0.5f },   FACE_NORMAL_2, RED },
        { { 0.5f,  0.5f,  0.5f },   FACE_NORMAL_2, RED },
        { { -0.5f,  0.5f,  0.5f },  FACE_NORMAL_2, RED },
        { { -0.5f, -0.5f,  0.5f },  FACE_NORMAL_2, RED },

        { { -0.5f,  0.5f,  0.5f },  FACE_NORMAL_3, RED },
        { { -0.5f,  0.5f, -0.5f },  FACE_NORMAL_3, RED },
        { { -0.5f, -0.5f, -0.5f },  FACE_NORMAL_3, RED },
        { { -0.5f, -0.5f, -0.5f },  FACE_NORMAL_3, RED },
        { { -0.5f, -0.5f,  0.5f },  FACE_NORMAL_3, RED },
        { { -0.5f,  0.5f,  0.5f },  FACE_NORMAL_3, RED },

        { { 0.5f,  0.5f,  0.5f },   FACE_NORMAL_4, RED },
        { { 0.5f,  0.5f, -0.5f },   FACE_NORMAL_4, RED },
        { { 0.5f, -0.5f, -0.5f },   FACE_NORMAL_4, RED },
        { { 0.5f, -0.5f, -0.5f },   FACE_NORMAL_4, RED },
        { { 0.5f, -0.5f,  0.5f },   FACE_NORMAL_4, RED },
        { { 0.5f,  0.5f,  0.5f },   FACE_NORMAL_4, RED },

        { { -0.5f, -0.5f, -0.5f },  FACE_NORMAL_5, RED }, 
        { { 0.5f, -0.5f, -0.5f },   FACE_NORMAL_5, RED }, 
        { { 0.5f, -0.5f,  0.5f },   FACE_NORMAL_5, RED },
        { { 0.5f, -0.5f,  0.5f },   FACE_NORMAL_5, RED },
        { { -0.5f, -0.5f,  0.5f },  FACE_NORMAL_5, RED },
        { { -0.5f, -0.5f, -0.5f },  FACE_NORMAL_5, RED },

        { { -0.5f,  0.5f, -0.5f },  FACE_NORMAL_6, RED },
        { { 0.5f,  0.5f, -0.5f },   FACE_NORMAL_6, RED },
        { { 0.5f,  0.5f,  0.5f },   FACE_NORMAL_6, RED },
        { { 0.5f,  0.5f,  0.5f },   FACE_NORMAL_6, RED },
        { { -0.5f,  0.5f,  0.5f },  FACE_NORMAL_6, RED },
        { { -0.5f,  0.5f, -0.5f },  FACE_NORMAL_6, RED },
    };

    game_vertex_buffer *VertexBuffer = &RenderCommands->VertexBuffer;
    game_vertex *Vertices = VertexBuffer->Vertices;

    for (u32 Index = 0;
         Index < CUBE_VERTEX_COUNT;
         Index++)
    {
        Vertices[Index] = ColoredCube[Index];
    }

    VertexBuffer->VertexCount = CUBE_VERTEX_COUNT;

    r32 Near = 1000.0f;
    r32 Far = 1000000.0f;

    local_persist vector_float_3 ModelRotation = { 0.0f, 0.0f, 0.0f };
    vector_float_3 ModelScale = { 1.0f, 1.0f, 1.0f };

    vector_float_3 ModelTranslations[3];
    /*
    ModelTranslations[0] = { 0.0f, 0.0f, 1500.0f };
    ModelTranslations[1] = { 800.0f, 0.0f, 1500.0f };
    ModelTranslations[2] = { -800.0f, 0.0f, 1500.0f };
    */
    ModelTranslations[0] = { 0.0f, 0.0f, 0.0f };
    ModelTranslations[1] = { 800.0f, 0.0f, 0.0f };
    ModelTranslations[2] = { -800.0f, 0.0f, 0.0f };

    ModelRotation.X += 0.005f;
    ModelRotation.Y += 0.009f;
    ModelRotation.Z += 0.001f;

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

    vector_float_3 CameraPosition = { 0.0f, 0.0f, -3.0f };
    vector_float_3 CameraTarget = { 0.0f, 0.0f, 0.0f };
    vector_float_3 CameraDirectionScaled = SubtractVector3(CameraPosition, CameraTarget);
    vector_float_3 CameraDirection = Normalize(CameraDirectionScaled);

    vector_float_3 Up = { 0.0f, 1.0f, 0.0f };

    vector_float_3 CrossUpAndCameraDirection = CrossProduct(Up, CameraDirection);
    vector_float_3 CameraRight = Normalize(CrossUpAndCameraDirection);
    vector_float_3 CameraUp = CrossProduct(CameraDirection, CameraRight);

    matrix LookAtAxes = { CameraRight.X,        CameraRight.Y,      CameraRight.Z,      0,
                          CameraUp.X,           CameraUp.Y,         CameraUp.Z,         0,
                          CameraDirection.X,    CameraDirection.Y,  CameraDirection.Z,  0,
                          0,                    0,                  0,                  1 };

    /*
    matrix LookAtPositions = { 1, 0, 0, -CameraPosition.X,
                               0, 1, 0, -CameraPosition.Y,
                               0, 0, 1, -CameraPosition.Z,
                               0, 0, 0, 1                   };
    */

    matrix LookAtPositions = { 1, 0, 0, 0,
                               0, 1, 0, 0,
                               0, 0, 1, 0,
                               -CameraPosition.X, -CameraPosition.Y,  -CameraPosition.Z, 1               };

    matrix LookAt = LookAtAxes * LookAtPositions;

    for (u32 InstanceIndex = 0;
         InstanceIndex < 3;
         InstanceIndex++)
    {
        vector_float_3 ModelTranslation = ModelTranslations[InstanceIndex];

        matrix Translate = { 1,                  0,                  0,                  0,
                             0,                  1,                  0,                  0,
                             0,                  0,                  1,                  0,
                             ModelTranslation.X, ModelTranslation.Y, ModelTranslation.Z, 1 };

        game_constants *Constants = &RenderCommands->Constants[InstanceIndex];
        Constants->Transform = RotateX * RotateY * RotateZ * Scale * Translate * LookAt;
        Constants->Projection = { (2 * Near / ViewportWidth), 0,                           0,                           0,
                                  0,                          (2 * Near / ViewportHeight), 0,                           0,
                                  0,                          0,                           (Far / (Far - Near)),        1,
                                  0,                          0,                           (Near * Far / (Near - Far)), 0 };

        Constants->LightVector = { 1.0f, -1.0f, 1.0f };
    }

    RenderCommands->InstancedMeshCount = 3;
}

