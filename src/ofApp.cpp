#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetVerticalSync(true);
    ofSetFrameRate(60);
    ofEnableDepthTest();
    
    myFbo.allocate(1024, 768, GL_RGBA);
    myGlitch.setup(&myFbo);
    
    myFbo.begin();
    ofClear(255, 255, 255, 0);
    myFbo.end();
    
    // nodes
    testNodes[0].setOrientation(ofVec3f(30,0,40));
    testNodes[1].setOrientation(ofVec3f(40,0,30));
    
    
    // cameras
    cam[0].setPosition(40, 40, 800);
    cam[1].setPosition(40, 100, -100);
    lookatIndex[1] = kNumTestNodes-1;
    
    
    // lights
    light.enable();
    light.setSpotlight();
    light.setPosition(50, 80, 150);
    light.lookAt(testNodes[0]);
    
    light.setAmbientColor(ofFloatColor(1.0, 0.6, 0.9, 1.0));
    light.setDiffuseColor(ofFloatColor(1.0, 0.6, 0.9));
    light.setSpecularColor(ofFloatColor(1.0, 1.0, 1.0));
    
    
// *****    sounds  *****
    
    //FFTのサイズとバッファサイズを設定
    fft_size = 512;
    buffer_size = fft_size * 2;
    
    //FFTサイズにあわせて出力結果の配列を左右2ch分準備
    input = new float[buffer_size];
    magnitude = new float[fft_size];
    phase = new float[fft_size];
    power = new float[fft_size];
    
    //    right = new float[buffer_size];
    //    magnitudeR = new float[fft_size];
    //    phaseR = new float[fft_size];
    //    powerR = new float[fft_size];
    
    //オーディオストリームの設定
    ofSoundStreamSetup(0, 2, this, 44100, buffer_size, 4);
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    ofEnableAlphaBlending();
    
    myFbo.begin();
    
    if (xFlag == false) {
        xAxis = -ofRandom(10);
        if (cam[0].getX() < -500) {
            xFlag = true;
        }
    }else if (xFlag == true) {
        xAxis = ofRandom(10);
        if (cam[0].getX() > 500) {
            xFlag = false;
        }
    }
    
    if (yFlag == false) {
        yAxis = -10;
        if (cam[0].getY() < -300) {
            yFlag = true;
        }
    }else if (yFlag == true) {
        yAxis = 10;
        if (cam[0].getY() > 300) {
            yFlag = false;
        }
    }
    
    if (zFlag == false && (cam[0].getX() > 400 || cam[0].getX() < -400)) {
        zAxis = -10;
        if (cam[0].getZ() < -500) {
            zFlag = true;
        }
    }else if (zFlag == true) {
        zAxis = 10;
        if (cam[0].getZ() > 500) {
            zFlag = false;
        }
    }
    
    cam[0].move(xAxis, yAxis, zAxis);
    
    drawFboTest();
    myFbo.end();

    
// *****    sounds  *****

    avg_power = 0.0;
    myfft.powerSpectrum(0, fft_size, input, buffer_size, magnitude, phase, power, &avg_power);
    
}

//--------------------------------------------------------------
void ofApp::drawFboTest(){
    
    float movementSpeed = .3;
    float cloudSize = ofGetWidth();
    float maxBoxSize = 50;
    float spacing = 50;
    int boxCount = 30;
    
    
    ofEnableAlphaBlending();
    ofClear(255, 255, 255, 0);
    /*
    fadeAmnt = 40;
    if(ofGetKeyPressed('z')){
        fadeAmnt = 1;
    }else if(ofGetKeyPressed('x')){
        fadeAmnt = 5;
    }else if(ofGetKeyPressed('c')){
        fadeAmnt = 15;
    }
    ofFill();
    ofSetColor(255,255,255, fadeAmnt);
    ofRect(0,0,ofGetWidth(),ofGetHeight());
    */
    
    
    for (int i=0; i<kNumCameras; i++) {
        if (lookatIndex[i] >= 0) {
            cam[i].lookAt(testNodes[lookatIndex[i]]);
        }
    }
    
    cam[0].begin();
    for(int i = -3; i < 3; i++) {
        ofPushMatrix();
        
        ofTranslate(i*300, 0, 0);
        ofSetColor(0,255,200);
        ofDrawGridPlane(1000);
        
        ofPopMatrix();
        
        
        ofPushMatrix();
        
        ofTranslate(0, i*300, 0);
        ofRotate(90, 0, 0, -1);
        ofSetColor(0,255,200);
        ofDrawGridPlane(1000);
        
        ofRotate(90, 0, 0, -1);
        
        ofPopMatrix();
    }
    
    
    for(int i = 0; i < boxCount; i++) {
        ofPushMatrix();
        float t = (ofGetElapsedTimef() + i * spacing) * movementSpeed;
        ofVec3f pos(
                    ofSignedNoise(t, 0, 0),
                    ofSignedNoise(0, t, 0),
                    ofSignedNoise(0, 0, t));
        
        float boxSize = maxBoxSize + magnitude[i]*5;
        
        pos *= cloudSize;
        ofTranslate(pos);
        ofRotateX(pos.x*.5);
        ofRotateY(pos.y*.5);
        ofRotateZ(pos.z*.5);
        
        ofFill();
        float temp = 50 + magnitude[i]*1000;
        ofSetColor(255);
        ofDrawBox(boxSize);
        
        ofPopMatrix();
    }

    cam[0].end();
    
    
// *****    sounds  *****
    
    //FFT解析した結果をもとに、グラフを生成
    float w = (float)ofGetWidth()/ (float)fft_size / 2.0f;
    for (int i = 0; i < fft_size; i++) {
        
        //塗りのアルファ値でFFT解析結果を表現
        ofColor col;
        col.setHsb(i * 255.0f / (float)fft_size, 255, 255, 255);
        ofSetColor(col);
        float h = magnitude[i] * ofGetHeight();
        ofRect(ofGetWidth()/2 - w * i, ofGetHeight()/2, w, h);
        ofRect(ofGetWidth()/2 + w * i, ofGetHeight()/2, w, -h);
    }
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    myGlitch.generateFx();
    ofSetColor(255);
    myFbo.draw(0, 0);

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    if (key == '1') myGlitch.setFx(OFXPOSTGLITCH_CONVERGENCE	, true);
    if (key == '2') myGlitch.setFx(OFXPOSTGLITCH_GLOW			, true);
    if (key == '3') myGlitch.setFx(OFXPOSTGLITCH_SHAKER			, true);
    if (key == '4') myGlitch.setFx(OFXPOSTGLITCH_CUTSLIDER		, true);
    if (key == '5') myGlitch.setFx(OFXPOSTGLITCH_TWIST			, true);
    if (key == '6')myGlitch.setFx(OFXPOSTGLITCH_INVERT			, true);
    if (key == '7') myGlitch.setFx(OFXPOSTGLITCH_NOISE			, true);
    if (key == '8') myGlitch.setFx(OFXPOSTGLITCH_SLITSCAN		, true);
    if (key == '9') myGlitch.setFx(OFXPOSTGLITCH_SWELL			, true);
    if (key == 'q') myGlitch.setFx(OFXPOSTGLITCH_CR_HIGHCONTRAST, true);
    if (key == 'w') myGlitch.setFx(OFXPOSTGLITCH_CR_BLUERAISE	, true);
    if (key == 'e') myGlitch.setFx(OFXPOSTGLITCH_CR_REDRAISE	, true);
    if (key == 'r') myGlitch.setFx(OFXPOSTGLITCH_CR_GREENRAISE	, true);
    if (key == 't') myGlitch.setFx(OFXPOSTGLITCH_CR_BLUEINVERT	, true);
    if (key == 'y') myGlitch.setFx(OFXPOSTGLITCH_CR_REDINVERT	, true);
    if (key == 'u') myGlitch.setFx(OFXPOSTGLITCH_CR_GREENINVERT	, true);


}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
    if (key == '1') myGlitch.setFx(OFXPOSTGLITCH_CONVERGENCE	, false);
    if (key == '2') myGlitch.setFx(OFXPOSTGLITCH_GLOW			, false);
    if (key == '3') myGlitch.setFx(OFXPOSTGLITCH_SHAKER			, false);
    if (key == '4') myGlitch.setFx(OFXPOSTGLITCH_CUTSLIDER		, false);
    if (key == '5') myGlitch.setFx(OFXPOSTGLITCH_TWIST			, false);
    if (key == '6') myGlitch.setFx(OFXPOSTGLITCH_INVERT			, false);
    if (key == '7') myGlitch.setFx(OFXPOSTGLITCH_NOISE			, false);
    if (key == '8') myGlitch.setFx(OFXPOSTGLITCH_SLITSCAN		, false);
    if (key == '9') myGlitch.setFx(OFXPOSTGLITCH_SWELL			, false);
    if (key == 'q') myGlitch.setFx(OFXPOSTGLITCH_CR_HIGHCONTRAST, false);
    if (key == 'w') myGlitch.setFx(OFXPOSTGLITCH_CR_BLUERAISE	, false);
    if (key == 'e') myGlitch.setFx(OFXPOSTGLITCH_CR_REDRAISE	, false);
    if (key == 'r') myGlitch.setFx(OFXPOSTGLITCH_CR_GREENRAISE	, false);
    if (key == 't') myGlitch.setFx(OFXPOSTGLITCH_CR_BLUEINVERT	, false);
    if (key == 'y') myGlitch.setFx(OFXPOSTGLITCH_CR_REDINVERT	, false);
    if (key == 'u') myGlitch.setFx(OFXPOSTGLITCH_CR_GREENINVERT	, false);
    
}
//--------------------------------------------------------------
void ofApp::audioIn(float* _input, int bufferSize, int nChannels) {
    
    input = _input;
    
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
