#include <pjsua2.hpp>
#include <iostream>
#include <pj/file_access.h>

#define CHECK(op)   do { \
			status = op; \
			if (status != PJ_SUCCESS) { \
			    err(#op, status); \
			    return status; \
			} \
		    } \
		    while (0)

#define THIS_FILE 	"pjsua2_demo.cpp"

using namespace pj;
#define USE_TEST 3

class MyEndpoint : public Endpoint
{
public:
    MyEndpoint() : Endpoint() {};
    virtual pj_status_t onCredAuth(OnCredAuthParam& prm)
    {
        std::cout << "*** Callback onCredAuth called ***" << std::endl;
        /* Return PJ_ENOTSUP to use
         * pjsip_auth_create_aka_response()/<b>libmilenage</b> (default),
         * if PJSIP_HAS_DIGEST_AKA_AUTH is defined.
         */
        return PJ_ENOTSUP;
    }
};

class MyAccount;

class MyCall : public Call
{
private:
    size_t idx;

    MyAccount* myAcc;
    AudioMediaCapture* pcm_capture;
    AudioMediaStream* pcm_stream;
    static vector<MyCall*> calls;

public:
    MyCall(Account& acc, int call_id = PJSUA_INVALID_ID)
        : Call(acc, call_id)
    {
        pcm_capture = NULL;
        pcm_stream = NULL;
        myAcc = (MyAccount*)&acc;

        idx = calls.size();
        calls.push_back(this);
    }

    ~MyCall()
    {
        if (pcm_capture)
            delete pcm_capture;
        if (pcm_stream)
            delete pcm_stream;
    }

    size_t get_idx();
    static MyCall* calls_lookup(size_t call_id);
    virtual void onCallState(OnCallStateParam& prm);
    virtual void onCallTransferRequest(OnCallTransferRequestParam& prm);
    virtual void onCallReplaced(OnCallReplacedParam& prm);
    virtual void onCallMediaState(OnCallMediaStateParam& prm);
    void putFrame(char* chunk, size_t datasize);
    void putFrameAsString(std::string s);
    void getFrames(char** s, size_t* datasize);
    std::string getFramesAsString();
};

class MyAccount : public Account
{
public:
    std::vector<Call*> calls;

public:
    MyAccount()
    {}

    ~MyAccount()
    {
        std::cout << "*** Account is being deleted: No of calls="
            << calls.size() << std::endl;

        for (std::vector<Call*>::iterator it = calls.begin();
            it != calls.end(); )
        {
            delete (*it);
            it = calls.erase(it);
        }
    }

    void removeCall(Call* call)
    {
        for (std::vector<Call*>::iterator it = calls.begin();
            it != calls.end(); ++it)
        {
            if (*it == call) {
                calls.erase(it);
                break;
            }
        }
    }

    virtual void onRegState(OnRegStateParam& prm)
    {
        AccountInfo ai = getInfo();
        std::cout << (ai.regIsActive ? "*** Register: code=" : "*** Unregister: code=")
            << prm.code << std::endl;
    }

    virtual void onIncomingCall(OnIncomingCallParam& iprm)
    {
        Call* call = new MyCall(*this, iprm.callId);
        CallInfo ci = call->getInfo();
        CallOpParam prm;

        std::cout << "*** Incoming Call: " << ci.remoteUri << " ["
            << ci.stateText << "]" << std::endl;

        calls.push_back(call);
        prm.statusCode = (pjsip_status_code)200;
        call->answer(prm);
    }
};