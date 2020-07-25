#include <stdio.h>

typedef struct RIFFCkHeader
{
  char ckID[4];
  int ckSize;

} RIFFCkHeader;

typedef struct RIFFHeader
{
  RIFFCkHeader riffChunkHeader;
  char format[4];

} RIFFHeader;

typedef struct WaveHeader
{
  RIFFCkHeader waveFormatChunkHeader;
  short waveFormatTag;
  short waveChannels;
  int waveSamplesPerSecond;
  int waveAvgBytesPerSecond;
  short waveBlockAlign;
  short bitsPerSample;
  RIFFCkHeader waveDataChunkHeader;

} WaveHeader;

enum WaveFormatTypes
{
  MICROSOFT_PCM = 0x0001,
  IBM_MULAW = 0x0101,
  IBM_ALAW = 0x0102,
  IBM_ADPCM = 0x0103,
  ADPCM = 0x0003
};

typedef struct WaveFile
{
  RIFFHeader riffHeder;
  WaveHeader waveHeader;
  short *waveBytes;
} WaveFile;

RIFFHeader readRiffHeader(FILE *handle);

WaveHeader readWaveHeader(FILE *handle);

void dumpHeaders(WaveFile waveFile);

int validatePCMFormat(WaveHeader waveHeader);

WaveFile readWaveFile(char *inputFilePath);
