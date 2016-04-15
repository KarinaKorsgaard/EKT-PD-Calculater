#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    string newClipboardContent = ofSystemTextBoxDialog("Setup port osc is coming from");
    
    ofxClipboard::copy(newClipboardContent); //<<<<<<<<<<<<<<<<<
    string clipboardContent = ofxClipboard::paste(); //<<<<<<<<<<<<<<<<<
    port = ofToInt(clipboardContent);
    
    newClipboardContent = ofSystemTextBoxDialog("Setup sending to port");
    
    ofxClipboard::copy(newClipboardContent); //<<<<<<<<<<<<<<<<<
    clipboardContent = ofxClipboard::paste(); //<<<<<<<<<<<<<<<<<
    sendTo = ofToInt(clipboardContent);
    
    //    newClipboardContent = ofSystemTextBoxDialog("IP");
    //
    //    ofxClipboard::copy(newClipboardContent); //<<<<<<<<<<<<<<<<<
    //    clipboardContent = ofxClipboard::paste(); //<<<<<<<<<<<<<<<<<
    //    host = ofToInt(clipboardContent);
    
    nameStuff();
    receiver.setup(port);
    sender.setup(HOST,sendTo);
    
    for(int i = 0; i<12;i++){
        for(int u = 0; u<6;u++){
            string theName =ofToString(i)+"/"+ofToString(u);
            Button b = *new Button;
            b.obj = u;
            b.table = i;
            buttons.push_back(b);
        }
    }
    ofSetFrameRate(30);
    
}

//--------------------------------------------------------------
void ofApp::update(){
    bool send = false;
    
    ofxOscBundle bundle;
    while(receiver.hasWaitingMessages()){
        send = true;
        ofxOscMessage sendMessage;
        ofxOscMessage msg;
        
        receiver.getNextMessage(msg);
        
        oscMessage = msg.getAddress();
        for(int i = 0; i<tablenames.size(); i++){
            for(int u = 0; u<othernames.size(); u++){
                
                if (msg.getAddress()==tablenames[i]+"/switch"+objnames[u]){
                    if(msg.getArgAsInt32(0)==1){
                        
                        buttons[i*6+u].on=true;
                        sendMessage.setAddress("/toVDMX/"+ofToString(i+1)+"/"+ofToString(u+1)+"/on");
                        sendMessage.addInt32Arg(1);
                        bundle.addMessage(sendMessage);
                        sendMessage.clear();
                        
                        
                    }else if(msg.getArgAsInt32(0)==0){
                        buttons[i*6+u].on=false;
                        msg.clear();
                        sendMessage.setAddress("/toVDMX/"+ofToString(i+1)+"/"+ofToString(u+1)+"/on");
                        sendMessage.addInt32Arg(0);
                        bundle.addMessage(sendMessage);
                        sendMessage.clear();
                        
                        
                        
                    }
                }
                
                else if (msg.getAddress()==tablenames[i]+othernames[u]){
                    
                    buttons[i*6+u].x=1-(msg.getArgAsFloat(0)/127);
                    sendMessage.setAddress("/toVDMX/"+ofToString(i+1)+"/"+ofToString(u+1)+"/X");
                    sendMessage.addFloatArg(buttons[i*6+u].x);
                    bundle.addMessage(sendMessage);
                    sendMessage.clear();
                    
                    buttons[i*6+u].y=(msg.getArgAsFloat(1))/127;;
                    sendMessage.setAddress("/toVDMX/"+ofToString(i+1)+"/"+ofToString(u+1)+"/Y");
                    sendMessage.addFloatArg(buttons[i*6+u].y);
                    bundle.addMessage(sendMessage);
                    sendMessage.clear();
                    
                    buttons[i*6+u].rot=1-(msg.getArgAsFloat(2)/(2*PI));
                    sendMessage.setAddress("/toVDMX/"+ofToString(i+1)+"/"+ofToString(u+1)+"/R");
                    buttons[i*6+u].update();
                    sendMessage.addFloatArg(buttons[i*6+u].rotation);
                    bundle.addMessage(sendMessage);
                    sendMessage.clear();
                    
                    
                    
                    
                }
            }
        }
        
    }
    
    if(send){
        sender.sendBundle(bundle);
        bundle.clear();
        
        
        //averaged values;
        float total = 0;
        float xAdded = 0;
        float yAdded = 0;
        float rotAdded = 0;
        
        vector<float>sepAdddedX;
        vector<float>sepAdddedY;
        vector<float>sepAdddedRot;
        
        vector<float>totalSep;
        
        sepAdddedX.resize(6);
        sepAdddedY.resize(6);
        sepAdddedRot.resize(6);
        
        totalSep.resize(6);
        
        
        for(int i = 0; i<buttons.size();i++){
            if(buttons[i].on){
                total ++;
                xAdded += buttons[i].x;
                yAdded += buttons[i].y;
                rotAdded+= buttons[i].rotation;
                for(int u = 0; u<6;u++){
                    if(buttons[i].obj==u){
                        totalSep[u]++;
                        sepAdddedX[u]+=buttons[i].x;
                        sepAdddedY[u]+=buttons[i].y;
                        sepAdddedRot[u]+=buttons[i].rotation;
                    }
                }
            }
        }
        ofxOscMessage m;
        m.setAddress("/total");
        m.addFloatArg(total);
        for(int i = 0; i<totalSep.size();i++){
            m.addFloatArg(totalSep[i]);
        }
        bundle.addMessage(m);
        m.clear();
        
        m.setAddress("/X");
        m.addFloatArg(xAdded);
        for(int i = 0; i<sepAdddedX.size();i++){
            m.addFloatArg(sepAdddedX[i]);
        }
        bundle.addMessage(m);
        m.clear();
        
        m.setAddress("/Y");
        m.addFloatArg(yAdded);
        for(int i = 0; i<sepAdddedX.size();i++){
            m.addFloatArg(sepAdddedY[i]);
        }
        bundle.addMessage(m);
        m.clear();
        
        m.setAddress("/R");
        m.addFloatArg(rotAdded);
        for(int i = 0; i<sepAdddedX.size();i++){
            m.addFloatArg(sepAdddedRot[i]);
        }
        bundle.addMessage(m);
        m.clear();
        senddedoscMessage = ofToString(total);
        sender.sendBundle(bundle);
    }
    
    ofSetWindowTitle("Framerate: "+ofToString(ofGetFrameRate(),0));
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetBackgroundColor(0);
    ofSetColor(255);
    
    ofDrawBitmapString("routing osc to " +ofToString(sendTo), 10, 20);
    ofDrawBitmapString("from port " +ofToString(port), 10, 40);
    ofDrawBitmapString("on ip localhost", 10, 60);
    ofDrawBitmapString(oscMessage, 10, 80);
    
    
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    //    ofxOscBundle bundle;
    //    for(int i = 0 ; i<buttons.size();i++){
    //        buttons[i].on = true;
    //        buttons[i].x = ofRandom(1);
    //        buttons[i].y = ofRandom(1);
    //        buttons[i].rot = ofRandom(1);
    //
    //
    //
    //        ofxOscMessage sendMessage;
    //
    //        sendMessage.setAddress("/toVDMX/"+ofToString(buttons[i].table)+"/"+ofToString(buttons[i].obj)+"/ROT");
    //        sendMessage.addFloatArg(buttons[i].rot);
    //        bundle.addMessage(sendMessage);
    //        sendMessage.clear();
    //
    //        sendMessage.setAddress("/toVDMX/"+ofToString(buttons[i].table)+"/"+ofToString(buttons[i].obj)+"/X");
    //        sendMessage.addFloatArg(buttons[i].x);
    //        bundle.addMessage(sendMessage);
    //        sendMessage.clear();
    //
    //        sendMessage.setAddress("/toVDMX/"+ofToString(buttons[i].table)+"/"+ofToString(buttons[i].obj)+"/Y");
    //        sendMessage.addFloatArg(buttons[i].y);
    //        bundle.addMessage(sendMessage);
    //        sendMessage.clear();
    //    }
    //    sender.sendBundle(bundle);
    
    
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
void ofApp::mouseEntered(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){
    
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

void ofApp::nameStuff(){
    
    tablenames.push_back("/tableOne");
    tablenames.push_back("/tableTwo");
    tablenames.push_back("/tableThree");
    tablenames.push_back("/tableFour");
    tablenames.push_back("/tableFive");
    tablenames.push_back("/tableSix");
    tablenames.push_back("/tableSeven");
    tablenames.push_back("/tableEight");
    tablenames.push_back("/tableNine");
    tablenames.push_back("/tableTen");
    tablenames.push_back("/tableEleven");
    tablenames.push_back("/tableTwelve");
    
    objnames.push_back("/zero");
    objnames.push_back("/one");
    objnames.push_back("/two");
    objnames.push_back("/three");
    objnames.push_back("/four");
    objnames.push_back("/five");
    
    othernames.push_back("/objZero");
    othernames.push_back("/objOne");
    othernames.push_back("/objTwo");
    othernames.push_back("/objThree");
    othernames.push_back("/objFour");
    othernames.push_back("/objFive");
    
}

