#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetVerticalSync(true);
    ofSetFrameRate(60);
    ofEnableDepthTest();
    
    myFbo_00.allocate(1920, 1200, GL_RGBA);
    myFbo_01.allocate(1920, 1200, GL_RGBA);
    myFbo_02.allocate(1920, 1200, GL_RGBA);
    myGlitch_00.setup(&myFbo_00);
    myGlitch_01.setup(&myFbo_01);
    myGlitch_02.setup(&myFbo_02);
    
    myFbo_00.begin();
        ofClear(255, 255, 255, 0);
    myFbo_00.end();
    
    myFbo_01.begin();
        ofClear(255, 255, 255, 0);
    myFbo_01.end();
    
    myFbo_02.begin();
        ofClear(255, 255, 255, 0);
    myFbo_02.end();
    
    
// nodes
    testNodes[0].setOrientation(ofVec3f(30,0,40));
    testNodes[1].setOrientation(ofVec3f(30,0,40));
    testNodes[2].setOrientation(current);
    
    
// cameras
    cam[0].setPosition(40, 40, 800);
    cam[1].setPosition(0, 100, 100);
    //    lookatIndex[1] = kNumTestNodes-1;
    
    
// lights
    light.enable();
    light.setSpotlight();
    light.setPosition(0, 80, 150);
    light.lookAt(testNodes[0]);
    
    light.setAmbientColor(ofColor(r, g, b, 255));
    light.setDiffuseColor(ofColor(r, g, b));
    light.setSpecularColor(ofColor(255, 255, 255));

    
    pathLines.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);

    
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
    
    
    for (int n=0; n<kNumCameras; n++) {
        cam[n].setPosition(camPosX, camPosY, camPosZ);
    }

    if (ofGetKeyPressed('z')){
        if (rFlag == false) {
            if (r <= 0) {
                rFlag = true;
            }else{
                r -=3;
            }
        }else if (rFlag == true) {
            if (r >= 255) {
                rFlag = false;
            }else{
                r +=3;
            }
        }
    }
    if (ofGetKeyPressed('x')) {
        if (gFlag == false) {
            if (g <= 0) {
                gFlag = true;
            }else{
                g -=3;
            }
        }else if (gFlag == true) {
            if (g >= 255) {
                gFlag = false;
            }else{
                g +=3;
            }
        }
    }
    if (ofGetKeyPressed('c')){
        if (bFlag == false) {
            if (b <= 0) {
                bFlag = true;
            }else{
                b -=3;
            }
        }else if (bFlag == true) {
            if (b >= 255) {
                bFlag = false;
            }else{
                b +=3;
            }
        }
    }
//    if (flick == true) {
//        isFinished = false;
//        if (r < 255) r+=5;
//        if (g < 255) g+=5;
//        if (b < 255) b+=5;
//        if (r >= 255 && g >= 255 && g >=255) {
//            flick = false;
//        }
//    }
    if (isFinished == true) {
//        flick = false;
        if (r > 10) r--;
        if (g > 10) g--;
        if (b > 10) b--;
//        if (r <= 0 && g <= 0 && g <= 0) {
//            isFinished = false;
//        }
    }
    light.setAmbientColor(ofColor(r, g, b, 255));
    light.setDiffuseColor(ofColor(r, g, b));
    

//  arcs
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
    
//  trail
    cam[2].setPosition(current.x, current.y+50, current.z-80);
    previous = current;
    
    float t = (2 + ofGetElapsedTimef()) * .2;
    current.x = ofSignedNoise(t, 0, 0);
    current.y = ofSignedNoise(0, t, 0);
    current.z = ofSignedNoise(0, 0, t);
    current *= 800;
    
    pathVertices.push_back(current);
    while(pathVertices.size() > 200) {
        pathVertices.pop_front();
    }
    
    pathLines.clear();
    for(unsigned int i = 0; i < pathVertices.size(); i++) {
        ofVec3f thisPoint = pathVertices[i];
        ofVec3f nextPoint = pathVertices[i+1];
        ofVec3f direction = (nextPoint - thisPoint);
        float distance = direction.length();
        ofVec3f unitDirection = direction.normalized();
        
        ofVec3f toTheLeft = unitDirection.getRotated(90, ofVec3f(0, 1, 1));
        ofVec3f toTheRight = unitDirection.getRotated(-90, ofVec3f(0, 1, 1));
        
        ofVec3f leftPoint = thisPoint+toTheLeft * thickness;
        ofVec3f rightPoint = thisPoint+toTheRight * thickness;
        pathLines.addVertex(ofVec3f(leftPoint.x, leftPoint.y, leftPoint.z));
        pathLines.addVertex(ofVec3f(rightPoint.x, rightPoint.y, rightPoint.z));
        int n = pathLines.getNumColors();
    }
    
    
    myFbo_00.begin();
        drawFboTest_00();
    myFbo_00.end();
    
    myFbo_01.begin();
        drawFboTest_01();
    myFbo_01.end();

    myFbo_02.begin();
        drawFboTest_02();
    myFbo_02.end();
    
    
// *****    sounds  *****

    avg_power = 0.0;
    myfft.powerSpectrum(0, fft_size, input, buffer_size, magnitude, phase, power, &avg_power);
    
}

//--------------------------------------------------------------
void ofApp::drawFboTest_00(){
    
    float movementSpeed = .3;
    float cloudSize = ofGetWidth();
    float minBoxSize = 50;
    float spacing = 50;
    int boxCount = 30;
    
    
    ofEnableAlphaBlending();
    ofClear(255, 255, 255, 0);
    ofBackgroundGradient(ofColor(0,0,255), ofColor(85, 78, 68), OF_GRADIENT_LINEAR);
    
    
    for (int i=0; i<kNumCameras; i++) {
        if (lookatIndex[i] >= 0) {
            cam[i].lookAt(testNodes[lookatIndex[i]]);
        }
    }
    
    cam[0].begin();
    
//  grids
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
    
//  boxes
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
//        ofColor col;
//        col.setHsb(i * 255.0f / (float)fft_size, 255, 255, 255);
//        ofSetColor(col);
        float h = magnitude[i] * ofGetHeight();
        ofRect(ofGetWidth()/2 - w * i, ofGetHeight()/2, w, h);
        ofRect(ofGetWidth()/2 + w * i, ofGetHeight()/2, w, -h);
    }
}

//--------------------------------------------------------------
void ofApp::drawFboTest_01(){

    ofEnableAlphaBlending();
    ofClear(255, 255, 255, 0);
    ofBackgroundGradient(ofColor(190,50,200), ofColor(85, 78, 68), OF_GRADIENT_LINEAR);
    
    cam[1].begin();

//  boxes
    ofSetLineWidth(1);
    ofSetColor(255);
    for (int i = 0; i < 22; i++) {
        for (int j = 0; j < 22; j++) {
            int height = magnitude[j*22+i]*300;
            box.set(20, height, 20);
            box.setPosition(i*80, 0+height, j*80);
            if (xFlag == true) {
                box.draw();
            }else{
                box.drawWireframe();
            }
        }
    }
    cam[1].end();


//  arcs
    if (magnitude[20] > 0.5 && ofRandom(100)<50) {
        triggerArc = true;
    }
    if (triggerArc) {
        ofPushMatrix();
            ofTranslate(ofGetWidth()/2, ofGetHeight()/2);
            ofBeginShape();
                ofSetColor(r,g,b,100);
                ofVertex(0, 0);
                for (int i = 30; i < 30+arc; i++){
                    ofVertex(sin(i/180.0f*PI)*20,
                             cos(i/180.0f*PI)*20);
                }
            ofEndShape();
            ofBeginShape();
                ofSetColor(255,0,0,0);
                ofCircle(0, 0, 150);

                ofSetColor(r,g,b,100);
                ofVertex(0, 0);
                for (int i = 0; i > revArc; i--){
                    ofVertex(sin(i/180.0f*PI)*200,
                             cos(i/180.0f*PI)*200);
                }
            ofEndShape();
            ofBeginShape();
                ofSetColor(255,0,0,0);
                ofCircle(0, 0, 280);
        
                ofSetColor(r,g,b,100);
                ofVertex(0, 0);
                for (int i = 260; i < 260+arc; i++){
                    ofVertex(sin(i/180.0f*PI)*300,
                             cos(i/180.0f*PI)*300);
                }
            ofEndShape(true);
        ofPopMatrix();
    }
    
    
//  circles
    if (magnitude[20] > 0.8 && ofRandom(100)<50) {
        triggerCircle = true;
    }
    if (triggerCircle) {
        for (int ang = 0; ang <= 360; ang+=20) {
            x = ofGetWidth()/2 + (rad * cos(ang*3.1415926/180));
            y = ofGetHeight()/2 + (rad * sin(ang*3.1415926/180));
            ofSetColor(r,g,b,100);
            ofCircle(x, y, 20);
        }
        
        rad += 20;
        if (rad > 800) {
            rad = 300;
            triggerCircle = false;
        }
    }
    
}

//--------------------------------------------------------------
void ofApp::drawFboTest_02(){

    ofEnableAlphaBlending();
    ofClear(255, 255, 255, 0);
    ofBackgroundGradient(ofColor(190,50,200), ofColor(85, 78, 68), OF_GRADIENT_LINEAR);
    
    cam[2].begin();
    
//  trail
        ofSetLineWidth(2);
        ofSetColor(255);
        pathLines.draw();

//  box
        string str = ofToString(current, 2);
        ofDrawBitmapString(str, current);
        ofTranslate(current);
        ofRotateX(current.x*.5);
        ofRotateY(current.y*.5);
        ofRotateZ(current.z*.5);
        bx.set(10+magnitude[20]*80);
        if (xFlag == true) {
            bx.draw();
        }else{
            bx.drawWireframe();
        }
    
//  frames
    ofNoFill();
    ofSetLineWidth(8);
    ofRect(-15, -15, 30, 30);
    ofRotateZ(10);
    ofSetLineWidth(5);
    ofRect(-25, -25, 50, 50);
    
//  triangles
    if (r > 100 && g > 100 && b > 100) {
        for(int i = 0; i < 100; i++) {
            ofPushMatrix();
            float t = (ofGetElapsedTimef() + i * 50) * .5;
            ofVec3f pos(
                        ofSignedNoise(t, 0, 0),
                        ofSignedNoise(0, t, 0),
                        ofSignedNoise(0, 0, t));
            
            pos *= ofGetWidth();
            ofTranslate(pos);
            ofRotateX(pos.x*.8);
            ofRotateY(pos.y*.8);
            ofRotateZ(pos.z*.8);
            
            ofFill();
            ofSetColor(255);
            ofTriangle(0, 0, 0, 50, 50, 50);
            ofPopMatrix();
        }
    }

    cam[2].end();
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofHideCursor();
    
    ofFill();
    myGlitch_00.generateFx();
    myGlitch_01.generateFx();
    myGlitch_02.generateFx();
    ofSetColor(255);
    
    if (r != 0 && g != 0 && b!= 0) {
    if (switchScene == 0) {
        myFbo_00.draw(0, 0);
    }else if (switchScene == 1) {
       myFbo_01.draw(0, 0);
    }else if (switchScene == 2) {
        myFbo_02.draw(0, 0);
    }
    }

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    if (key == ',') switchScene = 0;
    if (key == '.') switchScene = 1;
    if (key == '/') switchScene = 2;
    
    if (key == '1'){
        myGlitch_00.setFx(OFXPOSTGLITCH_CONVERGENCE, true);
        myGlitch_01.setFx(OFXPOSTGLITCH_CONVERGENCE, true);
        myGlitch_02.setFx(OFXPOSTGLITCH_CONVERGENCE, true);
    }
    if (key == '2'){
        myGlitch_00.setFx(OFXPOSTGLITCH_GLOW, true);
        myGlitch_01.setFx(OFXPOSTGLITCH_GLOW, true);
        myGlitch_02.setFx(OFXPOSTGLITCH_GLOW, true);
    }
    if (key == '3'){
        myGlitch_00.setFx(OFXPOSTGLITCH_SHAKER, true);
        myGlitch_01.setFx(OFXPOSTGLITCH_SHAKER, true);
        myGlitch_02.setFx(OFXPOSTGLITCH_SHAKER, true);
    }
    if (key == '4'){
        myGlitch_00.setFx(OFXPOSTGLITCH_CUTSLIDER, true);
        myGlitch_01.setFx(OFXPOSTGLITCH_CUTSLIDER, true);
        myGlitch_02.setFx(OFXPOSTGLITCH_CUTSLIDER, true);
    }
    if (key == '5'){
        myGlitch_00.setFx(OFXPOSTGLITCH_TWIST, true);
        myGlitch_01.setFx(OFXPOSTGLITCH_TWIST, true);
        myGlitch_02.setFx(OFXPOSTGLITCH_TWIST, true);
    }
    if (key == '6'){
        myGlitch_00.setFx(OFXPOSTGLITCH_INVERT, true);
        myGlitch_01.setFx(OFXPOSTGLITCH_INVERT, true);
        myGlitch_02.setFx(OFXPOSTGLITCH_INVERT, true);
    }
    if (key == '7'){
        myGlitch_00.setFx(OFXPOSTGLITCH_NOISE, true);
        myGlitch_01.setFx(OFXPOSTGLITCH_NOISE, true);
        myGlitch_02.setFx(OFXPOSTGLITCH_NOISE, true);
    }
    if (key == '8'){
        myGlitch_00.setFx(OFXPOSTGLITCH_SLITSCAN, true);
        myGlitch_01.setFx(OFXPOSTGLITCH_SLITSCAN, true);
        myGlitch_02.setFx(OFXPOSTGLITCH_SLITSCAN, true);
    }
    if (key == '9'){
        myGlitch_00.setFx(OFXPOSTGLITCH_SWELL, true);
        myGlitch_01.setFx(OFXPOSTGLITCH_SWELL, true);
        myGlitch_02.setFx(OFXPOSTGLITCH_SWELL, true);
    }
    if (key == 'q'){
        myGlitch_00.setFx(OFXPOSTGLITCH_CR_HIGHCONTRAST, true);
        myGlitch_01.setFx(OFXPOSTGLITCH_CR_HIGHCONTRAST, true);
        myGlitch_02.setFx(OFXPOSTGLITCH_CR_HIGHCONTRAST, true);
    }
    if (key == 'w'){
        myGlitch_00.setFx(OFXPOSTGLITCH_CR_BLUERAISE, true);
        myGlitch_01.setFx(OFXPOSTGLITCH_CR_BLUERAISE, true);
        myGlitch_02.setFx(OFXPOSTGLITCH_CR_BLUERAISE, true);
    }
    if (key == 'e'){
        myGlitch_00.setFx(OFXPOSTGLITCH_CR_REDRAISE, true);
        myGlitch_01.setFx(OFXPOSTGLITCH_CR_REDRAISE, true);
        myGlitch_02.setFx(OFXPOSTGLITCH_CR_REDRAISE, true);
    }
    if (key == 'r'){
        myGlitch_00.setFx(OFXPOSTGLITCH_CR_GREENRAISE, true);
        myGlitch_01.setFx(OFXPOSTGLITCH_CR_GREENRAISE, true);
        myGlitch_02.setFx(OFXPOSTGLITCH_CR_GREENRAISE, true);
    }
    if (key == 't'){
        myGlitch_00.setFx(OFXPOSTGLITCH_CR_BLUEINVERT, true);
        myGlitch_01.setFx(OFXPOSTGLITCH_CR_BLUEINVERT, true);
        myGlitch_02.setFx(OFXPOSTGLITCH_CR_BLUEINVERT, true);
    }
    if (key == 'y'){
        myGlitch_00.setFx(OFXPOSTGLITCH_CR_REDINVERT, true);
        myGlitch_01.setFx(OFXPOSTGLITCH_CR_REDINVERT, true);
        myGlitch_02.setFx(OFXPOSTGLITCH_CR_REDINVERT, true);
    }
    if (key == 'u'){
        myGlitch_00.setFx(OFXPOSTGLITCH_CR_GREENINVERT, true);
        myGlitch_01.setFx(OFXPOSTGLITCH_CR_GREENINVERT, true);
        myGlitch_02.setFx(OFXPOSTGLITCH_CR_GREENINVERT, true);
    }

//    if (key == ']') flick = true;
    if (key == '=') isFinished = true;
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
    if (key == '1'){
        myGlitch_00.setFx(OFXPOSTGLITCH_CONVERGENCE, false);
        myGlitch_01.setFx(OFXPOSTGLITCH_CONVERGENCE, false);
        myGlitch_02.setFx(OFXPOSTGLITCH_CONVERGENCE, false);
    }
    if (key == '2'){
        myGlitch_00.setFx(OFXPOSTGLITCH_GLOW, false);
        myGlitch_01.setFx(OFXPOSTGLITCH_GLOW, false);
        myGlitch_02.setFx(OFXPOSTGLITCH_GLOW, false);
    }
    if (key == '3'){
        myGlitch_00.setFx(OFXPOSTGLITCH_SHAKER, false);
        myGlitch_01.setFx(OFXPOSTGLITCH_SHAKER, false);
        myGlitch_02.setFx(OFXPOSTGLITCH_SHAKER, false);
    }
    if (key == '4'){
        myGlitch_00.setFx(OFXPOSTGLITCH_CUTSLIDER, false);
        myGlitch_01.setFx(OFXPOSTGLITCH_CUTSLIDER, false);
        myGlitch_02.setFx(OFXPOSTGLITCH_CUTSLIDER, false);
    }
    if (key == '5'){
        myGlitch_00.setFx(OFXPOSTGLITCH_TWIST, false);
        myGlitch_01.setFx(OFXPOSTGLITCH_TWIST, false);
        myGlitch_02.setFx(OFXPOSTGLITCH_TWIST, false);
    }
    if (key == '6'){
        myGlitch_00.setFx(OFXPOSTGLITCH_INVERT, false);
        myGlitch_01.setFx(OFXPOSTGLITCH_INVERT, false);
        myGlitch_02.setFx(OFXPOSTGLITCH_INVERT, false);
    }
    if (key == '7'){
        myGlitch_00.setFx(OFXPOSTGLITCH_NOISE, false);
        myGlitch_01.setFx(OFXPOSTGLITCH_NOISE, false);
        myGlitch_02.setFx(OFXPOSTGLITCH_NOISE, false);
    }
    if (key == '8'){
        myGlitch_00.setFx(OFXPOSTGLITCH_SLITSCAN, false);
        myGlitch_01.setFx(OFXPOSTGLITCH_SLITSCAN, false);
        myGlitch_02.setFx(OFXPOSTGLITCH_SLITSCAN, false);
    }
    if (key == '9'){
        myGlitch_00.setFx(OFXPOSTGLITCH_SWELL, false);
        myGlitch_01.setFx(OFXPOSTGLITCH_SWELL, false);
        myGlitch_02.setFx(OFXPOSTGLITCH_SWELL, false);
    }
    if (key == 'q'){
        myGlitch_00.setFx(OFXPOSTGLITCH_CR_HIGHCONTRAST, false);
        myGlitch_01.setFx(OFXPOSTGLITCH_CR_HIGHCONTRAST, false);
        myGlitch_02.setFx(OFXPOSTGLITCH_CR_HIGHCONTRAST, false);
    }
    if (key == 'w'){
        myGlitch_00.setFx(OFXPOSTGLITCH_CR_BLUERAISE, false);
        myGlitch_01.setFx(OFXPOSTGLITCH_CR_BLUERAISE, false);
        myGlitch_02.setFx(OFXPOSTGLITCH_CR_BLUERAISE, false);
    }
    if (key == 'e'){
        myGlitch_00.setFx(OFXPOSTGLITCH_CR_REDRAISE, false);
        myGlitch_01.setFx(OFXPOSTGLITCH_CR_REDRAISE, false);
        myGlitch_02.setFx(OFXPOSTGLITCH_CR_REDRAISE, false);
    }
    if (key == 'r'){
        myGlitch_00.setFx(OFXPOSTGLITCH_CR_GREENRAISE, false);
        myGlitch_01.setFx(OFXPOSTGLITCH_CR_GREENRAISE, false);
        myGlitch_02.setFx(OFXPOSTGLITCH_CR_GREENRAISE, false);
    }
    if (key == 't'){
        myGlitch_00.setFx(OFXPOSTGLITCH_CR_BLUEINVERT, false);
        myGlitch_01.setFx(OFXPOSTGLITCH_CR_BLUEINVERT, false);
        myGlitch_02.setFx(OFXPOSTGLITCH_CR_BLUEINVERT, false);
    }
    if (key == 'y'){
        myGlitch_00.setFx(OFXPOSTGLITCH_CR_REDINVERT, false);
        myGlitch_01.setFx(OFXPOSTGLITCH_CR_REDINVERT, false);
        myGlitch_02.setFx(OFXPOSTGLITCH_CR_REDINVERT, false);
    }
    if (key == 'u'){
        myGlitch_00.setFx(OFXPOSTGLITCH_CR_GREENINVERT, false);
        myGlitch_01.setFx(OFXPOSTGLITCH_CR_GREENINVERT, false);
        myGlitch_02.setFx(OFXPOSTGLITCH_CR_GREENINVERT, false);
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
