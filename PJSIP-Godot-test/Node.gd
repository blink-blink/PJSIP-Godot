extends Node2D

onready var box: RigidBody2D = $RigidBody2D
onready var asp: AudioStreamPlayer2D = get_node("RigidBody2D/AudioStreamPlayer2d")
onready var asp2: AudioStreamPlayer2D = get_node("RigidBody2D2/AudioStreamPlayer2d")
onready var aspic: AudioStreamPlayer = $AudioStreamPlayerIC

onready var pjsip = $PJSIPTest

var ms = 500

var phase = 0
var checkerTimer = 0

#this is a pointer casted as uint8_t
var this_call = null

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
	asp.stream.mix_rate = 8000
	
	#insert call-audiostreamsample pair
	this_call = pjsip.make_call(call_uri,asp.get_stream_playback());
	asp2.stream = asp.stream
	print(this_call)
	asp.play()

func _process(delta):	
	var idx = AudioServer.get_bus_index("Microphone")
	var audioeffectcapture:AudioEffectCapture = AudioServer.get_bus_effect(idx,0)
	#print(audioeffectcapture)
	
	var buffer = audioeffectcapture.get_buffer(2000)
	#print(buffer)
	
#	var buffer: PoolVector2Array
#	for i in range(0,120):
#		buffer.append(Vector2(1,1)*sin(phase*TAU))
#		phase = fmod(phase + 440.0/22050, 1.0)
#		#phase += delta*0.001
			
	#print(buffer)
	pjsip.push_frame(buffer,this_call)
	
	checkerTimer -= delta
	if checkerTimer <= 0:
		checkerTimer = 1;
		print(aspic.get_stream_playback().get_frames_available())

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
	aspic.play()
