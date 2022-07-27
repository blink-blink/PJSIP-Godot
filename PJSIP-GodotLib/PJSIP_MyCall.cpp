#include "PJSIP_Common.h"
#include <fstream>

vector<MyCall*> MyCall::calls;

size_t MyCall::get_idx()
{
    return idx;
}

MyCall* MyCall::calls_lookup(size_t call_id)
{
    if (calls.size() <= 0) return NULL;
    return calls[call_id];
}

void MyCall::onCallState(OnCallStateParam& prm)
{
    PJ_UNUSED_ARG(prm);

    CallInfo ci = getInfo();
    std::cout << "*** Call: " << ci.remoteUri << " [" << ci.stateText
        << "]" << std::endl;

    if (ci.state == PJSIP_INV_STATE_DISCONNECTED) {
        myAcc->removeCall(this);
        /* Delete the call */
        delete this;
    }
}

void MyCall::onCallMediaState(OnCallMediaStateParam& prm)
{
    PJ_UNUSED_ARG(prm);

    CallInfo ci = getInfo();
    AudioMedia aud_med;
    AudioMedia& play_dev_med =
        MyEndpoint::instance().audDevManager().getPlaybackDevMedia();
    AudioMedia& cap_dev_med = Endpoint::instance().audDevManager().getCaptureDevMedia();

    try {
        // Get the first audio media
        aud_med = getAudioMedia(-1);
    }
    catch (...) {
        std::cout << "Failed to get audio media" << std::endl;
        return;
    }

    // This will connect the wav file to the call audio media
    cap_dev_med.startTransmit(aud_med);


    // And this will connect the call audio media to the sound device/speaker
    aud_med.startTransmit(play_dev_med);

    if (!pcm_stream) {
        pcm_stream = new AudioMediaStream();
        pcm_stream->createMediaStream(ci.id);
        pcm_stream->startTransmit(aud_med);
        //pcm_stream->startTransmit(play_dev_med);
    }

    //this will connect the call audio media to the stream
    if (!pcm_capture) {
        std::cout << "audio media connected to stream" << std::endl;
        pcm_capture = new AudioMediaCapture();
        pcm_capture->createMediaCapture(ci.id);
        aud_med.startTransmit(*pcm_capture);
        //pcm_stream->startTransmit(*pcm_capture);
    }
}

void MyCall::putFrame(char* chunk, size_t datasize)
{
    //std::cout << "outgoing frame: " << chunk << "of size: " << strlen(chunk) <<std::endl;
    if (pcm_stream) {
        pcm_stream->putFrame(chunk, datasize);
    }
}

void MyCall::putFrameAsString(std::string s) {
    if (pcm_stream) {
        pcm_stream->putFrameAsString(s);
        //std::cout << "frames pushed to stream" << '\n';
    }
}

void MyCall::getFrames(char** s, size_t* datasize)
{
    if (pcm_capture) {
        pcm_capture->getFrames(s, datasize);
        //std::cout << "====fetch: " << *datasize << '\n';
        return;
    }
    std::cout << "no pcm capture" << std::endl;
}

std::string MyCall::getFramesAsString()
{
    if (pcm_capture) {
        std::string s = pcm_capture->getFramesAsString();
        //std::cout << "====fetch: " << s.length() << '\n';
        return s;
    }
    return "\0";
}

void MyCall::onCallTransferRequest(OnCallTransferRequestParam& prm)
{
    /* Create new Call for call transfer */
    prm.newCall = new MyCall(*myAcc);
}

void MyCall::onCallReplaced(OnCallReplacedParam& prm)
{
    /* Create new Call for call replace */
    prm.newCall = new MyCall(*myAcc, prm.newCallId);
}