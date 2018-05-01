#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

	//
	// set up camera stuff
	//
	cam.setDistance(10);
	cam.setNearClip(.1);
	cam.setFov(65.5);   // approx equivalent to 28mm in 35mm format
	ofSetVerticalSync(true);
	ofSetFrameRate(60);
	ofSetBackgroundColor(ofColor::black);

	
	// setup your objects here
    Particle rocket;
    rocket.color = ofColor::cyan;
    rocket.lifespan = 100000;
    rocket.position.set(0, 3, 0);
    rocket.radius = .6;
    rocket_sys.add(rocket);
    rocket_sys.addForce(&force);
    
    rocket_trail = new ParticleEmitter( new ParticleSystem() );
    rocket_trail->init();
    rocket_trail->setRate(10);
    
    soundPlayer.load("audio/rocket.wav");
    
}

//--------------------------------------------------------------
void ofApp::update(){

	// udpate your objects here
    rocket_sys.update();
    rocket_trail->setPosition(rocket_sys.particles[0].position);
    rocket_trail->update();
    rocket_trail->sys->update();
}

//--------------------------------------------------------------
void ofApp::draw(){

	cam.begin();

		// draw grid
		//
		ofPushMatrix();
		ofRotate(90, 0, 0, 1);
		ofSetLineWidth(1);
		ofSetColor(ofColor::dimGrey);
		ofDrawGridPlane();
		ofPopMatrix();

		// draw your objects here
        rocket_sys.draw();
        rocket_trail->draw();
        rocket_trail->sys->draw();
	cam.end();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

	switch (key) {
	case 'F':
	case 'f':
		ofToggleFullscreen();
		break;
	case OF_KEY_UP:
            force.add(ofVec3f(0, .1, 0));
            rocket_trail->setVelocity(ofVec3f(0, -5, 0));
            if (!rocket_trail->started)
                rocket_trail->start();
            if (!soundPlayer.isPlaying())
                soundPlayer.play();
		break;
	case OF_KEY_DOWN:
            force.add(ofVec3f(0, -.1, 0));
            rocket_trail->setVelocity(ofVec3f(0, 5, 0));
            if (!rocket_trail->started)
                rocket_trail->start();
            if (!soundPlayer.isPlaying())
                soundPlayer.play();
		break;
	case OF_KEY_LEFT:
            force.add(ofVec3f(-.1, 0, 0));
            rocket_trail->setVelocity(ofVec3f(5, 0, 0));
            if (!rocket_trail->started)
                rocket_trail->start();
            
            if (!soundPlayer.isPlaying())
                soundPlayer.play();
		break;
	case OF_KEY_RIGHT:
            force.add(ofVec3f(.1, 0, 0));
            rocket_trail->setVelocity(ofVec3f(-5, 0, 0));
            if (!rocket_trail->started)
                rocket_trail->start();
            if (!soundPlayer.isPlaying())
                soundPlayer.play();
		break;
	case 'h':
		break;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    rocket_trail->stop();
    soundPlayer.stop();
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

void ofApp::playSound() {
	if (soundFileLoaded) soundPlayer.play();
}
