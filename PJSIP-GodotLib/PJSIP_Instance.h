#include "PJSIP_Common.h"


class PJSIP_Instance
{
	//PJSIP
	pj_thread_t* this_thread;

	MyEndpoint ep;
	MyAccount* acc;
	
public:	
	PJSIP_Instance()
	{
		acc = NULL;
		try {
			ep.libCreate();
			this_thread = pj_thread_this();
		}
		catch (pj::Error& err) {
			std::cout << "Exception: " << err.info() << std::endl;
		}
	}

	~PJSIP_Instance()
	{
		ep.hangupAllCalls();

		try {
			ep.libDestroy();
		}
		catch (pj::Error& err) {
			std::cout << "Exception: " << err.info() << std::endl;
		}

		if (acc)
			delete(acc);
	}
	void initialize_endpoint(int port, int loglvl);
	string add_account(string username, string password, string domain, PJSIP_AudioStream* ASOwner);
	MyCall* make_call(string uri);
	void hangup_call(MyCall* call);
	void hangup_all_calls();
};