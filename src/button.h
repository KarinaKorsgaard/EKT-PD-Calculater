//
//  button.h
//  OSCtoVDMX
//
//  Created by Karina Jensen on 13/04/16.
//
//

#ifndef button_h
#define button_h


#include "ofMain.h"

class Button {

    public:
    
    float x,y,rot;
    bool on;
    int obj,table;
    float rotation;
    float previousRotation;
    
    void update(){
        if((previousRotation-rot)<-0.8){
            rotation = 0;
            previousRotation = rot;
        }
        else if((previousRotation-rot)>0.8){
            rotation = 1;
            previousRotation = rot;
        }else{
            rotation += (rot-previousRotation);
            previousRotation = rot;
        }
        
    }
};

#endif /* button_h */
