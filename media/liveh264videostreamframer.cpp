#include "liveh264videostreamframer.h"

#include "GroupsockHelper.hh" // gettimeofday  live555
#include "h264encoder.h"	//H264 Encode
#include <QDebug>


LiveH264VideoStreamFramer::LiveH264VideoStreamFramer(UsageEnvironment& env,FramedSource* inputSource, H264Encoder* pH264Enc):
    H264VideoStreamFramer(env, inputSource, False, False),
    m_pNalArray(NULL), m_iCurNalNum(0), m_iCurNal(0), m_iCurFrame(0),
    m_pH264Enc(pH264Enc)
{
    m_pFormatCtx=NULL;
    m_pCodecCtx=NULL;
    m_pImgConvertCtx=NULL;

    m_pFrame=NULL;
    m_pFrameYUV=NULL;

    fFrameRate = 20.0; // We assume a frame rate of 25 fps, unless we learn otherwise (from parsing a Sequence Parameter Set NAL unit)
    qDebug()<<"LiveH264VideoStreamFramer::LiveH264VideoStreamFramer";
}

LiveH264VideoStreamFramer::~LiveH264VideoStreamFramer()
{
    UnInitFFMpeg();

    m_pH264Enc->Destroy();
    //delete m_pH264Enc;
    m_pH264Enc = NULL;
    qDebug()<<"LiveH264VideoStreamFramer::~LiveH264VideoStreamFramer";
}


int LiveH264VideoStreamFramer::InitFFMpeg(void)
{
    int				i, videoindex;
    AVCodec			*pCodec;
    AVInputFormat *ifmt;

    try{
        av_register_all();
        avformat_network_init();
        m_pFormatCtx = avformat_alloc_context();

        //Register Device
        avdevice_register_all();

        //ifmt = av_find_input_format("dshow");
        //if (avformat_open_input(&pFormatCtx, "video=Integrated Webcam", ifmt, NULL) != 0)
        ifmt = av_find_input_format("gdigrab");
        if (avformat_open_input(&m_pFormatCtx, "desktop", ifmt, NULL) != 0)
        {
            qDebug()<<"Couldn't open input stream.（无法打开输入流）\n";
            return -1;
        }

        if (avformat_find_stream_info(m_pFormatCtx, NULL) < 0)
        {
            qDebug()<<"Couldn't find stream information.（无法获取流信息）\n";
            return -1;
        }
        videoindex = -1;
        for (i = 0; i < m_pFormatCtx->nb_streams; i++)
        {
            if (m_pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO)
            {
                videoindex = i;
                break;
            }
        }
        if (videoindex == -1)
        {
            qDebug()<<"Didn't find a video stream.（没有找到视频流）\n";
            return -1;
        }
        m_pCodecCtx = m_pFormatCtx->streams[videoindex]->codec;
        pCodec = avcodec_find_decoder(m_pCodecCtx->codec_id);
        if (pCodec == NULL)
        {
            qDebug()<<"Codec not found.（没有找到解码器）\n";
            return -1;
        }
        if (avcodec_open2(m_pCodecCtx, pCodec, NULL) < 0)
        {
            qDebug()<<"Could not open codec.（无法打开解码器）\n";
            return -1;
        }

        //如果宽度不是32的倍数，将会降低sws_scale的速度，故我们保证宽度为32的倍数。
        VIDEO_WIDTH = m_pCodecCtx->width / 32 * 32;
        //保证高度等比例缩放，再保证为16的倍数。
        VIDEO_HEIGHT = m_pCodecCtx->height / 16 * 16;

        m_pFrame = av_frame_alloc();
        m_pFrameYUV = av_frame_alloc();

        //AV_PIX_FMT_YUV420P与X264_CSP_I420
        uint8_t *out_buffer = (uint8_t *)av_malloc(avpicture_get_size(AV_PIX_FMT_YUV420P, VIDEO_WIDTH, VIDEO_HEIGHT));
        avpicture_fill((AVPicture *)m_pFrameYUV, out_buffer, AV_PIX_FMT_YUV420P, VIDEO_WIDTH, VIDEO_HEIGHT);

        m_pImgConvertCtx = sws_getContext(m_pCodecCtx->width, m_pCodecCtx->height, m_pCodecCtx->pix_fmt, VIDEO_WIDTH, VIDEO_HEIGHT, AV_PIX_FMT_YUV420P, SWS_POINT, NULL, NULL, NULL);

    }catch(...)
    {
        qDebug()<<"Init FFMpeg failed\n";
        return -1;
    }
    return 0;

}

void LiveH264VideoStreamFramer::UnInitFFMpeg(void)
{
    if (m_pFormatCtx != NULL)
        avformat_close_input(&m_pFormatCtx);

    if (m_pImgConvertCtx != NULL)
        av_free(m_pImgConvertCtx);

    if(m_pFrame)
        av_frame_free(&m_pFrame);
    if(m_pFrameYUV)
        av_frame_free(&m_pFrameYUV);

}

LiveH264VideoStreamFramer* LiveH264VideoStreamFramer::createNew(
    UsageEnvironment& env,
    FramedSource* inputSource)
{
    qDebug()<<"LiveH264VideoStreamFramer::createNew\n";

    H264Encoder* pH264Enc = new H264Encoder;
    LiveH264VideoStreamFramer* fr;
    fr = new LiveH264VideoStreamFramer(env, inputSource, pH264Enc);

    int n = fr->InitFFMpeg();
    if (n < 0)
    {
        env.setResultMsg("Initialize FFMpeg error");
        delete fr;
        return NULL;
    }

    // H264 Encode
    if (pH264Enc->Initialize(fr->VIDEO_WIDTH, fr->VIDEO_HEIGHT, 96 * 10, 25) < 0)
    {
        env.setResultMsg("Initialize x264 encoder error");
        return NULL;
    }

    return fr;
}

Boolean LiveH264VideoStreamFramer::currentNALUnitEndsAccessUnit()
{
    if (m_iCurNal >= m_iCurNalNum)
    {
        m_iCurFrame++;
        return True;
    }
    else
    {
        return False;
    }
}

// live555每一次调用doGetNextFrame()时，先查看是否还有上一个视频帧的NAL Unit没有
// 发送完成，如果没有，继续发送，如果没有，先从摄像头获取一个YUV的数据帧，x264
// 编码YUV成一个NAL Unit数组，再开始发送NAL数组，一次发送一个NAL Unit
void LiveH264VideoStreamFramer::doGetNextFrame()
{
    QMutexLocker locker(&_mutex);
    try{

        TNAL* pNal = NULL;
        unsigned char* pOrgImg=NULL;

        qDebug()<<"CurNal:"<<m_iCurNal<<" CurNalNum:"<<m_iCurNalNum<<" m_pNalArray:"<<m_pNalArray;

        //
        if ((m_pNalArray != NULL) && (m_iCurNal < m_iCurNalNum))
        {
            pNal = &m_pNalArray[m_iCurNal];
            qDebug()<<"have undelivered data!";
        }
        else
        {
            m_pH264Enc->CleanNAL(m_pNalArray, m_iCurNalNum);//free buff m_pNalArray=NULL
            m_iCurNal = 0;
            m_iCurNalNum=0;

            AVPacket* packet = (AVPacket *)av_malloc(sizeof(AVPacket));
            try{
                // 获取最新视频帧,YUV
                int ret,got_picture = 0;
                ret = av_read_frame(m_pFormatCtx, packet);
                if(ret<0)
                    qDebug()<<"av_read_frame fail!";
                else
                {
                    //qDebug()<<"av_read_frame succeed!";
                    ret = avcodec_decode_video2(m_pCodecCtx, m_pFrame, &got_picture, packet);
                    if(ret<0)
                        qDebug()<<"avcodec_decode_video2 return fail!";
                    if (got_picture){
                        sws_scale(m_pImgConvertCtx, (const uint8_t* const*)m_pFrame->data, m_pFrame->linesize, 0, m_pCodecCtx->height, m_pFrameYUV->data, m_pFrameYUV->linesize);
                    }else
                    {
                        qDebug()<<"avcodec_decode_video2 got_picture!=true!";
                    }
                    pOrgImg = m_pFrameYUV->data[0];
                    gettimeofday(&fPresentationTime, NULL);

                    //qDebug()<<"Frame["<<m_pCodecCtx->height<<","<<m_pCodecCtx->width<<"]";
                    // H264 Encode
                    m_pH264Enc->Encode(pOrgImg, m_pNalArray, m_iCurNalNum);
                    pNal = &m_pNalArray[m_iCurNal];
                }
            }catch(...)
            {
                qDebug()<<"av_read_frame or h264Encode on error!";
            }

            //printf(INF, "Frame[%d], Nal[%d:%d]: size = %d", m_iCurFrame, m_iCurNalNum, m_iCurNal, pNal->size);
            av_free_packet(packet);
        }
        m_iCurNal++;

        unsigned char* realData =NULL;
        unsigned int realLen =0;
        if(pNal!=NULL)
        {
            realData=pNal->data;
            realLen=pNal->size;
        }

        //qDebug()<<"Frame no:"<<m_iCurFrame<<", Nal["<<m_iCurNalNum<<":"<<m_iCurNal<<"]: size = "<< realLen;
        if (realLen < fMaxSize)
        {
            memcpy(fTo, realData, realLen);
        }
        else
        {
            memcpy(fTo, realData, fMaxSize);
            fNumTruncatedBytes = realLen - fMaxSize;
        }

        //表示单个视频或者音频帧所占用的时间间隔，也表示在fDurationInMicroseconds微秒时间后再次向Source进行getNextFrame操作，
        /*以视频为例，可以这样计算：
        视频帧率fFrameRate=25，那么我们每一次在doGetNextFrame中赋值fDurationInMicroseconds = 1000000/fFrameRate = 40000;
        注意ONVIF里面GetVideoSources的Framerate参数
        以音频为例，可以这样计算：
        假如音频的采样率为fSamplingFrequency=8000，那么我们每一次在doGetNextFrame中赋值fDurationInMicroseconds = 1000000/fSamplingFrequency = 125;
        */
        fDurationInMicroseconds = 200000;//原来值是200000;

        fFrameSize = realLen;
        //qDebug()<<"fNumTruncatedBytes:"<<fNumTruncatedBytes<<", fFrameSize:"<<fFrameSize<<" fPresentationTime:"<<fPresentationTime.tv_usec<<"-"<<fPresentationTime.tv_sec;


        afterGetting(this);

    }catch(...)
    {
        qDebug()<<"LiveH264VideoStreamFramer::doGetNextFrame() catch error\n";
        return ;
    }
}


