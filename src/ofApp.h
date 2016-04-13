#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxClipboard.h"
#include "button.h"

#define HOST "localhost.h"

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
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
    
    vector<Button>buttons;
    vector<string> tablenames;
    vector<string> objnames;
    vector<string> othernames;
    
    void nameStuff();
    //string host;
    int port;
    int sendTo;
    bool setupPort;
    ofxOscReceiver receiver;
    ofxOscSender sender;
    
    string oscMessage;
    

};
