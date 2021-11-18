#include "vario.h"
#include "game_startup_config.cpp"
#include "game_math.cpp"

#define CUBE_VERTEX_COUNT   8
#define CUBE_INDEX_COUNT    36

extern "C"
GAME_UPDATE_AND_RENDER(GameUpdateAndRender)
{
    // XYZ Vertices, RGB Color
    game_vertex ColoredCube[CUBE_VERTEX_COUNT] =
    {
        { {-0.5f,-0.5f,-0.5f}, {0, 0, 0} },
        { {-0.5f,-0.5f, 0.5f}, {0, 0, 1} },
        { {-0.5f, 0.5f,-0.5f}, {0, 1, 0} },
        { {-0.5f, 0.5f, 0.5f}, {0, 1, 1} },

        { {0.5f,-0.5f,-0.5f}, {1, 0, 0} },
        { {0.5f,-0.5f, 0.5f}, {1, 0, 1} },
        { {0.5f, 0.5f,-0.5f}, {1, 1, 0} },
        { {0.5f, 0.5f, 0.5f}, {1, 1, 1} }
    }; 

    u16 CubeIndices[CUBE_INDEX_COUNT] = 
    {
        0,2,1, // -x
        1,2,3,

        4,5,6, // +x
        5,7,6,

        0,1,5, // -y
        0,5,4,

        2,6,7, // +y
        2,7,3,

        0,4,6, // -z
        0,6,2,

        1,3,7, // +z
        1,7,5,
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

    u16 *IndexBuffer = VertexBuffer->Indices;

    for (u32 Index = 0;
         Index < CUBE_INDEX_COUNT;
         Index++)
    {
        IndexBuffer[Index] = CubeIndices[Index];
    }

    VertexBuffer->IndexCount = CUBE_INDEX_COUNT;

    r32 Near = 1000.0f;
    r32 Far = 1000000.0f;

    local_persist vector_float_3 ModelRotation = { 0.0f, 0.0f, 0.0f };
    vector_float_3 ModelScale = { 400.0f, 400.0f, 400.0f };
    vector_float_3 ModelTranslation = { 0.0f, 0.0f, 1500.0f };

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

    matrix Translate = { 1,                  0,                  0,                  0,
                         0,                  1,                  0,                  0,
                         0,                  0,                  1,                  0,
                         ModelTranslation.X, ModelTranslation.Y, ModelTranslation.Z, 1 };

    u32 ViewportWidth = RenderCommands->ViewportWidth;
    u32 ViewportHeight = RenderCommands->ViewportHeight;

    game_constants *Constants = &RenderCommands->Constants;
    Constants->Transform = RotateX * RotateY * RotateZ * Scale * Translate;
    Constants->Projection = { (2 * Near / ViewportWidth), 0,                           0,                           0,
                              0,                          (2 * Near / ViewportHeight), 0,                           0,
                              0,                          0,                           (Far / (Far - Near)),        1,
                              0,                          0,                           (Near * Far / (Near - Far)), 0 };


}

