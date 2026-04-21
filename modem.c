//simon parker

/*
  TODO:
    -func that calculates the power for a test frequency (DONE)
    -func that reads in wav data
    -func that reads a block of samples and deciphers the bit
    -func that takes in 10 blocks of samples and deciphers the byte
    -func that writes a byte to a file (does this need to be its own function?)


    code flow:
    read in 1 byte of data from the wav file (1600 samples, however many bytes of data that is (16 bits per sample, mono))
    for each bit in that byte (+2 for the start/stop), test which frequencies are dominant in that block (160 samples)
    write the decoded byte to the txt file
    print out the decoded byte
*/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdint.h>


#define SAMPLE_FREQUENCY 48000
#define SAMPLES_PER_BLOCK 160
#define SAMPLES_PER_BYTE 1600
#define MARK_FREQUENCY 2225
#define SPACE_FREQUENCY 2025
#define PATH "./siparker.wav"

//adds up the elements in an array
float sum_arr(float *arr, int num){
  float sum = 0.;
  for (int i = 0; i < num; i++){
    sum += arr[i];
  }
  return sum;
}

//multiplies 2 float arrays element wise and puts the results into a third array
void element_mult_arr(float *arr1, float *arr2, float *result_arr, int num){
  for (int i = 0; i < num; i++){
    result_arr[i] = arr1[i] * arr2[i];
  } 
}

//initializes the sine array with values so I don't need to recalculate each time I test for a frequency
void init_sine_arr(float *arr, int num, int freq, int sample_freq){
  for (int i = 0; i < num; i++){
    arr[i] = sinf((float) 2.0*M_PI*freq*i/sample_freq);
  } 
}

//same idea as the sine initializer
void init_cos_arr(float *arr, int num, int freq, int sample_freq){
  for (int i = 0; i < num; i++){
    arr[i] = cosf((float) 2.0*M_PI*freq*i/sample_freq);
  } 
}

//returns the tone power of a array of samples against a test frequency
float tone_power(int16_t *samples, float *test_sine, float *test_cos, int num_samples){
  float samples_f[num_samples];
  float I[num_samples];
  float Q[num_samples];
  for (int i = 0; i < num_samples; i++){
    samples_f[i] = (float) samples[i]; 
  }
  element_mult_arr(samples_f, test_cos, I, num_samples); 
  element_mult_arr(samples_f, test_sine, Q, num_samples); 
  return (float) pow(sum_arr(I, num_samples), 2) + pow(sum_arr(Q, num_samples), 2);
}

int main(){
  float *mark_sine = malloc(sizeof(float) * SAMPLES_PER_BLOCK); //used for precomputing the sine/cosine values needed for
  float *mark_cos = malloc(sizeof(float) * SAMPLES_PER_BLOCK);  //the tone power calculations
  float *space_sine = malloc(sizeof(float) * SAMPLES_PER_BLOCK);
  float *space_cos = malloc(sizeof(float) * SAMPLES_PER_BLOCK);

  int16_t *samples_i16 = malloc(sizeof(int16_t) * SAMPLES_PER_BLOCK); //for holding the current block of data
  float *samples_f = malloc(sizeof(float) * SAMPLES_PER_BLOCK);

  init_sine_arr(mark_sine, SAMPLES_PER_BLOCK, MARK_FREQUENCY, SAMPLE_FREQUENCY);
  init_cos_arr(mark_cos, SAMPLES_PER_BLOCK, MARK_FREQUENCY, SAMPLE_FREQUENCY);
  init_sine_arr(space_sine, SAMPLES_PER_BLOCK, SPACE_FREQUENCY, SAMPLE_FREQUENCY);
  init_cos_arr(space_cos, SAMPLES_PER_BLOCK, SPACE_FREQUENCY, SAMPLE_FREQUENCY);


  FILE *fp = fopen(PATH, "rb");
  if (fp == NULL) {
    perror("fopen, no siparker.wav file detected");
    return 1;
  }

  fseek(fp, 0L, SEEK_END);
  long f_size = ftell(fp);
  rewind(fp);
  int num_bytes = fize / (sizeof(int16_t) * SAMPLES_PER_BYTE); //number of bytes of data stored in the file
  for (int i = 0; i < num_bytes; i++){
    fread(samples_i16, sizeof(int16_t), SAMPLES_PER_BYTE, fp); //start bit
    for (int j = 0; j < 8; j++){
      fread(samples_i16, sizeof(int16_t), SAMPLES_PER_BYTE, fp); //start bit
      if (tone_power(samples_i16, mark_sine, mark_cosine, SAMPLES_PER_BLOCK) > 
            tone_power(samples_i16, space_sine, space_cosine, SAMPLES_PER_BLOCK)) { //if it's a 1
              
      }
      else{ //if it's a 0
        
      }
    }
    fread(samples_i16, sizeof(int16_t), SAMPLES_PER_BYTE, fp);
    
  }
  /*
  printf("space x mark power test: %lf\n", tone_power(space_sine, mark_sine, mark_cos, SAMPLES_PER_BLOCK));
  printf("mark x mark power test: %lf\n", tone_power(mark_sine, mark_sine, mark_cos, SAMPLES_PER_BLOCK));
  printf("space x space power test: %lf\n", tone_power(space_sine, space_sine, space_cos, SAMPLES_PER_BLOCK));
  printf("mark x space power test: %lf\n", tone_power(mark_sine, space_sine, space_cos, SAMPLES_PER_BLOCK));
  */

  free(mark_sine);
  free(mark_cos);
  free(space_sine);
  free(space_cos);
  free(samples_i16);
  free(samples_f);
}
