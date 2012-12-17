#ifndef _MONSTER
#define _MONSTER


#include "ofMain.h"
#include <vector>
#include <stdio.h>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string.h>
#include <dirent.h>

class Monster{

	public:

        void setup();
        void draw();

        void loadimages();
        void loadimages(int dir);
        void setupbodylocations();
        void setupbodylocations(char* filename);
        void setuppivotpoints();
        void setuppivotpoints(char* filename);
        void setupallpoints(int dir);

		vector<ofImage> bodyparts;
		//bodyparts will be:
        //0: head
        //1: torso
        //2: leftarmtop
        //3: leftarmbottom
        //4: leftweapon
        //5: rightarmtop
        //6: rightarmbottom
        //7: rightweapon
        //8: leftthigh
        //9: leftshin
        //10: rightthigh
        //11: rightshin
        vector<ofPoint> centers;
        vector<ofPoint> pivotpoints;
        //pivotpoints will be:
        //0: neck
        //1: doesnt matter
        //2: leftarmshoulder
        //3: leftarmelbow
        //4: leftwrist
        //5: rightarmshoulder
        //6: rightarmelbow
        //7: rightwrist
        //8: lefthip
        //9: leftknee
        //10: righthip
        //11: rightknee
        vector<int> bodypartangles;
        float scalesize;
        ofPoint actualcenter;
        vector<ofPoint> roadpoints;
        void setuproadpoints();
        float totaldistancetraveled;
        void moveactualcenter(float currenttime, float levelduration);
        void scalemonster(float currenttime, float levelduration);
        void update(float currenttime, float levelduration);

		ifstream documentinput;
		ofstream documentouput;
		vector<string> doc;

        int totalwidth, totalheight;
        int screenwidth, screenheight;
        vector<string> directories;
        void loaddirectories();
        void writetofiles(int dir);

        bool definingmonster;


};

#endif
