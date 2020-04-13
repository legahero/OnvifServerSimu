#ifndef H264ENCODER_H
#define H264ENCODER_H

#include "x264.h"

struct TNAL
{
    int size;
    unsigned char* data;
    TNAL(): size(0), data(NULL) {}
};


class H264Encoder
{
public:
    H264Encoder();
    virtual ~H264Encoder();

    // 初始化编码器
    int Initialize(int iWidth, int iHeight, int iRateBit = 96, int iFps = 25);
    // 对一帧对象进行编码，返回NAL数组
    int Encode(unsigned char* szYUVFrame, TNAL*& pNALArray, int& iNalNum);
    // 销毁NAL数组
    void CleanNAL(TNAL* &pNALArray, int iNalNum);
    // 销毁编码器
    int Destroy();

private:
    x264_param_t m_param;
    x264_picture_t m_pic;
    x264_t* m_h;

    unsigned char *m_pBuffer; //存放单个NAL
    int m_iBufferSize;//NAL的大小
    int m_iFrameNum;//帧数
};


#endif // H264ENCODER_H
