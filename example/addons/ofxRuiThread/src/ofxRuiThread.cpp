/* 
 * Copyright (c) 2009, Rui Madeira
 * this code is based on Memo Akten's ofxMSAThread: http://code.google.com/p/ofxmsaof/
 * and in the discussion in here: http://www.openframeworks.cc/forum/viewtopic.php?t=813
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * http://creativecommons.org/licenses/LGPL/2.1/
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */


#include "ofxRuiThread.h"

ofxRuiThread::ofxRuiThread(){
	bOkToRun = bRunOnce = false;
}

ofxRuiThread::~ofxRuiThread(){

}

void ofxRuiThread::start(){
	initAndRun();
}

void ofxRuiThread::initAndRun(){
	startThread(true, false);
	bOkToRun = true;
}

void ofxRuiThread::initAndSleep(){
	startThread(true, false);
	bOkToRun = false;
}

void ofxRuiThread::stop(){
	stopThread();
}

void ofxRuiThread::updateOnce(){
	blockLock();
	bOkToRun = true;
	bRunOnce = true;
	unlock();
}

bool ofxRuiThread::isUpdating(){
	//if we cant lock it means the thread is being updated
	bool bRunning = tryLock();
	if(bRunning)unlock();
	return bRunning;
}

void ofxRuiThread::beginUpdate(){
	blockLock();
	bOkToRun = true;
	unlock();
}

void ofxRuiThread::pauseUpdate(){
	blockLock();
	bOkToRun = false;
	unlock();
}

//this should be used in junction with updateOnce()
//to make sure your threads are synched with the main thread
//(check the example)
void ofxRuiThread::waitToFinish(){
	bool bRunning = true;
	while(bRunning){
		blockLock();
		if(!bOkToRun) bRunning = false;
		unlock();
	}
}

void ofxRuiThread::blockLock(){
	if(blocking){
		lock();
		return;	
	}
	blocking = true;
	lock();
	blocking = false;
}

bool ofxRuiThread::tryLock(){
	if(!blocking) return lock();
	blocking = false;
	bool bLocked = lock();
	blocking = true;
	return bLocked;
}

void ofxRuiThread::threadedFunction(){
	while(isThreadRunning()){
		blockLock();
		if(bOkToRun){
			//cout << "threadedFunction" << endl;
			updateThread();
			if(bRunOnce){
				bRunOnce = false;
				bOkToRun = false;
			}
		}
		unlock();
		ofSleepMillis(10);
	}
}

