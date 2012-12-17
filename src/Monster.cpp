#include "Monster.h"

void Monster::setup(){
    actualcenter.x=ofGetWidth()/2.0;
    actualcenter.y=ofGetHeight()/2.0;

    loaddirectories();
    /*
    loadimages();
    setupbodylocations();
    setuppivotpoints();
    */
    setupallpoints(0);
    setuproadpoints();

    for (int i=0;i<bodyparts.size();i++){
        bodypartangles.push_back(0);
    }
    int ymax,ymin,xmax,xmin;
    ymax=0; xmax=0;
    ymin=1000000; xmin=1000000;
    for (int i=0;i<bodyparts.size();i++){
        if (centers[i].x-bodyparts[i].getWidth()<xmin){
            xmin=centers[i].x-bodyparts[i].getWidth();
        }
        if (centers[i].x+bodyparts[i].getWidth()>xmax){
            xmax=centers[i].x+bodyparts[i].getWidth();
        }
        if (centers[i].y-bodyparts[i].getHeight()<ymin){
            ymin=centers[i].y-bodyparts[i].getHeight();
        }
        if (centers[i].y+bodyparts[i].getHeight()>ymax){
            ymax=centers[i].y+bodyparts[i].getHeight();
        }
    }
    totalwidth=xmax-xmin;
    totalheight=ymax-ymin;
    definingmonster=false;
}

void Monster::update(float currenttime, float levelduration){
    scalemonster(currenttime, levelduration);
    moveactualcenter(currenttime, levelduration);
}

void Monster::draw(){
    ofSetColor(255,255,255);
    ofPushMatrix();
    ofTranslate(actualcenter.x,actualcenter.y);
    ofScale(scalesize,scalesize,1);
        ofPushMatrix();
        ofTranslate(pivotpoints[0].x,pivotpoints[0].y,0); //this should be about pivot points, not centers
        ofRotateZ(bodypartangles[0]);
        ofTranslate(-pivotpoints[0].x,-pivotpoints[0].y,0);
        bodyparts[0].draw((centers[0].x-bodyparts[0].getWidth()/2),(centers[0].y-bodyparts[0].getHeight()/2));
        ofPopMatrix();

        //torso will never rotate
        bodyparts[1].draw((centers[1].x-bodyparts[1].getWidth()/2),(centers[1].y-bodyparts[1].getHeight()/2));

        //left arm
        ofPushMatrix();
        ofTranslate(pivotpoints[2].x,pivotpoints[2].y,0);
        ofRotateZ(bodypartangles[2]);
        ofTranslate(-pivotpoints[2].x,-pivotpoints[2].y,0);
        bodyparts[2].draw((centers[2].x-bodyparts[2].getWidth()/2),(centers[2].y-bodyparts[2].getHeight()/2));
            //rotate leftarmbottom in a rotated reference frame
            ofPushMatrix();
            ofTranslate(pivotpoints[3].x,pivotpoints[3].y,0);
            ofRotateZ(bodypartangles[3]);
            ofTranslate(-pivotpoints[3].x,-pivotpoints[3].y,0);
            bodyparts[3].draw((centers[3].x-bodyparts[3].getWidth()/2),(centers[3].y-bodyparts[3].getHeight()/2));
                //rotate leftweapon in a rotated reference frame
                ofPushMatrix();
                ofTranslate(pivotpoints[4].x,pivotpoints[4].y,0);
                ofRotateZ(bodypartangles[4]);
                ofTranslate(-pivotpoints[4].x,-pivotpoints[4].y,0);
                bodyparts[4].draw((centers[4].x-bodyparts[4].getWidth()/2),(centers[4].y-bodyparts[4].getHeight()/2));
                ofPopMatrix();
            ofPopMatrix();
        ofPopMatrix();

        //right arm
        ofPushMatrix();
        ofTranslate(pivotpoints[5].x,pivotpoints[5].y,0);
        ofRotateZ(bodypartangles[5]);
        ofTranslate(-pivotpoints[5].x,-pivotpoints[5].y,0);
        bodyparts[5].draw(centers[5].x-bodyparts[5].getWidth()/2,centers[5].y-bodyparts[5].getHeight()/2);
            //rotate leftarmbottom in a rotated reference frame
            ofPushMatrix();
            ofTranslate(pivotpoints[6].x,pivotpoints[6].y,0);
            ofRotateZ(bodypartangles[6]);
            ofTranslate(-pivotpoints[6].x,-pivotpoints[6].y,0);
            bodyparts[6].draw(centers[6].x-bodyparts[6].getWidth()/2,centers[6].y-bodyparts[6].getHeight()/2);
                //rotate leftweapon in a rotated reference frame
                ofPushMatrix();
                ofTranslate(pivotpoints[7].x,pivotpoints[7].y,0);
                ofRotateZ(bodypartangles[7]);
                ofTranslate(-pivotpoints[7].x,-pivotpoints[7].y,0);
                bodyparts[7].draw(centers[7].x-bodyparts[7].getWidth()/2,centers[7].y-bodyparts[7].getHeight()/2);
                ofPopMatrix();
            ofPopMatrix();
        ofPopMatrix();

        //leftleg
        ofPushMatrix();
        ofTranslate(pivotpoints[8].x,pivotpoints[8].y,0);
        ofRotateZ(bodypartangles[8]);
        ofTranslate(-pivotpoints[8].x,-pivotpoints[8].y,0);
        bodyparts[8].draw(centers[8].x-bodyparts[8].getWidth()/2,centers[8].y-bodyparts[8].getHeight()/2);
            //rotate leftshin in a rotated reference frame
            ofPushMatrix();
            ofTranslate(pivotpoints[9].x,pivotpoints[9].y,0);
            ofRotateZ(bodypartangles[9]);
            ofTranslate(-pivotpoints[9].x,-pivotpoints[9].y,0);
            bodyparts[9].draw(centers[9].x-bodyparts[9].getWidth()/2,centers[9].y-bodyparts[9].getHeight()/2);
            ofPopMatrix();
        ofPopMatrix();

        //right leg
        ofPushMatrix();
        ofTranslate(pivotpoints[10].x,pivotpoints[10].y,0);
        ofRotateZ(bodypartangles[10]);
        ofTranslate(-pivotpoints[10].x,-pivotpoints[10].y,0);
        bodyparts[10].draw(centers[10].x-bodyparts[10].getWidth()/2,centers[10].y-bodyparts[10].getHeight()/2);
            //rotate rightshin in a rotated reference frame
            ofPushMatrix();
            ofTranslate(pivotpoints[11].x,pivotpoints[11].y,0);
            ofRotateZ(bodypartangles[11]);
            ofTranslate(-pivotpoints[11].x,-pivotpoints[11].y,0);
            bodyparts[11].draw(centers[11].x-bodyparts[11].getWidth()/2,centers[11].y-bodyparts[11].getHeight()/2);
            ofPopMatrix();
        ofPopMatrix();

    ofPopMatrix();
}

void Monster::moveactualcenter(float currenttime, float levelduration){
    float percentcomplete=currenttime/levelduration;
    if (percentcomplete<1.0){
        float currentdistance=percentcomplete*totaldistancetraveled;
        for (int i=1;i<roadpoints.size();i++){
            float dist=ofDist(roadpoints[i-1].x,roadpoints[i-1].y,roadpoints[i].x,roadpoints[i].y);
            if (dist<=currentdistance){
                currentdistance-=dist;
            }
            else{
                currentdistance/=dist;
                actualcenter.x=(roadpoints[i-1].x+currentdistance*(roadpoints[i].x-roadpoints[i-1].x))*ofGetWidth()*0.75;
                if (!definingmonster){
                    actualcenter.y=(roadpoints[i-1].y+currentdistance*(roadpoints[i].y-roadpoints[i-1].y))*ofGetHeight()-centers[9].y*scalesize;
                }
                else {
                    actualcenter.y=(roadpoints[i-1].y+currentdistance*(roadpoints[i].y-roadpoints[i-1].y))*ofGetHeight();
                }
                // monster doesnt stay on the road
                break;
            }
        }
    }
}

void Monster::scalemonster(float currenttime, float levelduration){
    if (currenttime<levelduration){
        if (totalwidth/screenwidth<totalheight/screenheight){
            scalesize=(0.5+currenttime/levelduration*0.5)*screenwidth/totalwidth/2;
        }
        else {
            scalesize=(0.5+currenttime/levelduration*0.5)*screenheight/totalheight/2;
        }
    }
    else {
        scalesize=1.0;
    }
}

void Monster::loaddirectories(){
    DIR *pDIR;
    struct dirent *entry;
    if (pDIR=opendir("./data")){
        while (entry=readdir(pDIR)){
            if (strcmp(entry->d_name, ".")!=0 && strcmp(entry->d_name,"..")!=0){
                if ((entry->d_name)[0]=='_'){
                        char stringthing[260];
                        sprintf(stringthing,"%s",(entry->d_name));
                        directories.push_back(stringthing);
                    }
            }
        }

        closedir(pDIR);
    }
}

void Monster::loadimages(){
    ofImage currentimage;
    currentimage.loadImage("head.png");
    bodyparts.push_back(currentimage);
    currentimage.loadImage("torso.png");
    bodyparts.push_back(currentimage);
    currentimage.loadImage("leftarmtop.png");
    bodyparts.push_back(currentimage);
    currentimage.loadImage("leftarmbottom.png");
    bodyparts.push_back(currentimage);
    currentimage.loadImage("leftweapon.png");
    bodyparts.push_back(currentimage);
    currentimage.loadImage("rightarmtop.png");
    bodyparts.push_back(currentimage);
    currentimage.loadImage("rightarmbottom.png");
    bodyparts.push_back(currentimage);
    currentimage.loadImage("rightweapon.png");
    bodyparts.push_back(currentimage);
    currentimage.loadImage("leftthigh.png");
    bodyparts.push_back(currentimage);
    currentimage.loadImage("leftshin.png");
    bodyparts.push_back(currentimage);
    currentimage.loadImage("rightthigh.png");
    bodyparts.push_back(currentimage);
    currentimage.loadImage("rightshin.png");
    bodyparts.push_back(currentimage);

}

void Monster::loadimages(int dir){
    ofImage currentimage;
    char stringthing[260];

    sprintf(stringthing,"%s/%s",directories[dir].c_str(),"head.png");
    currentimage.loadImage(stringthing);
    bodyparts.push_back(currentimage);

    sprintf(stringthing,"%s/%s",directories[dir].c_str(),"torso.png");
    currentimage.loadImage(stringthing);
    bodyparts.push_back(currentimage);

    sprintf(stringthing,"%s/%s",directories[dir].c_str(),"leftarmtop.png");
    currentimage.loadImage(stringthing);
    bodyparts.push_back(currentimage);

    sprintf(stringthing,"%s/%s",directories[dir].c_str(),"leftarmbottom.png");
    currentimage.loadImage(stringthing);
    bodyparts.push_back(currentimage);

    sprintf(stringthing,"%s/%s",directories[dir].c_str(),"leftweapon.png");
    currentimage.loadImage(stringthing);
    bodyparts.push_back(currentimage);

    sprintf(stringthing,"%s/%s",directories[dir].c_str(),"rightarmtop.png");
    currentimage.loadImage(stringthing);
    bodyparts.push_back(currentimage);

    sprintf(stringthing,"%s/%s",directories[dir].c_str(),"rightarmbottom.png");
    currentimage.loadImage(stringthing);
    bodyparts.push_back(currentimage);

    sprintf(stringthing,"%s/%s",directories[dir].c_str(),"rightweapon.png");
    currentimage.loadImage(stringthing);
    bodyparts.push_back(currentimage);

    sprintf(stringthing,"%s/%s",directories[dir].c_str(),"leftthigh.png");
    currentimage.loadImage(stringthing);
    bodyparts.push_back(currentimage);

    sprintf(stringthing,"%s/%s",directories[dir].c_str(),"leftshin.png");
    currentimage.loadImage(stringthing);
    bodyparts.push_back(currentimage);

    sprintf(stringthing,"%s/%s",directories[dir].c_str(),"rightthigh.png");
    currentimage.loadImage(stringthing);
    bodyparts.push_back(currentimage);

    sprintf(stringthing,"%s/%s",directories[dir].c_str(),"rightshin.png");
    currentimage.loadImage(stringthing);
    bodyparts.push_back(currentimage);
}

void Monster::writetofiles(int dir){
    if (documentinput.is_open()){
        documentinput.close();
    }
    char stringthing[260];
    sprintf(stringthing,"data/%s/centers.txt",directories[dir].c_str());
    documentouput.open(stringthing);
    for (int i=0;i<centers.size();i++){
        documentouput<<centers[i].x<<"\n";
        documentouput<<centers[i].y<<"\n";
    }
    documentouput.close();

    sprintf(stringthing,"data/%s/pivotpoints.txt",directories[dir].c_str());
    documentouput.open(stringthing);
    for (int i=0;i<pivotpoints.size();i++){
        documentouput<<pivotpoints[i].x<<"\n";
        documentouput<<pivotpoints[i].y<<"\n";
    }
    documentouput.close();
}

void Monster::setupbodylocations(){
    ofPoint point;
    point.x=53.000000;
    point.y=-70.000000;
    centers.push_back(point);
    point.x=0.000000;
    point.y=0.000000;
    centers.push_back(point);
    point.x=-45.000000;
    point.y=-55.000000;
    centers.push_back(point);
    point.x=-141.000000;
    point.y=-34.000000;
    centers.push_back(point);
    point.x=-181.000000;
    point.y=-61.000000;
    centers.push_back(point);
    point.x=116.000000;
    point.y=-35.000000;
    centers.push_back(point);
    point.x=168.000000;
    point.y=-23.000000;
    centers.push_back(point);
    point.x=219.000000;
    point.y=-75.000000;
    centers.push_back(point);
    point.x=-58.000000;
    point.y=89.000000;
    centers.push_back(point);
    point.x=-53.000000;
    point.y=148.000000;
    centers.push_back(point);
    point.x=77.000000;
    point.y=76.000000;
    centers.push_back(point);
    point.x=89.000000;
    point.y=129.000000;
    centers.push_back(point);
}

void Monster::setupbodylocations(char* filename){
    while (doc.size()!=0){
        doc.pop_back();
    }
    while (centers.size()!=0){
        centers.pop_back();
    }
    documentinput.open(filename);
	string line;
	if (documentinput.is_open()){
	    while(documentinput.good()){
            getline(documentinput,line); //gets information line by line
            doc.push_back(line); //stores the txt file in a vector
	    }
    }
    ofPoint point;
    for (int i=0;i<doc.size()/2;i++){
        point.x=ofToFloat(doc[2*i]);
        point.y=ofToFloat(doc[2*i+1]);
        centers.push_back(point);
    }
    documentinput.close();

    while (centers.size()<12){
        point.x=0;
        point.y=0;
        centers.push_back(point);
    }
}

void Monster::setuppivotpoints(){
    ofPoint point;
    //temporary
    point.x=34.000000;
    point.y=-24.000000;
    pivotpoints.push_back(point);
    point.x=0.000000;
    point.y=0.000000;
    pivotpoints.push_back(point);
    point.x=-25.000000;
    point.y=-40.000000;
    pivotpoints.push_back(point);
    point.x=-64.000000;
    point.y=-34.000000;
    pivotpoints.push_back(point);
    point.x=-116.000000;
    point.y=-23.000000;
    pivotpoints.push_back(point);
    point.x=71.000000;
    point.y=-27.000000;
    pivotpoints.push_back(point);
    point.x=103.000000;
    point.y=-28.000000;
    pivotpoints.push_back(point);
    point.x=139.000000;
    point.y=-16.000000;
    pivotpoints.push_back(point);
    point.x=-45.000000;
    point.y=43.000000;
    pivotpoints.push_back(point);
    point.x=-39.000000;
    point.y=98.000000;
    pivotpoints.push_back(point);
    point.x=52.000000;
    point.y=50.000000;
    pivotpoints.push_back(point);
    point.x=69.000000;
    point.y=85.000000;
    pivotpoints.push_back(point);
}

void Monster::setuppivotpoints(char* filename){
    while (doc.size()!=0){
        doc.pop_back();
    }
    while (pivotpoints.size()!=0){
        pivotpoints.pop_back();
    }
    documentinput.open(filename);
	string line;
	if (documentinput.is_open()){
	    while(documentinput.good()){
            getline(documentinput,line); //gets information line by line
            doc.push_back(line); //stores the txt file in a vector
	    }
    }
    ofPoint point;
    for (int i=0;i<doc.size()/2;i++){
        point.x=ofToFloat(doc[2*i]);
        point.y=ofToFloat(doc[2*i+1]);
        pivotpoints.push_back(point);
    }
    documentinput.close();

    while (pivotpoints.size()<12){
        point.x=0;
        point.y=0;
        pivotpoints.push_back(point);
    }
}

void Monster::setupallpoints(int dir){
    loadimages(dir);
    char stringthing[260];
    sprintf(stringthing,"data/%s/%s",directories[dir].c_str(),"centers.txt");
    setupbodylocations(stringthing);
    sprintf(stringthing,"data/%s/%s",directories[dir].c_str(),"pivotpoints.txt");
    setuppivotpoints(stringthing);
}

void Monster::setuproadpoints(){
    //These points are relative to screen dimensions
    ofPoint point;
    point.x=0.535156;
    point.y=0.600260;
    roadpoints.push_back(point);
    point.x=0.480469;
    point.y=0.619792;
    roadpoints.push_back(point);
    point.x=0.576823;
    point.y=0.692708;
    roadpoints.push_back(point);
    point.x=0.354167;
    point.y=0.816406;
    roadpoints.push_back(point);
    point.x=0.490885;
    point.y=0.917969;
    roadpoints.push_back(point);

    totaldistancetraveled=0;
    for (int i=1;i<roadpoints.size();i++){
        totaldistancetraveled+=ofDist(roadpoints[i-1].x,roadpoints[i-1].y,roadpoints[i].x,roadpoints[i].y);
    }
}

