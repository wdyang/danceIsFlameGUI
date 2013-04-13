#ifndef TESTAPP
#define TESTAPP

#include "ofMain.h"
#include "ofxUI.h"
#include "ofxOsc.h"

#define HOST "localhost"
#define PORT 12345
#define PORT_FROM_VIZ 12346

class testApp : public ofBaseApp 
{
	public:
	void setup();
	void update();
	void draw();
	void exit(); 

	void keyPressed  (int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
	
    void drawGrid(float x, float y); 
    
	void setGUI1(); 
	void setGUI2(); 
	void setGUI3(); 	
	void setGUI4(); 	    
	
	ofxUICanvas *gui1;   	
	ofxUICanvas *gui2;   	
	ofxUICanvas *gui3;
    ofxUIScrollableCanvas *gui4;

    
	bool hideGUI; 
	
	float red, green, blue; 
	bool bdrawGrid; 
	bool bdrawPadding; 
	
	void guiEvent(ofxUIEventArgs &e);
    
    ofxUIMovingGraph *mg; 
    float *buffer; 
    ofImage *img;
    
    
    ofxOscSender sender;
    ofxOscReceiver receiver;
    
    void split(vector<string> &tokens, const string &text, char sep);
};

#endif