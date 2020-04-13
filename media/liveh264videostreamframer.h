#ifndef LIVEH264VIDEOSTREAMFRAMER_H
#define LIVEH264VIDEOSTREAMFRAMER_H

#include "H264VideoStreamFramer.hh"

extern "C"
{
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavdevice/avdevice.h"
#include "libswscale/swscale.h"
#include "libavutil/avutil.h"
}

#include <QMutex>

//*********************************************************************
// 获取摄像头视频YUV数据后，编码成H264，并推送给DSS
class H264Encoder;
struct TNAL;

class LiveH264VideoStreamFramer : public H264VideoStreamFramer
{
public:
    virtual ~LiveH264VideoStreamFramer();
    LiveH264VideoStreamFramer(UsageEnvironment& env,FramedSource* inputSource, H264Encoder* pH264Enc);

    static LiveH264VideoStreamFramer* createNew(UsageEnvironment& env, FramedSource* inputSource);
    virtual Boolean currentNALUnitEndsAccessUnit();
    virtual void doGetNextFrame();

    int InitFFMpeg(void);
    void UnInitFFMpeg(void);

public:
    int VIDEO_WIDTH, VIDEO_HEIGHT;

private:
    AVFormatContext	*m_pFormatCtx=NULL;
    AVCodecContext	*m_pCodecCtx=NULL;
    SwsContext *m_pImgConvertCtx=NULL;

    //用来存放从AVPacket中解码出来的原始数据，这个AVFrame的数据缓存空间通过调avcodec_decode_video分配和填充。
    AVFrame	*m_pFrame=NULL;
    //用来存放将解码出来的原始数据变换为需要的数据格式（例如RGB，RGBA）的数据，这个AVFrame需要手动的分配数据缓存空间。
    AVFrame *m_pFrameYUV=NULL;

    H264Encoder* m_pH264Enc;

    TNAL* m_pNalArray;
    int m_iCurNalNum;		//当前Frame一共有多少个NAL
    int m_iCurNal;		//当前使用的是第几个NAL
    unsigned int m_iCurFrame=0;

    QMutex _mutex;//lock
};


#endif // LIVEH264VIDEOSTREAMFRAMER_H
