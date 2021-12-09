
vector_float_3
ConvertCubeMapPositionToModelTranslation(cube_map_position Position, r32 CubeSideInMeters)
{
    vector_float_3 Result = { (Position.X*CubeSideInMeters + Position.Offset.X*CubeSideInMeters), 
                              (Position.Y*CubeSideInMeters + Position.Offset.Y*CubeSideInMeters), 
                              (Position.Z*CubeSideInMeters + Position.Offset.Z*CubeSideInMeters) };
    return (Result);
}
