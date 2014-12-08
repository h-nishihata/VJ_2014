#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetFrameRate(60);
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
    cam[0].setPosition(40, 40, 800);
    cam[1].setPosition(40, 100, -100);
    lookatIndex[1] = kNumTestNodes-1;
    
    
    // lights
    light.enable();
    light.setSpotlight();
    light.setPosition(50, 80, 150);
    light.lookAt(testNodes[0]);
    
    light.setAmbientColor(ofFloatColor(1.0, 1.0, 1.0, 1.0));
    light.setDiffuseColor(ofFloatColor(1.0, 1.0, 1.0));
    light.setSpecularColor(ofFloatColor(1.0, 1.0, 1.0));
    
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    for (int i = 0; i < WIDTH; i++) {
        for (int j = 0; j < HEIGHT; j++) {
            myVerts[j * WIDTH + i].set(i - WIDTH/2, j - HEIGHT/2, 0);
            myColor[j * WIDTH + i].set(0.5, 0.8, 1.0, 1.0);
        }
    }
    vbo.setVertexData(myVerts, NUM_PARTICLES, GL_DYNAMIC_DRAW);
    vbo.setColorData(myColor, NUM_PARTICLES, GL_DYNAMIC_DRAW);
    
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
void ofApp::update() {
    
    ofEnableAlphaBlending();
    
    
    // *****    SCENE 1  *****
    
    myFbo_00.begin();
    
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
    
    drawBoxes();
    myFbo_00.end();
    
    
    // *****    SCENE 2  *****
    
    for (int i = 0; i < WIDTH; i++) {
        for (int j = 0; j < HEIGHT; j++) {
            float x = sin(i * 0.1 + ofGetElapsedTimef()) * 10.0;
            float y = sin(j * 0.15 + ofGetElapsedTimef()) * 10.0;
            float z = x+y+magnitude[j]*100;
            myVerts[j * WIDTH + i] = ofVec3f(i - WIDTH/2, j - HEIGHT/2, z);
        }
    }
    vbo.updateVertexData(myVerts, NUM_PARTICLES);
    
    myFbo_01.begin();
    
//    cam[1].move(-xAxis, -yAxis, -zAxis);
    
    drawWave();
    myFbo_01.end();
    
    
    // *****    sounds  *****
    
    //オーディオ入力をFFT解析 (左右2ch)
    avg_power = 0.0;
//    avg_powerR = 0.0;
    myfft.powerSpectrum(0, fft_size, input, buffer_size, magnitude, phase, power, &avg_power);
//    myfft.powerSpectrum(0, fft_size, right, buffer_size, magnitudeR, phaseR, powerR, &avg_powerR);
    
}

//--------------------------------------------------------------
void ofApp::drawBoxes(){
    
    float movementSpeed = .3;
    float cloudSize = ofGetWidth()*2;
    float maxBoxSize = 30;
    float spacing = 50;
    int boxCount = 30;
    
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

        float boxSize = maxBoxSize + magnitude[i]*500;
        
        pos *= cloudSize;
        ofTranslate(pos);
        ofRotateX(pos.x);
        ofRotateY(pos.y);
        ofRotateZ(pos.z);
        
        ofFill();
        float temp = 50 + magnitude[i]*1000;
        ofSetColor(ofColor::fromHsb(temp*.9, temp*.3, 60+temp));
        ofDrawBox(boxSize + magnitude[i]*10);
        
        ofPopMatrix();
    }
    cam[0].end();
    
}

//--------------------------------------------------------------
void ofApp::drawWave(){
    
    
    ofEnableAlphaBlending();
    ofClear(255, 255, 255, 0);

    
    cam[1].begin();
    ofRotateX(90);
    vbo.draw(GL_POINTS, 0, NUM_PARTICLES);
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
void ofApp::audioIn(float* _input, int bufferSize, int nChannels) {
    
    input = _input;
    
}