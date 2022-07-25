#include "PJSIP_AudioStream.h"

void CallStreamPair::interpret_frames(std::string s, godot::PoolVector2Array* d)
{
	//mono
	//std::cout << "interpreting frames as mono" << '\n';

	for (int i = 0; i < s.length(); i += 2) {

		/*convert to signed PCM 16*/
		INT16 wc = ((s[i + 1] - 0x80) << 8);
		wc += (s[i] - 0x80) - 0x8000;

		/*Convert PCM to float 32*/
		float fc = ((float)wc) / (float)0x8000;
		if (fc > 1) fc = 1;
		if (fc < -1) fc = -1;

		d->append(godot::Vector2(1, 1) * fc);
	}

}

void CallStreamPair::interpret_frames_stereo(std::string s, godot::PoolVector2Array* d)
{
	//stereo
	//std::cout << "interpreting frames as stereo" << '\n';

	for (int i = 0; i < s.length(); i += 4) {

		/*convert to signed PCM 16*/
		INT16 wc1 = ((s[i + 1] - 0x80) << 8);
		wc1 += (s[i] - 0x80) - 0x8000;

		INT16 wc2 = ((s[i + 3] - 0x80) << 8);
		wc1 += (s[i + 2] - 0x80) - 0x8000;

		/*Convert PCM to float 32*/
		float fc1 = ((float)wc1) / (float)0x8000;
		if (fc1 > 1) fc1 = 1;
		if (fc1 < -1) fc1 = -1;

		float fc2 = ((float)wc2) / (float)0x8000;
		if (fc2 > 1) fc2 = 1;
		if (fc2 < -1) fc2 = -1;

		d->append(godot::Vector2(fc1, fc2));
	}

}

void CallStreamPair::frames_to_stream() 
{
	if (!call->hasMedia()) {
		Godot::print("no media", &stream);
		return;
	}
	//Godot::print("filling buffer", &stream);

	/* get frames */
	std::string s = call->getFramesAsString();

	/*create poolvec2array*/
	godot::PoolVector2Array d = PoolVector2Array();

	if (stereo)
		interpret_frames_stereo(s, &d);
	else
		interpret_frames(s, &d);

	stream->push_buffer(d);
}