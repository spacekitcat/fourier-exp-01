#include "wav-reader.h"

RIFFHeader readRiffHeader(FILE *handle)
{
  RIFFHeader writeTarget;
  fread(&writeTarget, sizeof(RIFFHeader), 1, handle);
  return writeTarget;
}

WaveHeader readWaveHeader(FILE *handle)
{
  WaveHeader writeTarget;
  fread(&writeTarget, sizeof(WaveHeader), 1, handle);
  return writeTarget;
}

void dumpHeaders(WaveFile waveFile)
{
  printf("riffHeader->riffChunkHeader.ckID\t= %.*s\n", 4, waveFile.riffHeder.riffChunkHeader.ckID);
  printf("riffHeader->riffChunkHeader.ckSize\t= %d\n", waveFile.riffHeder.riffChunkHeader.ckSize);
  printf("riffHeader->format\t\t= %.*s\n", 4, waveFile.riffHeder.format);

  printf("waveHeader->waveFormatChunkHeader.ckID\t= %.*s\n", 4, waveFile.waveHeader.waveFormatChunkHeader.ckID);
  printf("avHeader->waveFormatChunkHeader.ckSize\t= %d\n", waveFile.waveHeader.waveFormatChunkHeader.ckSize);
  printf("waveHeader->waveFormatTag\t\t= %d\n", waveFile.waveHeader.waveFormatTag);
  printf("waveHeader->waveChannels\t\t= %d\n", waveFile.waveHeader.waveChannels);
  printf("waveHeader->waveSamplesPerSecond\t\t= %d\n", waveFile.waveHeader.waveSamplesPerSecond);
  printf("waveHeader->waveAvgBytesPerSecond\t\t= %d\n", waveFile.waveHeader.waveAvgBytesPerSecond);
  printf("waveHeader->waveBlockAlign\t\t= %d\n", waveFile.waveHeader.waveBlockAlign);
  printf("waveHeader->bitsPerSample\t= %d\n", waveFile.waveHeader.bitsPerSample);
  printf("waveHeader->waveDataChunkHeader.ckID\t= %.*s\n", 4, waveFile.waveHeader.waveDataChunkHeader.ckID);
  printf("waveHeader->waveDataChunkHeader.ckSize\t= %d\n", waveFile.waveHeader.waveDataChunkHeader.ckSize);
}

int validatePCMFormat(WaveHeader waveHeader)
{
  switch (waveHeader.waveFormatTag)
  {
  case IBM_MULAW:
    printf("IBM mu-law format is not supported.\n\n");
    break;
  case IBM_ALAW:
    printf("IBM a-law format is not supported.\n\n");
    break;
  case IBM_ADPCM:
    printf("IBM AVC Adaptive Differential Pulse Code Modulation (ADPCM) format is not supported.\n\n");
    break;
  case ADPCM:
    printf("IMA ADPCM format is not supported.\n\n");
    break;
  case MICROSOFT_PCM:
    printf("Microsoft PCM format detected.\n\n");
    return 1;
    break;
  default:
    printf("This format is unrecognised.\n\n");
    break;
  }

  return 0;
}

WaveFile readWaveFile(char *inputFilePath)
{
  FILE *handle = fopen(inputFilePath, "rb");

  if (!handle)
  {
    printf("Failed to open input file %s\n", inputFilePath);
    exit(1);
  }
  WaveFile waveFile;
  waveFile.riffHeder = readRiffHeader(handle);
  waveFile.waveHeader = readWaveHeader(handle);
  dumpHeaders(waveFile);

  if (validatePCMFormat(waveFile.waveHeader) != 1)
  {
    printf("Exiting: unsupported format. Try converting it to a Microsoft PCM format WAV file.");
    exit(1);
  }

  short *waveBytes = malloc(waveFile.waveHeader.waveDataChunkHeader.ckSize);
  fread(waveBytes, waveFile.waveHeader.waveDataChunkHeader.ckSize, 1, handle);
  waveFile.waveBytes = waveBytes;
  fclose(handle);

  return waveFile;
}
