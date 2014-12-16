#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetVerticalSync(true);
    ofSetFrameRate(60);
    ofEnableDepthTest();
    
    myFbo.allocate(1440, 900, GL_RGBA);
    myFbo_.allocate(1440, 900, GL_RGBA);
    myGlitch.setup(&myFbo);
    myGlitch_.setup(&myFbo_);
    
    myFbo.begin();
        ofClear(255, 255, 255, 0);
    myFbo.end();
    
    myFbo_.begin();
        ofClear(255, 255, 255, 0);
    myFbo_.end();
    
    
    // nodes
    testNodes[0].setOrientation(ofVec3f(30,0,40));
    testNodes[1].setOrientation(ofVec3f(30,0,40));
    
    
    // cameras
    cam[0].setPosition(40, 40, 800);
    cam[1].setPosition(-200, 100, 200);
    lookatIndex[1] = kNumTestNodes-1;
    
    
    // lights
    light.enable();
    light.setSpotlight();
    light.setPosition(50, 80, 150);
    light.lookAt(testNodes[0]);
    
    light.setAmbientColor(ofColor(255, 100, 180, 255));
    light.setDiffuseColor(ofColor(255, 100, 190));
    light.setSpecularColor(ofColor(255, 255, 255));

    
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
    
    if (xFlag == false) {
        camPosX -= (1 + magnitude[20]*120);
        if (cam[0].getX() < -1000) {
            xFlag = true;
        }
    }else if (xFlag == true) {
        camPosX += (1 + magnitude[20]*120);
        if (cam[0].getX() > 1000) {
            xFlag = false;
        }
    }
    
    if (yFlag == false) {
        camPosY -= (1 + magnitude[20]*120);
        if (cam[0].getY() < -1000) {
            yFlag = true;
        }
    }else if (yFlag == true) {
        camPosY += (1 + magnitude[20]*120);
        if (cam[0].getY() > 1000) {
            yFlag = false;
        }
    }
    
    if (zFlag == false) {
        camPosZ -= (1 + magnitude[20]*120);
        if (cam[0].getZ() < -1000) {
            zFlag = true;
        }
    }else if (zFlag == true) {
        camPosZ += (1 + magnitude[20]*120);
        if (cam[0].getZ() > 1000) {
            zFlag = false;
        }
    }
    
    cam[0].setPosition(camPosX, camPosY, camPosZ);
    cam[1].setPosition(camPosX, camPosY, camPosZ);
    
    light.setAmbientColor(ofColor(r, g, b, 255));
    light.setDiffuseColor(ofColor(r, g, b));

    if (ofGetKeyPressed('z')){
        if (rFlag == false) {
            r --;
            if (r <= 0) {
                rFlag = true;
            }
        }else if (rFlag == true) {
            r ++;
            if (r >= 255) {
                rFlag = false;
            }
        }
    }
    
    if (ofGetKeyPressed('x')) {
        if (gFlag == false) {
            g --;
            if (g <= 0) {
                gFlag = true;
            }
        }else if (gFlag == true) {
            g ++;
            if (g >= 255) {
                gFlag = false;
            }
        }
    }
    
    if (ofGetKeyPressed('c')){
        if (bFlag == false) {
            b --;
            if (b <= 0) {
                bFlag = true;
            }
        }else if (bFlag == true) {
            b ++;
            if (b >= 255) {
                bFlag = false;
            }
        }
    }
    if (isFinished == true) {
        if (r > 0) r--;
        if (g > 0) g--;
        if (b > 0) b--;
    }
    
    
    if (arc < 360) {
        arc+=20;
    }else{
        arc = 0;
        triggerArc = false;
    }
    if (revArc > -360) {
        revArc-=20;
    }else{
        revArc = 0;
        triggerArc = false;
    }
    
    
    myFbo_.begin();
        drawFboTest_();
    myFbo_.end();
    
    myFbo.begin();
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
    float minBoxSize = 50;
    float spacing = 50;
    int boxCount = 30;
    
    
    ofEnableAlphaBlending();
    ofClear(255, 255, 255, 0);
    ofBackgroundGradient(ofColor(85, 78, 68), ofColor(0,0,255), OF_GRADIENT_LINEAR);
    
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
        
        float boxSize = minBoxSize + magnitude[i]*10;
        
        pos *= cloudSize;
        ofTranslate(pos);
        ofRotateX(pos.x*.8);
        ofRotateY(pos.y*.8);
        ofRotateZ(pos.z*.8);
        
        ofFill();
        ofSetColor(255);
        ofDrawBox(boxSize);
        
        ofPopMatrix();
    }

    cam[0].end();
    
    
// waves
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
void ofApp::drawFboTest_(){

    ofEnableAlphaBlending();
    ofClear(255, 255, 255, 0);
    ofBackgroundGradient(ofColor(85, 78, 68), ofColor(0,0,255), OF_GRADIENT_LINEAR);
    
    cam[1].begin();
    for (int i = 0; i < 22; i++) {
        for (int j = 0; j < 22; j++) {
            box[j].set(20, magnitude[i]*500, 20);
            box[j].setPosition(i*80, 0, j*80);
            box[j].drawWireframe();
        }
    }
    cam[1].end();


//  arcs
    if (magnitude[20] > 1.0) {
        triggerArc = true;
    }
    if (triggerArc) {
        ofPushMatrix();
            ofTranslate(ofGetWidth()/2, ofGetHeight()/2);
            ofBeginShape();
                ofSetColor(r,g,b,180);
                ofVertex(0, 0);
                for (int i = 30; i < 30+arc; i++){
                    ofVertex(sin(i/180.0f*PI)*20,
                             cos(i/180.0f*PI)*20);
                }
            ofEndShape();
            ofBeginShape();
                ofSetColor(255,0,0,0);
                ofCircle(0, 0, 150);

                ofSetColor(r,g,b,180);
                ofVertex(0, 0);
                for (int i = 0; i > revArc; i--){
                    ofVertex(sin(i/180.0f*PI)*200,
                             cos(i/180.0f*PI)*200);
                }
            ofEndShape();
            ofBeginShape();
                ofSetColor(255,0,0,0);
                ofCircle(0, 0, 280);
        
                ofSetColor(r,g,b,180);
                ofVertex(0, 0);
                for (int i = 260; i < 260+arc; i++){
                    ofVertex(sin(i/180.0f*PI)*300,
                             cos(i/180.0f*PI)*300);
                }
            ofEndShape(true);
        ofPopMatrix();
    }

}

//--------------------------------------------------------------
void ofApp::draw(){
    
    myGlitch.generateFx();
    myGlitch_.generateFx();
    ofSetColor(255);
    
    if (switchScene == 0) {
        myFbo.draw(0, 0);
    }else if (switchScene == 1) {
       myFbo_.draw(0, 0);
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    if (key == '1'){
        myGlitch.setFx(OFXPOSTGLITCH_CONVERGENCE, true);
        myGlitch_.setFx(OFXPOSTGLITCH_CONVERGENCE, true);
    }
    if (key == '2'){
        myGlitch.setFx(OFXPOSTGLITCH_GLOW, true);
        myGlitch_.setFx(OFXPOSTGLITCH_GLOW, true);
    }
    if (key == '3'){
        myGlitch.setFx(OFXPOSTGLITCH_SHAKER, true);
        myGlitch_.setFx(OFXPOSTGLITCH_SHAKER, true);
    }
    if (key == '4'){
        myGlitch.setFx(OFXPOSTGLITCH_CUTSLIDER, true);
        myGlitch_.setFx(OFXPOSTGLITCH_CUTSLIDER, true);
    }
    if (key == '5'){
        myGlitch.setFx(OFXPOSTGLITCH_TWIST, true);
        myGlitch_.setFx(OFXPOSTGLITCH_TWIST, true);
    }
    if (key == '6'){
        myGlitch.setFx(OFXPOSTGLITCH_INVERT, true);
        myGlitch_.setFx(OFXPOSTGLITCH_INVERT, true);
    }
    if (key == '7'){
        myGlitch.setFx(OFXPOSTGLITCH_NOISE, true);
        myGlitch_.setFx(OFXPOSTGLITCH_NOISE, true);
    }
    if (key == '8'){
        myGlitch.setFx(OFXPOSTGLITCH_SLITSCAN, true);
        myGlitch_.setFx(OFXPOSTGLITCH_SLITSCAN, true);
    }
    if (key == '9'){
        myGlitch.setFx(OFXPOSTGLITCH_SWELL, true);
        myGlitch_.setFx(OFXPOSTGLITCH_SWELL, true);
    }
    if (key == 'q'){
        myGlitch.setFx(OFXPOSTGLITCH_CR_HIGHCONTRAST, true);
        myGlitch_.setFx(OFXPOSTGLITCH_CR_HIGHCONTRAST, true);
    }
    if (key == 'w'){
        myGlitch.setFx(OFXPOSTGLITCH_CR_BLUERAISE, true);
        myGlitch_.setFx(OFXPOSTGLITCH_CR_BLUERAISE, true);
    }
    if (key == 'e'){
        myGlitch.setFx(OFXPOSTGLITCH_CR_REDRAISE, true);
        myGlitch_.setFx(OFXPOSTGLITCH_CR_REDRAISE, true);
    }
    if (key == 'r'){
        myGlitch.setFx(OFXPOSTGLITCH_CR_GREENRAISE, true);
        myGlitch_.setFx(OFXPOSTGLITCH_CR_GREENRAISE, true);
    }
    if (key == 't'){
        myGlitch.setFx(OFXPOSTGLITCH_CR_BLUEINVERT, true);
        myGlitch_.setFx(OFXPOSTGLITCH_CR_BLUEINVERT, true);
    }
    if (key == 'y'){
        myGlitch.setFx(OFXPOSTGLITCH_CR_REDINVERT, true);
        myGlitch_.setFx(OFXPOSTGLITCH_CR_REDINVERT,	true);
    }
    if (key == 'u'){
        myGlitch.setFx(OFXPOSTGLITCH_CR_GREENINVERT	, true);
        myGlitch_.setFx(OFXPOSTGLITCH_CR_GREENINVERT, true);
    }
    if (key == '.') switchScene = 0;
    if (key == '/') switchScene = 1;
    if (key == '=') isFinished = true;
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
    if (key == '1'){
        myGlitch.setFx(OFXPOSTGLITCH_CONVERGENCE, false);
        myGlitch_.setFx(OFXPOSTGLITCH_CONVERGENCE, false);
    }
    if (key == '2'){
        myGlitch.setFx(OFXPOSTGLITCH_GLOW, false);
        myGlitch_.setFx(OFXPOSTGLITCH_GLOW, false);
    }
    if (key == '3'){
        myGlitch.setFx(OFXPOSTGLITCH_SHAKER, false);
        myGlitch_.setFx(OFXPOSTGLITCH_SHAKER, false);
    }
    if (key == '4'){
        myGlitch.setFx(OFXPOSTGLITCH_CUTSLIDER, false);
        myGlitch_.setFx(OFXPOSTGLITCH_CUTSLIDER, false);
    }
    if (key == '5'){
        myGlitch.setFx(OFXPOSTGLITCH_TWIST, false);
        myGlitch_.setFx(OFXPOSTGLITCH_TWIST, false);
    }
    if (key == '6'){
        myGlitch.setFx(OFXPOSTGLITCH_INVERT, false);
        myGlitch_.setFx(OFXPOSTGLITCH_INVERT, false);
    }
    if (key == '7'){
        myGlitch.setFx(OFXPOSTGLITCH_NOISE, false);
        myGlitch_.setFx(OFXPOSTGLITCH_NOISE, false);
    }
    if (key == '8'){
        myGlitch.setFx(OFXPOSTGLITCH_SLITSCAN, false);
        myGlitch_.setFx(OFXPOSTGLITCH_SLITSCAN, false);
    }
    if (key == '9'){
        myGlitch.setFx(OFXPOSTGLITCH_SWELL, false);
        myGlitch_.setFx(OFXPOSTGLITCH_SWELL, false);
    }
    if (key == 'q'){
        myGlitch.setFx(OFXPOSTGLITCH_CR_HIGHCONTRAST, false);
        myGlitch_.setFx(OFXPOSTGLITCH_CR_HIGHCONTRAST, false);
    }
    if (key == 'w'){
        myGlitch.setFx(OFXPOSTGLITCH_CR_BLUERAISE, false);
        myGlitch_.setFx(OFXPOSTGLITCH_CR_BLUERAISE, false);
    }
    if (key == 'e'){
        myGlitch.setFx(OFXPOSTGLITCH_CR_REDRAISE, false);
        myGlitch_.setFx(OFXPOSTGLITCH_CR_REDRAISE, false);
    }
    if (key == 'r'){
        myGlitch.setFx(OFXPOSTGLITCH_CR_GREENRAISE, false);
        myGlitch_.setFx(OFXPOSTGLITCH_CR_GREENRAISE, false);
    }
    if (key == 't'){
        myGlitch.setFx(OFXPOSTGLITCH_CR_BLUEINVERT, false);
        myGlitch_.setFx(OFXPOSTGLITCH_CR_BLUEINVERT, false);
    }
    if (key == 'y'){
        myGlitch.setFx(OFXPOSTGLITCH_CR_REDINVERT, false);
        myGlitch_.setFx(OFXPOSTGLITCH_CR_REDINVERT,	false);
    }
    if (key == 'u'){
        myGlitch.setFx(OFXPOSTGLITCH_CR_GREENINVERT	, false);
        myGlitch_.setFx(OFXPOSTGLITCH_CR_GREENINVERT, false);
    }
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
