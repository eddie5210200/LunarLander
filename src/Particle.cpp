#include "Particle.h"


Particle::Particle() {

	// initialize particle with some reasonable values first;
	//
	velocity.set(0, 0, 0);
	acceleration.set(0, 0, 0);
	position.set(0, 0, 0);
	forces.set(0, 0, 0);
	lifespan = 5;
	birthtime = 0;
	radius = .1;
	damping = .99;
	mass = 1;
    color = ofColor::red;
    colorLifetime = 1;
}

void Particle::draw() {
    // interpolation
    // hue: 0 -> 25 (red -> orange)
    // saturation: 255 -> 0
    // brightness: 255 -> 0
    // alpha: 50 -> 0
    float newHue = 0 + (25.0 / (1000.0 * colorLifetime)) * (ofGetElapsedTimeMillis() - birthtime);
    float newSaturation = 255 - (255.0 / (1000.0 * colorLifetime)) * (ofGetElapsedTimeMillis() - birthtime);
    float newBrightness = 255 - (255.0 / (1000.0 * colorLifetime)) * (ofGetElapsedTimeMillis() - birthtime);
    float newAlpha = 50.0 - (50.0 / (1000.0 * colorLifetime)) * (ofGetElapsedTimeMillis() - birthtime);
    
    color.setHsb(newHue, newSaturation, newBrightness, newAlpha);

    ofSetColor(color);
	ofDrawSphere(position, radius);
}

// write your own integrator here.. (hint: it's only 3 lines of code)
//
void Particle::integrate() {

	// check for 0 framerate to avoid divide errors
	//
	float framerate = ofGetFrameRate();
	if (framerate < 1.0) return;

	// interval for this step
	//
	float dt = 1.0 / framerate;

	// update position based on velocity
	//
	position += (velocity * dt);

	// update acceleration with accumulated paritcles forces
	// remember :  (f = ma) OR (a = 1/m * f)
	//
	ofVec3f accel = acceleration;    // start with any acceleration already on the particle
	accel += (forces * (1.0 / mass));
	velocity += accel * dt;

	// add a little damping for good measure
	//
	velocity *= damping;

	// clear forces on particle (they get re-added each step)
	//
	forces.set(0, 0, 0);
}

//  return age in seconds
//
float Particle::age() {
	return (ofGetElapsedTimeMillis() - birthtime)/1000.0;
}
