#include "dev_alsa.h"

//初始化alsa device
int Alsa_Dev_init(Pcm_params *pcm)
{
    int ret;

    //allocate snd_pcm_hw_params_t
    snd_pcm_hw_params_alloca(&pcm->hwparams);

        //open pcm device
    if(snd_pcm_open(&pcm->pcm_handle, pcm->pcm_card_name, pcm->stream_direction, 0) < 0)
    {
        fprintf(stderr, "Error opening PCM device %s\n", pcm->pcm_card_name);
        return -1;
    }

    /* Init hwparams with full configuration space */
    if (snd_pcm_hw_params_any(pcm->pcm_handle, pcm->hwparams) < 0) {
      fprintf(stderr, "Can not configure this PCM device.\n");
      return -1;
    }

    //设置数据访问模式
    if (snd_pcm_hw_params_set_access(pcm->pcm_handle, pcm->hwparams, pcm->access_type) < 0) {

        fprintf(stderr, "Can not set hw params access\n");
        return -1;
    }
    //set sample format bit per sample
    if(snd_pcm_hw_params_set_format(pcm->pcm_handle, pcm->hwparams, pcm->sample_format) < 0)
    {
        fprintf(stderr, "snd_pcm_hw_params_set_format fail\n");
        return -1;

    }
    //set channel num
    if(snd_pcm_hw_params_set_channels(pcm->pcm_handle, pcm->hwparams, pcm->channel_num) < 0)
    {
        fprintf(stderr, "snd_pcm_hw_params_set_channels fail\n");
        return -1;
    }
    //set period
    if(snd_pcm_hw_params_set_periods(pcm->pcm_handle, pcm->hwparams, pcm->periods, 0) < 0)
    {
        fprintf(stderr, "snd_pcm_hw_params_set_periods fail\n");
        return -1;
    }

    //set sample rate
    if(snd_pcm_hw_params_set_rate_near(pcm->pcm_handle, pcm->hwparams, &pcm->sample_rate, 0) < 0)
    {
        fprintf(stderr, "snd_pcm_hw_params_set_rate_near fail\n");
        return -1;
    }
    //每帧大小  每bit样本长度 * channel_num / 8bit
    //buffer size 按帧计算器 周期数 * 每个周期时间
    /* Set buffer size (in frames). The resulting latency is given by */
        /* latency = periodsize * periods / (rate * bytes_per_frame)     */
    //pcm->frames = (pcm->period_szie * pcm->periods) >> 2;
    if (snd_pcm_hw_params_set_buffer_size(pcm->pcm_handle, pcm->hwparams, pcm->frames) < 0) {
      fprintf(stderr, "Error setting buffersize.\n");
      return(-1);
    }

            /* Apply HW parameter settings to */
    /* PCM device and prepare device  */
    if (snd_pcm_hw_params(pcm->pcm_handle, pcm->hwparams) < 0) {
      fprintf(stderr, "Error setting HW params.\n");
      return(-1);
    }

    return 0;

}
//播放
int Alsa_Playback(Pcm_params *pcm, char **buf)
{
    int pcmreturn;
    while ((pcmreturn = snd_pcm_writei(pcm->pcm_handle, buf, pcm->frames)) < 0) {
            snd_pcm_prepare(pcm->pcm_handle);
            fprintf(stderr, "<<<<<<<<<<<<<<< Buffer Underrun >>>>>>>>>>>>>>>\n");
      }
    return 0;
}

int Pcm_CloseDevice(Pcm_params *pcm)
{

        /* Stop PCM device and drop pending frames */
    snd_pcm_drop(pcm->pcm_handle);

    /* Stop PCM device after pending frames have been played */ 
    snd_pcm_drain(pcm->pcm_handle);

    return 0;
}

int Alsa_SetVolum(int mode, int ctr_value)
{
    snd_mixer_t *mixer;
    snd_mixer_elem_t *elem;
    snd_mixer_selem_channel_id_t channel;
    long value;
    long min_value, max_value;
    //open mixer
    snd_mixer_open(&mixer, 0);

    //连接到指定的pcm
    snd_mixer_attach(mixer, "default");
    //注册mixer
    snd_mixer_selem_register(mixer, NULL, NULL);

    //加载
    snd_mixer_load(mixer);

    //获取第一个selem
     elem = snd_mixer_first_elem(mixer);

    snd_mixer_selem_get_playback_volume_range(elem, &min_value, &max_value);
    printf("get volune range :min %d max %d\n", min_value, max_value);

    snd_mixer_selem_get_playback_dB_range(elem, &min_value, &max_value);
    printf("get db range :min %d max %d\n", min_value, max_value);
    //设置音量范围
    snd_mixer_selem_set_playback_volume_range(elem, 0, 255);
    //获取左右声道的音量
    snd_mixer_selem_get_playback_volume(elem, SND_MIXER_SCHN_FRONT_LEFT, &value);
    printf("left value :%d\n", value);

    snd_mixer_selem_get_playback_volume(elem, SND_MIXER_SCHN_FRONT_RIGHT, &value);
    printf("right value :%d\n", value); 

    if(mode == 1)
    {
        printf("nanhe 123");
        snd_mixer_selem_set_playback_volume(elem, SND_MIXER_SCHN_FRONT_LEFT, ctr_value);
        snd_mixer_selem_set_playback_volume(elem, SND_MIXER_SCHN_FRONT_RIGHT, ctr_value);

            snd_mixer_selem_get_playback_volume(elem, SND_MIXER_SCHN_FRONT_LEFT, &value);
    printf("123left value :%d\n", value);

    snd_mixer_selem_get_playback_volume(elem, SND_MIXER_SCHN_FRONT_RIGHT, &value);
    printf("123right value :%d\n", value); 

    }

}