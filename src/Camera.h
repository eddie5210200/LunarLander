#include "ofApp.h"

class Camera {
public:
	Camera();
	void camera_begin(int);
	void camera_end(int);
	//Cameras
	ofEasyCam cam;
	ofCamera front_view, down_view, far_view;
	ofVec3f spacecraft, far_position;
	bool setTarget;
	int camera_number; // 0-2 is normal camera, 3 is easycam
};