#pragma once

#include "ofMain.h"
#include "ofxPostGlitch.h"
#include "fft.h"

#define kNumTestNodes 4
#define kNumCameras 2

class ofApp : public ofBaseApp {
public:
    void setup();
    void update();
    void draw();
    void keyPressed  (int key);
    void keyReleased  (int key);
    void audioIn(float* input, int bufferSize, int nChannels);
    
    void drawBoxes();
    void drawWave();
    
    
// *****    visual  *****
    int camToView;
    int lookatIndex[kNumCameras];
    
    float xAxis, yAxis, zAxis;
    bool xFlag, yFlag, zFlag;
    
    ofLight light;
    ofCamera cam[kNumCameras];
    ofNode testNodes[kNumTestNodes];
    
    ofSpherePrimitive s;
    
    ofFbo			myFbo_00;
    ofFbo			myFbo_01;
    ofxPostGlitch	myGlitch_00;
    ofxPostGlitch	myGlitch_01;
    
    int switchScene;
    
// *****    sounds  *****
    //左右2chのFFTの入出力の値を確保
    float *left, *right;
    float *magnitudeL, *phaseL, *powerL;
    float *magnitudeR, *phaseR, *powerR;
    float avg_powerL, avg_powerR;
    //バッファーサイズとFFTサイズ
    int buffer_size;
    int fft_size;
    
    fft myfft; //FFTクラスのインスタンス
    
};