#include<cstdio>
#include<opencv2/highgui.hpp>
#include<opencv2/imgproc.hpp>
#define MAXCAPNUM 1
#define COLOURNUM 9
using namespace cv;
typedef struct{int lowH,highH,lowS,highS,lowV,highV;} Colour;
void analyse(Mat &src,Mat &dest,Colour obj){
    inRange(src,Scalar(obj.lowH,obj.lowS,obj.lowV),Scalar(obj.highH,obj.highS,obj.highV),dest);
    erode(dest,dest,getStructuringElement(MORPH_ELLIPSE,Size(5,5)));
    dilate(dest,dest,getStructuringElement(MORPH_ELLIPSE,Size(5,5)));
    dilate(dest,dest,getStructuringElement(MORPH_ELLIPSE,Size(5,5)));
    erode(dest,dest,getStructuringElement(MORPH_ELLIPSE,Size(5,5)));
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
    Colour COLOUR[COLOURNUM]={};
    FILE *f=fopen("data.csv","r");
    if(f==NULL){printf("Cannot open data file. Quitting...\n");return -1;}
    fscanf(f,",highH,lowH,highS,lowS,highV,lowV");
    for(int i=0;i<COLOURNUM;i++)fscanf(f,"%*s,%d,%d,%d,%d,%d,%d",&COLOUR[i].lowH,&COLOUR[i].highH,&COLOUR[i].lowS,&COLOUR[i].highS,&COLOUR[i].lowV,&COLOUR[i].highV);
    fclose(f);
    Mat img[MAXCAPNUM],imgHSV[MAXCAPNUM],imgColour[COLOURNUM][MAXCAPNUM];
    Moments mmt[COLOURNUM];
    for(;;)for(size_t i=0;(int)i<capNum;i++){
        if(!cap[i].read(img[i])){printf("Could not read from media source %d\n",(int)i);return -1;}
        cvtColor(img[i],imgHSV[i],COLOR_BGR2HSV);
        for(size_t j=0;j<COLOURNUM;j++){
            analyse(imgHSV[i],imgColour[j][i],COLOUR[i]);
            mmt[j]=moments(imgColour[j][i]);
            if(mmt[j].m00>10000){circle(img[i],Point2i(mmt[j].m10/mmt[j].m00,mmt[j].m01/mmt[j].m00),20,Scalar(75,50,210),4,3);}
        }
        imshow("Image"+std::to_string(i),img[i]);
        if(waitKey(1)==27){printf("ESC key pressed by user. Quitting...\n");return 0;}
    }
    return 0;
}
