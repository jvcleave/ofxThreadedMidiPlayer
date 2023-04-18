
#include "ofxThreadedMidiPlayer.h"



ofxThreadedMidiPlayer::ofxThreadedMidiPlayer(){
    count = 0;
    midiFileName = "";
    midiPort = 0;
    currentTime = 0.0;
    nextEventTime = 0.0;
    
    musicDurationInSeconds = 0;
    max_time = 0;
    myTime = 0;
    //        lastTimedBigMessage = new MIDITimedBigMessage();
    tracks = NULL;
    sequencer = NULL;
    midiout = NULL;
    doLoop = true;
    isReady = true;
    bIsInited = false;
}
ofxThreadedMidiPlayer::~ofxThreadedMidiPlayer(){
    cout << __PRETTY_FUNCTION__ << endl;
    stop();
    clean();
}
void ofxThreadedMidiPlayer::stop(){
    cout << __PRETTY_FUNCTION__ << endl;
    stopThread();
    waitForThread(false);
    //clean();
    
}
void ofxThreadedMidiPlayer::DumpMIDITimedBigMessage( const MIDITimedBigMessage& msg )
{
    char msgbuf[1024];
    lastTimedBigMessage.Copy(msg);

    // note that Sequencer generate SERVICE_BEAT_MARKER in files dump,
    // but files themselves not contain this meta event...
    // see MIDISequencer::beat_marker_msg.SetBeatMarker()
    /*
    if ( msg.IsBeatMarker() )
    {
        ofLog(OF_LOG_VERBOSE, "%8ld : %s <------------------>", msg.GetTime(), msg.MsgToText ( msgbuf ) );
    }
    else
    {
        ofLog(OF_LOG_VERBOSE, "%8ld : %s", msg.GetTime(), msg.MsgToText ( msgbuf ) );
    }
    
    if ( msg.IsSystemExclusive() )
    {
        ofLog(OF_LOG_VERBOSE, "SYSEX length: %d", msg.GetSysEx()->GetLengthSE() );
    }
    //*/
    
    
}
void ofxThreadedMidiPlayer::start(){
    startThread();
}

void ofxThreadedMidiPlayer::setup(string fileName, int portNumber, bool shouldLoop){
    midiFileName = fileName;
    midiPort = portNumber;
    doLoop = shouldLoop;
    clean();
}

void ofxThreadedMidiPlayer::dispatchMidiEvent( float currentTime, float timeDelta, vector<unsigned char>& message){
    ofxMidiMessage midiMessage(&message);
    
    if((message.at(0)) >= MIDI_SYSEX) {
        midiMessage.status = (MidiStatus)(message.at(0) & 0xFF);
        midiMessage.channel = 0;
    } else {
        midiMessage.status = (MidiStatus) (message.at(0) & 0xF0);
        midiMessage.channel = (int) (message.at(0) & 0x0F)+1;
    }
    
    midiMessage.deltatime = timeDelta;// deltatime;// * 1000; // convert s to ms
    midiMessage.portNum = midiPort;
   // midiMessage.portName = portName;
    
    switch(midiMessage.status) {
        case MIDI_NOTE_ON :
        case MIDI_NOTE_OFF:
            midiMessage.pitch = (int) message.at(1);
            midiMessage.velocity = (int) message.at(2);
            break;
        case MIDI_CONTROL_CHANGE:
            midiMessage.control = (int) message.at(1);
            midiMessage.value = (int) message.at(2);
            break;
        case MIDI_PROGRAM_CHANGE:
        case MIDI_AFTERTOUCH:
            midiMessage.value = (int) message.at(1);
            break;
        case MIDI_PITCH_BEND:
            midiMessage.value = (int) (message.at(2) << 7) +
            (int) message.at(1); // msb + lsb
            break;
        case MIDI_POLY_AFTERTOUCH:
            midiMessage.pitch = (int) message.at(1);
            midiMessage.value = (int) message.at(2);
            break;
        default:
            break;
    }
    
    ofNotifyEvent(midiEvent, midiMessage, this);
    
}
void ofxThreadedMidiPlayer::threadedFunction(){
    while(isThreadRunning()){
        
        init();
        
        myTime = 0;
        
        
        MIDITimedBigMessage event;
        int eventTrack = 0;
        
        for ( ; currentTime <= max_time && isThreadRunning(); currentTime += 10. ){
            // find all events that came before or a the current time
            while ( nextEventTime <= currentTime ){
                myTime++;
                if (myTime <currentTime){
                    sleep(1);
                    continue;
                }
                if(sequencer){
                    if ( sequencer->GetNextEvent ( &eventTrack, &event ) ){
                        
//                        ofLog ( OF_LOG_VERBOSE,
//                              "currentTime=%06.0f : nextEventTime=%06.0f : eventTrack=%02d",
//                              currentTime, nextEventTime, eventTrack );
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
                            
                            dispatchMidiEvent(currentTime, currentTime - nextEventTime,message);
                            
                            DumpMIDITimedBigMessage ( event );
                        }
                        if ( !sequencer->GetNextEventTimeMs ( &nextEventTime ) ){
                            ofLogVerbose("NO MORE EVENTS FOR SEQUENCE, LAST TIME CHECKED IS: " ,   ofToString(nextEventTime) );
                        }
                    }
                }
            }
        }
        count++;
        
        if(doLoop)
        {
             ofLogVerbose("isLooped: YES");
             //ofNotifyEvent(midiEvent, midiMessage, this);
             //midiOut.sendNoteOn(channel, note,  velocity);
             //ofxMidiOut::sendControlChange(int channel, int control, int value) {
             clean();
             init();
            isReady = true;
        }else {
            
            isReady = false;
        }
        
    }
}
void ofxThreadedMidiPlayer::clean(){
    cout << __PRETTY_FUNCTION__ << endl;
    if(tracks){
        delete tracks;
        tracks = NULL;
    }
    if(sequencer){
        delete sequencer;
        sequencer = NULL;
    }
    //        if(lastTimedBigMessage){
    //            delete lastTimedBigMessage;
    //            lastTimedBigMessage = NULL;
    //        }
    if(midiout){
        delete midiout;
        midiout = NULL;
    }
    bIsInited = false;
}

void ofxThreadedMidiPlayer::init(){
    if(!bIsInited){
        isReady = false;
        string filePath = ofToDataPath(midiFileName, true);
        
        MIDIFileReadStreamFile rs ( filePath.c_str() );
        
        if ( !rs.IsValid() )
        {
            ofLogError( "ERROR OPENING FILE AT: ",  filePath);
            
        }
        
        tracks = new MIDIMultiTrack();
        MIDIFileReadMultiTrack track_loader ( tracks );
        MIDIFileRead reader ( &rs, &track_loader );
        
        int numMidiTracks = reader.ReadNumTracks();
        int midiFormat = reader.GetFormat();
        
        tracks->ClearAndResize( numMidiTracks );
        cout << "numMidiTracks: " << numMidiTracks << endl;
        cout << "midiFormat: " << midiFormat << endl;
        
        if ( reader.Parse() ){
            cout << "reader parsed!: " << endl;
        }
        
        
        //MIDISequencer seq( &tracks );
        sequencer = new MIDISequencer ( tracks );//&seq;
        musicDurationInSeconds = sequencer->GetMusicDurationInSeconds();
        
        ofLogVerbose( "musicDurationInSeconds is ", ofToString(musicDurationInSeconds));
        
        midiout=new RtMidiOut();
        if (midiout->getPortCount()){
            midiout->openPort(midiPort);
            ofLogVerbose("Total ports: " ,   ofToString(midiout->getPortCount()) );
//            // Check inputs.
//            unsigned int nPorts = midiout->getPortCount();
//            std::string portName;
//            for ( unsigned int i=0; i<nPorts; i++ ) {
//              try {
//                portName = midiout->getPortName(i);
//              }
//              catch ( RtMidiError &error ) {
//                error.printMessage();
//              }
//              //ofLogVerbose("Using Port name: " ,   ofToString(portName) );
//              std::cout << "  Input Port #" << i+1 << ": " << portName << '\n';
//            }
            ofLogVerbose("Using Port name: " ,   ofToString(midiout->getPortName(midiPort)) );

        }
        
        currentTime = 0.0;
        nextEventTime = 0.0;
        if (!sequencer->GoToTimeMs ( currentTime )){
            ofLogError("Couldn't go to time in sequence: " ,   ofToString(currentTime) );
        }
        if ( !sequencer->GetNextEventTimeMs ( &nextEventTime ) ){
            ofLogVerbose("No next events for sequence", ofToString(nextEventTime));
        }
        max_time = (musicDurationInSeconds *1000.);
        bIsInited = true;
    }
}

