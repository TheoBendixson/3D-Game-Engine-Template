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
    vector_float_3 ModelScale = { 400.0f, 400.0f, 400.0f };

    vector_float_3 ModelTranslations[3];
    ModelTranslations[0] = { 0.0f, 0.0f, 1500.0f };
    ModelTranslations[1] = { 800.0f, 0.0f, 1500.0f };
    ModelTranslations[2] = { -800.0f, 0.0f, 1500.0f };

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
        Constants->Transform = RotateX * RotateY * RotateZ * Scale * Translate;
        Constants->Projection = { (2 * Near / ViewportWidth), 0,                           0,                           0,
                                  0,                          (2 * Near / ViewportHeight), 0,                           0,
                                  0,                          0,                           (Far / (Far - Near)),        1,
                                  0,                          0,                           (Near * Far / (Near - Far)), 0 };
    }

    RenderCommands->InstancedMeshCount = 3;
}

