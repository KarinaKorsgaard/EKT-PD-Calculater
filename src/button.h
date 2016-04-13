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
    float prot;
    
    void update(){
        if(abs(prot-rot)<0.5 && prot!=0 && prot!=rot){
            
            if(prot>rot){
                rotation+=abs(prot-rot);
            }
            
            
            if(prot<rot){
                rotation-=abs(prot-rot);
            }
            
        }
        if(rotation > 1)rotation = 1;
        if(rotation < 0)rotation = 0;

        if(!on){
            rotation=0;
            prot=0;
        }
        prot=rot;
        
    }
};

#endif /* button_h */
