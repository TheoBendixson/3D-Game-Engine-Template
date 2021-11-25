#include "vario.h"
#include "game_startup_config.cpp"
#include "game_math.cpp"

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

// TODO: (Ted)  Create some notion of a push buffer so we can tell the instancing system
//              which mesh to draw and with what color.
internal 
void LoadColoredCubeVertices(game_render_commands *RenderCommands, r32 *RGBColor)
{
    // XYZ Vertices, RGB Color
    // NOTE: (Ted) Counter-Clockwise Winding
    game_vertex ColoredCube[CUBE_VERTEX_COUNT] = 
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

    game_vertex_buffer *VertexBuffer = &RenderCommands->VertexBuffer;
    game_vertex *Vertices = (VertexBuffer->Vertices + VertexBuffer->VertexCount);

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


extern "C"
GAME_UPDATE_AND_RENDER(GameUpdateAndRender)
{
  
    // TODO: (Ted)  Move this to loading logic so it isn't happening once per frame.
    game_vertex_buffer *VertexBuffer = &RenderCommands->VertexBuffer;
    VertexBuffer->VertexCount = 0;
    VertexBuffer->ModelCount = 0;

    r32 Color[3];
    Color[0] = 0;
    Color[1] = 0;
    Color[2] = 1;

    LoadColoredCubeVertices(RenderCommands, Color);

    r32 Near = 1000.0f;
    r32 Far = 1000000.0f;

    local_persist vector_float_3 ModelRotation = { 0.0f, 0.0f, 0.0f };
    vector_float_3 ModelScale = { 1.0f, 1.0f, 1.0f };

    vector_float_3 ModelTranslations[3];
    ModelTranslations[0] = { 15.0f, 0.0f, 0.0f };
    ModelTranslations[1] = { 16.0f, 1.0f, 0.0f };
    ModelTranslations[2] = { 17.0f, 2.0f, 0.0f };

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

    vector_float_3 Eye = { 16.0f,  0.0f, 32.0f };
    vector_float_3 At = {  16.0f,  16.0f,  0.0f };
    vector_float_3 Up = {  0.0f,  1.0f,  0.0f };

    vector_float_3 ZAxis = Normalize(SubtractVector3(At, Eye)); 
    vector_float_3 XAxis = Normalize(CrossProduct(Up, ZAxis));
    vector_float_3 YAxis = CrossProduct(ZAxis, XAxis);

    matrix LookAt = { XAxis.X,                  YAxis.X,                    ZAxis.X,                    0,
                      XAxis.Y,                  YAxis.Y,                    ZAxis.Y,                    0,
                      XAxis.Z,                  YAxis.Z,                    ZAxis.Z,                    0,
                      -DotProduct(XAxis, Eye),  -DotProduct(YAxis, Eye),    -DotProduct(ZAxis, Eye),    1 };

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
        Constants->View = LookAt;
        Constants->Projection = { (2 * Near / ViewportWidth), 0,                           0,                           0,
                                  0,                          (2 * Near / ViewportHeight), 0,                           0,
                                  0,                          0,                           (Far / (Far - Near)),        1,
                                  0,                          0,                           (Near * Far / (Near - Far)), 0 };

        Constants->LightVector = { 1.0f, -1.0f, -1.0f };
    }

    RenderCommands->InstancedMeshCount = 3;
}

