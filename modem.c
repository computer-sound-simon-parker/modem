//simon parker

/*
  TODO:
    -func that writes a byte to a file (does this need to be its own function?)
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
#define PATH1 "./message.wav"
#define PATH2 "./message.txt"


//returns the dot product of 2 arrays as a float
float dot(float *arr1, float *arr2, int num){
  float sum = 0.;
  for (int i = 0; i < num; i++){
    sum += arr1[i] * arr2[i];
  } 
  return sum;
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
//note: incoming samples are little endian
float tone_power(int16_t *samples, float *test_sine, float *test_cos, int num_samples){
  float samples_f[num_samples];
  for (int i = 0; i < num_samples; i++){
    samples_f[i] = (float) samples[i]; 
  }
  return (float) pow(dot(samples_f, test_cos, num_samples), 2) + pow(dot(samples_f, test_sine, num_samples), 2);
}

int main(){
  float *mark_sine = malloc(sizeof(float) * SAMPLES_PER_BLOCK); //used for precomputing the sine/cosine values needed for
  float *mark_cos = malloc(sizeof(float) * SAMPLES_PER_BLOCK);  //the tone power calculations
  float *space_sine = malloc(sizeof(float) * SAMPLES_PER_BLOCK);
  float *space_cos = malloc(sizeof(float) * SAMPLES_PER_BLOCK);

  int16_t *samples_i16 = malloc(sizeof(int16_t) * SAMPLES_PER_BLOCK); //for holding the current block of data

  init_sine_arr(mark_sine, SAMPLES_PER_BLOCK, MARK_FREQUENCY, SAMPLE_FREQUENCY);
  init_cos_arr(mark_cos, SAMPLES_PER_BLOCK, MARK_FREQUENCY, SAMPLE_FREQUENCY);
  init_sine_arr(space_sine, SAMPLES_PER_BLOCK, SPACE_FREQUENCY, SAMPLE_FREQUENCY);
  init_cos_arr(space_cos, SAMPLES_PER_BLOCK, SPACE_FREQUENCY, SAMPLE_FREQUENCY);


  FILE *fp1 = fopen(PATH1, "rb");
  FILE *fp2 = fopen(PATH2, "wb");
  if (fp1 == NULL) {
    perror("fopen, no siparker.wav file detected");
    return 1;
  }

  fseek(fp1, 0L, SEEK_END);
  long f_size = ftell(fp1);
  rewind(fp1);
  int num_bytes = f_size / (sizeof(int16_t) * SAMPLES_PER_BYTE); //number of bytes of data stored in the file
  uint8_t least_sig, most_sig, data;
  for (int i = 0; i < num_bytes; i++){
    fread(samples_i16, sizeof(int16_t), SAMPLES_PER_BLOCK, fp1); //start bit
    data = 0;
    for (int j = 0; j < 8; j++){
      fread(samples_i16, sizeof(int16_t), SAMPLES_PER_BLOCK, fp1);
      for (int k = 0; k < SAMPLES_PER_BLOCK; k++){
        least_sig = samples_i16[k] & 0xff; //flipping samples to big endian so i can cast to float
        most_sig = ((uint16_t)samples_i16[k] >> 8) & 0xff;
        samples_i16[k] = ((int16_t) least_sig << 8) | most_sig;
      }
      if (tone_power(samples_i16, mark_sine, mark_cos, SAMPLES_PER_BLOCK) > 
            tone_power(samples_i16, space_sine, space_cos, SAMPLES_PER_BLOCK)) { //if it's a 1
        data = data | (1 << j);
      }
      else{ //if it's a 0
        data = data | (0 << j);
      }
    }
    fread(samples_i16, sizeof(int16_t), SAMPLES_PER_BLOCK, fp1);
    printf("%c", (char) data);
    fwrite(&data, 1, 1, fp2);
  }
  printf("\n");

  fclose(fp1);
  fclose(fp2);

  free(mark_sine);
  free(mark_cos);
  free(space_sine);
  free(space_cos);
  free(samples_i16);
}
