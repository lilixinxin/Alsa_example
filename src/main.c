#include "alsa/asoundlib.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include "dev_wav.h"

int rate = 44100;
int periods = 20;
snd_pcm_uframes_t periodsize = 8192; /* Periodsize (bytes) */


unsigned char *data;
int pcmreturn, l1, l2;
short s1, s2;
int frames;
static Wave_head Head_file;

int main(int argc, char *argv[])
{
    FILE *fp;
    int ret;

    char *wavefile = strdup(argv[1]);
    printf("wave file name:%s\n", wavefile);
    ret = Wave_Parse_Head(wavefile, &Head_file, &fp);
    if(fp == NULL)
    {
        fprintf(stderr, "parase wave head fail\n");
        return -1;
    }

#if 1
    //handle for alsa_device
    snd_pcm_t *pcm_handle;
    //pcm stream direction
    snd_pcm_stream_t stream = SND_PCM_STREAM_PLAYBACK;
    //specify configution for pcm
    snd_pcm_hw_params_t *hwparams;
    //pcm name like plughw:0,0
    char *pcm_card_name;

    //init pcm_card_name
    //pcm_card_name = strdup("plughw:0,0");
    pcm_card_name = strdup(argv[2]);
    
    //allocate snd_pcm_hw_params_t
    snd_pcm_hw_params_alloca(&hwparams);

    //open pcm device
    if(snd_pcm_open(&pcm_handle, pcm_card_name, stream, 0) < 0)
    {
        fprintf(stderr, "Error opening PCM device %s\n", pcm_card_name);
        return -1;
    }

    /* Init hwparams with full configuration space */
    if (snd_pcm_hw_params_any(pcm_handle, hwparams) < 0) {
      fprintf(stderr, "Can not configure this PCM device.\n");
      return(-1);
    }

    //设置数据为交错模式
    if (snd_pcm_hw_params_set_access(pcm_handle, hwparams, SND_PCM_ACCESS_RW_INTERLEAVED) < 0) {

        fprintf(stderr, "Can not set hw params access\n");
        return -1;
    }
    //set sample format
    if(snd_pcm_hw_params_set_format(pcm_handle, hwparams, SND_PCM_FORMAT_S16_LE) < 0)
    {
        fprintf(stderr, "snd_pcm_hw_params_set_format fail\n");
        return -1;

    }
    //set sample frequence
    rate = Head_file.sample_rate;
    if(snd_pcm_hw_params_set_rate_near(pcm_handle, hwparams, &rate, 0) < 0)
    {
        fprintf(stderr, "snd_pcm_hw_params_set_rate_near fail\n");
        return -1;
    }

    //set number of channel
    if(snd_pcm_hw_params_set_channels(pcm_handle, hwparams, Head_file.chanel_num) < 0)
    {
        fprintf(stderr, "snd_pcm_hw_params_set_channels fail\n");
        return -1;
    }

    //set period
    if(snd_pcm_hw_params_set_periods(pcm_handle, hwparams, periods, 0) < 0)
    {
        fprintf(stderr, "snd_pcm_hw_params_set_periods fail\n");
        return -1;
    }
    int buffsize;
    buffsize = periods * periodsize;
    char *buff = (unsigned char *)malloc(buffsize);

    frames = buffsize >> 2;
        /* Set buffer size (in frames). The resulting latency is given by */
        /* latency = periodsize * periods / (rate * bytes_per_frame)     */
    if (snd_pcm_hw_params_set_buffer_size(pcm_handle, hwparams, (periodsize * periods)>>2) < 0) {
      fprintf(stderr, "Error setting buffersize.\n");
      return(-1);
    }

	snd_pcm_hw_params_get_period_size(hwparams, &periodsize, 0);
	printf("frames value is : %ld \n", periodsize * 4);


    snd_pcm_uframes_t buffer_size;
	// snd_pcm_hw_params_get_buffer_size函数也是返回的以帧为单位的数，想换算成字节数，也需要根据不同bit乘以不同的数字
	snd_pcm_hw_params_get_buffer_size(hwparams, &buffer_size);
	printf("buffer_size value is : %ld \n", buffer_size * 4);

        /* Apply HW parameter settings to */
    /* PCM device and prepare device  */
    if (snd_pcm_hw_params(pcm_handle, hwparams) < 0) {
      fprintf(stderr, "Error setting HW params.\n");
      return(-1);
    }



    while(1) {
        //ret = fread(buff, 1, buffsize, fp);
    //fseek(fp, 0, SEEK_SET);
    ret = fread(buff, 1, buffsize, fp);
    printf("read len:%d\n",ret );
    if(ret == 0){
        printf("end of music file input! \n");
        exit(1);
    }
    
    if(ret < 0){
        
        printf("read pcm from file! \n");
        exit(1);
    }
        while ((pcmreturn = snd_pcm_writei(pcm_handle, buff, frames)) < 0) {
            snd_pcm_prepare(pcm_handle);
            fprintf(stderr, "<<<<<<<<<<<<<<< Buffer Underrun >>>>>>>>>>>>>>>\n");
      }
    }




    /* Stop PCM device and drop pending frames */
    snd_pcm_drop(pcm_handle);

    /* Stop PCM device after pending frames have been played */ 
    snd_pcm_drain(pcm_handle);

    fclose(fp);
    printf("hello");
    return 0;
#endif



}