#include <stdio.h>
#include <math.h>
#include <stdint.h>
#include <soundio/soundio.h>
#include <sndfile.h>
#include "ll.h"

#define PI 3.14159265

int main(int argc, char *argv[]) {
    int x = atoi(argv[1]);
    int i = x;
    printf("Collatz Conjecture for %d: \n", i);
    node *list = NULL;
    while (i > 1)
    {
        insert(&list, i);
        if ((i % 2) == 0)        
        {
            i = i / 2;
        }    
        else
            i = (i * 3) + 1;
    }
    insert(&list, 1);

    const char *filename = "output.wav";
    const int sample_rate = 44100;
    const int duration_seconds = 1; 
    double amplitude = 0.05; 
    const double fade_duration = 0.5; 

    SF_INFO sfinfo;
    SNDFILE *outfile;
    
    double theta_increment;
    short sample;

    sfinfo.samplerate = sample_rate;
    sfinfo.channels = 1;
    sfinfo.format = SF_FORMAT_WAV | SF_FORMAT_PCM_16;

    outfile = sf_open(filename, SFM_WRITE, &sfinfo);
    if (!outfile) {
        printf("Error: could not open file.\n");
        return 1;
    }
    display(&list);
    
    for (int c = 0; c < len(&list); c++) {
        int frequency = element(&list, c);
        double wavelength = (double)sample_rate / frequency;
        theta_increment = 2.0 * PI * frequency / sample_rate;
        double max_amplitude = (double)INT16_MAX * amplitude;

        for (int i = 0; i < fade_duration * sample_rate; i++) {
            double fade_in_factor = (double)i / (fade_duration * sample_rate);
            sample = (short)(max_amplitude * fade_in_factor * sin(i * theta_increment));

            if (sf_write_short(outfile, &sample, 1) != 1) {
                printf("Error writing to file.\n");
                return 1;
            }
        }

        for (int i = fade_duration * sample_rate; i < (duration_seconds - fade_duration) * sample_rate; i++) {
            
            sample = (short)(max_amplitude * sin(i * theta_increment));

            if (sf_write_short(outfile, &sample, 1) != 1) {
                printf("Error writing to file.\n");
                return 1;
            }
        }

        for (int i = 0; i < fade_duration * sample_rate; i++) {
            double fade_out_factor = 1.0 - ((double)i / (fade_duration * sample_rate));
            sample = (short)(max_amplitude * fade_out_factor * sin(i * theta_increment));

            if (sf_write_short(outfile, &sample, 1) != 1) {
                printf("Error writing to file.\n");
                return 1;
            }
        }
    }

    sf_close(outfile);
    printf("File written successfully: %s\n", filename);
    return 0;
}
