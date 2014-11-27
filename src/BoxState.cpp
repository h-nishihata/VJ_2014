#include "BoxState.h"

void BoxState::setup(){
    
    ofSetVerticalSync(true);
    ofEnableDepthTest();
    
    ofBackground(0, 0, 0);
    
    myFbo.allocate(1024, 768, GL_RGBA);
    myGlitch.setup(&myFbo);
    
    myFbo.begin();
    ofClear(255, 255, 255, 0);
    myFbo.end();
    
    
    // nodes
    testNodes[0].setOrientation(ofVec3f(30,0,40));
    testNodes[1].setOrientation(ofVec3f(40,0,30));
    
    
    // cameras
    camToView = 0;
    cam[0].setPosition(40, 40, 500);
    cam[1].setPosition(40, 300, 100);
    lookatIndex[1] = kNumTestNodes-1;
    
    
    // lights
    //    light.enable();
    //    light.setSpotlight();
    //    light.setPosition(50, 80, 150);
    //    light.lookAt(testNodes[0]);
    //
    //    light.setAmbientColor(ofFloatColor(0.7, 0.6, 0.8));
    //    light.setDiffuseColor(ofFloatColor(0.9, 0.6, 0.8));
    //    light.setSpecularColor(ofFloatColor(1.0, 1.0, 1.0));
    
}

//--------------------------------------------------------------
void BoxState::update(){
    
    ofEnableAlphaBlending();
    
    myFbo.begin();
    
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
    //    cam[1].move(-xAxis, -yAxis, -zAxis);
    
    
    drawFboTest();
    myFbo.end();
    
}

//--------------------------------------------------------------
void BoxState::drawFboTest(){
    
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
    
    cam[camToView].begin();
    
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
        
        ofNoFill();
        ofSetColor(255);
        ofSetLineWidth(5);
        ofDrawBox(boxSize * 1.1f);
        
        ofFill();
        ofSetColor(0, 255, 200, 180);
        //        ofSetColor(ofColor::fromHsb(sinf(t) * 128 + 128, 255, 255));
        ofDrawBox(boxSize * 1.1f);
        
        ofPopMatrix();
    }
    
    cam[camToView].end();
    
}

//--------------------------------------------------------------
void BoxState::draw(){
    
    myGlitch.setFx(OFXPOSTGLITCH_SLITSCAN, ofGetKeyPressed('e'));
    ofSetColor(255, 255, 255);
    myGlitch.generateFx();
    myFbo.draw(0, 0);
    
    if (ofGetKeyPressed('a')) {
        camToView = 0;
    }
    else if(ofGetKeyPressed('s')){
        camToView = 1;
    }
    
}

//--------------------------------------------------------------
string BoxState::getName(){
    return "box";
}