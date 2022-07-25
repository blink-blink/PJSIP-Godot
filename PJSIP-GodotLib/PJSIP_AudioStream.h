#pragma once
#include "Common.h"
#include "PJSIP_Instance.h"
#include "pjmedia-codec.h"
#include <AudioStreamGeneratorPlayback.hpp>
#include <Node.hpp>
#include <iostream>
#include <fstream>

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

class PJSIP_AudioStream : public Node
{
	GODOT_CLASS(PJSIP_AudioStream, Node);

	//Exposed Properties
	godot::String username;

	PJSIP_Instance* pi;
	std::vector<CallStreamPair> callStreamPair;
	float buffer = 0;

public:
	static void _register_methods();
	void _init();

	void _ready();
	void _process(float delta);
	void _physics_process(float delta);
	void queue_free();

	void fill_buffer();
	void initialize_endpoint(int port, int loglvl);
	void add_account(godot::String username, godot::String password, godot::String domain);
	size_t make_call(godot::String uri, AudioStreamGeneratorPlayback* stream);
	void hangup_all_calls();

	void push_frame(godot::PoolVector2Array frame, size_t call_id);
	void push_frame_stereo(godot::PoolVector2Array frame, size_t call_idx);
};

