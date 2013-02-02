#ifndef _WAV_UTILS_H
#define _WAV_UTILS_H

#define RIFF_ID		(('R'<<24) | ('I'<<16) | ('F'<<8) | 'F')
#define WAVE_ID		(('W'<<24) | ('A'<<16) | ('V'<<8) | 'E')
#define FMT_ID		(('f'<<24) | ('m'<<16) | ('t'<<8) | ' ')
#define DATA_ID		(('d'<<24) | ('a'<<16) | ('t'<<8) | 'a')
#define FACT_ID		(('f'<<24) | ('a'<<16) | ('c'<<8) | 't')

#define WAV_HEADER_SIZE		44

#define WAV_ERR_CHUNK_SIZE		(-1)
#define WAV_ERR_FILE_TYPE		(-2)
#define WAV_ERR_ILLEGAL_VALUE	(-3)
#define WAV_ERR_FORMAT_TYPE		(-4)
#define WAV_ERR_TRUNCATED		(-5)

#define WAV_FORMAT_PCM			(1)
#define WAV_FORMAT_IMA_ADPCM	(0x0011)

#define BUFFER_SIZE 256

#define SUCCESS 0
#define FAILURE 1

int raw_to_wav(char *infile_name, char *outfile_name, int num_channels, int sample_rate, int bits_per_sample, int data_length);
void make_header(int num_channels, int sample_rate, int bits_per_sample, char* header, int data_length);


#endif
