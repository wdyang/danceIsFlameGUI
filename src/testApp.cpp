#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup()
{
    ofSetFrameRate(60);
	ofEnableSmoothing(); 
	ofBackground(0); 
	
	setGUI1(); 
	setGUI2(); 
    setGUI3(); 
    setGUI4();     

    gui1->setDrawBack(false);
    gui2->setDrawBack(false);
    gui3->setDrawBack(false);
    gui4->setDrawBack(false);    

//    gui1->setDrawBack(true);
//    gui2->setDrawBack(true);
//    gui3->setDrawBack(true);
//    gui4->setDrawBack(true);    
    
//    gui1->setDrawPadding(false);
//    gui2->setDrawPadding(false);
//    gui3->setDrawPadding(false);
//    
    ofBackground(red, green, blue);
    
    sender.setup(HOST, PORT);
    receiver.setup(PORT_FROM_VIZ);
}


void testApp::split(vector<string> &tokens, const string &text, char sep) {
    int start = 0, end = 0;
    while ((end = text.find(sep, start)) != string::npos) {
        tokens.push_back(text.substr(start, end - start));
        start = end + 1;
    }
    tokens.push_back(text.substr(start));
}


//--------------------------------------------------------------
void testApp::update()
{
//    mg->addPoint(ofGetFrameRate());

    
    //receive message
    
    while(receiver.hasWaitingMessages()){
        ofxOscMessage m;
        receiver.getNextMessage(&m);
        
        string msg_string="";
        string raw_address;
        raw_address = m.getAddress();
        
        string buf;
        vector<string> address;
        split(address, raw_address, '/');
        for(int i=0; i<address.size(); i++){
            msg_string+=address[i]+":";
        }
        
        msg_string += ": ";
        for (int i=0; i<m.getNumArgs(); i++){
            msg_string += m.getArgTypeName(i);
            msg_string +=": ";
            if(m.getArgType(i) == OFXOSC_TYPE_INT32){
                msg_string += ofToString(m.getArgAsInt32(i));
            }else if(m.getArgType(i)==OFXOSC_TYPE_FLOAT){
                msg_string += ofToString(m.getArgAsFloat(i));
            }else if(m.getArgType(i)==OFXOSC_TYPE_STRING){
                msg_string += ofToString(m.getArgAsString(i));
            }else{
                msg_string += "unknown";
            }
        }
        //add the the display list
//        msg_strings[current_msg_string]=msg_string;
//        timers[current_msg_string] = ofGetElapsedTimef()+5.0f;
//        current_msg_string = (current_msg_string +1) % NUM_MSG_STRINGS;
//        //clear the next line
//        msg_strings[current_msg_string]="";
        cout<<msg_string<<endl;
        if(address[1]=="danceIsFireSetting"){
            if(address[2]=="status"){
                string msg = m.getArgAsString(0);
                ofxUITextArea *status = (ofxUITextArea *) gui1->getWidget("status");
                status->setTextString(msg);
            }else if(address[2] == "drawFluid"){
                int val = m.getArgAsInt32(0);
                ofxUIToggle *toggle = (ofxUIToggle *) gui1->getWidget("drawFluid");
                toggle->setValue((bool) val);
            }
        }
    }

    
}

//--------------------------------------------------------------
void testApp::draw()
{    
	ofBackground(red, green, blue, 255); 
	
	ofPushStyle(); 
	ofEnableBlendMode(OF_BLENDMODE_ALPHA); 


	if(bdrawGrid)
	{
		ofSetColor(255, 255, 255, 25);	
		drawGrid(8,8); 
	}
    
	ofPopStyle(); 
    
    ofSetRectMode(OF_RECTMODE_CENTER); 
}
//--------------------------------------------------------------
void testApp::guiEvent(ofxUIEventArgs &e)
{
    string msg;
	string name = e.widget->getName(); 
	int kind = e.widget->getKind(); 
	cout << "got event from: " << name << " kind: "<< ofToString(kind)<< endl;

    ofxOscMessage m;
    string address;
    address = "/danceIsFire/"+name;
    m.setAddress(address);
    
	if(name == "RED")
	{
		ofxUISlider *slider = (ofxUISlider *) e.widget; 
		cout << "RED " << slider->getScaledValue() << endl; 
		red = slider->getScaledValue();
        m.addFloatArg(red);
	}else if(name == "CONNECT"){
        ofxUIButton *button = (ofxUIButton *) e.widget;
		int val = button->getValue();
        m.addIntArg(val);
    }else if(name == "GREEN")
	{
		ofxUISlider *slider = (ofxUISlider *) e.widget; 
		cout << "GREEN " << slider->getScaledValue() << endl; 
		green = slider->getScaledValue();
        m.addFloatArg(green);
	}
	
	else if(name == "BLUE")
	{
		ofxUISlider *slider = (ofxUISlider *) e.widget; 
		cout << "BLUE " << slider->getScaledValue() << endl;
		blue = slider->getScaledValue();
		m.addFloatArg(blue);
	}
	else if(name == "DRAW GRID")
	{
		ofxUIButton *button = (ofxUIButton *) e.widget; 
		bdrawGrid = button->getValue();
        m.addIntArg(bdrawGrid);
	}
	else if(name == "D_GRID")
	{
		ofxUIToggle *toggle = (ofxUIToggle *) e.widget; 
		bdrawGrid = toggle->getValue();
        m.addIntArg(bdrawGrid);
	}
    else if(name == "TEXT INPUT")
    {
        ofxUITextInput *textinput = (ofxUITextInput *) e.widget; 
        if(textinput->getTriggerType() == OFX_UI_TEXTINPUT_ON_ENTER)
        {
            cout << "ON ENTER: "; 
//            ofUnregisterKeyEvents((testApp*)this); 
        }
        else if(textinput->getTriggerType() == OFX_UI_TEXTINPUT_ON_FOCUS)
        {
            cout << "ON FOCUS: "; 
        }
        else if(textinput->getTriggerType() == OFX_UI_TEXTINPUT_ON_UNFOCUS)
        {
            cout << "ON BLUR: "; 
//            ofRegisterKeyEvents(this);             
        }        
        string output = textinput->getTextString(); 
        cout << output << endl;
        m.addStringArg(output);
    }
	
    sender.sendMessage(m);
	
	
}
//--------------------------------------------------------------
void testApp::exit()
{
	delete gui1; 
	delete gui2; 
    delete gui3; 
    delete gui4; 
	delete[] buffer; 
    delete img; 
}
//--------------------------------------------------------------
void testApp::keyPressed(int key)
{
    if(gui2->hasKeyboardFocus())
    {
        return;  
    }
	switch (key) 
	{			
		case '`':
		{
			string hr;         
			ofImage img; 
			img.grabScreen(0,0,ofGetWidth(), ofGetHeight()); 
			if(ofGetHours() < 12)
			{
				hr = " AM"; 
			}
			else
			{
				hr = " PM"; 
			}
			img.saveImage("snapshots/OFXUI "+ofToString(ofGetYear())+"-"+ofToString(ofGetMonth())+"-"+ofToString(ofGetDay())+" at "+ofToString(ofGetHours(),2)+"."+ofToString(ofGetMinutes(),2)+"."+ofToString(ofGetSeconds(),2) +hr +".png");
		}						
			break; 
			
		case 'f':
			ofToggleFullscreen(); 
			break;

		case 'h':
            gui1->toggleVisible(); 
            gui2->toggleVisible(); 
            gui3->toggleVisible(); 
            gui4->toggleVisible();             
			break;

		case 'p':
			bdrawPadding = !bdrawPadding; 
			gui1->setDrawWidgetPaddingOutline(bdrawPadding); 			
			gui2->setDrawWidgetPaddingOutline(bdrawPadding); 			
			gui3->setDrawWidgetPaddingOutline(bdrawPadding); 			            
			gui4->setDrawWidgetPaddingOutline(bdrawPadding); 			                        
			break;			

		case '[':
			gui1->setDrawWidgetPadding(false); 			
			gui2->setDrawWidgetPadding(false);
			gui3->setDrawWidgetPadding(false);
			gui4->setDrawWidgetPadding(false);            
			break;			

		case ']':
			gui1->setDrawWidgetPadding(true); 			
			gui2->setDrawWidgetPadding(true);
			gui3->setDrawWidgetPadding(true);
			gui4->setDrawWidgetPadding(true);            
			break;			
			
        case '1':
            gui1->toggleVisible(); 
            break;

        case '2':
            gui2->toggleVisible(); 
            break;

        case '3':
            gui3->toggleVisible(); 
            break;

        case '4':
            gui4->toggleVisible(); 
            break;
            
		default:
			break;
	}
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y )
{ 
	
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button)
{
 
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button)
{

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button)
{

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h)
{
 

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}

void testApp::drawGrid(float x, float y)
{
    float w = ofGetWidth(); 
    float h = ofGetHeight(); 
    
    for(int i = 0; i < h; i+=y)
    {
        ofLine(0,i,w,i); 
    }
    
    for(int j = 0; j < w; j+=x)
    {
        ofLine(j,0,j,h); 
    }    
}

void testApp::setGUI1()
{
	red = 233; blue = 52; green = 27; 
	
	float dim = 16; 
	float xInit = OFX_UI_GLOBAL_WIDGET_SPACING; 
    float length = 255-xInit; 
	hideGUI = false; 
	
    vector<string> names; 
	names.push_back("RAD1");
	names.push_back("RAD2");
	names.push_back("RAD3");	
	
	gui1 = new ofxUICanvas(0, 0, length+xInit, ofGetHeight()); 
    gui1->addButton("CONNECT", false, dim, dim);
    gui1->addTextArea("status", "Not Connected");
    gui1->addToggle("drawFluid", true);
    
    gui1->addWidgetDown(new ofxUILabel("PANEL 1: BASICS", OFX_UI_FONT_LARGE));
    gui1->addWidgetDown(new ofxUILabel("Press 'h' to Hide GUIs", OFX_UI_FONT_LARGE)); 

    gui1->addSpacer(length-xInit, 2);
	gui1->addWidgetDown(new ofxUILabel("H SLIDERS", OFX_UI_FONT_MEDIUM)); 
	gui1->addSlider("RED", 0.0, 255.0, red, length-xInit, dim);
	gui1->addSlider("GREEN", 0.0, 255.0, green, length-xInit,dim);
	gui1->addSlider("BLUE", 0.0, 255.0, blue, length-xInit,dim);

    gui1->addSpacer(length-xInit, 2); 
    gui1->addWidgetDown(new ofxUILabel("V SLIDERS", OFX_UI_FONT_MEDIUM)); 
	gui1->addSlider("0", 0.0, 255.0, 150, dim, 160);
	gui1->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
	gui1->addSlider("1", 0.0, 255.0, 150, dim, 160);
	gui1->addSlider("2", 0.0, 255.0, 150, dim, 160);
	gui1->addSlider("3", 0.0, 255.0, 150, dim, 160);
	gui1->addSlider("4", 0.0, 255.0, 150, dim, 160);
	gui1->addSlider("5", 0.0, 255.0, 150, dim, 160);
	gui1->addSlider("6", 0.0, 255.0, 150, dim, 160);
	gui1->addSlider("7", 0.0, 255.0, 150, dim, 160);
	gui1->addSlider("8", 0.0, 255.0, 150, dim, 160);
	gui1->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
    
    gui1->addSpacer(length-xInit, 2);
	gui1->addRadio("RADIO HORIZONTAL", names, OFX_UI_ORIENTATION_HORIZONTAL, dim, dim); 
	gui1->addRadio("RADIO VERTICAL", names, OFX_UI_ORIENTATION_VERTICAL, dim, dim); 

    gui1->addSpacer(length-xInit, 2);
	gui1->addWidgetDown(new ofxUILabel("BUTTONS", OFX_UI_FONT_MEDIUM)); 
	gui1->addButton("DRAW GRID", false, dim, dim);
	gui1->addWidgetDown(new ofxUILabel("TOGGLES", OFX_UI_FONT_MEDIUM)); 
	gui1->addToggle( "D_GRID", false, dim, dim);
    
    gui1->addSpacer(length-xInit, 2);
    gui1->addWidgetDown(new ofxUILabel("RANGE SLIDER", OFX_UI_FONT_MEDIUM)); 
	gui1->addRangeSlider("RSLIDER", 0.0, 255.0, 50.0, 100.0, length-xInit,dim);

    gui1->addSpacer(length-xInit, 2);
	gui1->addWidgetDown(new ofxUILabel("2D PAD", OFX_UI_FONT_MEDIUM)); 
	gui1->add2DPad("PAD", ofPoint(0,length-xInit), ofPoint(0,120), ofPoint((length-xInit)*.5,120*.5), length-xInit,120);


    
    bdrawGrid = false; 
	bdrawPadding = false; 	
	ofAddListener(gui1->newGUIEvent,this,&testApp::guiEvent);
}

void testApp::setGUI2()
{	
	float dim = 16; 
	float xInit = OFX_UI_GLOBAL_WIDGET_SPACING; 
    float length = 255-xInit; 
    
    img = new ofImage(); 
    img->loadImage("nerd_me.png"); 
    buffer = new float[256];     
    for(int i = 0; i < 256; i++) { buffer[i] = ofNoise(i/100.0); }

    gui2 = new ofxUICanvas(length+xInit+2, 0, length+xInit, ofGetHeight());     
	gui2->addWidgetDown(new ofxUILabel("PANEL 2: ADVANCED", OFX_UI_FONT_LARGE)); 

    gui2->addSpacer(length-xInit, 2);
	gui2->addWidgetDown(new ofxUILabel("TEXT INPUT", OFX_UI_FONT_MEDIUM));
	gui2->setWidgetFontSize(OFX_UI_FONT_LARGE);
	gui2->addTextInput("TEXT INPUT", "Input Text", length-xInit);
    gui2->addWidgetDown(new ofxUILabel("AUTO CLEAR DISABLED", OFX_UI_FONT_MEDIUM));
    gui2->addTextInput("TEXT INPUT2", "Input Text", length-xInit)->setAutoClear(false);
	gui2->setWidgetFontSize(OFX_UI_FONT_MEDIUM);

    gui2->addSpacer(length-xInit, 2);
    gui2->addWidgetDown(new ofxUILabel("WAVEFORM DISPLAY", OFX_UI_FONT_MEDIUM));     
	gui2->addWidgetDown(new ofxUIWaveform(length-xInit, 64, buffer, 256, 0.0, 1.0, "WAVEFORM")); 
    gui2->addWidgetDown(new ofxUILabel("SPECTRUM DISPLAY", OFX_UI_FONT_MEDIUM));    
    gui2->addWidgetDown(new ofxUISpectrum(length-xInit, 64, buffer, 256, 0.0, 1.0, "SPECTRUM")); 

    gui2->addSpacer(length-xInit, 2);
    gui2->addWidgetDown(new ofxUILabel("IMAGE DISPLAY", OFX_UI_FONT_MEDIUM));
	gui2->addWidgetDown(new ofxUIImage(img->width*.5, img->height*.5, img, "IMAGE CAPTION")); 

    gui2->addSpacer(length-xInit, 2);
    gui2->addWidgetDown(new ofxUILabel("FPS LABEL", OFX_UI_FONT_MEDIUM));
    gui2->addWidgetDown(new ofxUIFPS(OFX_UI_FONT_MEDIUM)); 

    gui2->addSpacer(length-xInit, 2);
    gui2->addWidgetDown(new ofxUILabel("NUMBER DIALER", OFX_UI_FONT_MEDIUM));
    gui2->addWidgetDown(new ofxUINumberDialer(-10000, 10000, 5000, 1, "NUMBER DIALER", OFX_UI_FONT_MEDIUM)); 

    gui2->addSpacer(length-xInit, 2);
    gui2->addWidgetDown(new ofxUILabel("LABEL BUTTON", OFX_UI_FONT_MEDIUM));
    gui2->addLabelButton("LABEL BUTTON", false, length-xInit);
    gui2->addLabelButton("AUTOSIZE BUTTON", false);
    gui2->addLabelButton("MEDIUM BUTTON", false);
    
    gui2->addSpacer(length-xInit, 2);
    gui2->addWidgetDown(new ofxUILabel("LABEL TOGGLES", OFX_UI_FONT_MEDIUM));	
    gui2->addLabelToggle("LABEL TOGGLE", false, length-xInit);
    gui2->addLabelToggle("AUTOSIZE TOGGLE", false);
    gui2->addLabelToggle("MEDIUM TOGGLE", false);
    
	ofAddListener(gui2->newGUIEvent,this,&testApp::guiEvent);
}

void testApp::setGUI3()
{	
	float dim = 16; 
	float xInit = OFX_UI_GLOBAL_WIDGET_SPACING; 
    float length = 255-xInit; 
	gui3 = new ofxUICanvas(length*2+xInit*2+4, 0, length+xInit, ofGetHeight());     
    gui3->addWidgetDown(new ofxUILabel("PANEL 3: ADVANCED", OFX_UI_FONT_LARGE)); 	

    gui3->addSpacer(length-xInit, 2);
    gui3->addWidgetDown(new ofxUILabel("MATRIX", OFX_UI_FONT_MEDIUM)); 				
    gui3->addWidgetDown(new ofxUIToggleMatrix(dim, dim, 4, 4, "MATRIX1")); 
    gui3->addWidgetDown(new ofxUIToggleMatrix(dim*2.0, dim*2.0, 3, 6, "MATRIX2")); 
    gui3->addWidgetDown(new ofxUIToggleMatrix(dim*3.0, dim*3.0, 1, 4, "MATRIX3")); 

    gui3->addSpacer(length-xInit, 2);
    gui3->addWidgetDown(new ofxUILabel("IMAGE BUTTON", OFX_UI_FONT_MEDIUM)); 				    
    gui3->addWidgetDown(new ofxUIImageButton(dim*4.0, dim*4.0, false, "GUI/images/App.png","IMAGEBTN"));
    gui3->addWidgetDown(new ofxUILabel("IMAGE TOGGLE", OFX_UI_FONT_MEDIUM)); 				        
    gui3->addWidgetDown(new ofxUIImageToggle(dim*4.0, dim*4.0, false, "GUI/images/Preview.png","IMAGETGL")); 

    gui3->addSpacer(length-xInit, 2);
    gui3->addWidgetDown(new ofxUILabel("ROTARY SLIDER", OFX_UI_FONT_MEDIUM)); 			
    gui3->addWidgetDown(new ofxUIRotarySlider(dim*4, 0, 100, 50, "R2SLIDER")); 			
    gui3->addSpacer(length-xInit, 2);
    gui3->addWidgetDown(new ofxUILabel("DROP DOWN", OFX_UI_FONT_MEDIUM)); 				
        
    vector<string> items; 
    items.push_back("FIRST ITEM"); 
    items.push_back("SECOND ITEM"); 
    items.push_back("THIRD ITEM WHATS UP DOG"); 
    items.push_back("FOURTH ITEM"); 
    items.push_back("FIFTH ITEM"); 
    items.push_back("SIXTH ITEM");     
    
    gui3->addDropDownList("DROP DOWN LIST", items, 200);

	ofAddListener(gui3->newGUIEvent,this,&testApp::guiEvent);
}

void testApp::setGUI4()
{	
	float dim = 16; 
	float xInit = OFX_UI_GLOBAL_WIDGET_SPACING; 
    float length = 255-xInit; 
	gui4 = new ofxUIScrollableCanvas(length*3+xInit*3+6, 0, length+xInit, ofGetHeight());     
    gui4->addWidgetDown(new ofxUILabel("PANEL 4: SCROLLABLE", OFX_UI_FONT_LARGE)); 	
    
    gui4->addSpacer(length-xInit, 2);

    gui4->addWidgetDown(new ofxUILabel("BILABEL SLIDER", OFX_UI_FONT_MEDIUM)); 				
    gui4->addWidgetDown(new ofxUIBiLabelSlider(length-xInit, 0, 100, 50, "BILABEL", "HOT", "COLD", OFX_UI_FONT_MEDIUM));

    gui4->addWidgetDown(new ofxUILabel("MINIMAL SLIDER", OFX_UI_FONT_MEDIUM)); 				
    gui4->addWidgetDown(new ofxUIMinimalSlider(length-xInit, dim, 0, 100, 50.0, "MINIMAL",OFX_UI_FONT_MEDIUM));

    gui4->addSpacer(length-xInit, 2);
    
    gui4->addWidgetDown(new ofxUILabel("CIRCLE SLIDER", OFX_UI_FONT_MEDIUM)); 				
    gui4->addWidgetDown(new ofxUICircleSlider((length-xInit)*.5, 0, 100, 50.0, "NORTH SOUTH", OFX_UI_FONT_MEDIUM));    
    
    gui4->addSpacer(length-xInit, 2);
    gui4->addWidgetDown(new ofxUILabel("FPS SLIDER", OFX_UI_FONT_MEDIUM)); 				
    gui4->addFPSSlider("FPS SLIDER", length-xInit, dim);

    vector<float> buffer; 
    for(int i = 0; i < 256; i++)
    {
        buffer.push_back(0.0);
    }

    gui4->addWidgetDown(new ofxUILabel("MOVING GRAPH", OFX_UI_FONT_MEDIUM)); 				    
    mg = (ofxUIMovingGraph *) gui4->addWidgetDown(new ofxUIMovingGraph(length-xInit, 120, buffer, 256, 0, 400, "MOVING GRAPH"));
    
    gui4->addSpacer(length-xInit, 2);
    gui4->addWidgetDown(new ofxUILabel("IMAGE SAMPLER", OFX_UI_FONT_MEDIUM)); 				
    gui4->addWidgetDown(new ofxUIImageSampler(img->getWidth(), img->getHeight(), img, "SAMPLER"));
    gui4->addWidgetDown(new ofxUIMultiImageButton(dim*2, dim*2, false, "GUI/toggle.png", "IMAGE BUTTON"));
    gui4->addWidgetDown(new ofxUIMultiImageToggle(dim*2, dim*2, false, "GUI/toggle.png", "IMAGE BUTTON"));
    
    
	ofAddListener(gui4->newGUIEvent,this,&testApp::guiEvent);
}