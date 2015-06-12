#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    cout << "listening for osc messages on port " << PORT << "\n";
	receiver.setup(PORT);
    current_msg_string = 0;
    
    mainOutputSyphonServer.setName("boomboomOUT");
    
    gui.setup(); // most of the time you don't need a name
    gui.add(radius.setup( "radius", 140, 10, 300 ));

}

//--------------------------------------------------------------
void ofApp::update()
{
    //------------------------------------------------------------------->  this is OSC messager
	
	// hide old messages
	for(int i = 0; i < NUM_MSG_STRINGS; i++){
		if(timers[i] < ofGetElapsedTimef()){
			msg_strings[i] = "";
		}
	}
	
	// check for waiting messages
	while(receiver.hasWaitingMessages())
	{
		// get the next message
		ofxOscMessage m;
		receiver.getNextMessage(&m);
		
		
		// unrecognized message: display on the bottom of the screen
		string msg_string;
		msg_string = m.getAddress();
		
		msg_string += ": ";
		for(int i = 0; i < m.getNumArgs(); i++)
		{
			// get the argument type
			msg_string += m.getArgTypeName(i);
			msg_string += ":";
			// display the argument - make sure we get the right type
			if(m.getArgType(i) == OFXOSC_TYPE_INT32)
			{
				msg_string += ofToString(m.getArgAsInt32(i));
			}
			else if(m.getArgType(i) == OFXOSC_TYPE_FLOAT)
			{
				msg_string += ofToString(m.getArgAsFloat(i));
			}
			else if(m.getArgType(i) == OFXOSC_TYPE_STRING)
			{
				msg_string += m.getArgAsString(i);
			}
			else
			{
				msg_string += "unknown";
			}
			oscValue =  m.getArgAsFloat(i);
			oscMessage = m.getAddress();
			//cout << oscValue << endl;
			//cout << oscMessage << endl;
		}
		// add to the list of strings to display
		msg_strings[current_msg_string] = msg_string;
		timers[current_msg_string] = ofGetElapsedTimef() + 5.0f;
		current_msg_string = (current_msg_string + 1) % NUM_MSG_STRINGS;
		// clear the next line
		msg_strings[current_msg_string] = "";
	}
    if (oscMessage == "radius")
	{
        // set osc change the value and the gui value
    }
	
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofCircle(mouseX, mouseY, radius);
    mainOutputSyphonServer.publishScreen();
    
    gui.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

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
