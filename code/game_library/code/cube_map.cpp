
vector_float_3
ConvertCubeMapPositionToModelTranslation(cube_map_position Position)
{
    vector_float_3 Result = { ((Position.X - 0.5) + Position.Offset.X), 
                              ((Position.Y - 0.5) + Position.Offset.Y), 
                              ((Position.Z - 0.5) + Position.Offset.Z) };
    return (Result);
}
