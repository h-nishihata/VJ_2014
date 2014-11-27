#include "ofApp.h"
#include "CircleState.h"
#include "BoxState.h"
#include "ParticleState.h"

float *magnitudeL;
float *magnitudeR;
int fft_size;

//--------------------------------------------------------------
void ofApp::setup(){
    
    //    stateMachine.getSharedData().sharedMsg = "Here's shared message!";
    
    // StateMachineを初期化
    stateMachine.addState<CircleState>();
    stateMachine.addState<BoxState>();
    stateMachine.addState<ParticleState>();
    
    stateMachine.changeState("circle");
    
    
    
    //FFTのサイズとバッファサイズを設定
    fft_size = 512;
    buffer_size = fft_size * 2;
    
    //FFTサイズにあわせて出力結果の配列を左右2ch分準備
    left = new float[buffer_size];
    magnitudeL = new float[fft_size];
    phaseL = new float[fft_size];
    powerL = new float[fft_size];
    
    right = new float[buffer_size];
    magnitudeR = new float[fft_size];
    phaseR = new float[fft_size];
    powerR = new float[fft_size];
    
    //オーディオストリームの設定
    ofSoundStreamSetup(0, 2, this, 44100, buffer_size, 4);
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    //オーディオ入力をFFT解析 (左右2ch)
    avg_powerL = 0.0;
    avg_powerR = 0.0;
    myfft.powerSpectrum(0, fft_size, left, buffer_size,	magnitudeL, phaseL, powerL, &avg_powerL);
    myfft.powerSpectrum(0, fft_size, right, buffer_size, magnitudeR, phaseR, powerR, &avg_powerR);
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
}

//--------------------------------------------------------------
void ofApp::audioIn(float* input, int bufferSize, int nChannels) {
    
    //入力信号(input)を左右2ch(left, right)に分ける
    for (int i = 0; i < bufferSize; i++){
        left[i]		= input[i*2];
        right[i]	= input[i*2+1];
    }
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    if (key == '1') {
        stateMachine.changeState("circle");
    }
    if (key == '2') {
        stateMachine.changeState("box");
    }
    if (key == '3') {
        stateMachine.changeState("particle");
    }
    
    // fキーでフルスクリーン表示
    if(key == 'f'){
        ofToggleFullscreen();
    }
    
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
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
}
