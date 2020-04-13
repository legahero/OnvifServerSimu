#include "liveh264videoservermediasubsession.h"
#include "liveh264videostreamframer.h"

// H264LiveVideoServerMediaSubsession *********************************************************
LiveH264VideoServerMediaSubsession*
LiveH264VideoServerMediaSubsession::createNew(UsageEnvironment& env,Boolean reuseFirstSource)
{
    return new LiveH264VideoServerMediaSubsession(env, reuseFirstSource);
}

LiveH264VideoServerMediaSubsession::LiveH264VideoServerMediaSubsession(UsageEnvironment& env,
Boolean reuseFirstSource)
: OnDemandServerMediaSubsession(env, reuseFirstSource)
{
}

LiveH264VideoServerMediaSubsession::~LiveH264VideoServerMediaSubsession() {
}

FramedSource* LiveH264VideoServerMediaSubsession::createNewStreamSource(unsigned /*clientSessionId*/, unsigned& estBitrate)
{
    estBitrate = 96;
    // Create the video source:
    return LiveH264VideoStreamFramer::createNew(envir(), NULL);
}

RTPSink* LiveH264VideoServerMediaSubsession::createNewRTPSink(Groupsock* rtpGroupsock,
    unsigned char rtpPayloadTypeIfDynamic,
    FramedSource* /*inputSource*/) {
    return H264VideoRTPSink::createNew(envir(), rtpGroupsock, 96);
}

char const* LiveH264VideoServerMediaSubsession::sdpLines()
{
    return fSDPLines =
        "m=video 0 RTP/AVP 96\r\n"
        "c=IN IP4 0.0.0.0\r\n"
        "b=AS:96\r\n"
        "a=rtpmap:96 H264/90000\r\n"
        "a=fmtp:96 packetization-mode=1;profile-level-id=000000;sprop-parameter-sets=H264\r\n"
        "a=control:track1\r\n";//        //"a=control:track1\r\n";
}
