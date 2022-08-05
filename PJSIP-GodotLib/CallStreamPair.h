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


	/**
	* calls on call disconnect state, PJSIP_INV_STATE_DISCONNECTED
	*/
	void on_call_disconnect();


	/**
	* interprets signed PCM 16 frames. converts to f32
	*/
	void interpret_frames(std::string s, godot::PoolVector2Array* d);


	/**
	* interprets signed PCM 16 stereo frames. converts to f32
	*/
	void interpret_frames_stereo(std::string s, godot::PoolVector2Array* d);


	/**
	* captures incoming PCM frames and calls upon an interpreter method
	*/
	void frames_to_stream();


	/**
	* Sets this CallStreamPair to interpret frames as stereo
	*/
	void set_stereo(bool b) {
		stereo = b;
	}


	/**
	* returns if this CallStreamPair is set to stereo
	*/
	bool is_stereo() {
		return stereo;
	}
};