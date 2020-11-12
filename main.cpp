#include<cstdio>
#include<opencv2/highgui.hpp>
#include<opencv2/imgproc.hpp>
using namespace cv;
typedef struct{int lowH,highH,lowS,highS,lowV,highV;} Obj;
void analyse(Mat &src,Mat &dest,Obj obj){
    inRange(src,Scalar(obj.lowH,obj.lowS,obj.lowV),Scalar(obj.highH,obj.highS,obj.highV),dest);
    erode(dest,dest,getStructuringElement(MORPH_ELLIPSE,Size(5,5)));
    dilate(dest,dest,getStructuringElement(MORPH_ELLIPSE,Size(5,5)));
    dilate(dest,dest,getStructuringElement(MORPH_ELLIPSE,Size(5,5)));
    erode(dest,dest,getStructuringElement(MORPH_ELLIPSE,Size(5,5)));
}
int main(int argc,char **argv){
    VideoCapture cap0(0),cap1(2),cap2(5);
    if(!cap0.isOpened()||!cap1.isOpened()||!cap2.isOpened()){printf("Could not load media source\n");return -1;}
    Obj RED={170,180,115,205,115,240},BLUE={0,0,0,0,0,0};//TODO
    Mat img[3],imgHSV[3],imgRed[3],imgBlue[3];
    Moments mmtRed,mmtBlue;
    for(;;){
        if(!cap0.read(img[0])||!cap1.read(img[1])||!cap2.read(img[2])){printf("Could not read from media source\n");return -1;}
        for(size_t i=0;i<3;i++){
            cvtColor(img[i],imgHSV[i],COLOR_BGR2HSV);
            analyse(imgHSV[i],imgRed[i],RED);
            analyse(imgHSV[i],imgBlue[i],BLUE);
            mmtRed=moments(imgRed[i]);mmtBlue=moments(imgBlue[i]);
            if(mmtRed.m00>10000){circle(img[i],Point2i(mmtRed.m10/mmtRed.m00,mmtRed.m01/mmtRed.m00),20,Scalar(75,50,210),4,3);}
            if(mmtBlue.m00>10000){circle(img[i],Point2i(mmtBlue.m10/mmtBlue.m00,mmtBlue.m01/mmtBlue.m00),20,Scalar(210,150,25),4,3);}
        }
        imshow("Image1",img[0]);imshow("Image2",img[1]);imshow("Image3",img[2]);
        if(waitKey(1)==27){printf("ESC key pressed by user, quitting...\n");return 0;}
    }
    return 0;
}
