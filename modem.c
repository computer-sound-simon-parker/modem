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

#define SAMPLE_FREQUENCY 48000
#define SAMPLES_PER_BLOCK 160
#define MARK_FREQUENCY 2225
#define SPACE_FREQUENCY 2025

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
float tone_power(float *samples, float *test_sine, float *test_cos, int num_samples){
  float I[num_samples];
  float Q[num_samples];
  element_mult_arr(samples, test_cos, I, num_samples); 
  element_mult_arr(samples, test_sine, Q, num_samples); 
  return (float) pow(sum_arr(I, num_samples), 2) + pow(sum_arr(Q, num_samples), 2);
}

int main(){
  float *mark_sine = malloc(sizeof(float) * SAMPLES_PER_BLOCK);
  float *mark_cos = malloc(sizeof(float) * SAMPLES_PER_BLOCK);
  float *space_sine = malloc(sizeof(float) * SAMPLES_PER_BLOCK);
  float *space_cos = malloc(sizeof(float) * SAMPLES_PER_BLOCK);

  init_sine_arr(mark_sine, SAMPLES_PER_BLOCK, MARK_FREQUENCY, SAMPLE_FREQUENCY);
  init_cos_arr(mark_cos, SAMPLES_PER_BLOCK, MARK_FREQUENCY, SAMPLE_FREQUENCY);
  init_sine_arr(space_sine, SAMPLES_PER_BLOCK, SPACE_FREQUENCY, SAMPLE_FREQUENCY);
  init_cos_arr(space_cos, SAMPLES_PER_BLOCK, SPACE_FREQUENCY, SAMPLE_FREQUENCY);

  printf("space power test: %lf\n", tone_power(space_sine, mark_sine, mark_cos, SAMPLES_PER_BLOCK));
  printf("mark power test: %lf\n", tone_power(mark_sine, mark_sine, mark_cos, SAMPLES_PER_BLOCK));


  free(mark_sine);
  free(mark_cos);
  free(space_sine);
  free(space_cos);
}
