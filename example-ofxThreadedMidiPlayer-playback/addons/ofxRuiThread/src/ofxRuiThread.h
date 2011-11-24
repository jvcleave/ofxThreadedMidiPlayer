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



#pragma once

#include "ofThread.h"
#include "ofMain.h"

class ofxRuiThread:public ofThread{
protected:
	bool bOkToRun;
	bool bRunOnce;
	virtual void threadedFunction();
public:
	ofxRuiThread();
	virtual ~ofxRuiThread();
	
	virtual void start(); // same as initAndRun();
	virtual void initAndRun(); //init thread and start updating
	virtual void initAndSleep(); //init thread but dont update until you call updateOnce() or beginUpdate();
	virtual void stop(); //stops thread
	virtual void updateOnce(); //calls updateThread() only once
	virtual void beginUpdate(); //calls to updateThread() consecutively 
	virtual bool isUpdating(); //is updateThread() being called.
	virtual void pauseUpdate(); //pauses calls to updateThread(), use beginUpdate() to resume
	
	//this should be used in junction with updateOnce()
	//to make sure your threads are synched with the main thread
	//(check the example)
	virtual void waitToFinish(); //waits till updateThread() is finished
	virtual void blockLock(); //blocks the app until a lock is successful
	virtual bool tryLock(); //tries to lock and returns true if successful
	
	virtual void updateThread(){ //the function being called by the thread, overload it with your own
		printf("ofxRuiThread::updateThread - this function is updated by the thread, overload it with your own");
	}
};

