#pragma once

#include "ofMain.h"
#include "ofxAssimpModelLoader.h"
#include "ofxGui.h"
#include "box.h"
#include "ray.h"
#include "Octree.h"
#include  "ParticleSystem.h"
#include  "ParticleEmitter.h"
#include "Camera.h"

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
    void drawAxis(ofVec3f);
    void drawOctree(const Box &node, const bool onlySelectedVertexTree = false);
    void initLightingAndMaterials();
    void savePicture();
    void toggleShadedMode();
    void toggleWireframeMode();
    void togglePointsDisplay();
    void toggleSelectTerrain();
    void togglePointSelectedOctree();
    void setCameraTarget();
    bool doPointSelection();
    vector<int> getIntersectingVertices(Box &box, const Ray &ray);
    vector<int> getCollision(Box &box, const ofVec3f &point);
    void drawBox(const Box &box);
    ofVec3f getCenter(const ofMesh &);
    Box meshBounds(const ofMesh &);
    void subDivideBox8(const Box &b, vector<Box> &boxList);
    void generateTree(const Box &boundingBox, const ofMesh &mesh, int maxDepth, Octree &octree);
    void generateTreeNodes(Box &node, const std::map<int, ofVec3f> &vertexList, int currentDepth, int maxDepth);
    
    bool mouseIntersectPlane(ofVec3f planePoint, ofVec3f planeNorm, ofVec3f &point);
    
    ofVec3f center, bottom;
    float roverX,roverY,roverZ;
    ofEasyCam cam;
    ofxAssimpModelLoader mars, rover;
    ofMesh marsMesh;
    ofMesh roverMesh;
    ofLight light;
    Box boundingBox, roverBox;
    Octree octree;
    int octreeHighestDepth; // contains the highest depth of leaves, updated within generateTree()
    
    bool bAltKeyDown;
    bool bCtrlKeyDown;
    bool bShaded;
    bool bWireframe;
    bool bDisplayPoints;
    bool bPointSelected;
    bool bPointSelectedOctree;
    bool bHide;
    
    bool bRoverLoaded;
    bool bTerrainSelected;
    bool landed;
    
	//thurster emission
	ofxPanel gui;
	bool show_gui;

    ofVec3f selectedPoint;
    ofVec3f intersectPoint;
    
    ofxIntSlider sliderOctreeDepth;
    ofxFloatSlider gravity;
    
    const float selectionRange = 4.0;
    const int octreeMaxDepth = 40;
    
    ParticleSystem sys;
	ParticleEmitter thruster_emitter;
    ThrusterForce thruster;
    ParticleEmitter engine;
    Particle ship;

	//Camera
	Camera* camera;

    ImpulseForce impulseForce;  // create the impulse force
    
    ofSoundPlayer soundPlayer;
    bool soundFileLoaded = false;
};

