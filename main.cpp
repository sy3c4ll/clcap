#include<cstdio>
#include<opencv2/highgui.hpp>
#include<opencv2/imgproc.hpp>
using namespace cv;
typedef struct{int lowB,highB,lowG,highG,lowR,highR;} Obj;
void analyse(Mat &src,Mat &dest,Obj obj){
    inRange(src,Scalar(obj.lowB,obj.lowG,obj.lowR),Scalar(obj.highB,obj.highG,obj.highR),dest);
    erode(dest,dest,getStructuringElement(MORPH_ELLIPSE,Size(5,5)));
    dilate(dest,dest,getStructuringElement(MORPH_ELLIPSE,Size(5,5)));
    dilate(dest,dest,getStructuringElement(MORPH_ELLIPSE,Size(5,5)));
    erode(dest,dest,getStructuringElement(MORPH_ELLIPSE,Size(5,5)));
}
int main(int argc,char **argv){
    VideoCapture cap(0);
    if(!cap.isOpened()){printf("Could not load media source\n");return -1;}
    Obj RED={0,150,0,100,160,255},BLUE={140,255,100,200,0,50};
    Mat img,imgRed,imgBlue;
    for(;;){
        if(!cap.read(img)){printf("Could not read from media source\n");return -1;}
        analyse(img,imgRed,RED);
        analyse(img,imgBlue,BLUE);
        Moments mmtRed=moments(imgRed),mmtBlue=moments(imgBlue);
        if(mmtRed.m00>10000){circle(img,Point2i(mmtRed.m10/mmtRed.m00,mmtRed.m01/mmtRed.m00),20,Scalar(75,50,210),4,3);}
        if(mmtBlue.m00>10000){circle(img,Point2i(mmtBlue.m10/mmtBlue.m00,mmtBlue.m01/mmtBlue.m00),20,Scalar(210,150,25),4,3);}
        imshow("Image",img);
        //imshow("Red Image",imgRed);
        //imshow("Blue Image",imgBlue);
        if(waitKey(1)==27){printf("ESC key pressed by user, quitting...\n");return 0;}
    }
    return 0;
}
