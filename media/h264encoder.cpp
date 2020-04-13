#include "h264encoder.h"

#include <stdio.h>
using namespace std;
#include <string.h> // strerror()
#include <stdlib.h>
#include <QDebug>


void *my_malloc( int i_size )
{
#ifdef SYS_MACOSX
    // Mac OS X always returns 16 bytes aligned memory
    return malloc( i_size );
#elif defined( HAVE_MALLOC_H )
    return memalign( 16, i_size );
#else
    uint8_t * buf;
    uint8_t * align_buf;
    buf = (uint8_t *) malloc( i_size + 15 + sizeof( void ** ) +
              sizeof( int ) );
    align_buf = buf + 15 + sizeof( void ** ) + sizeof( int );
    align_buf -= (intptr_t) align_buf & 15;
    *( (void **) ( align_buf - sizeof( void ** ) ) ) = buf;
    *( (int *) ( align_buf - sizeof( void ** ) - sizeof( int ) ) ) = i_size;
    return align_buf;
#endif
}

void my_free( void *p )
{
    if( p )
    {
#if defined( HAVE_MALLOC_H ) || defined( SYS_MACOSX )
        free( p );
#else
        free( *( ( ( void **) p ) - 1 ) );
#endif
    p = NULL;
    }
}

H264Encoder::H264Encoder()
{
    m_h = NULL;
    m_pBuffer = NULL;
    m_iBufferSize = 0;
    m_iFrameNum = 0;
    try {
        x264_param_default(&m_param);
    } catch (...) {
        QMessageLogger(__FILE__, __LINE__, 0).debug()<<" x264_param_default error!";
    }

}

H264Encoder::~H264Encoder()
{
    if(NULL!=m_pBuffer)
        delete [] m_pBuffer;
}

int H264Encoder::Initialize(int iWidth, int iHeight, int iRateBit, int iFps)
{
    m_param.i_width = iWidth;
    m_param.i_height = iHeight;

    m_param.i_fps_num = iFps;
    m_param.i_fps_den = 1;

    m_param.rc.i_bitrate = iRateBit;
    m_param.rc.i_rc_method = X264_RC_ABR;

    m_param.i_frame_reference = 4; /* 参考帧的最大帧数 */
    //m_param.i_keyint_max = 8;
    //m_param.i_keyint_min = 4;

    /*
    //减少帧延时的方法，也就是（zerolatency 设置）
    m_param.rc.i_lookahead = 0;
    m_param.i_sync_lookahead = 0;
    m_param.i_bframe = 0;
    m_param.b_sliced_threads = 1;
    m_param.b_vfr_input = 0;
    m_param.rc.b_mb_tree = 0;
    //减少帧延时的方法，也就是（zerolatency 设置）
*/
    /* 根据输入参数param初始化总结构 x264_t *h     */
    if( ( m_h = x264_encoder_open( &m_param ) ) == NULL )
    {
        fprintf( stderr, "x264 [error]: x264_encoder_open failed\n" );
        return -1;
    }

    x264_picture_alloc( &m_pic, X264_CSP_I420, m_param.i_width, m_param.i_height );
    m_pic.i_type = X264_TYPE_AUTO;
    m_pic.i_qpplus1 = 0;

    return 0;
}

int H264Encoder::Destroy()
{
    x264_picture_clean( &m_pic );

    // TODO: clean m_h
    //x264_encoder_close  (m_h); //?????

   return 0;
}

//FILE* ff1 ;
int H264Encoder::Encode(unsigned char* szYUVFrame, TNAL*& pNALArray, int& iNalNum)
{
    // 可以优化为m_pic中保存一个指针,直接执行szYUVFrame
    memcpy(m_pic.img.plane[0], szYUVFrame, m_param.i_width * m_param.i_height*3 / 2);

    m_pic.i_pts = (int64_t)m_iFrameNum * m_param.i_fps_den;

    x264_picture_t pic_out;
    x264_nal_t *nal;
    int i_nal, i; // nal的个数

    int iResult   = 0;
    iResult=x264_encoder_encode( m_h, &nal, &i_nal, &m_pic, &pic_out );
    if( iResult < 0 )
    {
        //fprintf( stderr, "x264 [error]: x264_encoder_encode failed\n" );
        QMessageLogger(__FILE__, __LINE__, 0).debug()<<"x264 [error]: x264_encoder_encode failed";
        return -1;
    }
    if( iResult = 0 )
    {
        qDebug()<<"encode succeed,But it was cached.";
    }

    int i_size = 0;
    pNALArray = new TNAL[i_nal+1];
    memset(pNALArray, 0, sizeof(TNAL)*(i_nal+1));

    for( i = 0; i < i_nal; i++ )
    {
        if( m_iBufferSize < nal[i].i_payload * 3/2 + 4 )
        {
            m_iBufferSize = nal[i].i_payload * 2 + 4;
            qDebug()<<"m_iBufferSize="<<m_iBufferSize<<endl;//291846,396308


            //my_free( m_pBuffer );
            //m_pBuffer = (uint8_t*)my_malloc( m_iBufferSize );

            if(m_pBuffer)
            {
                //free(m_pBuffer);
                delete [] m_pBuffer;
            }
            //m_pBuffer = (uint8_t*) malloc( m_iBufferSize+16);
            m_pBuffer=new unsigned char[m_iBufferSize+16];
            qDebug()<<"malloc m_pBuffer="<<m_pBuffer;

        }

        i_size = m_iBufferSize;

        x264_nal_encode( m_pBuffer, &i_size, 1, &nal[i] );


        //去掉buffer中前面的 00 00 00 01 才是真正的nal unit
        pNALArray[i].size = i_size;
        pNALArray[i].data = new unsigned char[i_size+16];//i_size+1
        memcpy(pNALArray[i].data, m_pBuffer, i_size);
    }

    iNalNum = i_nal;
    m_iFrameNum++;
    return 0;
}

//free buff
void H264Encoder::CleanNAL(TNAL* &pNALArray, int iNalNum)
{
    for(int i = 0; i < iNalNum; i++)
    {
        if(NULL!=pNALArray[i].data)
            delete []pNALArray[i].data;
        pNALArray[i].data = NULL;
    }
    delete []pNALArray;
    pNALArray = NULL;
}

