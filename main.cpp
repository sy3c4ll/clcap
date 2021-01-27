#include<opencv2/imgproc.hpp>
#include<opencv2/highgui.hpp>
#include<iostream>
#include<cmath>
#define MAXCAPNUM 4
#define COLOURNUM 9
#define CAPWIDTH  16
#define CAPHEIGHT 25
#define THETA_W 0.62301683130
#define THETA_H 0.38407130834
typedef struct{int lowH,highH,lowS,highS,lowV,highV;} ColourRange;
//주황색, 노란색, 연녹색, 초록색, 진청색, 남색, 진밤색, 빨간색, 핑크색
const ColourRange COLOUR[COLOURNUM]={{  8, 13,100,180,180,230},
                                     { 18, 26,114,193, 87,212},
                                     { 45, 57, 80,150,100,160},
                                     { 78, 80,100,150,100,180},
                                     {102,114,107,216, 30,145},
                                     {100,105,160,210,170,210},
                                     {158,163, 70,130,100,130},
                                     {163,168,120,160,170,230},
                                     {173,178, 80,110,180,210}};
cv::Scalar ScalarHSV(uchar H,uchar S,uchar V){
    cv::Mat tmp(1,1,CV_8UC3,cv::Scalar(H,S,V));
    cvtColor(tmp,tmp,cv::COLOR_HSV2BGR);
    return (cv::Scalar)tmp.at<cv::Vec3b>(0,0);
}
int main(int argc,char **argv){
    cv::VideoCapture cap[MAXCAPNUM];int capNum=0;
    cv::Mat img[MAXCAPNUM],imgHSV[MAXCAPNUM],imgColour[COLOURNUM][MAXCAPNUM];
    cv::Moments mmt[COLOURNUM][MAXCAPNUM];
    cv::Point2i loc[COLOURNUM][MAXCAPNUM];
    cv::Point3i pos[COLOURNUM];
    int frame=0;
    for(size_t i=0;i<100;i++){
        cap[capNum].open(i,cv::CAP_ANY);
        if(cap[capNum].isOpened()){capNum++;printf("Webcam %d detected on channel %d\n",capNum,(int)i);}
        if(capNum>=MAXCAPNUM)break;
    }
    if(capNum<=0){printf("No webcams found, terminating\n");exit(-1);}
    else{
        cv::Mat tmp;
        namedWindow("clcap",cv::WINDOW_AUTOSIZE);
        printf("%d webcams found, checking locations. Press Space to continue\n",capNum);
        printf("Top left\n");while(cv::waitKey(1)!=' ')cap[0].read(tmp);
        printf("Top right\n");while(cv::waitKey(1)!=' ')cap[1].read(tmp);
        printf("Bottom left\n");while(cv::waitKey(1)!=' ')cap[2].read(tmp);
        printf("Bottom right\n");while(cv::waitKey(1)!=' ')cap[3].read(tmp);
    }
    for(;;){
        for(size_t i=0;(int)i<capNum;i++){
            if(!cap[i].read(img[i])){printf("Could not read from media source %d\n",(int)i);exit(-1);}
            cvtColor(img[i],imgHSV[i],cv::COLOR_BGR2HSV);
            for(size_t j=0;j<COLOURNUM;j++){
                inRange(imgHSV[i],cv::Scalar(COLOUR[j].lowH,COLOUR[j].lowS,COLOUR[j].lowV),cv::Scalar(COLOUR[j].highH,COLOUR[j].highS,COLOUR[j].highV),imgColour[j][i]);
                erode(imgColour[j][i],imgColour[j][i],getStructuringElement(cv::MORPH_ELLIPSE,cv::Size(5,5)));
                dilate(imgColour[j][i],imgColour[j][i],getStructuringElement(cv::MORPH_ELLIPSE,cv::Size(5,5)));
                dilate(imgColour[j][i],imgColour[j][i],getStructuringElement(cv::MORPH_ELLIPSE,cv::Size(5,5)));
                erode(imgColour[j][i],imgColour[j][i],getStructuringElement(cv::MORPH_ELLIPSE,cv::Size(5,5)));
                mmt[j][i]=moments(imgColour[j][i]);
                if(mmt[j][i].m00>10000)circle(img[i],loc[j][i]=cv::Point2i(mmt[j][i].m10/mmt[j][i].m00,mmt[j][i].m01/mmt[j][i].m00),20,ScalarHSV((COLOUR[j].lowH+COLOUR[j].highH)/2,(COLOUR[j].lowS+COLOUR[j].highS)/2,(COLOUR[j].lowV+COLOUR[j].highV)/2),4,3);
            }
            imshow("Webcam "+std::to_string(i+1),img[i]);
            if(cv::waitKey(1)=='q'){printf("Session ended by user\n");exit(0);}
        }
        if(frame++%5==0){
            for(size_t i=0;i<COLOURNUM;i++){
                pos[i].z=(img[0].cols*CAPWIDTH/2.0/(loc[i][0].x-loc[i][1].x)/tan(THETA_W)+img[0].cols*CAPWIDTH/2.0/(loc[i][2].x-loc[i][3].x)/tan(THETA_W)+img[0].rows*CAPHEIGHT/2.0/(loc[i][0].y-loc[i][2].y)/tan(THETA_H)+img[0].rows*CAPHEIGHT/2.0/(loc[i][1].y-loc[i][3].y)/tan(THETA_H))/4;
                pos[i].x=(double)img[0].cols*CAPWIDTH*(loc[i][0].x-img[0].cols/2)/(loc[i][0].x-loc[i][1].x)/img[0].cols;
                pos[i].y=(double)img[0].rows*CAPHEIGHT*(loc[i][0].y-img[0].rows/2)/(loc[i][0].y-loc[i][2].y)/img[0].rows;
                printf("Colour %d: x=%2.2d y=%2.2d z=%2.2d\n",(int)i,pos[i].x,pos[i].y,pos[i].z);
            }
            puts("");
        }
    }
    return 0;
}
