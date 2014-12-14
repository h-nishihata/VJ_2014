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
        void audioIn(float* input, int bufferSize, int nChannels);

    
        ofFbo myFbo;
        int fadeAmnt;

        
        ofNode testNodes[kNumTestNodes];
        int lookatIndex[kNumCameras];

        ofCamera cam[kNumCameras];
        float xAxis, yAxis, zAxis;
        bool xFlag, yFlag, zFlag;

        
        ofLight light;
        ofxPostGlitch	myGlitch;
    
    
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