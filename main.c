#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "wav-reader.h"

#define MIN_FREQ 0
#define MAX_FREQ 44100 / 2
#define PI 3.1415926535

typedef struct ComplexNumber
{
  // Magnitude
  double reComponent;

  // Phase
  double imComponent;

} ComplexNumber;


double min(double x, double y)
{
  return x > y ? y : x;
}

void initialiseComplexNumberList(ComplexNumber *frequencyTally)
{
  for (int i = 0; i < MAX_FREQ; ++i)
  {
    frequencyTally[i].imComponent = 0.0f;
    frequencyTally[i].reComponent = 0.0f;
  }
}

void doFrequencyDomainFourierTransform(short *wavSampleSet, int windowSize, int wavSampleSetRate, ComplexNumber *frequencyTally)
{
  short *wavSampleSetIterator = wavSampleSet;
  int frameOffset = 0;

  initialiseComplexNumberList(frequencyTally);
  for (unsigned int freq = 0; freq < MAX_FREQ; ++freq)
  {
    for (unsigned int frame = frameOffset; frame < frameOffset + windowSize; frame++)
    {
      short signal = *wavSampleSetIterator++;
      double rotation = (-2 * PI * frame * freq) / wavSampleSetRate;
      frequencyTally[freq].reComponent += (signal / 1000) * cos(rotation);
      frequencyTally[freq].imComponent += (signal / 1000) * -sin(rotation);
    }

    wavSampleSetIterator = wavSampleSet;
  }
}

double reduceComplexVectorToScaler(ComplexNumber n)
{
  return sqrt(n.reComponent * n.reComponent + n.imComponent * n.imComponent);
}

void printGraphRow(int width)
{
  printf("[");
  for (int i = 0; i < width; ++i)
  {
    printf("+");
  }
  printf("]\n");
}

int main(int argc, char **argv)
{
  if (argc < 2)
  {
    printf("\nIncorrect number of parameters.\n");
    printf("\tUSAGE:\n\t\t./fourierexp01 <path-to-wav-file>\n\n");
    return 1;
  }

  char *inputFile = argv[1];

  WaveFile waveFile = readWaveFile(inputFile);
  ComplexNumber frequencyTally[MAX_FREQ];
  doFrequencyDomainFourierTransform(waveFile.waveBytes, 1024, waveFile.waveHeader.waveSamplesPerSecond, frequencyTally);

  double spectrum[MAX_FREQ];
  for (int i = 0; i < MAX_FREQ; ++i)
  {
    double d = reduceComplexVectorToScaler(frequencyTally[i]);
    d = d < 1 ? 0 : 20 * log(d);
    spectrum[i] = min(d, 200);
  }

  int step = 50;
  for (int i = 0; i < MAX_FREQ - step; i += step)
  {
    double average = 0.0;
    for (int j = 0; j < step; ++j)
    {
      average += spectrum[i + j];
    }
    average /= step;
    printf("%dhz\t", i);
    printGraphRow(floor(average));
  }

  free(waveFile.waveBytes);
  return 0;
}
