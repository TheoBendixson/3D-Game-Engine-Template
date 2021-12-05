
// TODO: (Ted)  Move the tile side into this.
vector_float_3
ConvertCubeMapPositionToModelTranslation(cube_map_position Position)
{
    r32 Space = 200.0f;

    vector_float_3 Result = { ((Position.X*Space) + Position.Offset.X), 
                              ((Position.Y*Space) + Position.Offset.Y), 
                              ((Position.Z*Space) + Position.Offset.Z) };
    return (Result);
}
