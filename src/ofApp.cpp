#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetFrameRate(60);
    ofSetVerticalSync(true);
    ofEnableDepthTest();
    
    ofBackground(0);
    
    //buffers
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
    cam[0].setPosition(40, 40, 800);
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
    
    
    // boxes
    for (int s=0; s<numBoxes; s++) {
        pos[s] = ofVec3f(ofRandom(500)-250, ofRandom(500)-250, ofRandom(500)-250);
        box[s].setPosition(pos[s]);
        box[s].rotate(ofRandom(360), 1, 1, 1);
        box[s].set(ofRandom(100));
        vel[s] = ofVec3f(ofRandom(-1,1), ofRandom(-1,1), ofRandom(-1,1));
    }
    
    
    // waves
    width = 200;
    height = 200;
    for (int y = 0; y < height; y++){
        for (int x = 0; x<width; x++){
    //            mainMesh.addVertex(ofPoint(x,y,0));
            mainMesh.addColor(ofFloatColor(1.0,0,0));
        }
    }
    for (int y = 0; y<height-1; y++){
        for (int x=0; x<width-1; x++){
            mainMesh.addIndex(x+y*width);
            mainMesh.addIndex((x+1)+y*width);
            mainMesh.addIndex(x+(y+1)*width);
            
            mainMesh.addIndex((x+1)+y*width);
            mainMesh.addIndex((x+1)+(y+1)*width);
            mainMesh.addIndex(x+(y+1)*width);
        }
    }
    extrusionAmount = 300.0;
    
    
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
    
    
    // *****    SCENE 1: BOX  *****
    
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
    
    mainMesh.clearVertices();
    for (int i = 0; i<width; i++){
        for (int j=0; j<height; j++){
            float x = sin(i*0.1 + ofGetElapsedTimef())*10.0;
            float y = sin(j*0.15 + ofGetElapsedTimef())*10.0;
            float z = x + y;
            mainMesh.addVertex(ofVec3f(i - width/2, j - height/2, z));
        }
    }
    
    myFbo_01.begin();
    
//    cam[1].move(-xAxis, -yAxis, -zAxis);
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
    
    ofEnableAlphaBlending();
    ofClear(255, 255, 255, 0);
    ofBackground(0+col, 0+col, 0+col);
    
    for (int i=0; i<kNumCameras; i++) {
        if (lookatIndex[i] >= 0) {
            cam[i].lookAt(testNodes[lookatIndex[i]]);
        }
    }
    
    
    cam[0].begin();
    for(int i = 0; i < numBoxes; i++) {
        
        box[i].setPosition(pos[i]);
        pos[i] += vel[i];
        
        int temp = 50 + magnitudeL[i]*1000;
        ofSetColor(ofColor::fromHsb(temp*.9, temp*.3, 60+temp));
        
        ofVec3f gravity = ofVec3f(0,0,0) - pos[i];
        if(gravity.length() > 300) {
            gravity.normalize();
            vel[i] += gravity/10;
        }

        pos[i].x += ofRandom(-magnitudeL[i]*10, magnitudeL[i]*10);
        pos[i].y += ofRandom(-magnitudeL[i]*10, magnitudeL[i]*10);
        pos[i].z += ofRandom(-magnitudeL[i]*10, magnitudeL[i]*10);
        
        box[i].draw();
        
    }
    cam[0].end();
    
}

//--------------------------------------------------------------
void ofApp::drawWave(){
    
    cam[1].begin();
    
    ofEnableAlphaBlending();
    ofClear(255, 255, 255, 0);
    mainMesh.drawFaces();
    
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
        case 'c':
            if (col < 255) {
                col += 5;
            }
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