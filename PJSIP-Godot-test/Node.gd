extends Node2D

onready var box: RigidBody2D = $RigidBody2D
onready var asp: AudioStreamPlayer2D = get_node("RigidBody2D/AudioStreamPlayer")
onready var pjsip = $PJSIPTest
var ms = 500

var is_calling = false

func _ready():
	#add account
	var username = "801";
	var password = "aelhotIQzaoQDY5zfmnP76Xpw";
	var domain = "192.168.195.1:5060";
	var port = 29801;
	#var domain = "localhost";
	#var port = 5060;

	print("adding account: "+username);
	pjsip.add_account(username, password, domain, port, 1);

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
	pjsip.make_call(call_uri,asp.get_stream_playback());
	asp.play()

func _physics_process(delta):
	box.applied_force.x = 0
	if Input.is_action_pressed("left"):
		#print("left")
		box.applied_force.x = -ms
		
	if Input.is_action_pressed("right"):
		#print("right")
		box.applied_force.x = ms
	
	#print(asp.get_stream_playback().get_frames_available())
