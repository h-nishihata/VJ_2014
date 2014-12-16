#pragma once

#include "ofMain.h"
#include "ofxPostGlitch.h"
#include "fft.h"

#define kNumTestNodes 2
#define kNumCameras 2

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

        void drawFboTest();
        void drawFboTest_();
        void audioIn(float* input, int bufferSize, int nChannels);

    
        ofFbo myFbo;
        ofFbo myFbo_;
        int fadeAmnt;
        int switchScene;
    
        ofNode testNodes[kNumTestNodes];
        int lookatIndex[kNumCameras];

        ofCamera cam[kNumCameras];

        float camPosX =  30;
        float camPosY =  0;
        float camPosZ =  40;
        bool xFlag, yFlag, zFlag;

        ofLight light;
        bool rFlag, gFlag, bFlag = true;
        int r,g,b;
    
        ofBoxPrimitive box[512];
        ofVboMesh	mVboBox;
        int arc, revArc;
        bool triggerArc;
    
        ofxPostGlitch	myGlitch;
        ofxPostGlitch	myGlitch_;
    
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