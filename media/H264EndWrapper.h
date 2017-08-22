#ifndef _X264WRAPPER_H_
#define _X264WRAPPER_H_

//#include "stdint.h"
#include "x264.h"

struct TNAL
{
    int size;
    unsigned char* data;
    TNAL(): size(0), data(NULL) {}
};


class H264EncWrapper
{
public:
    H264EncWrapper();
    virtual ~H264EncWrapper();

    // 初始化编码器
    int Initialize(int iWidth, int iHeight, int iRateBit = 96, int iFps = 25);
    // 对一帧对象进行编码，返回NAL数组
    int Encode(unsigned char* szYUVFrame, TNAL*& pNALArray, int& iNalNum);
    // 销毁NAL数组
    void CleanNAL(TNAL* pNALArray, int iNalNum);
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

#endif

