#include "PJSIP_AudioStream.h"
//#include "PJSUA2_demo.h"

void PJSIP_AudioStream::_register_methods()
{
	register_method("_ready", &PJSIP_AudioStream::_ready);
	register_method("_process", &PJSIP_AudioStream::_process);
	register_method("_physics_process", &PJSIP_AudioStream::_physics_process);
	register_method("queue_free", &PJSIP_AudioStream::queue_free);

	register_method("add_account", &PJSIP_AudioStream::add_account);
	register_method("make_call", &PJSIP_AudioStream::make_call);
	register_method("hangup_all_calls", &PJSIP_AudioStream::hangup_all_calls);

	register_property("username", &PJSIP_AudioStream::username, godot::String());
}

void PJSIP_AudioStream::_init()
{	
}

void PJSIP_AudioStream::_ready()
{
	Godot::print("Creating PJSIP Instance");
	// Init library
	pi = new PJSIP_Instance;
}

void PJSIP_AudioStream::_process(float delta)
{
	//fill_buffer();
}

void PJSIP_AudioStream::_physics_process(float delta)
{
}

void PJSIP_AudioStream::queue_free()
{
	Godot::print("Deleting PJSIP Instance");
	delete(pi);
}

void PJSIP_AudioStream::fill_buffer()
{
	if (callStreamPair.empty()) {
		return;
	}
	//for every calls, stream to corresponding godot::audiostream
	for (auto& csp : callStreamPair)
	{
		csp.frames_to_stream();
	}
}

void PJSIP_AudioStream::make_call(godot::String uri, godot::AudioStreamSample* stream)
{
	Godot::print("making call");

	MyCall* call = pi->make_call(uri.alloc_c_string());
	CallStreamPair csp = CallStreamPair(call, stream);
	callStreamPair.push_back(csp);
	//csp.frames_to_stream();
}

void PJSIP_AudioStream::hangup_all_calls()
{
	Godot::print("hanging up calls");
	pi->hangup_all_calls();
}

void PJSIP_AudioStream::add_account(godot::String username, godot::String password, godot::String domain, int port, int loglvl)
{
	pi->add_account(username.alloc_c_string(), password.alloc_c_string(), domain.alloc_c_string(), port, loglvl);
}
