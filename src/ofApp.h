#pragma once

#include "ofMain.h"
#include "ofxStateMachine.h"
#include "SharedData.h"
#include "fft.h"

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
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    itg::ofxStateMachine<SharedData> stateMachine;
    
    void audioIn(float* input, int bufferSize, int nChannels);
    
    //左右2chのFFTの入出力の値を確保
    float *left, *right;
    float *phaseL, *powerL;
    float *phaseR, *powerR;
    float avg_powerL, avg_powerR;
    
    //バッファーサイズとFFTサイズ
    int buffer_size;
    
    fft myfft; //FFTクラスのインスタンス
    
};