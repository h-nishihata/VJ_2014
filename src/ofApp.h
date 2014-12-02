#pragma once

#include "ofMain.h"
#include "ofxPostGlitch.h"
#include "fft.h"

#define kNumTestNodes 4
#define kNumCameras 2
#define numBoxes 50

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
    
    int width;
    int height;
    float extrusionAmount;
    ofVboMesh mainMesh;
    
    ofBoxPrimitive box[numBoxes];
    ofVec3f pos[numBoxes];
    ofVec3f vel[numBoxes];
    
    
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