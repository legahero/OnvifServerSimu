#include "H264LiveVideoServerMediaSubsession.h"
#include "CamH264VideoStreamFramer.h"

// H264LiveVideoServerMediaSubsession *********************************************************
H264LiveVideoServerMediaSubsession*
H264LiveVideoServerMediaSubsession::createNew(UsageEnvironment& env,Boolean reuseFirstSource)
{
	return new H264LiveVideoServerMediaSubsession(env, reuseFirstSource);
}

H264LiveVideoServerMediaSubsession::H264LiveVideoServerMediaSubsession(UsageEnvironment& env,
Boolean reuseFirstSource)
: OnDemandServerMediaSubsession(env, reuseFirstSource)
{
}

H264LiveVideoServerMediaSubsession::~H264LiveVideoServerMediaSubsession() {
}

FramedSource* H264LiveVideoServerMediaSubsession::createNewStreamSource(unsigned /*clientSessionId*/, unsigned& estBitrate)
{
	estBitrate = 96;
	// Create the video source:
	return CamH264VideoStreamFramer::createNew(envir(), NULL);
}

RTPSink* H264LiveVideoServerMediaSubsession::createNewRTPSink(Groupsock* rtpGroupsock,
	unsigned char rtpPayloadTypeIfDynamic,
	FramedSource* /*inputSource*/) {
	return H264VideoRTPSink::createNew(envir(), rtpGroupsock, 96);
}

char const* H264LiveVideoServerMediaSubsession::sdpLines()
{
	return fSDPLines =
		"m=video 0 RTP/AVP 96\r\n"
		"c=IN IP4 0.0.0.0\r\n"
		"b=AS:96\r\n"
		"a=rtpmap:96 H264/90000\r\n"
		"a=fmtp:96 packetization-mode=1;profile-level-id=000000;sprop-parameter-sets=H264\r\n"
		"a=control:track1\r\n";
}
