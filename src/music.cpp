#include "music.h"

void music::setup(){
/*
	c.loadSound("Electric_Guitar/C.mp3");
	d.loadSound("Electric_Guitar/D.mp3");
	e.loadSound("Electric_Guitar/E.mp3");
	f.loadSound("Electric_Guitar/F.mp3");
	g.loadSound("Electric_Guitar/G.mp3");
	a.loadSound("Electric_Guitar/A.mp3");
	b.loadSound("Electric_Guitar/B.mp3");
	highc.loadSound("Electric_Guitar/HIGHC.mp3");
	*/

    /*
    musiclist.push_back("Backgrnd_Music/Backgrounddefault.mp3");
    musiclist.push_back("Backgrnd_Music/Background1.mp3");
    */
    vector<int> scale1;
    scale1.push_back(131);
    scale1.push_back(147);
    scale1.push_back(165);
    scale1.push_back(175);
    scale1.push_back(196);
    scale1.push_back(220);
    scale1.push_back(247);
    scale1.push_back(262);
    scales.push_back(scale1); //C3major
    scale1[0]=(131*2);
    scale1[1]=(147*2);
    scale1[2]=(165*2);
    scale1[3]=(175*2);
    scale1[4]=(196*2);
    scale1[5]=(220*2);
    scale1[6]=(247*2);
    scale1[7]=(262*2);
    scales.push_back(scale1); //C4major
    scale1[0]=(220);
    scale1[1]=(247);
    scale1[2]=(277);
    scale1[3]=(294);
    scale1[4]=(330);
    scale1[5]=(370);
    scale1[6]=(415);
    scale1[7]=(440);
    scales.push_back(scale1); //A3major

    loadinstrumentlist();
    currentinstrumentdirectory=2;
    scalenumber=2;
    loadnotes(currentinstrumentdirectory);
    loadbackgroundmusiclist();
//    background(0);
    //fillans();

    iscorrect.push_back(false);
    iscorrect.push_back(false);
    iscorrect.push_back(false);
    iscorrect.push_back(false);
    iscorrect.push_back(false);
    iscorrect.push_back(false);
    iscorrect.push_back(false);
    iscorrect.push_back(false);
    iscorrect.push_back(false);

    isplaying.push_back(false);
    isplaying.push_back(false);
    isplaying.push_back(false);
    isplaying.push_back(false);
    isplaying.push_back(false);
    isplaying.push_back(false);
    isplaying.push_back(false);
    isplaying.push_back(false);
    isplaying.push_back(false);

    stoptimes.push_back(-10000);
    stoptimes.push_back(-10000);
    stoptimes.push_back(-10000);
    stoptimes.push_back(-10000);
    stoptimes.push_back(-10000);
    stoptimes.push_back(-10000);
    stoptimes.push_back(-10000);
    stoptimes.push_back(-10000);
    stoptimes.push_back(-10000);


    //this is the default background music when simon.setup is run
}

void music::updatelists(){
    isplaying[0]=c.getIsPlaying();
    isplaying[1]=d.getIsPlaying();
    isplaying[2]=e.getIsPlaying();
    isplaying[3]=f.getIsPlaying();
    isplaying[4]=g.getIsPlaying();
    isplaying[5]=a.getIsPlaying();
    isplaying[6]=b.getIsPlaying();
    isplaying[7]=highc.getIsPlaying();
}

void music::background(int x){
    if (musiclist.size()>x){
        bgmusic.stop();
        loadbackground(x);
        playbackground(0.1);
    }
                //on line 166 of testapp.cpp, we can send simon.background(ofRandom(0,1)), which picks a random background music
}

void music::stopbackground(){
    if (bgmusic.getIsPlaying()){
        bgmusic.stop();
    }
}

void music::loadbackground(int x){
    if (musiclist.size()>x){
        bgmusic.loadSound(musiclist[x]);
    }
}

void music::loadbackground(){
    //loadbackground((int)ofRandom(-0.5,musiclist.size()-0.5));
    loadbackground(0);
}

void music::playbackground(float vol){
    bgmusic.setVolume(vol);
    bgmusic.setLoop(true);
    bgmusic.play();
}

void music::playnote(char note, float vol){
    if (note=='A') {c.setVolume(vol); c.play();}
    if (note=='B') {d.setVolume(vol); d.play();}
    if (note=='C') {e.setVolume(vol); e.play();}
    if (note=='D') {f.setVolume(vol); f.play();}
    if (note=='E') {g.setVolume(vol); g.play();}
    if (note=='F') {a.setVolume(vol); a.play();}
    if (note=='G') {b.setVolume(vol); b.play();}
    if (note=='H') {highc.setVolume(vol); highc.play();}
}

void music::stopnote(char note){
    if (note=='A') c.stop();
    else if (note=='B') d.stop();
    else if (note=='C') e.stop();
    else if (note=='D') f.stop();
    else if (note=='E') g.stop();
    else if (note=='F') a.stop();
    else if (note=='G') b.stop();
    else if (note=='H') highc.stop();
}

void music::stopnote(int note){
    stopnote((char)(note+65));
}

bool music::playgame(vector<char> played){
    numbercorrect=0;
    for(int i=0;i<8;i++)
    {
        printf("key is:%c\n",key[i]);
        if (key[i]==played[i]) {
            numbercorrect+=1;
            iscorrect[i]=true;
        }
        else iscorrect[i]=false;
        //check if the strings are identical
    }
    printf("%i\n",numbercorrect);
    if (numbercorrect==8) {
        for (int i=0;i<8;i++){
            iscorrect[i]=false;
        }
        return true;
        }
    else
    {
        return false;
    }
}

void music::fillansrand(){
    int ans[8];
    for (int i=0;i<8;i++)
    {
        ans[i]= (int)ofRandom(-0.5,7.5);
    }
    while (key.size()<8){
        key.push_back('I');
    }
    translatekey(ans);
}

void music::fillansknown(){
  int ans[8];
  int x= (int)ofRandom(-0.5,3.5);
  if (x==0) {ans[0]=0;ans[1]=1;ans[2]=2;ans[3]=3;ans[4]=4;ans[5]=5;ans[6]=6;ans[7]=7;int ans[8];}
  else if (x==1) {ans[0]=0;ans[1]=1;ans[2]=2;ans[3]=3;ans[4]=4;ans[5]=5;ans[6]=6;ans[7]=7;int ans[8];}
  else if (x==2) {ans[0]=0;ans[1]=1;ans[2]=2;ans[3]=3;ans[4]=4;ans[5]=5;ans[6]=6;ans[7]=7;int ans[8];}
  else if (x==3) {ans[0]=0;ans[1]=1;ans[2]=2;ans[3]=3;ans[4]=4;ans[5]=5;ans[6]=6;ans[7]=7;int ans[8];}

  while (key.size()<8){key.push_back('I');}
  translatekey(ans);
}

void music::translatekey(int* ans) {
    for (int i=0;i<8;i++)
    {
        if ((int)ans[i]==0) key[i]='A';
        else if ((int)ans[i]==1) key[i]='B';
        else if ((int)ans[i]==2) key[i]='C';
        else if ((int)ans[i]==3) key[i]='D';
        else if ((int)ans[i]==4) key[i]='E';
        else if ((int)ans[i]==5) key[i]='F';
        else if ((int)ans[i]==6) key[i]='G';
        else if ((int)ans[i]==7) key[i]='H';
    }
}

void music::playkey(int vol){ //the way i wrote this, it can't play in the middle of a round...
    /*we must have a separate function which plays the answer.
    it should be called on every level up, and also can be an option in the middle of a round for a quick refresher
    since we want that feature, we cant just make a playnote() commanded embedded within the loop in fillans()
    or...since it might be confusing to have that in the middle of the round(what do they push), instead, after hearing the string at the beginning of the level
    just give them the chance to either pick- "listen again" or "play"
    */
    int starttime,currenttime;
    for (int i=0;i<8;i++)
    {
        starttime=ofGetElapsedTimeMillis();
        currenttime=ofGetElapsedTimeMillis();
        playnote(key[i],vol);
        while (currenttime-starttime<500){ //play for 500ms, then stop
            currenttime=ofGetElapsedTimeMillis();
        }
        stopnote(key[i]);
        //delay(1 second)
    }


}

void music::playstring(int vol,vector<char> string){
    int starttime,currenttime;
    for (int i=0;i<8;i++)
    {
        starttime=ofGetElapsedTimeMillis();
        currenttime=ofGetElapsedTimeMillis();
        playnote(string[i],vol);
        while(currenttime-starttime<500){
            currenttime=ofGetElapsedTimeMillis();
        }
        stopnote(string[i]);
    }
}

void music::loadbackgroundmusiclist(){
    DIR *pDIR;
    struct dirent *entry;
    if (pDIR=opendir("./data/Backgrnd_Music")){
        while (entry=readdir(pDIR)){
            if (strcmp(entry->d_name, ".")!=0 && strcmp(entry->d_name,"..")!=0){
                if ((entry->d_name)[strlen(entry->d_name)-4]=='.' &&
                    (entry->d_name)[strlen(entry->d_name)-3]=='m' &&
                    (entry->d_name)[strlen(entry->d_name)-2]=='p' &&
                    (entry->d_name)[strlen(entry->d_name)-1]=='3')
                    {
                        char stringthing[260];
                        sprintf(stringthing,"Backgrnd_Music/%s",(entry->d_name));
                        musiclist.push_back(stringthing);
                    }
            }
        }
        closedir(pDIR);
    }
}

void music::loadinstrumentlist(){
    DIR *pDIR;
    struct dirent *entry;
    if (pDIR=opendir("./data")){
        while (entry=readdir(pDIR)){
            if (strcmp(entry->d_name, ".")!=0 && strcmp(entry->d_name,"..")!=0){
                if ((entry->d_name)[0]=='I' ){
                        char stringthing[260];
                        sprintf(stringthing,"%s/",(entry->d_name));
                        instrumentlist.push_back(stringthing);
                        printf("%s\n",stringthing);
                    }
            }
        }
        closedir(pDIR);
    }
}

void music::loadnotes(int dir){
    char stringthing[260];
    sprintf(stringthing,"%s%i.mp3",instrumentlist[dir].c_str(),scales[scalenumber][0]);
    c.loadSound(stringthing);
    sprintf(stringthing,"%s%i.mp3",instrumentlist[dir].c_str(),scales[scalenumber][1]);
    d.loadSound(stringthing);
    sprintf(stringthing,"%s%i.mp3",instrumentlist[dir].c_str(),scales[scalenumber][2]);
    e.loadSound(stringthing);
    sprintf(stringthing,"%s%i.mp3",instrumentlist[dir].c_str(),scales[scalenumber][3]);
    f.loadSound(stringthing);
    sprintf(stringthing,"%s%i.mp3",instrumentlist[dir].c_str(),scales[scalenumber][4]);
    g.loadSound(stringthing);
    sprintf(stringthing,"%s%i.mp3",instrumentlist[dir].c_str(),scales[scalenumber][5]);
    a.loadSound(stringthing);
    sprintf(stringthing,"%s%i.mp3",instrumentlist[dir].c_str(),scales[scalenumber][6]);
    b.loadSound(stringthing);
    sprintf(stringthing,"%s%i.mp3",instrumentlist[dir].c_str(),scales[scalenumber][7]);
    highc.loadSound(stringthing);
}



