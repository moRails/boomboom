#pragma once

#include "ofMain.h"
#include "ofxSyphon.h"
#include "ofxGui.h"
#include "ofxOsc.h"
#include "ofxBox2d.h"
#include "ofxKinect.h"
#include "ofxOpenCv.h"

// listen on port 12345
#define PORT 12345
#define NUM_MSG_STRINGS 20

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
    
    ofTexture tex;
    
	ofxSyphonServer mainOutputSyphonServer;
    
    ofxFloatSlider radius;
    ofxPanel gui;
    
    ofxOscReceiver receiver;
    int current_msg_string;
    string msg_strings[NUM_MSG_STRINGS];
    float timers[NUM_MSG_STRINGS];
    string oscMessage;
	float oscValue;
    
    vector <ofPolyline>                 lines;
	ofxBox2d                            box2d;
	vector <shared_ptr<ofxBox2dCircle> >		circles;
	vector <shared_ptr<ofxBox2dEdge> >       edges;
    
    ofxKinect kinect;
    int angle;
    
    ofxCvColorImage colorImg;
	
	ofxCvGrayscaleImage grayImage; // grayscale depth image
	ofxCvGrayscaleImage grayThreshNear; // the near thresholded image
	ofxCvGrayscaleImage grayThreshFar; // the far thresholded image
	
	ofxCvContourFinder contourFinder;
		
};
