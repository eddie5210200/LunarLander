#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofSetFrameRate(60);
	//camera setup
	camera = new Camera();
	camera->setup();
	//gui setup
	show_gui = true;
	gui.add(thrust_force.setup("Change Thrust Force", 20, 1, 100));
	gui.add(gravity.setup("Change Gravity", 3, 1, 100));
	//setup emitters for thruster effect
	thrusters.sys->addForce(new GravityForce(ofVec3f(0, -gravity, 0)));
	thrusters.setVelocity(ofVec3f(0, 0, 0));
	thrusters.setGroupSize(500);
	thrusters.setEmitterType(DiscEmitter);
}

//--------------------------------------------------------------
void ofApp::update() {
	//thrusters
	thrusters.update();
	//get spacecraft position
}

//--------------------------------------------------------------
void ofApp::draw() {
	ofBackground(ofColor::black);
	//draw gui
	if (show_gui) gui.draw();
	//start camera
	camera->camera_begin();
	ofPushMatrix();

	//draw assets






	

	
	//close camera
	ofPopMatrix();
	camera->camera_end();

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	switch (key) {
	//camera controls
	case '1':
		camera->camera_number = 0;
		break;
	case '2':
		camera->camera_number = 1;
		break;
	case '3':
		camera->camera_number = 2;
		break;
	case '4':
		camera->camera_number = 3;
		if (camera->cam.getMouseInputEnabled()) camera->cam.disableMouseInput();
		else camera->cam.enableMouseInput();
		break;
	case 'r':
		camera->setTarget = true;
		break;
	//show gui
	case 'G':
	case 'g':
		if (show_gui) show_gui = false;
		else show_gui = true;
		break;
	default:
		break;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}
