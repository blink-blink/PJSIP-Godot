extends Node2D

onready var box: RigidBody2D = $RigidBody2D
onready var asp: AudioStreamPlayer2D = get_node("RigidBody2D/AudioStreamPlayer2d")
onready var asp2: AudioStreamPlayer2D = get_node("RigidBody2D2/AudioStreamPlayer2d")
onready var aspic: AudioStreamPlayer = $AudioStreamPlayerIC

onready var pjsip = $PJSIPTest

var ms = 500

var phase = 0
var checkerTimer = 0
var sample_rate = 16000

#this is a pointer casted as uint8_t
var outgoing_call = null
var incoming_call = null

var is_calling = false

func _ready():
	#add account
	var username = "801";
	var password = "IDm2xg64DCER3j6arR22Tob1d";
	var domain = "192.168.195.1:5060";
	var port = 29801;
	#var domain = "localhost";
	#var port = 5060;

	print("initalizing endpoint...");
	pjsip.initialize_endpoint(port,1)
	
	print("adding account: "+username);
	pjsip.add_account(username, password, domain);
	
	pjsip.buffer_incoming_call_to_stream(aspic.get_stream_playback())

func _unhandled_input(event):
	if event.is_action_pressed("ui_accept"):
		is_calling = !is_calling
		if is_calling:
			call_test()
		else:
			pjsip.hangup_all_calls()
	
	if event.is_action_pressed("ui_cancel"):
		pass
		#asp.play()
		#print(asp.stream.get_data())

func call_test():
	var call_uri = "sip:801@192.168.195.1:5060";
	#var call_uri = "sip:801@localhost";

	print("testing call on uri: "+call_uri);
	
	#stream settings
	asp.stream.mix_rate = sample_rate
	
	#insert call-audiostreamsample pair
	outgoing_call = pjsip.make_call(call_uri,asp.get_stream_playback());
	asp2.stream = asp.stream
	print(outgoing_call)
	asp.play()
	
	# start streaming audio effect capture of bus: Microphone
	var idx = AudioServer.get_bus_index("Microphone")
	var audioeffectcapture:AudioEffectCapture = AudioServer.get_bus_effect(idx,0)
	
	if not stream_thread:
		stream_thread = Thread.new()
	if stream_thread.is_active():
		while stream_thread.is_alive():
			pass
		stream_thread.wait_to_finish()
	
	stream_thread.start(self, "stream_capture", [audioeffectcapture, outgoing_call])

var stream_thread: Thread
var is_streaming: bool = false

func stream_capture(params: Array):
	var aec = params[0]
	var call_idx = params[1]
	
	var usec_delay = 1000
	while is_calling:
		var msec_start = OS.get_system_time_msecs()
		
		var frames_available = aec.get_frames_available()
		if frames_available >= 320:
			var buffer = aec.get_buffer(320)
			pjsip.push_frame(buffer, call_idx)
		
		var msec_taken = OS.get_system_time_msecs() - msec_start
		if msec_taken*1000 < usec_delay:
			OS.delay_usec(usec_delay - msec_taken*1000)

func _process(delta):
	checkerTimer -= delta
	if checkerTimer <= 0:
		checkerTimer = 1;
#		print(aspic.get_stream_playback().get_frames_available())

func _physics_process(delta):
	box.applied_force.x = 0
	if Input.is_action_pressed("left"):
		#print("left")
		box.applied_force.x = -ms
		
	if Input.is_action_pressed("right"):
		#print("right")
		box.applied_force.x = ms
	
	#print(asp.get_stream_playback().get_frames_available())


func _on_PJSIPTest_on_incoming_call(call_idx):
	print("call received: ", call_idx)
	incoming_call = call_idx
	aspic.play()
	pjsip.buffer_incoming_call_to_stream(aspic.get_stream_playback())
