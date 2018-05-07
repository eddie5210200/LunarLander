#include "ofMain.h"

class Camera {
public:
	void setup();
	void camera_begin();
	void camera_end();
	//Cameras
	ofEasyCam cam, far_view;
	ofCamera front_view, down_view;
	ofVec3f spacecraft, far_position;
	bool setTarget;
	int camera_number; // 0-2 is normal camera, 3 is easycam
	Camera() {}
};
