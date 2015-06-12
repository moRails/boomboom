#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
   
    //-- SYPHON
    mainOutputSyphonServer.setName("boomboomOUT");
    
    //-- GUI
    gui.setup(); // most of the time you don't need a name
    gui.add(radius.setup( "radius", 140, 10, 300 ));
    gui.add(distMax.setup( "distMax", 1500, 200, 8000 ));
    gui.add(distMin.setup( "distMin", 200, 100, 7500 ));
    
    //-- OSC
    cout << "listening for osc messages on port " << PORT << "\n";
	receiver.setup(PORT);
    current_msg_string = 0;
    
    //-- Box2d
	box2d.init();
	box2d.setGravity(0, 30);
	box2d.createGround();
	box2d.setFPS(60.0);
    
    //-- Kinect
    // enable depth->video image calibration
	kinect.setRegistration(true);
    
	kinect.init();
	//kinect.init(true); // shows infrared instead of RGB video image
	//kinect.init(false, false); // disable video image (faster fps)
	
	kinect.open();		// opens first available kinect
	//kinect.open(1);	// open a kinect by id, starting with 0 (sorted by serial # lexicographically))
	//kinect.open("A00362A08602047A");	// open a kinect using it's unique serial #
	
	// print the intrinsic IR sensor values
	if(kinect.isConnected()) {
		ofLogNotice() << "sensor-emitter dist: " << kinect.getSensorEmitterDistance() << "cm";
		ofLogNotice() << "sensor-camera dist:  " << kinect.getSensorCameraDistance() << "cm";
		ofLogNotice() << "zero plane pixel size: " << kinect.getZeroPlanePixelSize() << "mm";
		ofLogNotice() << "zero plane dist: " << kinect.getZeroPlaneDistance() << "mm";
	}
    
    // zero the tilt on startup
	angle = 0;
	kinect.setCameraTiltAngle(angle);
    
    monImage.loadImage ("monImage.png");



}

//--------------------------------------------------------------
void ofApp::update()
{
    //------------------------------------------------------------------->  this is KINECT
    kinect.update();
    if(kinect.isFrameNew())
	{
		int maDistance;
		for (int i=0;i< kinect.getWidth();i++)
		{
			for (int j=0;j< kinect.getHeight();j++)
			{
				maDistance = kinect.getDistanceAt(i,j);
				if (maDistance > distMax || maDistance < distMin )
				{
					ofColor myColor;
					myColor.set(0, 0, 0);
					monImage.setColor(i, j, myColor);
				}
				else
				{
                    ofColor myColor;
					myColor.set(255, 255, 255);
					monImage.setColor(i, j, myColor);

					
				}
			}
		}
        monImage.update();
    }
    //------------------------------------------------------------------->  this is BOX2D
    // add some circles every so often
	if((int)ofRandom(0, 10) == 0) {
		shared_ptr<ofxBox2dCircle> c = shared_ptr<ofxBox2dCircle>(new ofxBox2dCircle);
		c.get()->setPhysics(0.2, 0.2, 0.002);
		c.get()->setup(box2d.getWorld(), ofRandom(20, 50), -20, ofRandom(3, 10));
        c.get()->setVelocity(0, 15); // shoot them down!
		circles.push_back(c);
	}
	
	box2d.update();
    
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
    
    //------------------------------------------------------------------->  this is KINECT
    // draw from the live kinect
    kinect.drawDepth(220, 10, 200, 150);
    kinect.draw(440, 10, 200, 150);
    
    fbo.begin();
    {
        monImage.draw(0,200, 640, 480);
    }
    fbo.end();
    fbo.draw( 0, 0 );
    
    //------------------------------------------------------------------->  this is BOX2D
    // some circles :)
	for (int i=0; i<circles.size(); i++) {
		ofFill();
		ofSetHexColor(0xc0dd3b);
		circles[i].get()->draw();
	}
	
	ofSetHexColor(0x444342);
	ofNoFill();
	for (int i=0; i<lines.size(); i++) {
		lines[i].draw();
	}
	for (int i=0; i<edges.size(); i++) {
		edges[i].get()->draw();
	}

    ofCircle(mouseX, mouseY, radius);
    //------------------------------------------------------------------->  this is SYPHON
    mainOutputSyphonServer.publishScreen();
    //------------------------------------------------------------------->  this is GUI
    gui.draw();
}


//--------------------------------------------------------------
void ofApp::exit() {
	kinect.setCameraTiltAngle(0); // zero the tilt on exit
	kinect.close();
	
#ifdef USE_TWO_KINECTS
	kinect2.close();
#endif
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    //------------------------------------------------------------------->  this is BOX2D
    if(key == 'c') {
		lines.clear();
		edges.clear();
	}
    
    switch (key) {
        case 'w':
			kinect.enableDepthNearValueWhite(!kinect.isDepthNearValueWhite());
			break;
			
	}



}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    //------------------------------------------------------------------->  this is BOX2D
    lines.back().addVertex(x, y);
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    //------------------------------------------------------------------->  this is BOX2D
    lines.push_back(ofPolyline());
	lines.back().addVertex(x, y);
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    //------------------------------------------------------------------->  this is BOX2D
    shared_ptr <ofxBox2dEdge> edge = shared_ptr<ofxBox2dEdge>(new ofxBox2dEdge);
	lines.back().simplify();
	
	for (int i=0; i<lines.back().size(); i++) {
		edge.get()->addVertex(lines.back()[i]);
	}
	
	//poly.setPhysics(1, .2, 1);  // uncomment this to see it fall!
	edge.get()->create(box2d.getWorld());
	edges.push_back(edge);
	
	//lines.clear();

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
