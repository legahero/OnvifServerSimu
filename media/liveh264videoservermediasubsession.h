#ifndef LIVEH264VIDEOSERVERMEDIASUBSESSION_H
#define LIVEH264VIDEOSERVERMEDIASUBSESSION_H

#include "H264VideoRTPSink.hh"
#include "ByteStreamFileSource.hh"
#include "H264VideoStreamFramer.hh"
#include "FileServerMediaSubsession.hh"

class LiveH264VideoServerMediaSubsession : public OnDemandServerMediaSubsession
{
public:
    static LiveH264VideoServerMediaSubsession*
        createNew(UsageEnvironment& env, Boolean reuseFirstSource);

private:
    LiveH264VideoServerMediaSubsession(UsageEnvironment& env,Boolean reuseFirstSource);
    // called only by createNew();
    virtual ~LiveH264VideoServerMediaSubsession();

private:
    // redefined virtual functions
    virtual FramedSource* createNewStreamSource(unsigned clientSessionId,unsigned& estBitrate);
    virtual RTPSink* createNewRTPSink(Groupsock* rtpGroupsock,unsigned char rtpPayloadTypeIfDynamic,
        FramedSource* inputSource);
protected:
    virtual char const* sdpLines();
};

#endif // LIVEH264VIDEOSERVERMEDIASUBSESSION_H
