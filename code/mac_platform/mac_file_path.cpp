void
MacBuildAppFilePath(mac_app_path *Path)
{
	uint32 buffsize = sizeof(Path->Filename);
    if (_NSGetExecutablePath(Path->Filename, &buffsize) == 0) {
		for(char *Scan = Path->Filename;
			*Scan;
			++Scan)
		{
			if(*Scan == '/')
			{
				Path->OnePastLastAppFileNameSlash = Scan + 1;
			}
		}
    }
}

int
StringLength(char *String)
{
    int Count = 0;
    while(*String++)
    {
        ++Count;
    }
    return(Count);
}

void CatStrings(size_t SourceACount, char *SourceA,
                size_t SourceBCount, char *SourceB,
                size_t DestCount, char *Dest)
{
    // TODO: Dest bounds checking!
    for(int Index = 0;
        Index < SourceACount;
        ++Index)
    {
        *Dest++ = *SourceA++;
    }

    for(int Index = 0;
        Index < SourceBCount;
        ++Index)
    {
        *Dest++ = *SourceB++;
    }

    *Dest++ = 0;
}

void
MacBuildAppPathFileName(mac_app_path *Path, char *Filename, int DestCount, char *Dest)
{
	CatStrings(Path->OnePastLastAppFileNameSlash - Path->Filename, Path->Filename,
			   StringLength(Filename), Filename,
			   DestCount, Dest);
}

char * ConvertAbsoluteURLToFileURL(NSURL *FileURL)
{
    NSMutableString *FilePath = [[FileURL absoluteString] mutableCopy];
    [FilePath replaceOccurrencesOfString: @"file://" 
                              withString: @""
                                 options: NSCaseInsensitiveSearch
                                   range: NSMakeRange(0,7)]; 
    char *LocalFilename = (char *)[FilePath cStringUsingEncoding: NSUTF8StringEncoding];
    [FilePath release];
    return (LocalFilename);

}
