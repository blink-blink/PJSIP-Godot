#include "PJSIP_Common.h"

static void test1(MyEndpoint& ep)
{
    // Init library
    EpConfig ep_cfg;
    ep_cfg.logConfig.level = 2;
    ep.libInit(ep_cfg);

    // Transport
    TransportConfig tcfg;
    tcfg.port = 29801;
    ep.transportCreate(PJSIP_TRANSPORT_UDP, tcfg);

    // Start library
    ep.libStart();
    std::cout << "*** PJSUA2 STARTED ***" << std::endl;

    // Add account
    AccountConfig acc_cfg;
    acc_cfg.idUri = "sip:801@192.168.195.1:5060";

#if PJSIP_HAS_DIGEST_AKA_AUTH
    AuthCredInfo aci("Digest", "*", "test", PJSIP_CRED_DATA_EXT_AKA | PJSIP_CRED_DATA_PLAIN_PASSWD, "passwd");
    aci.akaK = "passwd";
#else
    AuthCredInfo aci("digest", "*", "801", 0, "jpbQXQPnJ5weX6MAIg1Z8GduO");
#endif

    acc_cfg.sipConfig.authCreds.push_back(aci);
    MyAccount* acc(new MyAccount);
    try {
        acc->create(acc_cfg);
    }
    catch (...) {
        std::cout << "Adding account failed" << std::endl;
    }

    pj_thread_sleep(2000);

    // Make outgoing call
    Call* call = new MyCall(*acc);
    acc->calls.push_back(call);
    CallOpParam prm(true);
    prm.opt.audioCount = 1;
    prm.opt.videoCount = 0;
    call->makeCall("sip:801@192.168.195.1:5060", prm);

    // Hangup all calls
    pj_thread_sleep(15000);
    ep.hangupAllCalls();
    pj_thread_sleep(4000);

    // Destroy library
    std::cout << "*** PJSUA2 SHUTTING DOWN ***" << std::endl;
    delete acc; /* Will delete all calls too */
}

namespace PJSUA2_Test{
    using pj::Error;

extern "C"
int test()
{
    int ret = 0;
    MyEndpoint ep;

    try {
        ep.libCreate();

        test1(ep);

        ret = PJ_SUCCESS;
    }
    catch (Error& err) {
        std::cout << "Exception: " << err.info() << std::endl;
        ret = 1;
    }

    try {
        ep.libDestroy();
    }
    catch (Error& err) {
        std::cout << "Exception: " << err.info() << std::endl;
        ret = 1;
    }

    if (ret == PJ_SUCCESS) {
        std::cout << "Success" << std::endl;
    }
    else {
        std::cout << "Error Found" << std::endl;
    }

    return ret;
}


}