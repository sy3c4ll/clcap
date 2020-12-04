#include<cstdio>
#include<opencv2/highgui.hpp>
#include<opencv2/imgproc.hpp>
#define MAXCAPNUM 4
#define COLOURNUM 9
using namespace cv;
typedef struct{int lowH,highH,lowS,highS,lowV,highV;} Colour;
//주황색, 노란색, 연녹색, 초록색, 진청색, 남색, 진밤색, 빨간색, 핑크색
const Colour COLOUR[COLOURNUM]={{8,13,100,180,180,230},
                                {21,26,180,250,180,240},
                                {45,57,80,150,100,160},
                                {78,80,100,150,100,180},
                                {100,105,100,150,180,200},
                                {100,105,160,210,170,210},
                                {158,163,70,130,100,130},
                                {163,168,120,160,170,230},
                                {173,178,80,110,180,210}};
Scalar ScalarHSV(uchar H,uchar S,uchar V){
    Mat tmp(1,1,CV_8UC3,Scalar(H,S,V));
    cvtColor(tmp,tmp,COLOR_HSV2BGR);
    return (Scalar)tmp.at<Vec3b>(0,0);
}
int main(int argc,char **argv){
    VideoCapture cap[MAXCAPNUM];int capNum=0;
    Mat img[MAXCAPNUM],imgHSV[MAXCAPNUM],imgColour[COLOURNUM][MAXCAPNUM];
    Moments mmt[COLOURNUM][MAXCAPNUM];Point2i loc[COLOURNUM][MAXCAPNUM];
    for(size_t i=0;i<100;i++){
        cap[capNum].open(i,CAP_ANY);
        if(cap[capNum].isOpened()){capNum++;printf("Webcam %d detected. Port %d\n",capNum,(int)i);}
        if(capNum>=MAXCAPNUM)break;
    }
    if(capNum<=0){printf("No webcams found, terminating.\n");return -1;}
    else printf("%d webcams found.\n",capNum);
    for(;;)for(size_t i=0;(int)i<capNum;i++){
        if(!cap[i].read(img[i])){printf("Could not read from media source %d\n",(int)i);return -1;}
        cvtColor(img[i],imgHSV[i],COLOR_BGR2HSV);
        for(size_t j=0;j<COLOURNUM;j++){
            inRange(imgHSV[i],Scalar(COLOUR[j].lowH,COLOUR[j].lowS,COLOUR[j].lowV),Scalar(COLOUR[j].highH,COLOUR[j].highS,COLOUR[j].highV),imgColour[j][i]);
            erode(imgColour[j][i],imgColour[j][i],getStructuringElement(MORPH_ELLIPSE,Size(5,5)));
            dilate(imgColour[j][i],imgColour[j][i],getStructuringElement(MORPH_ELLIPSE,Size(5,5)));
            dilate(imgColour[j][i],imgColour[j][i],getStructuringElement(MORPH_ELLIPSE,Size(5,5)));
            erode(imgColour[j][i],imgColour[j][i],getStructuringElement(MORPH_ELLIPSE,Size(5,5)));
            mmt[j][i]=moments(imgColour[j][i]);
            if(mmt[j][i].m00>10000)circle(img[i],loc[j][i]=Point2i(mmt[j][i].m10/mmt[j][i].m00,mmt[j][i].m01/mmt[j][i].m00),20,ScalarHSV((COLOUR[j].lowH+COLOUR[j].highH)/2,(COLOUR[j].lowS+COLOUR[j].highS)/2,(COLOUR[j].lowV+COLOUR[j].highV)/2),4,3);
        }
        imshow("Image "+std::to_string(i+1),img[i]);
        if(waitKey(1)=='q'){printf("Session ended by user.\n");return 0;}
    }
    return 0;
}
