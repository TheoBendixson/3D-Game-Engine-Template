#include "vario.h"
#include "game_startup_config.cpp"

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
}

