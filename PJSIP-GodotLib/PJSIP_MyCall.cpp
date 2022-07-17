#include "PJSIP_Common.h"

void MyCall::onCallState(OnCallStateParam& prm)
{
    PJ_UNUSED_ARG(prm);

    CallInfo ci = getInfo();
    std::cout << "*** Call: " << ci.remoteUri << " [" << ci.stateText
        << "]" << std::endl;

    if (ci.state == PJSIP_INV_STATE_DISCONNECTED) {
        //myAcc->removeCall(this);
        /* Delete the call */
        //delete this;
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

    //this will connect the call audio media to the stream
    /*if (!pcm_capture) {
        std::cout << "audio media connected to stream" << std::endl;
        pcm_capture = new AudioMediaCapture();
        pcm_capture->createMediaCapture(ci.id);
        aud_med.startTransmit(*pcm_capture);
    }*/
    // And this will connect the call audio media to the sound device/speaker
    aud_med.startTransmit(play_dev_med);
}

char* MyCall::getFrames()
{
    if (pcm_capture) {
        char* s;
        size_t datasize;
        pcm_capture->getFrames(&s, &datasize);
        return s;
    }
    return '\0';
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