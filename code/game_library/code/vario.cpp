#include "vario.h"
#include "game_startup_config.cpp"
#include "game_math.cpp"
#include "cube_map.cpp"
#include "model_loader.cpp"

extern "C"
GAME_UPDATE_AND_RENDER(GameUpdateAndRender)
{
    // TODO: (Ted)  Move this to loading logic so it isn't happening once per frame.
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

    r32 Near = 1000.0f;
    r32 Far = 1000000.0f;

    local_persist vector_float_3 ModelRotation = { 0.0f, 0.0f, 0.0f };
    vector_float_3 ModelScale = { 1.0f, 1.0f, 1.0f };

    // NOTE: (Ted)  Ultimately a tile map position turns into model translations which place a model some distance away from the center of the
    //              world.
    //
    //              Each cube model is 1 tile across at scale, so should be translated over by 0.5 if its tile map position is one.

    cube_map_position Positions[3];
    cube_map_position P1 = {};
    cube_map_position P2 = {};
    P2.X = 1;
    P2.Y = 1;

    cube_map_position P3 = {};
    P3.X = 2;
    P3.Y = 2;

    Positions[0] = P1;
    Positions[1] = P2;
    Positions[2] = P3;

    vector_float_3 ModelTranslations[3];
    ModelTranslations[0] = ConvertCubeMapPositionToModelTranslation(P1);
    ModelTranslations[1] = ConvertCubeMapPositionToModelTranslation(P2);
    ModelTranslations[2] = ConvertCubeMapPositionToModelTranslation(P3);

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

    vector_float_3 Eye = { 0.0f,  0.0f, -16.0f };
    vector_float_3 At = {  0.0f,  0.0f,  0.0f };
    vector_float_3 Up = {  0.0f,  1.0f,  0.0f };

    vector_float_3 ZAxis = Normalize(SubtractVector3(At, Eye)); 
    vector_float_3 XAxis = Normalize(CrossProduct(Up, ZAxis));
    vector_float_3 YAxis = CrossProduct(ZAxis, XAxis);

    matrix LookAt = { XAxis.X,                  YAxis.X,                    ZAxis.X,                    0,
                      XAxis.Y,                  YAxis.Y,                    ZAxis.Y,                    0,
                      XAxis.Z,                  YAxis.Z,                    ZAxis.Z,                    0,
                      -DotProduct(XAxis, Eye),  -DotProduct(YAxis, Eye),    -DotProduct(ZAxis, Eye),    1 };

    u32 InstanceModelIndices[3];
    InstanceModelIndices[0] = 0;
    InstanceModelIndices[1] = 1;
    InstanceModelIndices[2] = 2;

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

        Constants->LightVector = { -1.0f, -1.0f, 1.0f };

        RenderCommands->InstanceModelIndices[InstanceIndex] = InstanceModelIndices[InstanceIndex];
    }

    RenderCommands->InstancedMeshCount = 3;
}

