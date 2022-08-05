#pragma once

#include "PJSIP_Instance.h"
#include "pjmedia-codec.h"

#include <AudioStreamGeneratorPlayback.hpp>
#include <Node.hpp>
#include <iostream>
#include <fstream>

class PJSIP_AudioStream : public Node
{
	GODOT_CLASS(PJSIP_AudioStream, Node);

	PJSIP_Instance* pi;
	float buffer = 0;

	/**
	 * An array containing all CallStreamPairs
	 */
	std::vector<CallStreamPair*> callStreamPair;
	
	/**
	 * An array of AudioStreamGeneratorPlayback to be paired with incoming calls.
	 */
	std::vector<AudioStreamGeneratorPlayback*> buffer_streams;

public:
	static void _register_methods();	
	
	
	/**
	 * Godot's __init(). Refer to the official documentation for
	 * more info. https://docs.godotengine.org/en/stable/index.html
	 */
	void _init();

	void _ready();
	/**
	 * Godot's __process(delta). Refer to the official documentation for 
	 * more info. https://docs.godotengine.org/en/stable/index.html
	 */
	void _process(float delta);


	/**
	 * Godot's __process(delta). Refer to the official documentation for 
	 * more info. https://docs.godotengine.org/en/stable/index.html
	 */
	void _physics_process(float delta);

	/**
	 * Godot's __process(delta). Refer to the official documentation for
	 * more info. https://docs.godotengine.org/en/stable/index.html
	 */
	void queue_free();


	/**
	* Returns a CallStreamPair* containing the first call with call_id
	 */
	CallStreamPair* callStreamPair_lookup(size_t call_id);

	/**
	* Creates a CallStreamPair. 
	 */
	size_t make_CallStreamPair(MyCall* call, AudioStreamGeneratorPlayback* stream);

	/**
	* Adds an AudioStreamGeneratorPlayback to an array to be used by incoming 
	* calls. 
	 */
	void buffer_incoming_call_to_stream(AudioStreamGeneratorPlayback* stream);

	/**
	* Makes a CallStreamPair with the incoming call and pops the 
	* AudioStreamGeneratorPlayback array. Called on _onIncomingCall()
	*/
	void call_to_buffer_stream(MyCall* call);


	/**
	* Fills AudioStream with the pcm frames from the call. Iterates through every
	* CallStreamPair and calls on frames_to_stream() method
	*/
	void fill_buffer();

	/**
	* Initializes an PJSIP endpoint using the default codec (Speex/16000).
	*/
	void initialize_endpoint(int port, int loglvl);

	/**
	* Initializes an PJSIP endpoint with the specified codec.
	*/
	void initialize_endpoint2(godot::String codec, int port, int loglvl);

	/**
	* Adds a PJSIP account.
	*/
	void add_account(godot::String username, godot::String password, godot::String domain);
	//void set_use_pj_capture(bool b);

	/**
	* Makes a PJSIP call on the specified uri.
	*/
	size_t make_call(godot::String uri, AudioStreamGeneratorPlayback* stream);


	/**
	* hangup a call with the specified call_id
	*/
	void hangup_call(size_t call_id);

	/**
	* Hangsup all calls
	*/
	void hangup_all_calls();

	/**
	* Configures a CallStreamPair containing a call with the specified call_id
	* to interpret frames as stereo. 
	*/
	void set_stereo(size_t call_id, bool b);

	/**
	* Streams/pushes frame to a call. Expects godot::PoolVector2Array containing 
	* f32 (x,y) pcm
	*/
	void push_frame(godot::PoolVector2Array frame, size_t call_id);

	/**
	* Streams/pushes frame to a call. Expects godot::PoolByteArray containing signed
	* 16 PCM data
	*/
	void push_frame2(godot::PoolByteArray frame, size_t call_id);

	/**
	* Streams/pushes frame to a call but as stereo. Expects godot::PoolVector2Array containing
	* f32 (x,y) pcm. Use set_stereo() to configure CallStreamPair to interpret stereo frames.
	*/
	void push_frame_stereo(godot::PoolVector2Array frame, size_t call_idx);
};

