#include <stdio.h>
#include <string.h>
#include "dev_wav.h"


int Wave_Parse_Head(char *wav_name, Wave_head* headinfo, FILE **fp)
{
    //FILE *fp;

    *fp = fopen(wav_name, "rb");
    if(*fp == NULL)
    {
        fprintf(stderr, "open wave file fail\n");
        return -1;
    }

    fseek(*fp, 0, SEEK_SET);
    int wav_header_size  = fread(headinfo, 1, sizeof(*headinfo), *fp);
    printf("wav file size\t\t\t%d\n", wav_header_size);
    printf("RIFF_ID\t\t\t 0x%c%c%c%c\n", headinfo->RIFF_ID[0], headinfo->RIFF_ID[1], headinfo->RIFF_ID[2], headinfo->RIFF_ID[3]);
    printf("riff size \t\t\t%d\n", headinfo->RIFF_size);
    printf("file format\t\t\t :%c%c%c%c\n", headinfo->format[0], headinfo->format[1], headinfo->format[2], headinfo->format[3]);

	printf("编码格式代码: 	\t\t\t 	%d 			\n", headinfo->audio_format);
	printf("声道数:			\t 		%d			\n", headinfo->chanel_num);
	printf("采样频率:		\t 		%d			\n", headinfo->sample_rate);
	printf("传输速率： 		\t\t 	%d 			\n", headinfo->byte_rate);
	printf("数据块对齐单位: \t\t\t 	%d			\n", headinfo->block_align);
	printf("采样位数(长度):		\t 		%d			\n", headinfo->bitspersample);

    return 0;
}
