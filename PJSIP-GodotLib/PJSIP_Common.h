#include "Common.h"
#include <pjsua2.hpp>
#include <iostream>
#include <pj/file_access.h>

using namespace pj;

struct CallStreamPair;
class PJSIP_AudioStream;

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
    static vector<MyCall*> calls;
    static size_t counter;
    size_t idx;

    MyAccount* myAcc;
    AudioMediaCapture* pcm_capture;
    AudioMediaStream* pcm_stream;
    CallStreamPair* csp;

public:
    MyCall(Account& acc, int call_id = PJSUA_INVALID_ID)
        : Call(acc, call_id)
    {
        csp = NULL;
        pcm_capture = NULL;
        pcm_stream = NULL;
        myAcc = (MyAccount*)&acc;

        if (calls.size() <= 0) counter = 0;
        idx = counter;
        ++counter;

        calls.push_back(this);
    }

    ~MyCall()
    {
        if (pcm_capture)
            delete pcm_capture;
        if (pcm_stream)
            delete pcm_stream;
    }

    /**
    * getters
    */
    size_t get_idx();


    /**
    * Returns MyCall with the specified call_id
    */
    static MyCall* calls_lookup(size_t call_id);


    /**
    * called on call state. refer to PJSIP Documentation for more info, 
    * https://www.pjsip.org/docs/book-latest/html/index.html
    */
    virtual void onCallState(OnCallStateParam& prm);


    /**
    * Refer to PJSIP Documentation for more info, 
    * https://www.pjsip.org/docs/book-latest/html/index.html
    */
    virtual void onCallTransferRequest(OnCallTransferRequestParam& prm);


    /**
    * Refer to PJSIP Documentation for more info, 
    * https://www.pjsip.org/docs/book-latest/html/index.html
    */
    virtual void onCallReplaced(OnCallReplacedParam& prm);
    
    
    /**
    * Refer to PJSIP Documentation for more info,
    * https://www.pjsip.org/docs/book-latest/html/index.html
    */
    virtual void onCallMediaState(OnCallMediaStateParam& prm);


    /**
    * Sets the CallStreampPair this call is specified
    */
    void setCallStreamPair(CallStreamPair* csp);


    /**
    * calls on the modified PJLIB Library method, putFrame().
    */
    void putFrame(char* chunk, size_t datasize);


    /**
    * calls on the modified PJLIB Library method, putFrameAsString().
    */
    void putFrameAsString(std::string s);


    /**
    * calls on the modified PJLIB Library method, getFrames().
    */
    void getFrames(char** s, size_t* datasize);


    /**
    * calls on the modified PJLIB Library method, getFrameAsString().
    */
    std::string getFramesAsString();
};

class MyAccount : public Account
{
private:
    PJSIP_AudioStream* ASOwner;
public:
    std::vector<Call*> calls;

public:
    MyAccount(PJSIP_AudioStream* ASOwner)
    {
        this->ASOwner = ASOwner;
    }
    /*MyAccount()
    {}*/

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


    /**
    * Removes a call from the call array. Not used
    */
    void removeCall(Call* call);


    /**
    * Refer to PJSIP Documentation for more info,
    * https://www.pjsip.org/docs/book-latest/html/index.html
    */
    void _onRegState(OnRegStateParam& prm);


    /**
    * Refer to PJSIP Documentation for more info,
    * https://www.pjsip.org/docs/book-latest/html/index.html
    */
    void _onIncomingCall(OnIncomingCallParam& iprm);

    virtual void onRegState(OnRegStateParam& prm) {
        _onRegState(prm);
    }

    virtual void onIncomingCall(OnIncomingCallParam& iprm) {
        _onIncomingCall(iprm);
    }
};