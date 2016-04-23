#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
	ofSetLogLevel(OF_LOG_VERBOSE);
	
	bool shouldSequenceLoop = false;
	int midiPortNumber = 0;
	
	player.setup("snoop_dogg-the_shiznit.mid", midiPortNumber, shouldSequenceLoop);
	
    ofAddListener(player.midiEvent, this, &ofApp::midiEvent);
    
}

//--------------------------------------------------------------
void ofApp::exit(){
    player.stop();
}
//--------------------------------------------------------------
void ofApp::update(){

	ofSetWindowTitle(ofToString(ofGetFrameRate()));
}
//--------------------------------------------------------------
void ofApp::midiEvent(ofxMidiMessage& m){

    
}
//--------------------------------------------------------------
void ofApp::draw(){
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    if (key == 'a'){
        player.start();
    }else if (key == 's'){
        player.stop();
    }
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}