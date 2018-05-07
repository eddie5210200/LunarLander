#pragma once

#include "Camera.h"
#include "ParticleEmitter.h"
#include "ofxAssimpModelLoader.h"
#include "ofxGui.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		//Asset
		ofxAssimpModelLoader terrain, spacecraft;

		//Camera
		Camera* camera;

		//Lighting
		ofLight keyLight, fillLight, rimLight;

		//Gui and list of gui controllers
		ofxPanel gui;
		ofxFloatSlider gravity, thrust_force, thrust_life;
		bool show_gui;
		
		//Thrust Particle Effect
		ParticleEmitter thrusters;
		//Octree

		//Collision

		
};
