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
        void exit();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
        void clearAll();
    
    //------------------------------------------------------------------->  this is SYPHON
    ofTexture tex;
    
	ofxSyphonServer mainOutputSyphonServer;
    ofxSyphonServer toProject;
    //------------------------------------------------------------------->  this is GUI
    ofxFloatSlider radius;
    ofxFloatSlider distMax, distMin, simplification;
    ofxIntSlider colorRed, colorGreen, colorBlue, holeSizeMin, sizeMin, sizeMax;
    ofxIntSlider randomMax;
    ofxIntSlider thresholdValue;
    ofxIntSlider itemNumber;
    ofxIntSlider itemRandomMin, itemRandomMax;
    ofxToggle showImage;
    ofxToggle showShape;
    ofxToggle showLines;
    ofxToggle showCircles;
    ofxToggle showItems;
    ofxToggle showBackground;
    ofxToggle produceNewItems;
    ofxToggle blurGray;
    ofxToggle securitySpeed;
    ofxIntSlider blurLevel;
    ofxFloatSlider physicsA, physicsB, physicsC;
    ofxPanel gui;
    //------------------------------------------------------------------->  this is OSC
    ofxOscReceiver receiver;
    int current_msg_string;
    string msg_strings[NUM_MSG_STRINGS];
    float timers[NUM_MSG_STRINGS];
    string oscMessage;
	float oscValue;
    //------------------------------------------------------------------->  this is BOX2D
    vector <ofPolyline>                 lines;
	ofxBox2d                            box2d;
	vector <shared_ptr<ofxBox2dCircle> >		circles;
	vector <shared_ptr<ofxBox2dEdge> >       edges;
    //------------------------------------------------------------------->  this is KINECT
    ofxKinect kinect;
    int angle;
    //------------------------------------------------------------------->  this is OPEN CV
    ofxCvColorImage colorImg;
	
	ofxCvGrayscaleImage grayImage; // grayscale depth image
    ofxCvGrayscaleImage grayBg;
	ofxCvGrayscaleImage grayThreshNear; // the near thresholded image
	ofxCvGrayscaleImage grayThreshFar; // the far thresholded image
	ofxCvGrayscaleImage grayDiff;
    
	ofxCvContourFinder  contourFinder;
    int 				threshold;
    bool				bLearnBakground;
    
    
    ofImage monImage, myBackground;// image vide de ref�rence - image de d�part noir
    int totalItems;
    ofImage myItem[28];
    ofImage fond;
    ofFbo fbo;
    ofMesh myMesh;
    int widthOfTheWindow, heightOfTheWindow;
    bool showGui;
    int randomItem;
    vector<int> itemNumberFall;
    
    
};
