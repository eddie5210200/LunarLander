#include "Camera.h"

void Camera::setup() {
	spacecraft = ofVec3f(0, 5, 0);
	far_position = ofVec3f(40, 10, 0);

	cam.setDistance(10);
	cam.setNearClip(.1);
	cam.setFov(65.5);
	cam.disableMouseInput();

	front_view.setNearClip(.1);
	front_view.setFov(65.5);

	down_view.setNearClip(.1);
	down_view.setFov(65.5);

	far_view.setDistance(30);
	far_view.setFarClip(2000);
	far_view.setFov(65.5);
	far_view.disableMouseInput();

	camera_number = 0;
	setTarget = false;
}

void Camera::camera_begin() {
	//set and start camera position
	switch (camera_number) {
	case 0:
		//ofTranslate(spacecraft);
		front_view.setPosition(spacecraft + ofVec3f(0, 2, -2));
		front_view.begin();
		break;
	case 1:
		//ofTranslate(spacecraft);
		down_view.setPosition(spacecraft);
		//down_view.setDistance(10);
        down_view.setTarget(ofVec3f(spacecraft.x, 0, spacecraft.z));
		down_view.begin();
		break;
	case 2:
		//ofTranslate(far_position);
		far_view.setPosition(far_position);
		far_view.begin();
		far_view.setTarget(spacecraft);
		break;
	case 3:
		cam.begin();
		if (setTarget) {
			cam.setTarget(spacecraft);
			setTarget = false;
		}
		break;
	}
}
void Camera::camera_end() {
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

