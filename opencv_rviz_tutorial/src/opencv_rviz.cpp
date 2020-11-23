#include <ros/ros.h>
#include <cv_bridge/cv_bridge.h>
#include <opencv2/highgui/highgui.hpp>

#include <sensor_msgs/Image.h>
#include <sensor_msgs/image_encodings.h>

#include <termios.h>

using namespace cv;
using namespace std;

int KeybordInput()
{
  struct termios org_term;

  char input_key = 0;

  tcgetattr(STDIN_FILENO, &org_term);

  struct termios new_term = org_term;

  new_term.c_lflag &= ~(ECHO | ICANON);

  new_term.c_cc[VMIN] = 0;
  new_term.c_cc[VTIME] = 0;

  tcsetattr(STDIN_FILENO, TCSANOW, &new_term);

  read(STDIN_FILENO, &input_key, 1);

  tcsetattr(STDIN_FILENO, TCSANOW, &org_term);

  return input_key;
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "opencv_rviz_node");
  ros::NodeHandle nh;

  sensor_msgs::Image opencv_sensor_image;
  cv_bridge::CvImage cv_bridge;
  ros::Publisher opencv_sensor_image_publish = nh.advertise<sensor_msgs::Image>("opencv_image", 1000);

  std_msgs::Header header;
  ros::Rate loop_rate(60); // 60Hz

  cout << "OpenCV version : " << CV_VERSION << endl;
  cout << "Major version : " << CV_MAJOR_VERSION << endl;

  ROS_INFO("opencv_rviz_node open");

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

//  namedWindow("video", 1); // 윈도우 창 만들기



  while(ros::ok()){
    vc >> img; // vc의 카메라 이미지를 img 행렬에 대입

    flip(img, img, 1); // 영상 이미지 좌우 반전


    cv_bridge = cv_bridge::CvImage(header,sensor_msgs::image_encodings::BGR8,img);
    cv_bridge.toImageMsg(opencv_sensor_image);

    opencv_sensor_image.header.stamp = ros::Time::now();
    opencv_sensor_image.header.frame_id = "opencv_rviz_frame";

    opencv_sensor_image_publish.publish(opencv_sensor_image);

//    imshow("video", img); // 영상 출력

    if(KeybordInput() == 27) break; // esc 누르면 종료
    loop_rate.sleep();
    ros::spinOnce();
  }

  ROS_INFO("opencv_rviz_node close");
  return 0;

}
