#include "PJSIP_AudioStream.h"
//#include "PJSUA2_demo.h"

void PJSIP_AudioStream::_register_methods()
{
	register_method("_ready", &PJSIP_AudioStream::_ready);
	register_method("_process", &PJSIP_AudioStream::_process);
	register_method("_physics_process", &PJSIP_AudioStream::_physics_process);
	register_method("queue_free", &PJSIP_AudioStream::queue_free);

	register_method("initialize_endpoint", &PJSIP_AudioStream::initialize_endpoint);
	register_method("add_account", &PJSIP_AudioStream::add_account);
	register_method("make_call", &PJSIP_AudioStream::make_call);
	register_method("hangup_all_calls", &PJSIP_AudioStream::hangup_all_calls);
	register_method("push_frame", &PJSIP_AudioStream::push_frame);
	register_method("push_frame_stereo", &PJSIP_AudioStream::push_frame_stereo);
	register_method("buffer_incoming_call_to_stream", &PJSIP_AudioStream::buffer_incoming_call_to_stream);

	//properties
	register_property("username", &PJSIP_AudioStream::username, godot::String());

	//signals
	register_signal<PJSIP_AudioStream>((char*)"on_incoming_call", "call_idx", GODOT_VARIANT_TYPE_INT);
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
	buffer += delta;
	if (buffer >= 0.05){
		fill_buffer();
		buffer = 0;
	}
}

void PJSIP_AudioStream::_physics_process(float delta)
{
}

void PJSIP_AudioStream::queue_free()
{
	Godot::print("Deleting PJSIP Instance");
	delete(pi);
}

size_t PJSIP_AudioStream::make_CallStreamPair(MyCall* call, AudioStreamGeneratorPlayback* stream)
{
	CallStreamPair csp = CallStreamPair(call, stream);
	callStreamPair.push_back(csp);
	Godot::print("call stream pair created");
	csp.frames_to_stream();
	std::cout << "call at: " << call->get_idx() << '\n';
	return call->get_idx();
}

void PJSIP_AudioStream::buffer_incoming_call_to_stream(AudioStreamGeneratorPlayback* stream)
{
	buffer_streams.push_back(stream);
}

void PJSIP_AudioStream::call_to_buffer_stream(MyCall* call)
{

	if (buffer_streams.size() > 0 ) {
		std::cout << "emitting on incoming call signal \n";
		emit_signal("on_incoming_call", make_CallStreamPair(call, buffer_streams.front()));
		buffer_streams.erase(buffer_streams.begin());
	}
	else {
		Godot::print("Error: Empty AudioStream Buffer");	
	}
		
}

void PJSIP_AudioStream::fill_buffer()
{
	if (callStreamPair.empty()) {
		return;
	}
	//for every calls, stream to corresponding godot::audiostream
	for (auto& csp : callStreamPair)
	{
		if (csp.call->isActive()) {
			csp.frames_to_stream();
		}
	}
}

void PJSIP_AudioStream::initialize_endpoint(int port, int loglvl)
{
	pi->initialize_endpoint(port, loglvl);
}

size_t PJSIP_AudioStream::make_call(godot::String uri, AudioStreamGeneratorPlayback* stream)
{
	Godot::print("making call");

	MyCall* call = pi->make_call(uri.alloc_c_string());
	if (call){
		return make_CallStreamPair(call, stream);
	}
	else 
		Godot::print("Error in making call");
	return NULL;
}

void PJSIP_AudioStream::hangup_all_calls()
{
	Godot::print("hanging up calls");
	pi->hangup_all_calls();
}

void PJSIP_AudioStream::push_frame(godot::PoolVector2Array frame, size_t call_idx)
{
	MyCall* call = MyCall::calls_lookup(call_idx);
	if (!call) return;

	std::string s;

	for (int i = 0; i < frame.size(); i++) 
	{

		//since mono just get x
		float fc = frame[i].x;

		// float 32 to PCM16
		int16_t wc = (int16_t)(fc * 0x8000);

		//lil endian
		char c = (wc >> 0) & 0xFF;
		s += c;
		c = (wc >> 8) & 0xFF;
		s += c;

	}
	//std::cout << "string built \n";
	//push frames to call
	call->putFrameAsString(s);
}

void PJSIP_AudioStream::push_frame_stereo(godot::PoolVector2Array frame, size_t call_idx)
{
	//std::cout << "pushing frames stereo \n";
	MyCall* call = MyCall::calls_lookup(call_idx);
	if (!call) return;

	std::string s;
	//std::cout << "===buffer: " << frame.size() << '\n';

	for (int i = 0; i < frame.size(); i ++) {

		//since stereo get xy
		float fc1 = frame[i].x;
		float fc2 = frame[i].y;
		
		//std::cout << fc1 << fc2 << '\n';

		// float 32 to PCM
		int16_t wc1 = (int16_t)(fc1 * 0x8000);
		int16_t wc2 = (int16_t)(fc2 * 0x8000);

		//std::cout << wc1 << wc2 << '\n';

		//append string (little endian)
		char c = (wc1 >> 0) & 0xFF;
		s += c;
		c = (wc1 >> 8) & 0xFF;
		s += c;


		c = (wc2 >> 0) & 0xFF;
		s += c;
		c = (wc2 >> 8) & 0xFF;
		s += c;
	}

	//std::cout << "frame built: " << s << '\n';

	//push frames to call
	call->putFrameAsString(s);
}

void PJSIP_AudioStream::add_account(godot::String username, godot::String password, godot::String domain)
{
	pi->add_account(username.alloc_c_string(), password.alloc_c_string(), domain.alloc_c_string(), this);
}
