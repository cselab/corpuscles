enum {
    TAG_ASCII,
    TAG_LONG,
    TAG_SHORT,
    TAG_SHORT_OR_LONG,
};
enum {
    TAG_ONE,
    TAG_MANY,
};
enum {
    NewSubfileType = 254,
    ImageWidth = 256,
    ImageLength = 257,
    BitsPerSample = 258,
    PhotometricInterpretation = 262,
    ImageDescription = 270,
    StripOffsets = 273,
    SamplesPerPixel = 277,
    RowsPerStrip = 278,
    StripByteCounts = 279,
};
static const int Tag[] = {
    NewSubfileType,
    ImageWidth,
    ImageLength,
    BitsPerSample,
    PhotometricInterpretation,
    ImageDescription,
    StripOffsets,
    SamplesPerPixel,
    RowsPerStrip,
    StripByteCounts,
};
static int TagType[] = {
    TAG_LONG,
    TAG_SHORT_OR_LONG,
    TAG_SHORT_OR_LONG,
    TAG_SHORT,
    TAG_SHORT,
    TAG_ASCII,
    TAG_SHORT_OR_LONG,
    TAG_SHORT,
    TAG_SHORT_OR_LONG,
    TAG_SHORT_OR_LONG,
};
static const char *TagName[] = {
    "NewSubfileType",
    "ImageWidth",
    "ImageLength",
    "BitsPerSample",
    "PhotometricInterpretation",
    "ImageDescription",
    "StripOffsets",
    "SamplesPerPixel",
    "RowsPerStrip",
    "StripByteCounts",
};
static const int TagCount[] = {
    TAG_ONE,
    TAG_ONE,
    TAG_ONE,
    TAG_MANY,
    TAG_ONE,
    TAG_MANY,
    TAG_MANY,
    TAG_ONE,
    TAG_ONE,
    TAG_MANY,
};
