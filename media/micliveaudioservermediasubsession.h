#ifndef MICLIVEAUDIOSERVERMEDIASUBSESSION_H
#define MICLIVEAUDIOSERVERMEDIASUBSESSION_H

#include "SimpleRTPSink.hh"
#include "ByteStreamFileSource.hh"
#include "FileServerMediaSubsession.hh"

class MicLiveAudioServerMediaSubsession : public OnDemandServerMediaSubsession
{
public:
    static MicLiveAudioServerMediaSubsession*
        createNew(UsageEnvironment& env, Boolean reuseFirstSource);

private:
    MicLiveAudioServerMediaSubsession(UsageEnvironment& env,Boolean reuseFirstSource);
    // called only by createNew();
    virtual ~MicLiveAudioServerMediaSubsession();

private:
    // redefined virtual functions
    virtual FramedSource* createNewStreamSource(unsigned clientSessionId,
        unsigned& estBitrate);
    virtual RTPSink* createNewRTPSink(Groupsock* rtpGroupsock,
        unsigned char rtpPayloadTypeIfDynamic,
        FramedSource* inputSource);
protected:
    virtual char const* sdpLines();
};

#endif // MICLIVEAUDIOSERVERMEDIASUBSESSION_H
