#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetVerticalSync(true);
    ofEnableDepthTest();
    
    ofBackground(0, 0, 0);
    
    myFbo_00.allocate(1024, 768, GL_RGBA);
    myFbo_01.allocate(1024, 768, GL_RGBA);
    myGlitch_00.setup(&myFbo_00);
    myGlitch_01.setup(&myFbo_01);
    
    myFbo_00.begin();
    ofClear(255, 255, 255, 0);
    myFbo_00.end();
    
    myFbo_01.begin();
    ofClear(255, 255, 255, 0);
    myFbo_01.end();
    
    
    // nodes
    testNodes[0].setOrientation(ofVec3f(30,0,40));
    testNodes[1].setOrientation(ofVec3f(40,0,30));
    
    
    // cameras
    camToView = 0;
    cam[0].setPosition(40, 40, 500);
    cam[1].setPosition(40, 300, 100);
    lookatIndex[1] = kNumTestNodes-1;
    
    
    // lights
    light.enable();
    light.setSpotlight();
    light.setPosition(50, 80, 150);
    light.lookAt(testNodes[0]);
    
    light.setAmbientColor(ofFloatColor(1.0, 1.0, 1.0, 1.0));
    light.setDiffuseColor(ofFloatColor(1.0, 1.0, 1.0));
    light.setSpecularColor(ofFloatColor(1.0, 1.0, 1.0));
    
    
    // *****    sounds  *****
    
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
void ofApp::update() {
    
    ofEnableAlphaBlending();
    
    
    // *****    SCENE 1  *****
    
    myFbo_00.begin();
    
    if (xFlag == false) {
        xAxis = -10;
        if (cam[0].getX() < -300) {
            xFlag = true;
        }
    }else if (xFlag == true) {
        xAxis = 10;
        if (cam[0].getX() > 300) {
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
    
    if (zFlag == false) {
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
    
    drawBoxes();
    myFbo_00.end();
    
    
    // *****    SCENE 2  *****
    
    myFbo_01.begin();
    
    cam[1].move(-xAxis, -yAxis, -zAxis);
    
    drawWave();
    myFbo_01.end();
    
    
    // *****    sounds  *****
    
    //オーディオ入力をFFT解析 (左右2ch)
    avg_powerL = 0.0;
    avg_powerR = 0.0;
    myfft.powerSpectrum(0, fft_size, left, buffer_size,	magnitudeL, phaseL, powerL, &avg_powerL);
    myfft.powerSpectrum(0, fft_size, right, buffer_size, magnitudeR, phaseR, powerR, &avg_powerR);
    
}

//--------------------------------------------------------------
void ofApp::drawBoxes(){
    
    float movementSpeed = .1;
    float cloudSize = ofGetWidth() / 2;
    float maxBoxSize = 100;
    float spacing = 1;
    int boxCount = 100;
    
    ofEnableAlphaBlending();
    ofClear(255, 255, 255, 0);
    
    for (int i=0; i<kNumCameras; i++) {
        if (lookatIndex[i] >= 0) {
            cam[i].lookAt(testNodes[lookatIndex[i]]);
        }
    }
    
    cam[0].begin();
    
    for(int i = 0; i < boxCount; i++) {
        ofPushMatrix();
        
        float t = (ofGetElapsedTimef() + i * spacing) * movementSpeed;
        ofVec3f pos(
                    ofSignedNoise(t, 0, 0),
                    ofSignedNoise(0, t, 0),
                    ofSignedNoise(0, 0, t));
        
        float boxSize = maxBoxSize * ofNoise(pos.x, pos.y, pos.z);
        
        pos *= cloudSize;
        ofTranslate(pos);
        ofRotateX(pos.x);
        ofRotateY(pos.y);
        ofRotateZ(pos.z);
        
        //        ofNoFill();
        //        ofSetColor(255);
        //        ofSetLineWidth(5);
        //        ofDrawBox(boxSize * 1.1f);
        
        ofFill();
        ofSetColor(0, 200, 200);
        //        ofSetColor(ofColor::fromHsb(sinf(t) * 128 + 128, 255, 255));
        ofDrawBox(boxSize * 1.1f);
        
        ofPopMatrix();
    }
    
    cam[0].end();
    
}

//--------------------------------------------------------------
void ofApp::drawWave(){
    
    cam[1].begin();
    
    ofEnableAlphaBlending();
    ofClear(255, 255, 255, 0);
    ofSetColor(255);
    s.set(100,16);
    s.setPosition(300, 0, 40);
    s.draw();
    
    cam[1].end();
}

//--------------------------------------------------------------
void ofApp::draw() {
    
    myGlitch_00.generateFx();
    myGlitch_01.generateFx();
    ofSetColor(255, 255, 255);
    
    if (switchScene == 0) {
        myFbo_00.draw(0, 0);
    }
    else if(switchScene == 1){
        myFbo_01.draw(0,0);
    }
    
    // *****    sounds  *****
    
    /*
     //FFT解析した結果をもとに、グラフを生成
     float w = (float)ofGetWidth()/ (float)fft_size / 2.0f;
     for (int i = 0; i < fft_size; i++) {
     
     //塗りのアルファ値でFFT解析結果を表現
     ofColor col;
     col.setHsb(i * 255.0f / (float)fft_size, 255, 255, 31);
     ofSetColor(col);
     
     ofCircle(ofGetWidth()/2 - w * i, ofGetHeight()/2, magnitudeL[i] * ofGetWidth()/100.0); //左
     ofCircle(ofGetWidth()/2 + w * i, ofGetHeight()/2, magnitudeR[i] * ofGetWidth()/100.0); //右
     }
     */
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    switch (key) {
        case 'e':
            myGlitch_00.setFx(OFXPOSTGLITCH_SLITSCAN, true);
            myGlitch_01.setFx(OFXPOSTGLITCH_TWIST, true);
            break;
        case '1':
            switchScene = 0;
            break;
        case '2':
            switchScene = 1;
            break;
        default:
            break;
    }
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
    if (key == 'e'){
        myGlitch_00.setFx(OFXPOSTGLITCH_SLITSCAN, false);
        myGlitch_01.setFx(OFXPOSTGLITCH_TWIST, false);
    }
    
}

//--------------------------------------------------------------
void ofApp::audioIn(float* input, int bufferSize, int nChannels) {
    
    //入力信号(input)を左右2ch(left, right)に分ける
    for (int i = 0; i < bufferSize; i++){
        left[i]		= input[i*2];
        right[i]	= input[i*2+1];
    }
}