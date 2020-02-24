/**
 * Read and parse a wave file
 *
 **/
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "wave.h"
#include <stdint.h>
#include <limits.h>
#include <sys/resource.h>

#define TRUE 1 
#define FALSE 0
typedef long long unsigned int uint64;
// WAVE header structure

unsigned char buffer4[4];
unsigned char buffer2[2];
char* seconds_to_time(float seconds);


void correlate(float input[], int input_length,float filter[],
              int filter_length,float output[]);

 FILE *ptr;
 char *filename;
 struct HEADER header;
int main(int argc, char **argv) {

 filename = (char*) malloc(sizeof(char) * 1024);
 if (filename == NULL) {
   printf("Error in malloc\n");
   exit(1);
 }

 // get file path
 char cwd[1024];
 if (getcwd(cwd, sizeof(cwd)) != NULL) {
   
	strcpy(filename, cwd);

	// get filename from command line
	if (argc < 2) {
	  printf("No wave file specified\n");
	  return 0;
	}
	
  	strcat(filename, "/");
	strcat(filename, argv[1]);
	printf("%s\n", filename);
 }

 // open file
 printf("Opening  file..\n");
 ptr = fopen(filename, "rb");
 if (ptr == NULL) {
	printf("Error opening file\n");
	exit(1);
 }
 
 int read = 0;
 
 // read header parts

 read = fread(header.riff, sizeof(header.riff), 1, ptr);
 printf("(1-4): %s \n", header.riff); 

 read = fread(buffer4, sizeof(buffer4), 1, ptr);
 printf("%u %u %u %u\n", buffer4[0], buffer4[1], buffer4[2], buffer4[3]);
 
 // convert little endian to big endian 4 byte int
 header.overall_size  = buffer4[0] | (buffer4[1]<<8) | 	(buffer4[2]<<16) | (buffer4[3]<<24);
 printf("(5-8) Overall size: bytes:%u, Kb:%u \n", header.overall_size, header.overall_size/1024);

 read = fread(header.wave, sizeof(header.wave), 1, ptr);
 printf("(9-12) Wave marker: %s\n", header.wave);

 read = fread(header.fmt_chunk_marker, sizeof(header.fmt_chunk_marker), 1, ptr);
 printf("(13-16) Fmt marker: %s\n", header.fmt_chunk_marker);
 

 read = fread(buffer4, sizeof(buffer4), 1, ptr);
 printf("%u %u %u %u\n", buffer4[0], buffer4[1], buffer4[2], buffer4[3]);

 // convert little endian to big endian 4 byte integer
 header.length_of_fmt = buffer4[0] |(buffer4[1] << 8) | (buffer4[2] << 16) | (buffer4[3] << 24);
 printf("(17-20) Length of Fmt header: %u \n", header.length_of_fmt);

 read = fread(buffer2, sizeof(buffer2), 1, ptr);
 printf("%u %u \n", buffer2[0], buffer2[1]);  
 header.format_type = buffer2[0] | (buffer2[1] << 8);
 char format_name[10] = "";
 if (header.format_type == 1)
   strcpy(format_name,"PCM"); 
 else if (header.format_type == 6)
  strcpy(format_name, "A-law");
 else if (header.format_type == 7)
  strcpy(format_name, "Mu-law");

 printf("(21-22) Format type: %u %s \n", header.format_type, format_name);

 read = fread(buffer2, sizeof(buffer2), 1, ptr);
 printf("%u %u \n", buffer2[0], buffer2[1]);

 header.channels = buffer2[0] | (buffer2[1] << 8);
 printf("(23-24) Channels: %u \n", header.channels);

 read = fread(buffer4, sizeof(buffer4), 1, ptr);
 printf("%u %u %u %u\n", buffer4[0], buffer4[1], buffer4[2], buffer4[3]);

 header.sample_rate = buffer4[0] | (buffer4[1] << 8) | (buffer4[2] << 16) | (buffer4[3] << 24);

 printf("(25-28) Sample rate: %u\n", header.sample_rate);

 read = fread(buffer4, sizeof(buffer4), 1, ptr);
 printf("%u %u %u %u\n", buffer4[0], buffer4[1], buffer4[2], buffer4[3]);

 header.byterate  = buffer4[0] | (buffer4[1] << 8) | (buffer4[2] << 16) | (buffer4[3] << 24);
 printf("(29-32) Byte Rate: %u , Bit Rate:%u\n", header.byterate, header.byterate*8);

 read = fread(buffer2, sizeof(buffer2), 1, ptr);
 printf("%u %u \n", buffer2[0], buffer2[1]);

 header.block_align = buffer2[0] | (buffer2[1] << 8);
 printf("(33-34) Block Alignment: %u \n", header.block_align);

 read = fread(buffer2, sizeof(buffer2), 1, ptr);
 printf("%u %u \n", buffer2[0], buffer2[1]);

 header.bits_per_sample = buffer2[0] | (buffer2[1] << 8);
 printf("(35-36) Bits per sample: %u \n", header.bits_per_sample);

 read = fread(header.data_chunk_header, sizeof(header.data_chunk_header), 1, ptr);
 printf("(37-40) Data Marker: %s \n", header.data_chunk_header);

 read = fread(buffer4, sizeof(buffer4), 1, ptr);
 printf("%u %u %u %u\n", buffer4[0], buffer4[1], buffer4[2], buffer4[3]);

 header.data_size = buffer4[0] | (buffer4[1] << 8) | (buffer4[2] << 16) | (buffer4[3] << 24 );
 printf("(41-44) Size of data chunk: %u \n", header.data_size);


 // calculate no.of samples
 long num_samples = (8 * header.data_size) / (header.channels * header.bits_per_sample);
 printf("Number of samples:%lu \n", num_samples);

 long size_of_each_sample = (header.channels * header.bits_per_sample) / 8;
 printf("Size of each sample:%ld bytes\n", size_of_each_sample);

 // calculate duration of file
 float duration_in_seconds = (float) header.overall_size / header.byterate;
 printf("Approx.Duration in seconds=%f\n", duration_in_seconds);
 printf("Approx.Duration in h:m:s=%s\n", seconds_to_time(duration_in_seconds));
 
 printf("=============================================================================\n");



 
 float my_data[num_samples];



 // read each sample from data chunk if PCM 
 for(int i = 0 ; i<num_samples;i++)
   my_data[i] = 0.0;
 if (header.format_type == 1) { // PCM
    printf("Do you wish to continue? Y/N?");
	char c = 'n';
	scanf("%c", &c);
	if (c == 'Y' || c == 'y') { 
		long i =0;
		char data_buffer[size_of_each_sample];
		
		int  size_is_correct = TRUE;
		// make sure that the bytes-per-sample is completely divisible by num.of channels
		long bytes_in_each_channel = (size_of_each_sample / header.channels);
		
		if ((bytes_in_each_channel  * header.channels) != size_of_each_sample) {
			printf("Error: %ld x %ud <> %ld\n", bytes_in_each_channel, header.channels, size_of_each_sample);
			size_is_correct = FALSE;
		}
 
		if (size_is_correct) { 
					// the valid amplitude range for values based on the bits per sample
			long low_limit = 0l;
			long high_limit = 0l;

			switch (header.bits_per_sample) {
				case 8:
					low_limit = -128;
					high_limit = 127;
					break;
				case 16:
					low_limit = -32768;
					high_limit = 32767;
					break;
				case 32:
					low_limit = -2147483648;
					high_limit = 2147483647;
					break;
			}					

			printf("\n\n.Valid range for data values : %ld to %ld \n", low_limit, high_limit);
			for (i =1; i <= num_samples; i++) {
				printf("Sample %ld / %ld \n", i, num_samples);
				read = fread(data_buffer, sizeof(data_buffer), 1, ptr);
				if (read == 1) {
				
					// dump the data read
					unsigned int  xchannels = 0;
					int data_in_channel = 0;
				 
					for (xchannels = 0; xchannels < header.channels; xchannels ++ ) {
					  
						// convert data from little endian to big endian based on bytes in each channel sample
						if (bytes_in_each_channel == 4) {
						  
						        data_in_channel = data_buffer[0] | (data_buffer[1]<<8) | (data_buffer[2]<<16) | (data_buffer[3]<<24);
							my_data[i-1] = (float)(data_buffer[3] | (data_buffer[2]<<8) | (data_buffer[1]<<16) | (data_buffer[0]<<24));
						}
						else if (bytes_in_each_channel == 2) {
							data_in_channel = data_buffer[0] | (data_buffer[1] << 8);
							my_data[i-1] = (float)(data_buffer[1] | (data_buffer[0] << 8));
						}
						else if (bytes_in_each_channel == 1) {
							data_in_channel = data_buffer[0];
							my_data[i-1] = (float)data_buffer[0];
						}
		
						// check if value was in range
						if (data_in_channel < low_limit || data_in_channel > high_limit)
							printf("**value out of range\n");		    

					}

					printf("\n");
				}
				else {
					printf("Error reading file. %d bytes\n", read);
					break;
				}

			} // 	for (i =1; i <= num_samples; i++) {




			
		} // 	if (size_is_correct) { 

		//---------------------------------TEST--------------------------------------- customize your filter
 
		//-----TEST COPY
		int filter_length = 1;
		float filter[1] = {1};
		//-----TEST COPY

		
		//+++++TEST SHIFT

		//int filter_length = 100000;
		//float filter[100000] = {0};
		
		//filter[99999] = 1;          //shift LEFT		
		//filter[0] = 1;            //shift RIGHT
		
		//++++++TEST SHIFT
		
		//---------------------------------TEST----------------------------------------

		float my_output[num_samples];
        
		//correlate the filter
		printf("correlating...\n");
		correlate( my_data , num_samples , filter , filter_length, my_output ); //implement with SIMD
		
		unsigned int final_results[num_samples];

		//cast the results to prepare for writing on file
		for(int i = 0 ; i<num_samples; i++){
		  
		  int temp = (int)my_output[i];
		  final_results[i] = temp;
  
		}

 
		FILE* output;
		output = fopen("./output.wav","wb");


		char a[100] ;
		FILE* st;
		st = fopen(filename, "rb");
    
		fread(&a[0], sizeof(char), 44, st);
		fwrite(&a[0], sizeof(char) , 44, output); //write header(same as input)
		
		//fwrite((void*)final_results,sizeof(int),num_samples,output) doesn't work for little-endian :)
		for (int i = 0 ; i < num_samples; i++){

		  int temp = final_results[i];
		  int writeable = ((temp>>24)&0xff) | ((temp<<8)&0xff0000) | ((temp>>8)&0xff00) | ((temp<<24)&0xff000000);
		  fwrite((void*)&writeable, sizeof(int), 1, output);
		}
		

		printf("output file has been created !\n");

	 } // if (c == 'Y' || c == 'y') { 
 } //  if (header.format_type == 1) { 

 
 printf("Closing input file..\n");
 fclose(ptr);

 return 0;

}



/**
 * Convert seconds into hh:mm:ss format
 * Params:
 *	seconds - seconds value
 * Returns: hms - formatted string
 **/
 char* seconds_to_time(float raw_seconds) {
  char *hms;
  int hours, hours_residue, minutes, seconds, milliseconds;
  hms = (char*) malloc(100);

  sprintf(hms, "%f", raw_seconds);

  hours = (int) raw_seconds/3600;
  hours_residue = (int) raw_seconds % 3600;
  minutes = hours_residue/60;
  seconds = hours_residue % 60;
  milliseconds = 0;

  // get the decimal part of raw_seconds to get milliseconds
  char *pos;
  pos = strchr(hms, '.');
  int ipos = (int) (pos - hms);
  char decimalpart[15];
  memset(decimalpart, ' ', sizeof(decimalpart));
  strncpy(decimalpart, &hms[ipos+1], 3);
  milliseconds = atoi(decimalpart);	

  
  sprintf(hms, "%d:%d:%d.%d", hours, minutes, seconds, milliseconds);
  return hms;
}





// void correlate(float input[], int input_length,float filter[], int filter_length,float output[]){

//     for( int i = 0; i < input_length; i++){ //for every element in input
        
//         float res = 0;
        
//         for(int j = 0; j < filter_length; j++){ //for every element in filter

//             int real = j - filter_length/2;

//             if( i + real < 0 || i + real >= input_length){
//                 res += 0; //nothing
//             }else{
//                 res += input[i+real]*filter[j];
//             }
//         }

//         output[i] = res;
        
//     }

// }
