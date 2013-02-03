#ifndef _WAV_UTILS_H
#define _WAV_UTILS_H

#define RIFF_ID		(('R'<<24) | ('I'<<16) | ('F'<<8) | 'F')
#define WAVE_ID		(('W'<<24) | ('A'<<16) | ('V'<<8) | 'E')
#define FMT_ID		(('f'<<24) | ('m'<<16) | ('t'<<8) | ' ')
#define DATA_ID		(('d'<<24) | ('a'<<16) | ('t'<<8) | 'a')
#define FACT_ID		(('f'<<24) | ('a'<<16) | ('c'<<8) | 't')

#define CHUNK_SIZE			0x24080000
#define SUBCHUNK_1_SIZE		0x10000000

#define AUDIO_FORMAT		0x0100
#define HEADER_NUM_CHANNELS		0x0100

#define WAV_HEADER_SIZE		44

//#define SAMPLE_RATE			0x22560000
#define BITS_PER_SAMPLE		16

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

int raw_to_wav(char *infile_name, char *outfile_name, int data_length);
void make_header(char* header, int data_length);


#endif
