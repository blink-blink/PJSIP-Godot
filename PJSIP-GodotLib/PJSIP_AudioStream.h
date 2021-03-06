#pragma once
#include "Common.h"

#include "PJSIP_Instance.h"
#include "pjmedia-codec.h"

#include <AudioStreamGeneratorPlayback.hpp>
#include <Node.hpp>
#include <iostream>
#include <fstream>

class PJSIP_AudioStream : public Node
{
	GODOT_CLASS(PJSIP_AudioStream, Node);

	//Exposed Properties
	godot::String username;

	PJSIP_Instance* pi;
	float buffer = 0;

	std::vector<CallStreamPair*> callStreamPair;
	std::vector<AudioStreamGeneratorPlayback*> buffer_streams;

public:
	static void _register_methods();
	void _init();

	void _ready();
	void _process(float delta);
	void _physics_process(float delta);
	void queue_free();

	CallStreamPair* callStreamPair_lookup(size_t call_id);
	size_t make_CallStreamPair(MyCall* call, AudioStreamGeneratorPlayback* stream);
	void buffer_incoming_call_to_stream(AudioStreamGeneratorPlayback* stream);
	void call_to_buffer_stream(MyCall* call);

	void fill_buffer();
	void initialize_endpoint(int port, int loglvl);
	void initialize_endpoint2(godot::String codec, int port, int loglvl);
	void add_account(godot::String username, godot::String password, godot::String domain);
	void set_use_pj_capture(bool b);
	size_t make_call(godot::String uri, AudioStreamGeneratorPlayback* stream);
	void hangup_call(size_t call_id);
	void hangup_all_calls();
	void set_stereo(size_t call_id, bool b);

	void push_frame(godot::PoolVector2Array frame, size_t call_id);
	void push_frame2(godot::PoolByteArray frame, size_t call_id);
	void push_frame_stereo(godot::PoolVector2Array frame, size_t call_idx);
};

