#pragma once

#include "ofMain.h"
#include "ofxPostGlitch.h"
#include "fft.h"

#define kNumTestNodes 3
#define kNumCameras 3

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

        void drawFboTest_00();
        void drawFboTest_01();
        void drawFboTest_02();
        void audioIn(float* input, int bufferSize, int nChannels);

//  buffers
        ofFbo myFbo_00;
        ofFbo myFbo_01;
        ofFbo myFbo_02;
        int fadeAmnt;
        int switchScene;
//  nodes
        ofNode testNodes[kNumTestNodes];
        int lookatIndex[kNumCameras];
//  cameras
        ofCamera cam[kNumCameras];

        float camPosX =  30;
        float camPosY =  0;
        float camPosZ =  40;
        bool xFlag, yFlag, zFlag;
//  light
        ofLight light;
        int r,g,b;
        bool rFlag, gFlag, bFlag;
//  boxes
        ofBoxPrimitive box;
        ofBoxPrimitive bx;
        ofVboMesh	mVboBox;
//  arcs & circles
        int arc, revArc;
        bool triggerArc;
        float x,y;
        int rad = 300;
        bool triggerCircle;
//  strokes
        ofVec3f previous, current;
        deque<ofVec3f> pathVertices;
        ofMesh pathLines;
        int thickness = 10;
//  glitch
        ofxPostGlitch	myGlitch_00;
        ofxPostGlitch	myGlitch_01;
        ofxPostGlitch	myGlitch_02;
    
//        bool flick;
        bool isFinished;

    
// *****    sounds  *****
    
        //左右2chのFFTの入出力の値を確保
        float *input;
        float *magnitude, *phase, *power;
        //    float *magnitudeR, *phaseR, *powerR;
        float avg_power;
        //バッファーサイズとFFTサイズ
        int buffer_size;
        int fft_size;
        
        fft myfft; //FFTクラスのインスタンス
    
};