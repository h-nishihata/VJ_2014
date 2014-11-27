#pragma once

#include "ofxState.h"
#include "SharedData.h"
#include "fft.h"
#include "ofxPostGlitch.h"

#define num 1
#define kNumTestNodes 4
#define kNumCameras 2

extern float *magnitudeL;
extern float *magnitudeR;
extern int fft_size;

class BoxState : public itg::ofxState<SharedData> {
    void setup();
    void update();
    void draw();
    string getName();
    
    void drawFboTest();
    
    
    int camToView;
    int lookatIndex[kNumCameras];
    
    float xAxis, yAxis, zAxis;
    bool xFlag, yFlag, zFlag;
    
    ofNode testNodes[kNumTestNodes];
    
    ofLight light;
    ofCamera cam[kNumCameras];
    
    ofFbo			myFbo;
    ofxPostGlitch	myGlitch;
    
};