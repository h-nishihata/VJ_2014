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
    ofFbo			myFbo_00;
    ofFbo			myFbo_01;
    ofxPostGlitch	myGlitch_00;
    ofxPostGlitch	myGlitch_01;
    
    int switchScene;
    int col;
    
    int lookatIndex[kNumCameras];
    ofNode testNodes[kNumTestNodes];
    
    float xAxis, yAxis, zAxis;
    bool xFlag, yFlag, zFlag;
    ofCamera cam[kNumCameras];

    ofLight light;
    
    static const int WIDTH = 200;
    static const int HEIGHT = 200;
    static const int NUM_PARTICLES = WIDTH * HEIGHT;
    ofVbo vbo;
    ofVec3f myVerts[NUM_PARTICLES];
    ofFloatColor myColor[NUM_PARTICLES];

    
// *****    sounds  *****
    //左右2chのFFTの入出力の値を確保
    float *input;
    float *magnitude, *phase, *power;
//    float *magnitudeR, *phaseR, *powerR;
    float avg_power;
    //バッファーサイズとFFTサイズ
    int buffer_size;
    int fft_size;
    
    fft myfft; //FFTクラスのインスタンス
    
};