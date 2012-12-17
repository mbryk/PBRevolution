#ifndef _MUSIC_H
#define _MUSIC_H

#include "ofMain.h"
#include <vector>
#include <stdio.h>
#include <cstdlib>
#include <iostream>
#include <string.h>
#include <fstream>
#include <dirent.h>

class music{
    public:
        bool playgame(vector<char> played);
        void setup();
        void playnote(char note, float vol);
        void stopnote(char note);
        void stopnote(int note);
        void background(int x);
        void stopbackground();
        void playkey(int vol);
        void loadbackgroundmusiclist();
        void loadbackground(int x);
        void loadbackground();
        void playbackground(float vol);
        void loadnotes(int dir);
        void loadinstrumentlist();
        void fillansrand();
        void fillansknown();
        void translatekey(int* ans);
        void playstring(int vol, vector<char> string);
        void updatelists();

        ofSoundPlayer c;
        ofSoundPlayer d;
        ofSoundPlayer e;
        ofSoundPlayer f;
        ofSoundPlayer g;
        ofSoundPlayer a;
        ofSoundPlayer b;
        ofSoundPlayer highc;
        ofSoundPlayer bgmusic;
        vector<string> musiclist;
        vector<string> instrumentlist;
        //vector<int*> songkeylist;
        vector<char> key;
        vector<bool> iscorrect;
        int numbercorrect;
        int currentinstrumentdirectory;
        vector<vector<int> > scales;
        int scalenumber;
        vector<bool> isplaying;
        vector<int> stoptimes;

    protected:
    private:
};

#endif // MUSIC_H
