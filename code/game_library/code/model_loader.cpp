
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

void LoadColoredCubeVertices(game_render_commands *RenderCommands, r32 *RGBColor)
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

    game_vertex_buffer *VertexBuffer = &RenderCommands->VertexBuffer;
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


extern "C"
GAME_LOAD_3D_MODELS(GameLoad3DModels)
{
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
}
