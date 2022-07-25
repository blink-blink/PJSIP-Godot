
#include "PJSIP_Instance.h"

#define ERROR_CODE_0 "Account Exists"
#define ERROR_CODE_1 "Account Failed"

void PJSIP_Instance::initialize_endpoint(int port, int loglvl)
{
    if (ep.libGetState() > PJSUA_STATE_CREATED) {
        std::cout << "Endpoint lib past created state" << '\n';
        return;
    }

    try {

        //register thread
        /*pj_thread_desc desc;
        pj_thread_t* this_thread;
        pj_bzero(desc, sizeof(desc));
        pj_thread_register("thread", desc, &this_thread);*/
        ep.libRegisterThread("epInit");

        // Init library
        EpConfig ep_cfg;
        ep_cfg.logConfig.level = loglvl;
        ep.libInit(ep_cfg);

        //prioritize speex/8000
        string codecID = "speex/8000";
        for (auto codec : ep.codecEnum2()) {
            int prio = 0;
            if (codec.codecId.find(codecID) != std::string::npos) {
                std::cout << codecID << " codec found" << "\n";
                prio = 255;
            }
            ep.codecSetPriority(codec.codecId, prio);
        }

        // Transport
        TransportConfig tcfg;
        tcfg.port = port;
        ep.transportCreate(PJSIP_TRANSPORT_UDP, tcfg);

        // Start library
        ep.libStart();
        std::cout << "*** PJSUA2 STARTED ***" << std::endl;

    }
    catch (Error err) {
        std::cout << "Exception: " << err.info() << std::endl;
    }
}

string PJSIP_Instance::add_account(string username, string password, string domain)
{
    std::cout << ("PJSIP_Instance::add_account: " + username) << std::endl;

    //register thread
    ep.libRegisterThread("addAccount");

    // Add account
    AccountConfig acc_cfg;
    acc_cfg.idUri = "sip:"+username+"@"+domain;

    AuthCredInfo aci("digest", "*", username, 0, password);

    acc_cfg.sipConfig.authCreds.push_back(aci);

    acc = (new MyAccount);
    try {
        acc->create(acc_cfg);
    }
    catch (...) {
        return ERROR_CODE_1;
    }

    return "success";
}

MyCall* PJSIP_Instance::make_call(string uri)
{
    std::cout<<("PJSIP_Instance::make call: " + uri) << std::endl;

    try {
        //register thread
        /*pj_thread_desc desc;
        pj_thread_t* this_thread;
        pj_bzero(desc, sizeof(desc));
        pj_thread_register("thread", desc, &this_thread);*/
        ep.libRegisterThread("make call");

        MyCall* call = new MyCall(*acc);
        acc->calls.push_back(call);
        CallOpParam prm(true);
        prm.opt.audioCount = 1;
        prm.opt.videoCount = 0;

        call->makeCall(uri, prm);

        return call;
    }
    catch (Error& err) {
        std::cout << "Exception: " << err.info() << std::endl;
    }
    return NULL;
}

void PJSIP_Instance::hangup_all_calls()
{
    std::cout << ("PJSIP_Instance::hanging up call") << std::endl;
    try {
        //register thread
        /*pj_thread_desc desc;
        pj_thread_t* this_thread;
        pj_bzero(desc, sizeof(desc));
        pj_thread_register("thread", desc, &this_thread);*/
        ep.libRegisterThread("hangup");

        ep.hangupAllCalls();
    }
    catch (Error err) {
        std::cout << "Exception: " << err.info() << std::endl;
    }
}
