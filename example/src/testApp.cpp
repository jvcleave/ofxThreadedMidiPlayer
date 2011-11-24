#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup()
{
	ofSetLogLevel(OF_LOG_VERBOSE);
	
	ofxThreadedMidiPlayer * thread = new ofxThreadedMidiPlayer();
	bool shouldSequenceLoop = false;
	int midiPortNumber = 0;
	
	thread->setup("snoop_dogg-the_shiznit.mid", midiPortNumber, shouldSequenceLoop);
	thread->initAndSleep();
	threads.push_back(thread);
	
	
}



//--------------------------------------------------------------
void testApp::update(){
	for(int i=0; i<threads.size(); i++)
	{
		if(threads[i]->isReady)
		{
			threads[i]->updateOnce();
		}
		
	}
	ofSetWindowTitle(ofToString(ofGetFrameRate()));
}

//--------------------------------------------------------------
void testApp::draw()
{
	for(int i=0; i<threads.size(); i++)
	{
		if ( threads[i]->lastTimedBigMessage != NULL )
		{
			char msgbuf[1024];
			ofDrawBitmapString(threads[i]->lastTimedBigMessage->MsgToText ( msgbuf ), 20, 40);
			if (threads[i]->lastTimedBigMessage->IsNoteOn())
			{
				cout << "threads[i]->lastTimedBigMessage->GetVelocity(): " << (int)threads[i]->lastTimedBigMessage->GetVelocity() << endl;
			}
		}
	}
	
	ofDrawBitmapString(ofToString(ofGetFrameNum()), 20, 20);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}