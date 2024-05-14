//"Toy Instrument"
//For 2024 MT Capstone
//Rae Quarles - 4/29/24
//V 2.2

//This file works with the "Toy Instrument" sculpture by receiving OSC messages from Processing to change parameters in real time
//2.2 Update:
////No longer trying to change one synth continuously
////Using Additive Synthesis

//// find your localhost
NetAddr.localAddr

////Start your SynthDef
(
SynthDef.new(\weirdKey, {
	arg freq = 220, atk=0.005, dur=40, rel=25, amp=1.5, pan=0, har=1, mod=554.365;
	var sig, env, filter, a;
	sig = PMOsc.ar(freq, mod, 2, 3, 0, 1);            //OOoo a weird phase modded osc
	sig = sig + SinOsc.ar(freq * har);
	env = EnvGen.kr(Env.new([0,1,0], [atk, dur, rel], [1, -1]), doneAction:2);
	sig = Pan2.ar(sig, pan, amp);
	sig = sig * env;
	Out.ar(0, sig);
}).add;
SynthDef.new(\blip, {
	arg freq=40, nharm=12, detune=0.2, dur=3, gate=1, pan=0, amp=1, out=0;
	var sig, env;
	env = EnvGen.kr(Env.adsr(0.05, 0.1, 1.0, dur), gate);        //Makes an envelope with an off/on gate
	sig = Blip.ar(freq * LFNoise1.kr(0.2!16).bipolar(detune.neg, detune).midiratio, nharm
	);           //Detunes the signal from -detune to + in midi terms
	sig = sig * LFNoise1.kr(0.5!16).exprange(0.1, 1);    //And augments the amplitude
	sig = Splay.ar(sig); //mixes these 16 channels we made into stereo
	sig = Balance2.ar(sig[0], sig[1], pan); //Takes this random data and lets you place it into left and right
	sig = sig * env * amp; //Multiply by our env and amp
	Out.ar(out, sig);
}).add
)


////CHORD: GREEN BUTTON
(
OSCdef (\chordStart, {
	arg msg;
	~x.set(\amp, 0.0);//Stop the last tone
	~x.free;
	~x = Synth("weirdKey", [\freq, msg[1], \mod, (msg[1]*rrand(0.5, 3.5)), \amp, 1.5]);//Play a tone for 30s at the freq specified
	"GREEN PRESSED; Start Tone At: ".post;
	msg[1].postln;//Post the messages!!
}, "/greenOn");//When green button is pressed

OSCdef(\chord, {
	arg msg;
	~y.free;//stop the last partial
	~y = Synth("weirdKey", [freq:msg[1], dur:rrand(1,10), rel:rrand(3,10)]);//Play a tone at the partial to x, with a random duration and release
	"Partial Tone At: ".post;
	msg[1].postln;//Post the messages!!
}, "/chord");//When in mode 1 (green) and car passes
)

////SEQUENCER: YELLOW BUTTON
(
OSCdef (\seqStart, {
	arg msg;
	~x.set(\amp, 0.0);//Stop the last tone
	~x.free;
	"YELLOW PRESSED; Start the Sequencer".postln; //Post the messages!!
}, "/yellowOn");//When green button is pressed

OSCdef(\seq, {
	arg msg;
	~y.free;//stop the last note
	~y = Synth("weirdKey", [freq:msg[1], dur:rrand(1,10), rel:rrand(3,10)]);//Play each note that comes in
	"Partial Tone At: ".post;
	msg[1].postln;//Post the messages!!
}, "/seq");//When in mode 2 (yellow) and car passes
)

////DETUNE: RED BUTTON
(
OSCdef (\detuneStart, {
	arg msg;
	~x.set(\amp, 0.0);//Stop the last tone
	~x.free;
	~x = Synth("blip", [\freq, msg[1], \nharm, rrand(4,12), \amp, 2.0]);//Play a tone for 30s at the freq specified
	"RED PRESSED; Start Tone at: ".post;
	msg[1].postln; //Post the messages!!
}, "/redOn");//When green button is pressed

OSCdef(\detune, {
	arg msg;
	~y.free;//stop the last detune note
	~y = Synth("blip", [freq:msg[2], detune:msg[1], dur:rrand(1,5), \amp, 2.0]);//Play the same tone but with detunee
	"Detuned Note Amount: ".post;
	msg[2].postln;//Post the messages!!
}, "/detune");//When in mode 3 (red) and car passes
)

  