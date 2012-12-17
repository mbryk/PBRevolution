#ifndef _TEST_APP
#define _TEST_APP

#define _USE_MUSIC


#include "ofMain.h"
#include "Monster.h"
#include <vector>
#include <list>
#include <iostream>
#include <fstream>

#include "ofArduino.h"
#include "ofSerial.h"

#include "music.h"

class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void exit();

        //for freeplay
        ofImage piano;
        ofImage exitsign;

        //for the monster
        Monster monster;
		ofImage background;
		int mouseX, mouseY;
		int closestcenter;
		int currentpivotpoint;
		ofPoint clicklocation;

		//for the pad on screen
        vector<int> lightedsquares;
        vector<int> colors;
        void lightthesquares(int ct);
        void lightthesquares(int startx, int starty, int width, int height, int ct);

        //for Arduino connection
        ofSerial serial;
        list<int> serialbuffer;
        list<int> recentpresses;
		bool connectedtoArduino;
		void attemptArduinoConnection();
		int comport;
		int comporttextsize;
		ofTrueTypeFont comportfont, connectedtoArduinofont;


        #ifdef _USE_MUSIC
            music simon;
        #endif

        //for actual game mechanics
        int level, answerkeylength;
        bool leveledup, menuscreen, gameover, timerstarted,starttimerstarted,updatedtimerNlevel,freeplay, randombg;
        float levelduration, starttime, currenttime,startertimer;
        ofTrueTypeFont	font,fontbigger; //for printing out the level and time remaining
        vector<char> currentans;
        int numbercorrect;
        void displaycurrentans();
        void resetvariables();
        float backgroundvolume;
        float notevolume;
        bool backgroundloaded;
        ofImage bloodyscreen;

        //for debouncing the switches
        char* calibrname;
		ifstream calibrationdocumentinput;
		ofstream calibrationdocumentouput;
		vector<string> calibrationdoc;
		void loadfile();
		void writetofile();
		vector<int> debouncetimes;
		void calibratedebounce();
		vector<int> calibrationamounts;

        //stores 'A','B','C' etc for comparison
		vector<char> keys;

        //for the menuscreen
		vector<ofPoint> buttoncorners;
		vector<ofPoint> buttonsizes;
		vector<ofImage> buttonimages;
		bool calibratingdebounce, changingbgvol, changingnotevol, definingroad, setupmenuscreen, needtodefinemonster,choosecomport;
		float setupmenuscreenoption, setupmenunumberofoptions;
		void loadmainmenuimages();
		void loadsetupmenuimages();
		int determinetextsize(string fontfilename, int width, int height, string stringthing);
		vector<string> setupmenulist;
		void setupsetupmenufont();
		ofTrueTypeFont setupmenufont, gamescreentextfont;
		char setupmenustring[255];
		int calibratestarttime;



};

#endif
