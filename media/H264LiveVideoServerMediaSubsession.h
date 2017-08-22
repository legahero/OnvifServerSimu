#include "H264VideoRTPSink.hh"
#include "ByteStreamFileSource.hh"
#include "H264VideoStreamFramer.hh"
#include "FileServerMediaSubsession.hh"

class H264LiveVideoServerMediaSubsession : public OnDemandServerMediaSubsession{
public:
	static H264LiveVideoServerMediaSubsession*
		createNew(UsageEnvironment& env, Boolean reuseFirstSource);

private:
	H264LiveVideoServerMediaSubsession(UsageEnvironment& env,
		Boolean reuseFirstSource);
	// called only by createNew();
	virtual ~H264LiveVideoServerMediaSubsession();

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
