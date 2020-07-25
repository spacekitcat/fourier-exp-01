# untitled Fourer transform experiments

Fourier transforms are really cool and I want to learn the maths and concepts behind it all.  

## Journal

## 25 July

The program reads a wave file (MS-PCM format only), transforms the PCM signal from the time domain to the frequency domain and then draws a graph of the frequency bins. The frequency bins show an estimated contribution for each frequency, so if you pass in `resources/440hz.wav`, you should expect to see a spike near 440hz. (each set of 50 bins is together to scale the graph size)

## Building

Optimistically, you should only have to check out the code and use the build scripts generated with Autotools.

```bash
git clone github.com/spacekitcat/<repository-name>
cd <repository-name>
./configure
make
```

## Learning Material

[Brief review of complex numbers](https://stanford.edu/~boyd/ee102/complex-primer.pdf)
