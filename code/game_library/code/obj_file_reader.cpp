
r32
RealDigitFromTextCharacter(char Character)
{
    r32 Digit = 0.0f;

    if (Character == '1')
    {
        Digit = 1.0f;
    } 
    else if (Character == '2')
    {
        Digit = 2.0f;
    }
    else if (Character == '3')
    {
        Digit = 3.0f;
    }
    else if (Character == '4')
    {
        Digit = 4.0f;
    }
    else if (Character == '5')
    {
        Digit = 5.0f;
    }
    else if (Character == '6')
    {
        Digit = 6.0f;
    }
    else if (Character == '7')
    {
        Digit = 7.0f;
    }
    else if (Character == '8')
    {
        Digit = 8.0f;
    }
    else if (Character == '9')
    {
        Digit = 9.0f;
    }

    return Digit;
}

u32
UnsignedDigitFromTextCharacter(char Character)
{
    u32 Digit = 0;

    if(Character == '1')
    {
         Digit = 1;
    } 
    else if (Character == '2')
    {
         Digit = 2;
    }
    else if (Character == '3')
    {
         Digit = 3;
    }
    else if (Character == '4')
    {
         Digit = 4;
    }
    else if (Character == '5')
    {
         Digit = 5;
    }
    else if (Character == '6')
    {
         Digit = 6;
    }
    else if (Character == '7')
    {
         Digit = 7;
    }
    else if (Character == '8')
    {
         Digit = 8;
    }
    else if (Character == '9')
    {
         Digit = 9;
    }

    return Digit;
}

struct obj_scan_result
{
    r32 Value;
    char *AdvancedScan;
};

obj_scan_result
ConstructFloatFromScan(char *Scan, u32 SignificantDigits)
{
    obj_scan_result Result = {};
    r32 VertexValue = 0.0f;

    char First = *Scan++;
    b32 IsNegative = false;

    r32 TensDigit = 0.0f;

    if (First == '-')
    {
        IsNegative = true;

        char TensPlace = *Scan++;
        TensDigit = RealDigitFromTextCharacter(TensPlace);
    } else
    {
        TensDigit = RealDigitFromTextCharacter(First);
    }

    VertexValue += TensDigit;

    Scan++;
    char TenthsPlace = *Scan++;
    r32 TenthsDigit = RealDigitFromTextCharacter(TenthsPlace);
    VertexValue += TenthsDigit*0.1f;

    char HundredthsPlace = *Scan++;
    r32 HundredthsDigit = RealDigitFromTextCharacter(HundredthsPlace);
    VertexValue += HundredthsDigit*0.01f;

    char ThousandthsPlace = *Scan++;
    r32 ThousandthsDigit = RealDigitFromTextCharacter(ThousandthsPlace);
    VertexValue += ThousandthsDigit*0.001f;

    char TenThousandthsPlace = *Scan++;
    r32 TenThousandthsDigit = RealDigitFromTextCharacter(TenThousandthsPlace);
    VertexValue += TenThousandthsDigit*0.0001f;

    if (SignificantDigits > 4)
    {
        char HundredThousandthsPlace = *Scan++;
        r32 HundredThousandthsDigit = RealDigitFromTextCharacter(HundredThousandthsPlace);
        VertexValue += HundredThousandthsDigit*0.00001f;

        char MillionthsPlace = *Scan++;
        r32 MillionthsDigit = RealDigitFromTextCharacter(MillionthsPlace);
        VertexValue += MillionthsDigit*0.000001f;
    }

    if (IsNegative)
    {
        VertexValue *= -1;
    }

    // Do this to get to the next value.
    Scan++;

    Result.Value = VertexValue;
    Result.AdvancedScan = Scan;

    return (Result);
}

char*
ScanToLineStartingWithCharacter(char StartCharacter, char *Scan, char *Line)
{
    b32 Scanning = true;

    while(Scanning)
    {
        if (*Line == StartCharacter)
        {
            Scanning = false;
        } else
        {
            // NOTE: (Ted)  Skip Lines
            while(Scan++)
            {
                if(*Scan == '\n')
                {
                    break;
                }
            }

            Scan++;
            Line = Scan;
            continue;
        }
    }

    return Scan;
}

u32
GetFaceLookupIndexFromCharacters(char *Characters, u32 CharacterCount)
{
    u32 FaceLookupIndex = 0;
    u32 OnesDigit = 0;
    u32 TensDigit = 0;
    u32 HundredsDigit = 0;

    if(CharacterCount == 1)
    {
        char DigitCharacter = Characters[0];
        OnesDigit = UnsignedDigitFromTextCharacter(DigitCharacter);
    } else if (CharacterCount == 2)
    {
        char TensDigitCharacter = Characters[0];
        char OnesDigitCharacter = Characters[1];
        TensDigit = UnsignedDigitFromTextCharacter(TensDigitCharacter);
        OnesDigit = UnsignedDigitFromTextCharacter(OnesDigitCharacter);
    } else if (CharacterCount == 3)
    {
        char HundredsDigitCharacter = Characters[0];
        char TensDigitCharacter = Characters[1];
        char OnesDigitCharacter = Characters[2];
        HundredsDigit = UnsignedDigitFromTextCharacter(HundredsDigitCharacter);
        TensDigit = UnsignedDigitFromTextCharacter(TensDigitCharacter);
        OnesDigit = UnsignedDigitFromTextCharacter(OnesDigitCharacter);
    }
     
    FaceLookupIndex = OnesDigit + 10*TensDigit + 100*HundredsDigit;

    return (FaceLookupIndex);
}

struct obj_face_scan
{
    u32 CharacterCount;
    char Characters[3];
    char *AdvancedScan;
};

obj_face_scan
GetFaceCharactersUpToToken(char *Scan, char Token)
{
    obj_face_scan Result = {};
    u32 CharacterCount = 0;

    while(*Scan != Token)
    {
        Result.Characters[CharacterCount] = *Scan;
        CharacterCount++;
        Scan++;
    }

    Scan++;

    Result.CharacterCount = CharacterCount;
    Result.AdvancedScan = Scan;
    return (Result);
}
