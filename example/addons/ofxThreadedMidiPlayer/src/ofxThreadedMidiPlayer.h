#pragma once

#include "ofMain.h"

#include <jdksmidi/world.h>
#include <jdksmidi/track.h>
#include <jdksmidi/multitrack.h>
#include <jdksmidi/filereadmultitrack.h>
#include <jdksmidi/fileread.h>
#include <jdksmidi/fileshow.h>
#include <jdksmidi/filewritemultitrack.h>
#include <jdksmidi/sequencer.h>
#include "ofxMidi.h"
#include "ofxRuiThread.h"



using namespace jdksmidi;

class ofxThreadedMidiPlayer: public ofxRuiThread{
public:
	int count;
	bool isReady;
	string midiFileName;
	int midiPort;
	float currentTime;
	float nextEventTime;
	
	double musicDurationInSeconds;
	float max_time;
	long myTime;
	bool doLoop;
	MIDISequencer *sequencer;
	MIDITimedBigMessage *lastTimedBigMessage;
	
	RtMidiOut *midiout;
	
	
	ofxThreadedMidiPlayer(){
		count = 0;
		midiFileName = "";
		midiPort = 0;
		currentTime = 0.0;
		nextEventTime = 0.0;
		
		musicDurationInSeconds = 0;
		max_time = 0;
		myTime = 0;
		lastTimedBigMessage = new MIDITimedBigMessage();
		sequencer = NULL;
		midiout = NULL;
		doLoop = true;
		isReady = true;
	}
	
	void DumpMIDITimedBigMessage( const MIDITimedBigMessage *msg )
	{
		if ( msg )
		{
			char msgbuf[1024];
			lastTimedBigMessage->Copy(*msg);
			// note that Sequencer generate SERVICE_BEAT_MARKER in files dump,
			// but files themselves not contain this meta event...
			// see MIDISequencer::beat_marker_msg.SetBeatMarker()
			if ( msg->IsBeatMarker() )
			{
				ofLog(OF_LOG_VERBOSE, "%8ld : %s <------------------>", msg->GetTime(), msg->MsgToText ( msgbuf ) );
			}
			else
			{
				ofLog(OF_LOG_VERBOSE, "%8ld : %s", msg->GetTime(), msg->MsgToText ( msgbuf ) );
			}
			
			if ( msg->IsSystemExclusive() )
			{
				ofLog(OF_LOG_VERBOSE, "SYSEX length: %d", msg->GetSysEx()->GetLengthSE() );
			}
			
			
		}
	}
	void setup(string fileName, int portNumber, bool shouldLoop = true)
	{
		midiFileName = fileName;
		midiPort = portNumber;
		doLoop = shouldLoop;
	}
	void updateThread()
	{
		isReady = false;
		MIDITimedBigMessage event;
		int eventTrack = 0;
		string filePath = ofToDataPath(midiFileName, true);
		
		MIDIFileReadStreamFile rs ( filePath.c_str() );
		
		if ( !rs.IsValid() )
		{
			ofLogError( "ERROR OPENING FILE AT: ",  filePath);
			
		}
		
		MIDIMultiTrack tracks; 
		MIDIFileReadMultiTrack track_loader ( &tracks );
		MIDIFileRead reader ( &rs, &track_loader );
		
		int numMidiTracks = reader.ReadNumTracks();
		int midiFormat = reader.GetFormat();
		
		tracks.ClearAndResize( numMidiTracks );
		cout << "numMidiTracks: " << numMidiTracks << endl;
		cout << "midiFormat: " << midiFormat << endl;
		
		if ( reader.Parse() )
		{
			cout << "reader parsed!: " << endl;
		}
		
		
		MIDISequencer seq( &tracks );
		sequencer = &seq;
		musicDurationInSeconds = sequencer->GetMusicDurationInSeconds();
		
		ofLogVerbose( "musicDurationInSeconds is ", ofToString(musicDurationInSeconds));
		
		if(musicDurationInSeconds > 0)
		{
			eventTrack = 0;	
		}
		midiout=new RtMidiOut();
		if (midiout->getPortCount())
		{
			midiout->openPort(midiPort);
			ofLogVerbose("Using Port name: " ,   ofToString(midiout->getPortName(0)) );
			//std::cout << "Using Port name: \"" << midiout->getPortName(0)<< "\"" << std::endl;
		}
		
		currentTime = 0.0;
		nextEventTime = 0.0;
		if (!sequencer->GoToTimeMs ( currentTime ))
		{
			ofLogError("Couldn't go to time in sequence: " ,   ofToString(currentTime) );
		}
		if ( !sequencer->GetNextEventTimeMs ( &nextEventTime ) )
		{
			ofLogVerbose("No next events for sequence", ofToString(nextEventTime));
		}
		max_time = (musicDurationInSeconds *1000.);
		myTime = 0;
		
		
		
		for ( ; currentTime < max_time; currentTime += 10. )
		{
			// find all events that came before or a the current time
			while ( nextEventTime <= currentTime )
			{
				myTime++;
				if (myTime <currentTime)
				{
					ofSleepMillis(1);
					continue;
				}
				if ( sequencer->GetNextEvent ( &eventTrack, &event ) )
				{
					
					ofLog ( OF_LOG_VERBOSE, 
							 "currentTime=%06.0f : nextEventTime=%06.0f : eventTrack=%02d",
							 currentTime, nextEventTime, eventTrack );
					MIDITimedBigMessage *msg=&event;
					if (msg->GetLength() > 0)
					{
						vector<unsigned char> message;
						message.push_back(msg->GetStatus());
						if (msg->GetLength()>0) message.push_back(msg->GetByte1());
						if (msg->GetLength()>1) message.push_back(msg->GetByte2());
						if (msg->GetLength()>2) message.push_back(msg->GetByte3());
						if (msg->GetLength()>3) message.push_back(msg->GetByte4());
						if (msg->GetLength()>4) message.push_back(msg->GetByte5());
						message.resize(msg->GetLength());
						midiout->sendMessage(&message);
						
						DumpMIDITimedBigMessage ( &event );
					}
					if ( !sequencer->GetNextEventTimeMs ( &nextEventTime ) )
					{
						ofLogVerbose("NO MORE EVENTS FOR SEQUENCE, LAST TIME CHECKED IS: " ,   ofToString(nextEventTime) );
						

					}
				}
			}
		}
		
		count++;	
		
		if(doLoop)
		{
			isReady = true;
		}else {
			
			isReady = false;
		}
	}
};
