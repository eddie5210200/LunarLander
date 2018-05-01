#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofSetFrameRate(60);
	//camera setup
	cam.camera_number = 0;
	//gui setup
	show_gui = true;
	gui.add(thrust_force.setup("Change Thrust Force", 20, 1, 100));
	gui.add(gravity.setup("Change Gravity", 3, 1, 100));
	//setup emitters for thruster effect
	thrusters.sys->addForce(new GravityForce(ofVec3f(0, -gravity, 0)));
	thrusters.setVelocity(ofVec3f(0, 0, 0));
	thrusters.setGroupSize(500);
}

//--------------------------------------------------------------
void ofApp::update() {
	//thrusters
	thrusters.setLifespan(lifespan);
	thrusters.setRate(rate);
	thrusters.setParticleRadius(radius);
	thrusters.setMass(mass);
	thrusters.update();
	//get spacecraft position
}

//--------------------------------------------------------------
void ofApp::draw() {
	//draw gui
	if (show_gui) gui.draw();
	//start camera
	cam.camera_begin(cam.camera_number);
	ofPushMatrix();

	//draw assets






	

	ofPopMatrix();
	//close camera
	cam.camera_end(cam.camera_number);

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	switch (key) {
	//camera controls
	case 'C':
	case 'c':
		cam.camera_number++;
		cam.camera_number %= 3;
	case 'F':
	case 'f':
		cam.camera_number = 3;
		if (cam.cam.getMouseInputEnabled()) cam.cam.disableMouseInput();
		else cam.cam.enableMouseInput();
		break;
	case 'r':
		cam.setTarget = true;
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
