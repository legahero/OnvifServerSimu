#include "micliveaudioservermediasubsession.h"
#include "qmicaudiostreamframer.h"

#include <QDebug>

MicLiveAudioServerMediaSubsession*
MicLiveAudioServerMediaSubsession::createNew(UsageEnvironment& env,Boolean reuseFirstSource)
{
    return new MicLiveAudioServerMediaSubsession(env, reuseFirstSource);
}

MicLiveAudioServerMediaSubsession::MicLiveAudioServerMediaSubsession(UsageEnvironment& env,
Boolean reuseFirstSource):OnDemandServerMediaSubsession(env, reuseFirstSource)
{
}

MicLiveAudioServerMediaSubsession::~MicLiveAudioServerMediaSubsession() {
}

FramedSource* MicLiveAudioServerMediaSubsession::createNewStreamSource(unsigned /*clientSessionId*/, unsigned& estBitrate)
{
    estBitrate = 96; // kbps, estimate
    // Create the video source:
    return QMICAudioStreamFramer::createNew(envir(), NULL);
}

RTPSink* MicLiveAudioServerMediaSubsession::createNewRTPSink(Groupsock* rtpGroupsock,
    unsigned char rtpPayloadTypeIfDynamic,FramedSource* /*inputSource*/) {

    int m_timeStampFrequency=44100;
    char m_audioType[]="G726-40";
    //m_timeStampFrequency，根据实际情况填写16000或者8000等采样率
    //m_audioType，编码方式，例如："G726-40"。这个准确的定义，请参照MediaSession::createSourceObjects中的内容
    return SimpleRTPSink::createNew(envir(), rtpGroupsock, rtpPayloadTypeIfDynamic,
    m_timeStampFrequency, "audio", m_audioType, 1, False, False);
}

char const* MicLiveAudioServerMediaSubsession::sdpLines()
{
    return fSDPLines =
        "m=audio 0 RTP/AVP 97\r\n"
        "c=IN IP4 0.0.0.0\r\n"
        "b=AS:97\r\n"
        "a=rtpmap:97 mpeg4-generic/44100/2\r\n"
        "a=fmtp:97 packetization-mode=1;profile-level-id=000000;sprop-parameter-sets=H264\r\n"
        "a=control:track2\r\n";
}

