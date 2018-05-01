#include "camera.h"
Camera::Camera() {
	spacecraft = ofVec3f(0, 0, 0);
	far_position = ofVec3f(0, 0, 0);

	cam.setDistance(10);
	cam.setNearClip(.1);
	cam.setFov(65.5);

	front_view.setNearClip(.1);
	front_view.setFov(65.5);

	down_view.setNearClip(.1);
	down_view.setFov(65.5);

	far_view.setNearClip(1);
	far_view.setFarClip(2000);
	far_view.setFov(65.5);

	camera_number = 0;
	setTarget = false;
}
void camera::camera_begin(int cam_number) {
	//set and start camera position
	switch (camera_number) {
	case 0:
		ofTranslate(spacecraft);
		front_view.setPosition(0, 0, 0);
		front_view.begin();
		break;
	case 1:
		ofTranslate(spacecraft);
		ofRotateY(90); //or 270 to look downward
		down_view.setPosition(0, 0, 0);
		down_view.begin();
		break;
	case 2:
		ofTranslate(far_position);
		front_view.setPosition(0, 0, 0);
		far_view.begin();
		break;
	case 3:
		if (setTarget) {
			cam.setTarget(spacecraft);
			setTarget = false;
		}
		cam.begin();
		break;
	}
}
void camera::camera_end(int cam_number) {
	switch (camera_number) {
	case 0:
		front_view.end();
		break;
	case 1:
		down_view.end();
		break;
	case 2:
		far_view.end();
		break;
	case 3:
		cam.end();
		break;
	}
}