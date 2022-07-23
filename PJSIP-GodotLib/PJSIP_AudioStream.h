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
	//float phase = 0;
	CallStreamPair() {};

	CallStreamPair(MyCall* call, AudioStreamGeneratorPlayback* stream) {
		this->call = call;
		this->stream = stream;
	}

	void frames_to_stream() {
		if (!call->hasMedia()) {
			Godot::print("no media", &stream);
			return;
		}
		//Godot::print("filling buffer", &stream);

		/* get frames */
		std::string s = call->getFramesAsString();

		/*create poolvec2array*/
		godot::PoolVector2Array d = PoolVector2Array();

		/* get codec data*/
		/*pjmedia_stream* st;
		pjmedia_stream_info* sti;
		pjmedia_stream_get_info(st, sti);
		pjmedia_codec_info cdi = sti->fmt;
		pjmedia_codec_param* cdp = sti->param;*/

		for (int i = 0; i < s.length(); i += 2) {

			/*decoder*/
			INT16 wc = ((s[i+1]-0x80) << 8);
			wc += (s[i] - 0x80) -0x8000;

			/*Convert PCM to float 32*/
			float fc = ((float)wc) / (float)0x8000;
			if (fc > 1) fc = 1;
			if (fc < -1) fc = -1;

			d.append(godot::Vector2(1, 1) * fc);
		}
		//for (char& c : s) {
		//	/*float fc = ((float)c) / (float)0x8000;
		//	if (fc > 1) fc = 1;
		//	if (fc < -1) fc = -1;*/

		//	INT16 wc = (INT16)(c-0x80) << 8;

		//	/*INT16 wc = (INT16)(c) << 8;
		//	wc -= 0x8000;*/

		//	//INT16 wc = (INT16)(c);

		//	//float a = (float)(c-0x80) / (float)0x80;
		//	//std::cout << fc << '\n';

		//	d.append(godot::Vector2(1,1)*wc);

		//	//d.append(godot::Vector2(1, 1) * sin(phase*Math_TAU));
		//	//phase += 0.01;
		//}

		stream->push_buffer(d);
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
	void add_account(godot::String username, godot::String password, godot::String domain, int port, int loglvl);
	void make_call(godot::String uri, AudioStreamGeneratorPlayback* stream);
	void hangup_all_calls();
};

