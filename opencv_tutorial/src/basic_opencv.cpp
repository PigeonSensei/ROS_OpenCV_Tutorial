#include <ros/ros.h>
#include <cv_bridge/cv_bridge.h>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;

int main(int argc, char **argv)
{
  ros::init(argc, argv, "basic_opencv");
  ros::NodeHandle nh;

  cout << "OpenCV version : " << CV_VERSION << endl;
  cout << "Major version : " << CV_MAJOR_VERSION << endl;
  ROS_INFO("Hello world!");

  double width;
  double height;
  double exposure;
  double brightness; // 변수 선언

  Mat img; // 행렬 선언

  VideoCapture vc(0); // 카메라 번호 선언

  width = vc.get(CAP_PROP_FRAME_WIDTH);
  height = vc.get(CAP_PROP_FRAME_HEIGHT);
  exposure = vc.get(CAP_PROP_EXPOSURE);
  brightness = vc.get(CAP_PROP_BRIGHTNESS); // 카메라 정보 얻기

  cout << "width : " << width << endl;
  cout << "height : " << height << endl;
  cout << "exposure : " << exposure << endl;
  cout << "brightness : " << brightness << endl; // 카메라 정보 출력

  namedWindow("video", 1); // 윈도우 창 만들기

  ros::Rate loop_rate(60); // 60Hz

  while(ros::ok()){
    vc >> img; // vc의 카메라 이미지를 img 행렬에 대입

    flip(img, img, 1); // 영상 이미지 좌우 반전

    imshow("video", img); // 영상 출력

    if(waitKey(10) == 27) break; // esc 누르면 종료
    loop_rate.sleep();
    ros::spinOnce();
  }

  return 0;


}
