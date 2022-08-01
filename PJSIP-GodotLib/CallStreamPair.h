#include "PJSIP_AudioStream.h"
#include "AudioStreamGeneratorPlayback.hpp"

struct CallStreamPair
{
	MyCall* call;
	AudioStreamGeneratorPlayback* stream;

	bool stereo = false;
	//float phase = 0;

	CallStreamPair() {};

	CallStreamPair(MyCall* call, AudioStreamGeneratorPlayback* stream) {
		this->call = call;
		this->stream = stream;
	}

	~CallStreamPair() { std::cout << "calling destructor \n"; }

	void on_call_disconnect();

	void interpret_frames(std::string s, godot::PoolVector2Array* d);

	void interpret_frames_stereo(std::string s, godot::PoolVector2Array* d);

	void frames_to_stream();

	void set_stereo(bool b) {
		stereo = b;
	}

	bool is_stereo() {
		return stereo;
	}
};