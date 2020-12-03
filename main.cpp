#include<cstdio>
#include<opencv2/highgui.hpp>
#include<opencv2/imgproc.hpp>
#define MAXCAPNUM 1
#define COLOURNUM 2
#define SEARCHWINDOW 200
#define max(a,b) ((a)>(b)?(a):(b))
#define min(a,b) ((a)<(b)?(a):(b))
using namespace cv;
typedef struct{int lowH,highH,lowS,highS,lowV,highV;} Colour;
inline void analyse(Mat &img,Colour obj){
    inRange(img,Scalar(obj.lowH,obj.lowS,obj.lowV),Scalar(obj.highH,obj.highS,obj.highV),img);
    erode(img,img,getStructuringElement(MORPH_ELLIPSE,Size(5,5)));
    dilate(img,img,getStructuringElement(MORPH_ELLIPSE,Size(5,5)));
    dilate(img,img,getStructuringElement(MORPH_ELLIPSE,Size(5,5)));
    erode(img,img,getStructuringElement(MORPH_ELLIPSE,Size(5,5)));
}
int main(int argc,char **argv){
    VideoCapture cap[MAXCAPNUM];int capNum=0;
    for(size_t i=0;i<100;i++){
        cap[capNum].open(i,CAP_ANY);
        if(cap[capNum].isOpened()){capNum++;printf("Webcam %d detected. Port %d\n",capNum,(int)i);}
        if(capNum>=MAXCAPNUM)break;
        if(waitKey(1)==27){printf("Stopping search...\n");break;}
    }
    if(capNum<=0){printf("No cameras found. Quitting...\n");return -1;}
    else printf("%d webcams found.\n",capNum);
    //{20,55,84},{22,50,85}주황색{18,62,74},{20,54,75}
    //{47,89,89},{48,88,89}노란색{46,82,76},{46,82,76}
    //{95,54,53},{95,49,59}연녹색{112,43,42},{107,39,47}
    //{137,52,54},{139,49,59}초록색{150,47,51},{150,47,51}
    //{203,46,77},{204,52,76}진청색{201,50,76},{202,51,76}
    //{206,69,77},{206,66,80}남색{206,81,71},{205,80,72}
    //{324,40,44},{326,38,45}진밤색{319,33,41},{318,32,42}
    //{332,57,83},{331,54,84}빨간색{328,55,71},{329,53,71}
    //{351,39,78},{352,36,79}핑크색{349,36,75},{347,35,74}
    //0: RED, 1: BLUE
    Colour COLOUR[COLOURNUM]={{170,180,115,205,115,240},{100,120,120,250,140,220}};
    Mat img[MAXCAPNUM],imgHSV[MAXCAPNUM],imgColour[COLOURNUM][MAXCAPNUM];
    Moments mmt[COLOURNUM];
    int prevX[COLOURNUM][MAXCAPNUM]={},prevY[COLOURNUM][MAXCAPNUM]={};
    bool prevExists[COLOURNUM][MAXCAPNUM]={};
    for(;;)for(size_t i=0;(int)i<capNum;i++){
        if(!cap[i].read(img[i])){printf("Could not read from media source %d\n",(int)i);return -1;}
        cvtColor(img[i],imgHSV[i],COLOR_BGR2HSV);
        for(size_t j=0;j<COLOURNUM;j++){
            if(prevExists[j][i])imgColour[j][i]=imgHSV[i](Range(max(prevX[j][i]-SEARCHWINDOW,0),min(prevX[j][i]+SEARCHWINDOW,imgHSV[i].cols)),Range(max(prevY[j][i]-SEARCHWINDOW,0),min(prevY[j][i]+SEARCHWINDOW,imgHSV[i].rows)));
            else imgColour[j][i]=imgHSV[i];
            analyse(imgColour[j][i],COLOUR[j]);
            mmt[j]=moments(imgColour[j][i]);
            if(mmt[j].m00>10000){
                prevX[j][i]+=mmt[j].m10/mmt[j].m00-SEARCHWINDOW;
                prevY[j][i]+=mmt[j].m01/mmt[j].m00-SEARCHWINDOW;
                circle(img[i],Point2i(prevX[j][i],prevY[j][i]),20,Scalar(75,50,210),4,3);
                prevExists[j][i]=true;
            }
        }
        imshow("Image "+std::to_string(i),img[i]);
        if(waitKey(1)==27){printf("ESC key pressed by user. Quitting...\n");return 0;}
    }
    return 0;
}
