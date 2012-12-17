#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    //sets up menuscreen
    ofSetWindowTitle("Pete's Beats Revolutionized");
        //empty for now
    char stringthings[255];
    sprintf(stringthings,"Connected to Arduino:\nFalse");
    int size=determinetextsize("KGLesBouquinistesdeParis.ttf", ofGetWidth()/3,50,stringthings);
    connectedtoArduinofont.loadFont("KGLesBouquinistesdeParis.ttf",size);

    //sets up game portion
    starttime=ofGetSystemTime();
    background.loadImage("roadbackground.jpg");
    background.resize(ofGetWidth()*0.75,ofGetHeight());
    monster.setup();
    monster.screenheight=ofGetHeight();
    monster.screenwidth=ofGetWidth();
    lightedsquares.push_back(-10000);
    lightedsquares.push_back(-10000);
    lightedsquares.push_back(-10000);
    lightedsquares.push_back(-10000);
    lightedsquares.push_back(-10000);//This one is the middle 'key'
    lightedsquares.push_back(-10000);
    lightedsquares.push_back(-10000);
    lightedsquares.push_back(-10000);
    lightedsquares.push_back(-10000);

    keys.push_back('A');
    keys.push_back('B');
    keys.push_back('C');
    keys.push_back('D');
    keys.push_back('Z');//This one is the middle 'key'
    keys.push_back('E');
    keys.push_back('F');
    keys.push_back('G');
    keys.push_back('H');

    colors.push_back(0xff0000);
    colors.push_back(0xffff00);
    colors.push_back(0x00ff00);
    colors.push_back(0xff00ff);
    colors.push_back(0xffffff);//This one is the middle 'key'
    colors.push_back(0x00ffff);
    colors.push_back(0xffffff);
    colors.push_back(0x0000ff);
    colors.push_back(0x000000);

    comport=4;
    attemptArduinoConnection();

    #ifdef _USE_MUSIC
        simon.setup();
        simon.loadbackground();
    #endif

    resetvariables();

    notevolume=1.0;
    backgroundvolume=0.1;
    setupmenuscreenoption=1;

    calibrname="dbcalibration.txt";
    loadfile();

    ofPoint point;
    for (int i=0;i<3;i++){
        for (int j=0;j<3;j++){
            point.x=ofGetWidth()*j/3;
            point.y=ofGetHeight()*i/3;
            buttoncorners.push_back(point);
            point.x=ofGetWidth()/3;
            point.y=ofGetHeight()/3;
            buttonsizes.push_back(point);
        }
    }

    ofImage buttonimage;
    buttonimages.push_back(buttonimage);
    buttonimages.push_back(buttonimage);
    buttonimages.push_back(buttonimage);
    buttonimages.push_back(buttonimage);
    buttonimages.push_back(buttonimage);
    buttonimages.push_back(buttonimage);
    buttonimages.push_back(buttonimage);
    buttonimages.push_back(buttonimage);
    buttonimages.push_back(buttonimage);
    loadmainmenuimages();

    font.loadFont("SPETETRIAL.ttf", ofGetWidth()/55*1.5);
    fontbigger.loadFont("SPETETRIAL.ttf", ofGetWidth()/55*2.5);
    setupmenunumberofoptions=7;

    setupmenulist.push_back("Calibrate Debounce");
    setupmenulist.push_back("Change Background Volume");
    setupmenulist.push_back("Change Note Volume");
    setupmenulist.push_back("Create a new Road");
    setupmenulist.push_back("Create a new Monster");
    setupmenulist.push_back("Play Random Strings");
    setupmenulist.push_back("Choose COM Port");
    setupsetupmenufont();

    gamescreentextfont.loadFont("MarkerScript.ttf",determinetextsize("MarkerScript.ttf",ofGetWidth()/2,ofGetHeight()*0.75,"Get Ready:\nLevel Begins In:\nA"));

    //sets up dying screen
        //empty right now
    bloodyscreen.loadImage("Blood_Splatter.png");
}

void testApp::update(){
    if (menuscreen) {
        //if on menuscreen, then run a different update loop
        if (connectedtoArduino){
                int byte=0;
                while (serial.available()>0){ //checks if there is something in the serial buffer
                    byte=serial.readByte(); //if so, store it in variable byte
                    serialbuffer.push_back(byte);
                }

                while (!serialbuffer.empty()){
                    //printf("%c\n",serialbuffer.front());
                    int time=ofGetElapsedTimeMillis();
                    for (int i=0;i<keys.size();i++){
                        if (serialbuffer.front()==keys[i]){
                            if (time-lightedsquares[i]>debouncetimes[i]){
                                lightedsquares[i]=time;
                            }
                        }
                    }
                    serialbuffer.pop_front();
                }
            }
            for (int i=0;i<lightedsquares.size();i++){
                if (ofGetElapsedTimeMillis()-lightedsquares[i]<debouncetimes[i]){
                    lightedsquares[i]=-10000;
                    //Accounts for any bounce in the signal
                    int st=ofGetElapsedTimeMillis();
                    int ct=ofGetElapsedTimeMillis();
                    while (ct-st<debouncetimes[i]){
                        ct=ofGetElapsedTimeMillis();
                    }
                    serial.flush();
                    //trigger whatever needs to be triggered
                    if (setupmenuscreen){
                        switch (i){
                            //setup: Calibrate debounce, adjust backgroundvolume, adjust notevolume, create own monster, create own road
                            case 0: break; //
                            case 1: //up
                                if (calibratingdebounce){ break;} //wont do anything
                                else if (changingbgvol){ backgroundvolume+=0.05; if (backgroundvolume>1.0) {backgroundvolume=1.0;} break;}
                                else if (changingnotevol){ notevolume+=0.05; if (notevolume>1.0) {notevolume=1.0;} break;}
                                else if (definingroad){ break;} //wont do anything
                                else if (needtodefinemonster){ break;} //wont do anything
                                else if (choosecomport) { comport--; if (comport<0) {comport=0;} break;}
                                else {setupmenuscreenoption--; if (setupmenuscreenoption<1) {setupmenuscreenoption=1;} break; }
                            case 2: break; //
                            case 3: //back
                                if (calibratingdebounce){ calibratingdebounce=false; break;}
                                else if (changingbgvol){ changingbgvol=false; buttoncorners[4].x=ofGetWidth()/3; buttonsizes[4].x=ofGetWidth()/3; break;}
                                else if (changingnotevol){ changingnotevol=false; buttoncorners[4].x=ofGetWidth()/3; buttonsizes[4].x=ofGetWidth()/3; break;}
                                else if (definingroad){ definingroad=false; break;}
                                else if (needtodefinemonster){ break;}
                                else if (choosecomport) { choosecomport=false; attemptArduinoConnection(); break;}
                            case 4: break; //empty: center square
                            case 5: //enter, play something if necessary
                                if (calibratingdebounce){ calibratingdebounce=false; break;}
                                else if (changingbgvol){ changingbgvol=false; buttoncorners[4].x=ofGetWidth()/3; buttonsizes[4].x=ofGetWidth()/3; break;}
                                else if (changingnotevol){ changingnotevol=false; buttoncorners[4].x=ofGetWidth()/3; buttonsizes[4].x=ofGetWidth()/3; break;}
                                else if (definingroad){ definingroad=false; break;}
                                else if (needtodefinemonster){ break;}
                                else if (choosecomport) { choosecomport=false; attemptArduinoConnection(); break;}
                                else if (setupmenuscreenoption==1){
                                    calibratingdebounce=true;
                                    calibratestarttime=ofGetElapsedTimeMillis();
                                    break;
                                }
                                else if (setupmenuscreenoption==2){ changingbgvol=true; buttoncorners[4].x=ofGetWidth()/3+20; buttonsizes[4].x=ofGetWidth()/3-20; break;}
                                else if (setupmenuscreenoption==3){ changingnotevol=true; buttoncorners[4].x=ofGetWidth()/3+20; buttonsizes[4].x=ofGetWidth()/3-20; break;}
                                else if (setupmenuscreenoption==4){ definingroad=true; break;}
                                else if (setupmenuscreenoption==5){
                                    needtodefinemonster=true;
                                    monster.definingmonster=true;
                                    buttoncorners[8].x=ofGetWidth()-100;
                                    buttoncorners[8].y=ofGetHeight()-100;
                                    buttonsizes[8].x=100;
                                    buttonsizes[8].y=100;
                                }
                                else if (setupmenuscreenoption==6) { randombg=true; break; }
                                else if (setupmenuscreenoption==7) {
                                    choosecomport=true;
                                    string stringthings="COM1\nCOM2\nCOM3\nCOM4\nCOM5\nCOM6\nCOM7\nCOM8\nCOM9";
                                    int size=determinetextsize("KGLesBouquinistesdeParis.ttf", 0,ofGetHeight()/3-10,stringthings);
                                    comportfont.loadFont("KGLesBouquinistesdeParis.ttf",size);
                                    break;
                                    }
                            case 6: break;
                            case 7: //down
                                if (calibratingdebounce){ break;}
                                else if (changingbgvol){ backgroundvolume-=0.05; if (backgroundvolume<0.0) {backgroundvolume=0.0;} break;}
                                else if (changingnotevol){ notevolume-=0.05; if (notevolume<0.0) {notevolume=0.0;} break;}
                                else if (definingroad){ break;}
                                else if (needtodefinemonster){ break;}
                                else if (choosecomport) { comport++; if (comport>9) {comport=9;} break;}
                                else {setupmenuscreenoption++; if (setupmenuscreenoption>setupmenunumberofoptions) {setupmenuscreenoption=setupmenunumberofoptions;} break; }
                            case 8: //revert all to false, go back to the main menu
                                if (calibratingdebounce){ calibratingdebounce=false;}
                                else if (changingbgvol){ changingbgvol=false; buttoncorners[4].x=ofGetWidth()/3; buttonsizes[4].x=ofGetWidth()/3;}
                                else if (changingnotevol){ changingnotevol=false; buttoncorners[4].x=ofGetWidth()/3; buttonsizes[4].x=ofGetWidth()/3;}
                                else if (definingroad){ definingroad=false;}
                                else if (needtodefinemonster){
                                    needtodefinemonster=false;
                                    monster.definingmonster=false;
                                    buttoncorners[8].x=ofGetWidth()*2/3;
                                    buttoncorners[8].y=ofGetHeight()*2/3;
                                    buttonsizes[8].x=ofGetHeight()/3;
                                    buttonsizes[8].y=ofGetHeight()/3;
                                    }
                                else if (choosecomport) { choosecomport=false; attemptArduinoConnection();}
                                setupmenuscreenoption=1; setupmenuscreen=false; loadmainmenuimages(); break; //main menu
                        }
                    }
                    else if(freeplay){
                    //if you pressed on exit, freeplay=false
                        switch(i){
                            case 0: break;
                            case 1: break;
                            case 2: break;
                            case 3: break;
                            case 4: break;
                            case 5: break;
                            case 6: break;
                            case 7: break;
                            case 8: freeplay=false;
                        }

                    }
                    else {
                        switch (i){
                            case 0: attemptArduinoConnection(); break; //Reconnect to Arduino
                            case 1:
                                menuscreen=false;
                                startertimer=ofGetElapsedTimeMillis();
                                break; //start game
                            case 2: break; //Instructions
                            case 3: break; //Multiplayer
                            case 4: break; //empty: center square
                            case 5: menuscreen=false; freeplay=true; starttime=ofGetElapsedTimeMillis(); break; //Free Play
                            case 6: setupmenuscreen=true; loadsetupmenuimages(); break; //setup: Calibrate Arduino, adjust volume, create own monster, create own road
                            case 7: std::exit(1); break; //exit game
                            case 8: break; //Credits
                        }
                    }
                }
            }

    }
    else if (freeplay){
        //simon.loadbackground();
        //simon.playbackground(backgroundvolume);
        currenttime=ofGetElapsedTimeMillis()-starttime;
        if (connectedtoArduino){
            int byte=0;
            while (serial.available()>0){ //checks if there is something in the serial buffer
                byte=serial.readByte(); //if so, store it in variable byte
                serialbuffer.push_back(byte);
            }
            while (!serialbuffer.empty()){
                int time=ofGetElapsedTimeMillis();
                for (int i=0;i<keys.size();i++){
                    if (serialbuffer.front()==keys[i]){
                        if (time-lightedsquares[i]>debouncetimes[i]){
                            lightedsquares[i]=time;
                            #ifdef _USE_MUSIC
                                simon.playnote(keys[i],notevolume);
                                simon.stoptimes[i]=currenttime+debouncetimes[i];
                                simon.updatelists();
                            #endif
                        }
                    }
                }
                serialbuffer.pop_front();
            }
        }
        currenttime=ofGetElapsedTimeMillis()-starttime;
        for (int i=0;i<8;i++){
            if (i<4){
                if (simon.isplaying[i]){
                    if (currenttime>simon.stoptimes[i]){
                        simon.stopnote(i);
                        simon.updatelists();
                    }
                }
            }
            else{
                if (simon.isplaying[i]){
                    if (currenttime>simon.stoptimes[i+1]){
                        simon.stopnote(i);
                        simon.updatelists();
                    }
                }
            }
        }
    }
    else{
        if (leveledup){
            if (level==0){
                if (ofGetElapsedTimeMillis()-startertimer>3000){ //this line makes sure it draws countdown timer
                    level++;
                    //draw screen
                    //draw countdown
                    //play answer key
                    if (randombg){
                        simon.fillansrand();
                        }
                    else{
                        simon.fillansknown();
                        }
                    //simon.playkey()
                    simon.playkey(notevolume);
                    //simon.background((int) ofRandom(0,1));
                    starttime=ofGetElapsedTimeMillis();
                    starttimerstarted=false;
                    updatedtimerNlevel=false;
                    leveledup=false;
                    //playkey here
                    simon.playbackground(backgroundvolume);
                }
            }
            else{
                if (!starttimerstarted){
                    simon.stopbackground();
                    startertimer=ofGetElapsedTimeMillis();
                    starttimerstarted=true;
                    backgroundloaded=false;
                }
                if (!backgroundloaded && ofGetElapsedTimeMillis()-startertimer>500){
                    simon.loadbackground(); //this takes a couple of seconds...
                    backgroundloaded=true;
                }
                if (ofGetElapsedTimeMillis()-startertimer>5000){
                    if (!updatedtimerNlevel){
                        currenttime=0; //reset the timer
                        level++; //increase the level
                        updatedtimerNlevel=true;
                        if (level<13) levelduration=120000-(level-1)*10000;
                        else levelduration=10000;
                        numbercorrect=0;
                        int times=ofGetElapsedTimeMillis();
                        #ifdef _USE_MUSIC
                            simon.fillansrand();
                        #endif
                        startertimer+=ofGetElapsedTimeMillis()-times;
                    }
                    //redraw the screen
                    if (ofGetElapsedTimeMillis()-startertimer>8000){
                        //show a countdown timer
                        //play the next ans
                        //then start the next level
                        simon.playkey(notevolume);
                        starttime=ofGetElapsedTimeMillis();
                        starttimerstarted=false;
                        leveledup=false;
                        updatedtimerNlevel=false;
                        simon.updatelists();
                        //play the new answer key here
                        simon.playbackground(backgroundvolume);
                    }
                }
            }
        }
        else{
            currenttime=ofGetElapsedTimeMillis()-starttime;
            for (int i=0;i<8;i++){
                if (i<4){
                    if (simon.isplaying[i]){
                        if (currenttime>simon.stoptimes[i]){
                            simon.stopnote(i);
                            simon.updatelists();
                        }
                    }
                }
                else{
                    if (simon.isplaying[i]){
                        if (currenttime>simon.stoptimes[i+1]){
                            simon.stopnote(i);
                            simon.updatelists();
                        }
                    }
                }
            }
        }

        if (levelduration-currenttime<0){
            gameover=true;
            #ifdef _USE_MUSIC
                simon.stopbackground();
            #endif
            ofSetBackgroundAuto(false);
            bloodyscreen.draw(0,0,ofGetWidth(),ofGetHeight());
        }

        if (!gameover){
            monster.update(currenttime,levelduration);
            if (connectedtoArduino){
                int byte=0;
                while (serial.available()>0){ //checks if there is something in the serial buffer
                    byte=serial.readByte(); //if so, store it in variable byte
                    serialbuffer.push_back(byte);
                }

                while (!serialbuffer.empty()){
                    //printf("%c\n",serialbuffer.front());
                    int time=ofGetElapsedTimeMillis();
                    for (int i=0;i<keys.size();i++){
                        if (serialbuffer.front()==keys[i]){
                            if (time-lightedsquares[i]>debouncetimes[i]){
                                lightedsquares[i]=time;
                                currentans.push_back(keys[i]);
                                #ifdef _USE_MUSIC
                                    simon.playnote(keys[i],notevolume);
                                    simon.stoptimes[i]=currenttime+debouncetimes[i];
                                    simon.updatelists();
                                #endif
                                if (currentans.size()>=answerkeylength){
                                    //if !(simon.playgame(currentans)) NEXT LEVEL!!!
                                    #ifdef _USE_MUSIC
                                        leveledup=simon.playgame(currentans);
                                        numbercorrect=simon.numbercorrect;
                                        if (leveledup) {
                                            for (int i=0;i<8;i++){
                                                simon.iscorrect[i]=false;
                                            }
                                        }
                                    #endif
                                    while (currentans.size()!=0){
                                        currentans.pop_back();
                                    }
                                }
                            }
                        }
                    }
                    serialbuffer.pop_front();
                }
            }

        }
        else{
            //draw gameover screen
            if (levelduration-currenttime<-3000){ //wait three seconds, then go back to menu
                resetvariables();
                ofSetBackgroundAuto(true);
            }
        }
    }
}

void testApp::draw(){
    if (menuscreen){
        if (setupmenuscreen){
            if (calibratingdebounce){
                buttonimages[4].loadImage("MenuImages/calibrate.jpg");
                if (ofGetElapsedTimeMillis()-calibratestarttime>200){
                    calibratedebounce();
                    calibratingdebounce=false;
                }
            }
            else if (changingbgvol){
                ofSetColor(0,255,0);
                ofRect(buttoncorners[4].x-20,buttoncorners[4].y+(buttonsizes[4].y-20)*(1-backgroundvolume),20,20);
                ofSetColor(255,255,255);
                buttonimages[4].loadImage("MenuImages/backgroundvolume.png");
                }
            else if (changingnotevol){
                ofSetColor(255,0,0);
                ofRect(buttoncorners[4].x-20,buttoncorners[4].y+(buttonsizes[4].y-20)*(1-notevolume),20,20);
                ofSetColor(255,255,255);
                buttonimages[4].loadImage("MenuImages/notevolume.png");
                }
            else if (definingroad){
                buttonimages[4].clear();
                background.draw(0,0,ofGetWidth()*0.75,ofGetHeight());
            }
            else if (needtodefinemonster){
                buttonimages[4].clear();
                monster.scalemonster(1.2,1.0);
                monster.moveactualcenter(0,120);
                monster.draw();
                ofSetColor(255,255,255);
                buttonimages[8].draw(buttoncorners[8].x,buttoncorners[8].y,buttonsizes[8].x,buttonsizes[8].y);
            }
            else if (choosecomport){
                ofSetColor(0,0,0);
                ofRect(buttoncorners[4].x,
                       buttoncorners[4].y+(buttonsizes[4].y-10)/(9)*(comport-1)+comportfont.stringHeight("C")/2,
                       10,10);
                string stringthing;
                stringthing="COM1\nCOM2\nCOM3\nCOM4\nCOM5\nCOM6\nCOM7\nCOM8\nCOM9";
                comportfont.drawString(stringthing,
                                ofGetWidth()/3+20,
                                ofGetHeight()/3+comportfont.stringHeight("C"));
                buttonimages[4].clear();
                ofSetColor(255,255,255);
            }
            else {
                ofSetColor(0,0,0);
                ofRect(buttoncorners[4].x,
                       buttoncorners[4].y+(buttonsizes[4].y-10)/(setupmenunumberofoptions)*(setupmenuscreenoption-1)+setupmenufont.stringHeight("A")/2,
                       10,10);
                setupmenufont.drawString(setupmenustring,
                                ofGetWidth()/3+20,
                                ofGetHeight()/3+setupmenufont.stringHeight("A"));
                buttonimages[4].clear();
                ofSetColor(255,255,255);
            }

        }
        if (!definingroad && !needtodefinemonster){
            ofBackground(255,255,255);
            for (int i=0;i<buttoncorners.size();i++){
                ofSetColor(255,255,255);
                if (buttonimages[i].getWidth()!=0)
                buttonimages[i].draw(buttoncorners[i].x,buttoncorners[i].y,buttonsizes[i].x,buttonsizes[i].y);
            }
            char stringthing[255];
            ofSetColor(100,100,100);
            sprintf(stringthing,"Connected to Arduino:    \n%s",(connectedtoArduino)?"True":"False");
            connectedtoArduinofont.drawString(stringthing,
                            ofGetWidth()/6-connectedtoArduinofont.stringWidth(stringthing)/2,
                            ofGetHeight()/3-connectedtoArduinofont.stringHeight("C"));
            ofSetColor(255,255,255);

        }

    }
    else if(freeplay){
        piano.loadImage("rolluppiano.jpg");
        piano.draw(0,0,ofGetWidth(),ofGetHeight()*0.25);
        lightthesquares(ofGetWidth()/2-ofGetHeight()*0.75/2,ofGetHeight()*0.25,ofGetWidth(),ofGetHeight()*0.75,ofGetElapsedTimeMillis());
        //exitsign.loadImage("exit.jpg");
        //exitsign.resize(ofGetWidth()*0.2,ofGetHeight()*0.2);
        //exitsign.draw(0,ofGetHeight()*0.25,ofGetWidth(),ofGetHeight());
    }
    else{
        if (!gameover){
            background.draw(0,0,ofGetWidth()*0.75,ofGetHeight());
            ofSetColor(0,0,0);
            ofRect(ofGetWidth()*0.75,0,ofGetWidth()*0.25,ofGetHeight());
            monster.draw();

            //lights the squares as they are pressed
            lightthesquares(ofGetElapsedTimeMillis());

            //shows what has already been pressed
            displaycurrentans();

            //level indicator
            font.loadFont("SPETETRIAL.ttf", ofGetWidth()/55*1.5);
            fontbigger.loadFont("SPETETRIAL.ttf", ofGetWidth()/55*2.5);
            char stringthing[255];
            sprintf(stringthing, "Level %i", level);
            fontbigger.drawString(stringthing,ofGetWidth()*0.75,ofGetHeight()/2*0.2);

            //time remaining indicator
            ofSetColor(255,0,0);
            sprintf(stringthing, "Time Remaining:\n%02i:%02i:%02i",
                    (int)(levelduration-currenttime)/60000,
                    (int)((int)(levelduration-currenttime)-(int)(levelduration-currenttime)/60000*60000)/1000,
                    (int)((int)(levelduration-currenttime)-(int)(levelduration-currenttime)/1000*1000)/10);
            font.drawString(stringthing,ofGetWidth()*0.75,ofGetHeight()/2*0.4);
            ofSetColor(255,255,255);

            font.loadFont("SPETETRIAL.ttf", ofGetWidth()/55*1.4);
            ofSetColor(255,255,255);
            sprintf(stringthing, "Number Correct: %i",numbercorrect);
            font.drawString(stringthing,ofGetWidth()*0.75,ofGetHeight()/2*0.7);
            ofSetColor(255,255,255);

                //need to show the number of keys that were correct in the last sequence
            if (leveledup){
                if (level!=0){
                    if (ofGetElapsedTimeMillis()-startertimer<5000){ //pause for 5 seconds
                        //show a nice job, time it took to finish level
                        char stringthing[255];
                        sprintf(stringthing, "NICE WORK!\nTime Taken:\n %02i:%02i:%02i",
                                (int)(currenttime)/60000,
                                (int)((int)(currenttime)-(int)(currenttime)/60000*60000)/1000,
                                (int)((int)(currenttime)-(int)(currenttime)/1000*1000)/10);
                        ofSetColor(0,255,255);
                        gamescreentextfont.drawString(stringthing,
                                               ofGetWidth()*0.75/2-gamescreentextfont.stringWidth(stringthing)/2,
                                               ofGetHeight()/2-gamescreentextfont.stringHeight(stringthing)/2+gamescreentextfont.stringHeight("A"));
                        ofSetColor(255,255,255);
                    }
                    else if (ofGetElapsedTimeMillis()-startertimer<8000){ //then show a countdown timer
                        char stringthing[255];
                        sprintf(stringthing, "  Get Ready!\nLevel %02i starts in:\n    %02i",
                                level,
                                (int)(4-(ofGetElapsedTimeMillis()-startertimer-5000)/1000));
                        ofSetColor(0,255,255);
                        gamescreentextfont.drawString(stringthing,
                                               ofGetWidth()*0.75/2-gamescreentextfont.stringWidth(stringthing)/2,
                                               ofGetHeight()/2-gamescreentextfont.stringHeight(stringthing)/2+gamescreentextfont.stringHeight("A"));
                        ofSetColor(255,255,255);
                    }
                }
                else {
                    if (ofGetElapsedTimeMillis()-startertimer<3000){ //then show a countdown timer
                        char stringthing[255];
                        sprintf(stringthing, "  Get Ready!\nLevel 01 starts in:\n    %02i",
                                (int)(4-(ofGetElapsedTimeMillis()-startertimer)/1000));
                        ofSetColor(0,255,255);
                        gamescreentextfont.drawString(stringthing,
                                               ofGetWidth()*0.75/2-gamescreentextfont.stringWidth(stringthing)/2,
                                               ofGetHeight()/2-gamescreentextfont.stringHeight(stringthing)/2+gamescreentextfont.stringHeight("A"));
                        ofSetColor(255,255,255);
                    }
                }

            }
        }
        else {
            //draw gameoverscreen
        }
    }
}

void testApp::keyPressed(int key){
    if (needtodefinemonster){
        if (key=='p'){
            //print the monster centers
            for (int i=0;i<monster.centers.size();i++){
                printf("point.x=%f;\npoint.y=%f;\ncenters.push_back(point);\n",monster.centers[i].x,monster.centers[i].y);
            }
            printf("\n");
            for (int i=0;i<monster.pivotpoints.size();i++){
                printf("point.x=%f;\npoint.y=%f;\npivotpoints.push_back(point);\n",monster.pivotpoints[i].x,monster.pivotpoints[i].y);
            }
        }
        else if (key=='e'){
            ofPoint point;
            point.x=mouseX-monster.actualcenter.x;
            point.y=mouseY-monster.actualcenter.y;
            monster.pivotpoints[currentpivotpoint]=point;
            printf("Pivot point %i is now %f,%f\n",currentpivotpoint,point.x,point.y);
        }
        else if (key=='q'){
            currentpivotpoint++;
            if (currentpivotpoint>11) currentpivotpoint=11;
            printf("current pivot point is: %i\n",currentpivotpoint);
        }
        else if (key=='w'){
            currentpivotpoint--;
            if (currentpivotpoint<0) currentpivotpoint=0;
            printf("current pivot point is: %i\n",currentpivotpoint);
        }
    }
    else {
        //for testing purposes, maybe we should leave this as a cheat? YAY I LOVE CHEATS!
        if (key=='+'){
            leveledup=true;
        }
        else if (key=='-'){
            level--;
            if (level<1) level=1;
        }
        else if (key=='r'){
            definingroad=!definingroad;
            printf("Defining road is now %s\n", (definingroad)?"true":"false");
            if (!definingroad){
                for (int i=0;i<monster.roadpoints.size();i++){
                    printf("point.x=%f;\npoint.y=%f;\nroadpoints.push_back(point);\n",monster.roadpoints[i].x/(ofGetWidth()*0.75),monster.roadpoints[i].y/(ofGetHeight()));
                }
            }
        }
        else if (key=='p'){
            gameover=true;
            //simon.stopbackground();
            resetvariables();
        }
        if (!menuscreen && !gameover){
            int time=ofGetElapsedTimeMillis();
            char keypress;
            if (key=='7') keypress='A';
            else if (key=='8') keypress='B';
            else if (key=='9') keypress='C';
            else if (key=='4') keypress='D';
            else if (key=='6') keypress='E';
            else if (key=='1') keypress='F';
            else if (key=='2') keypress='G';
            else if (key=='3') keypress='H';
            for (int i=0;i<keys.size();i++){
                if (keypress==keys[i]){
                    if (time-lightedsquares[i]>debouncetimes[i]){
                        lightedsquares[i]=time;
                        if (!freeplay) {currentans.push_back(keys[i]);}
                        #ifdef _USE_MUSIC
                            simon.playnote(keys[i],notevolume);
                            simon.stoptimes[i]=currenttime+debouncetimes[i];
                            simon.updatelists();
                        #endif
                        if (currentans.size()>=answerkeylength){
                            //if !(simon.playgame(currentans)) NEXT LEVEL!!!
                            #ifdef _USE_MUSIC
                                leveledup=simon.playgame(currentans);
                                numbercorrect=simon.numbercorrect;
                                if (leveledup) {
                                    for (int i=0;i<8;i++){
                                        simon.iscorrect[i]=false;
                                    }
                                }
                            #endif
                            while (currentans.size()!=0){
                                currentans.pop_back();
                            }
                        }
                    }
                }
            }
        }
    }
}

void testApp::keyReleased(int key){
}

void testApp::mouseMoved(int x, int y ){
    mouseX=x;
    mouseY=y;
}

void testApp::mouseDragged(int x, int y, int button){
    //button is 0 when left click
    //button is 2 when right click
    if (needtodefinemonster){
        if (closestcenter==1){
            monster.actualcenter.x=x*monster.scalesize;
            monster.actualcenter.y=y*monster.scalesize;
        }
        else{
            if (button==0){
                monster.centers[closestcenter].x=(x-monster.actualcenter.x);
                monster.centers[closestcenter].y=(y-monster.actualcenter.y);
            }
            else if (button==2){
                if (clicklocation.x>monster.actualcenter.x){
                    monster.bodypartangles[closestcenter]+=(y-clicklocation.y)/ofGetHeight()*180;
                }
                else{
                    monster.bodypartangles[closestcenter]-=(y-clicklocation.y)/ofGetHeight()*180;
                }
                if (clicklocation.y<monster.actualcenter.y){
                    monster.bodypartangles[closestcenter]+=(x-clicklocation.x)/ofGetWidth()*180;
                }
                else{
                    monster.bodypartangles[closestcenter]-=(x-clicklocation.x)/ofGetWidth()*180;
                }
                clicklocation.y=y;
                clicklocation.x=x;
            }
        }
    }
}

void testApp::mousePressed(int x, int y, int button){
    if (menuscreen){
        if (needtodefinemonster){
            clicklocation.x=x;
            clicklocation.y=y;
            if (button==0){
                closestcenter=0;

                float closestdistance=ofGetWidth()*ofGetHeight()*500;
                float currentdist;
                for (int i=0;i<monster.centers.size();i++){
                    currentdist=ofDist(x,y,monster.centers[i].x+monster.actualcenter.x,monster.centers[i].y+monster.actualcenter.y);
                    if (currentdist<closestdistance){
                        closestcenter=i;
                        closestdistance=currentdist;
                    }
                }
            }
        }

        else{
            if (definingroad){
                ofPoint point1;
                point1.x=x;
                point1.y=y;
                monster.roadpoints.push_back(point1);
            }
        }
    }
    else{
    }
}

void testApp::mouseReleased(int x, int y, int button){ //copy and paste switch cases from update loop
    if (menuscreen){
        for (int i=0;i<buttoncorners.size();i++){
            if (x>buttoncorners[i].x && x<buttoncorners[i].x+buttonsizes[i].x){
                if (y>buttoncorners[i].y && y<buttoncorners[i].y+buttonsizes[i].y){
                    if (setupmenuscreen){
                        switch (i){
                            //setup: Calibrate debounce, adjust backgroundvolume, adjust notevolume, create own monster, create own road
                            case 0: break; //
                            case 1: //up
                                if (calibratingdebounce){ break;} //wont do anything
                                else if (changingbgvol){ backgroundvolume+=0.05; if (backgroundvolume>1.0) {backgroundvolume=1.0;} break;}
                                else if (changingnotevol){ notevolume+=0.05; if (notevolume>1.0) {notevolume=1.0;} break;}
                                else if (definingroad){ break;} //wont do anything
                                else if (needtodefinemonster){ break;} //wont do anything
                                else if (choosecomport) { comport--; if (comport<0) {comport=0;} break;}
                                else {setupmenuscreenoption--; if (setupmenuscreenoption<1) {setupmenuscreenoption=1;} break; }
                            case 2: break; //
                            case 3: //back
                                if (calibratingdebounce){ calibratingdebounce=false; break;}
                                else if (changingbgvol){ changingbgvol=false; buttoncorners[4].x=ofGetWidth()/3; buttonsizes[4].x=ofGetWidth()/3; break;}
                                else if (changingnotevol){ changingnotevol=false; buttoncorners[4].x=ofGetWidth()/3; buttonsizes[4].x=ofGetWidth()/3; break;}
                                else if (definingroad){ definingroad=false; break;}
                                else if (needtodefinemonster){ break;}
                                else if (choosecomport) { choosecomport=false; attemptArduinoConnection(); break;}
                            case 4: break; //empty: center square
                            case 5: //enter, play something if necessary
                                if (calibratingdebounce){ calibratingdebounce=false; break;}
                                else if (changingbgvol){ changingbgvol=false; buttoncorners[4].x=ofGetWidth()/3; buttonsizes[4].x=ofGetWidth()/3; break;}
                                else if (changingnotevol){ changingnotevol=false; buttoncorners[4].x=ofGetWidth()/3; buttonsizes[4].x=ofGetWidth()/3; break;}
                                else if (definingroad){ definingroad=false; break;}
                                else if (needtodefinemonster){ break;}
                                else if (choosecomport) { choosecomport=false; attemptArduinoConnection(); break;}
                                else if (setupmenuscreenoption==1){
                                    calibratingdebounce=true;
                                    calibratestarttime=ofGetElapsedTimeMillis();
                                    break;
                                }
                                else if (setupmenuscreenoption==2){ changingbgvol=true; buttoncorners[4].x=ofGetWidth()/3+20; buttonsizes[4].x=ofGetWidth()/3-20; break;}
                                else if (setupmenuscreenoption==3){ changingnotevol=true; buttoncorners[4].x=ofGetWidth()/3+20; buttonsizes[4].x=ofGetWidth()/3-20; break;}
                                else if (setupmenuscreenoption==4){ definingroad=true; break;}
                                else if (setupmenuscreenoption==5){
                                    needtodefinemonster=true;
                                    monster.definingmonster=true;
                                    buttoncorners[8].x=ofGetWidth()-100;
                                    buttoncorners[8].y=ofGetHeight()-100;
                                    buttonsizes[8].x=100;
                                    buttonsizes[8].y=100;
                                }
                                else if (setupmenuscreenoption==6) { randombg=true; break; }
                                else if (setupmenuscreenoption==7) {
                                    choosecomport=true;
                                    string stringthings="COM1\nCOM2\nCOM3\nCOM4\nCOM5\nCOM6\nCOM7\nCOM8\nCOM9";
                                    int size=determinetextsize("KGLesBouquinistesdeParis.ttf", 0,ofGetHeight()/3-10,stringthings);
                                    comportfont.loadFont("KGLesBouquinistesdeParis.ttf",size);
                                    break;
                                    }
                            case 6: break;
                            case 7: //down
                                if (calibratingdebounce){ break;}
                                else if (changingbgvol){ backgroundvolume-=0.05; if (backgroundvolume<0.0) {backgroundvolume=0.0;} break;}
                                else if (changingnotevol){ notevolume-=0.05; if (notevolume<0.0) {notevolume=0.0;} break;}
                                else if (definingroad){ break;}
                                else if (needtodefinemonster){ break;}
                                else if (choosecomport) { comport++; if (comport>9) {comport=9;} break;}
                                else {setupmenuscreenoption++; if (setupmenuscreenoption>setupmenunumberofoptions) {setupmenuscreenoption=setupmenunumberofoptions;} break; }
                            case 8: //revert all to false, go back to the main menu
                                if (calibratingdebounce){ calibratingdebounce=false;}
                                else if (changingbgvol){ changingbgvol=false; buttoncorners[4].x=ofGetWidth()/3; buttonsizes[4].x=ofGetWidth()/3;}
                                else if (changingnotevol){ changingnotevol=false; buttoncorners[4].x=ofGetWidth()/3; buttonsizes[4].x=ofGetWidth()/3;}
                                else if (definingroad){ definingroad=false;}
                                else if (needtodefinemonster){
                                    needtodefinemonster=false;
                                    monster.definingmonster=false;
                                    buttoncorners[8].x=ofGetWidth()*2/3;
                                    buttoncorners[8].y=ofGetHeight()*2/3;
                                    buttonsizes[8].x=ofGetHeight()/3;
                                    buttonsizes[8].y=ofGetHeight()/3;
                                    }
                                else if (choosecomport) { choosecomport=false; attemptArduinoConnection();}
                                setupmenuscreenoption=1; setupmenuscreen=false; loadmainmenuimages(); break; //main menu
                        }
                    }
                    else if(freeplay){
                    //if you pressed on exit, freeplay=false
                        switch(i){
                            case 0: break;
                            case 1: break;
                            case 2: break;
                            case 3: break;
                            case 4: break;
                            case 5: break;
                            case 6: break;
                            case 7: break;
                            case 8: freeplay=false;
                        }

                    }
                    else {
                        switch (i){
                            case 0: attemptArduinoConnection(); break; //Reconnect to Arduino
                            case 1: menuscreen=false; startertimer=ofGetElapsedTimeMillis(); break; //start game
                            case 2: break; //Instructions
                            case 3: break; //Multiplayer
                            case 4: break; //empty: center square
                            case 5: menuscreen=false; freeplay=true; break; //Free Play
                            case 6: setupmenuscreen=true; loadsetupmenuimages(); break; //setup: Calibrate Arduino, adjust volume, create own monster, create own road
                            case 7: std::exit(1); break; //exit game
                            case 8: break; //Credits
                        }
                    }
                }
            }
        }

    }
}

void testApp::windowResized(int w, int h){
    monster.screenheight=ofGetHeight();
    monster.screenwidth=ofGetWidth();
    ofPoint point;
    for (int i=0;i<3;i++){
        for (int j=0;j<3;j++){
            int ref=3*i+j;
            point.x=ofGetWidth()*j/3;
            point.y=ofGetHeight()*i/3;
            buttoncorners[ref]=point;
            point.x=ofGetWidth()/3;
            point.y=ofGetHeight()/3;
            buttonsizes[ref]=point;
        }
    }
}

void testApp::exit(){
    serial.flush();
    serial.close();
    printf("\nProgram Terminated. Goodbye Sucker.\n");
}

void testApp::resetvariables(){
    level=0;
    menuscreen=true;
    setupmenuscreen=false;
    freeplay=false;
    definingroad=false;
    leveledup=true;
    gameover=false;
    starttimerstarted=false;
    levelduration=120000;
    answerkeylength=8;
    needtodefinemonster=false;
    updatedtimerNlevel=false;
    numbercorrect=0;
    startertimer=0;
    starttime=0;
    currenttime=0;
    for (int i=0;i<lightedsquares.size();i++){
        lightedsquares[i]=-10000;
    }
    choosecomport=false;
    while (currentans.size()!=0){
        currentans.pop_back();
    }
    for (int i=0;i<8;i++){
        simon.iscorrect[i]=false;
    }
    simon.stopbackground();
}

void testApp::attemptArduinoConnection(){
    if (connectedtoArduino){
        serial.flush();
        serial.close();
    }
    serial.enumerateDevices();
    connectedtoArduino=false;

    char stringthing[255];
    sprintf(stringthing,"COM%i",comport);
    if (serial.setup(stringthing,9600)){
        int connectiontime=ofGetElapsedTimeMillis();
        bool done=false;

        while (!done){
            if (ofGetElapsedTimeMillis()-connectiontime>5000){
                done=true;
            }
            if (serial.available()>0){
                char byte=serial.readByte();
                if (byte=='I'){
                    serial.flush();
                    serial.writeByte('I');
                    printf("\nConnection Established\n");
                    connectedtoArduino=true;
                    done=true;
                    serial.flush();
                }
                else printf("%c",byte);
            }
        }
    }

    if (!connectedtoArduino) {
        printf("\nFailed to connect to the Arduino, try again\n");
    }
}

void testApp::lightthesquares(int ct){
    for (int i=0;i<3;i++){
        for (int j=0;j<3;j++){
            int ref=3*j+i;
            ofSetColor(0xaaaaaa);
            if (ct-lightedsquares[ref]<debouncetimes[ref]) {
                ofSetColor(colors[ref]);
            }
            ofRect(ofGetWidth()*0.75+i*ofGetWidth()*0.25/3.0,
                   ofGetHeight()*0.5+j*ofGetWidth()*0.25/3.0,
                   ofGetWidth()*0.25/3.0,
                   ofGetWidth()*0.25/3.0);

            //black border
            ofSetColor(0,0,0);
            ofNoFill();
            ofRect(ofGetWidth()*0.75+i*ofGetWidth()*0.25/3.0,
                   ofGetHeight()*0.5+j*ofGetWidth()*0.25/3.0,
                   ofGetWidth()*0.25/3.0,
                   ofGetWidth()*0.25/3.0);
            ofFill();
            ofSetColor(255,255,255);
        }
    }
}

void testApp::lightthesquares(int startx, int starty, int width, int height, int ct){
    if (height>width){
        for (int i=0;i<3;i++){
            for (int j=0;j<3;j++){
                int ref=3*j+i;
                ofSetColor(0xaaaaaa);
                if (ct-lightedsquares[ref]<debouncetimes[ref]) {
                    ofSetColor(colors[ref]);
                }
                ofRect(startx+i*width/3.0,
                       starty+j*width/3.0,
                       width/3.0,
                       width/3.0);

                //black border
                ofSetColor(0,0,0);
                ofNoFill();
                ofRect(startx+i*width/3.0,
                       starty+j*width/3.0,
                       width/3.0,
                       width/3.0);
                ofFill();
                ofSetColor(255,255,255);
            }
        }
    }
    else{
        for (int i=0;i<3;i++){
            for (int j=0;j<3;j++){
                int ref=3*j+i;
                ofSetColor(0xaaaaaa);
                if (ct-lightedsquares[ref]<debouncetimes[ref]) {
                    ofSetColor(colors[ref]);
                }
                ofRect(startx+i*height/3.0,
                       starty+j*height/3.0,
                       height/3.0,
                       height/3.0);

                //black border
                ofSetColor(0,0,0);
                ofNoFill();
                ofRect(startx+i*height/3.0,
                       starty+j*height/3.0,
                       height/3.0,
                       height/3.0);
                ofFill();
                ofSetColor(255,255,255);
            }
        }
    }
}

void testApp::displaycurrentans(){
    for (int i=0;i<currentans.size();i++){
        for (int j=0;j<keys.size();j++){
            if (currentans[i]==keys[j]){
                ofSetColor(colors[j]);
                break;
            }
        }
        ofRect(ofGetWidth()*0.75+i*ofGetWidth()*0.25/answerkeylength,
               ofGetHeight()/2-ofGetWidth()*0.25/answerkeylength,
               ofGetWidth()*0.25/answerkeylength,
               ofGetWidth()*0.25/answerkeylength);
    }
    for (int i=0;i<8;i++){
        if (simon.iscorrect[i]){
            for (int j=0;j<8;j++){
                if (simon.key[i]==(char)(65+j)){
                    if (j>=4){j++;}
                    ofSetColor(colors[j]);
                    break;
                }
            }
        }
        else {
            ofSetColor(0,0,0);
        }
        ofRect(ofGetWidth()*0.75+i*ofGetWidth()*0.25/answerkeylength,
           ofGetHeight()/2-2*ofGetWidth()*0.25/answerkeylength,
           ofGetWidth()*0.25/answerkeylength,
           ofGetWidth()*0.25/answerkeylength);
    }
    ofNoFill();
    ofSetColor(255,255,255);
    ofRect(ofGetWidth()*0.75, ofGetHeight()/2-ofGetWidth()*0.25/answerkeylength,
           ofGetWidth()*0.25, ofGetWidth()*0.25/answerkeylength);
    ofFill();
}

void testApp::writetofile(){
    if (calibrationdocumentinput.is_open()){
        calibrationdocumentinput.close();
    }
    calibrationdocumentouput.open(calibrname);
    for (int i=0;i<debouncetimes.size();i++){
        calibrationdocumentouput<<debouncetimes[i]<<" "<<calibrationamounts[i]<<"\n";
    }
    calibrationdocumentouput.close();
}

void testApp::loadfile(){
    while (calibrationdoc.size()!=0){
        calibrationdoc.pop_back();
    }
    calibrationdocumentinput.open(calibrname);
	string line;
	if (calibrationdocumentinput.is_open()){
	    while(calibrationdocumentinput.good()){
            getline(calibrationdocumentinput,line); //gets information line by line
            calibrationdoc.push_back(line); //stores the txt file in a vector
	    }
    }
    if (calibrationdoc.size()>=9){
        while (debouncetimes.size()!=0){
            debouncetimes.pop_back();
        }
        while (calibrationamounts.size()!=0){
            calibrationamounts.pop_back();
        }
        for (int i=0;i<9;i++){
            size_t len=calibrationdoc[i].find(" ");
            debouncetimes.push_back(ofToFloat(calibrationdoc[i].substr(0,len)));
            calibrationamounts.push_back(ofToFloat(calibrationdoc[i].substr(len+1)));
        }
    }
    calibrationdocumentinput.close();
}

void testApp::calibratedebounce(){
    printf("\nPlease press a key\n");
    if (connectedtoArduino){
        bool done=false;
        while (!serialbuffer.empty()){ //empty out the serial buffer, just in case
            serialbuffer.pop_front();
        }
        serial.flush();

        while (!done){
            int byte=0;
            if (serial.available()>0){
                int calibrationstarttime=ofGetElapsedTimeMillis();
                int calibrationendtime=ofGetElapsedTimeMillis();
                done=true;
                byte=serial.readByte();
                int key=byte;

                int i=ofGetElapsedTimeMillis();
                while (i<calibrationstarttime+1000){ //for 1 second
                    i=ofGetElapsedTimeMillis();
                    while (serial.available()>0){ //check if something is in serial port
                        byte=serial.readByte();
                        i=ofGetElapsedTimeMillis();
                        if (byte==key){
                            calibrationendtime=i; //update time if good data
                        }
                    }
                    i=ofGetElapsedTimeMillis();
                }
                while (!serialbuffer.empty()){ //empty out the serial buffer, just in case
                    serialbuffer.pop_front();
                }
                serial.flush(); //flush out serial port, just in case

                //change debounce times as necessary
                for (int i=0;i<keys.size();i++){
                    if (key==keys[i]){
                        debouncetimes[i]*=calibrationamounts[i];
                        debouncetimes[i]+=calibrationendtime-calibrationstarttime; //weighted average the two times
                        debouncetimes[i]/=calibrationamounts[i]+1;                 //This way, it'll average out to the actual bounce time

                        calibrationamounts[i]+=1;
                        break;
                    }
                }
                writetofile();
                printf("\nThank You\n");
            }
        }
    }
    else {
        printf("\n***Connect to Arduino first***\n");
    }
}

void testApp::loadmainmenuimages(){
    buttonimages[0].loadImage("MenuImages/reconnecttoarduino.jpg");
    buttonimages[1].loadImage("MenuImages/startgame.jpg");
    buttonimages[2].loadImage("MenuImages/instructions.jpg");
    buttonimages[3].loadImage("MenuImages/multiplayer.jpg");
    buttonimages[4].loadImage("MenuImages/title.jpg");
    buttonimages[5].loadImage("MenuImages/freeplay.jpg");
    buttonimages[6].loadImage("MenuImages/options.jpg");
    buttonimages[7].loadImage("MenuImages/exitgame.jpg");
    buttonimages[8].loadImage("MenuImages/credits.jpg");
}

void testApp::loadsetupmenuimages(){
    buttonimages[0].clear();
    buttonimages[1].loadImage("MenuImages/up.jpg");
    buttonimages[2].clear();
    buttonimages[3].loadImage("MenuImages/back.jpg");
    buttonimages[5].loadImage("MenuImages/enter.jpg");
    buttonimages[6].clear();
    buttonimages[7].loadImage("MenuImages/down.jpg");
    buttonimages[8].loadImage("MenuImages/backtomenu.jpg");
}

int testApp::determinetextsize(string fontfilename, int width, int height, string stringthing){
    ofTrueTypeFont fonter;
    int size=10;
    fonter.loadFont(fontfilename,size);
    bool done=false;
    if (width==0 && height!=0){
        while (!done){
            fonter.loadFont(fontfilename,size);
            if (fonter.stringHeight(stringthing)==0){
                done=true;
            }
            else if (abs(height-fonter.stringHeight(stringthing))<5){
                done=true;
            }
            else if (fonter.stringHeight(stringthing)<height){
                size++;
                if (size>72){
                    done=true;
                }
            }
            else if (fonter.stringHeight(stringthing)>height){
                size--;
                if (size<1){
                    done=true;
                }
            }
        }
    }
    else if (height==0 && width!=0){
        while (!done){
            fonter.loadFont(fontfilename,size);
            if (fonter.stringWidth(stringthing)==0){
                done=true;
            }
            else if (abs(width-fonter.stringWidth(stringthing))<5){
                done=true;
            }
            else if (fonter.stringWidth(stringthing)<width){
                size++;
                if (size>72){
                    done=true;
                }
            }
            else if (fonter.stringWidth(stringthing)>width){
                size--;
                if (size<1){
                    done=true;
                }
            }
        }
    }
    else if (height!=0 && width!=0){
        ofTrueTypeFont fonter;
        int size1=10;
        int size2=10;
        done=false;
        while (!done){
            fonter.loadFont(fontfilename,size1);
            if (fonter.stringWidth(stringthing)==0){
                done=true;
            }
            else if (abs(width-fonter.stringWidth(stringthing))<5){
                done=true;
            }
            else if (fonter.stringWidth(stringthing)<width){
                size1++;
                if (size1>72){
                    done=true;
                }
            }
            else if (fonter.stringWidth(stringthing)>width){
                size1--;
                if (size1<1){
                    done=true;
                }
            }
        }
        done=false;
        while (!done){
            fonter.loadFont(fontfilename,size2);
            if (fonter.stringHeight(stringthing)==0){
                done=true;
            }
            else if (abs(height-fonter.stringHeight(stringthing))<5){
                done=true;
            }
            else if (fonter.stringHeight(stringthing)<height){
                size2++;
                if (size2>72){
                    done=true;
                }
            }
            else if (fonter.stringHeight(stringthing)>height){
                size2--;
                if (size2<1){
                    done=true;
                }
            }
        }
        if (size1<=size2) return size1;
        else return size2;
    }
    return size;
}

void testApp::setupsetupmenufont(){
    string stringthings;
    for (int i=0;i<setupmenulist.size();i++){
        stringthings+=setupmenulist[i];
        stringthings+="\n";
    }
    sprintf(setupmenustring,stringthings.c_str());
    ofSetColor(0,0,0);
    int size=determinetextsize("KGLesBouquinistesdeParis.ttf", 0,ofGetHeight()/3-10,stringthings);
    setupmenufont.loadFont("KGLesBouquinistesdeParis.ttf",size);
}



