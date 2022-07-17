#pragma once
#include "Common.h"
#include "PJSIP_Instance.h"
#include <AudioStreamSample.hpp>
#include <Node.hpp>
#include <iostream>
#include <fstream>

struct CallStreamPair
{
	MyCall* call;
	AudioStreamSample* stream;
	CallStreamPair() {};

	CallStreamPair(MyCall* call, AudioStreamSample* stream) {
		this->call = call;
		this->stream = stream;
	}

	void frames_to_stream() {
		if (!call->hasMedia()) {
			//Godot::print("no media", &stream);
			return;
		}
		//Godot::print("filling buffer", &stream);

		std::ofstream output;
		output.open("output.lpcm", std::ios_base::app);
		char* cs = call->getFrames();
		output << cs;
		output.close();
		std::string s = cs;

		godot::PoolByteArray d = PoolByteArray();
		for (char& c : s) {
			d.append(c-128);
		}

		PoolByteArray sd = stream->get_data();
		sd.append_array(d);
		stream->set_data(sd);
	}
};

class PJSIP_AudioStream : public Node
{
	GODOT_CLASS(PJSIP_AudioStream, AudioStream);

	//Exposed Properties
	godot::String username;

	PJSIP_Instance* pi;
	std::vector<CallStreamPair> callStreamPair;

public:
	static void _register_methods();
	void _init();

	void _ready();
	void _process(float delta);
	void _physics_process(float delta);
	void queue_free();

	void fill_buffer();
	void add_account(godot::String username, godot::String password, godot::String domain, int port, int loglvl);
	void make_call(godot::String uri, godot::AudioStreamSample* stream);
	void hangup_all_calls();
};

