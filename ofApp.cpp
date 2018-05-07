//--------------------------------------------------------------
//
//  Kevin M. Smith
//
//  Mars HiRise Project - startup scene
//
//  This is an openFrameworks 3D scene that includes an EasyCam
//  and example 3D geometry which I have reconstructed from Mars
//  HiRis photographs taken the Mars Reconnaisance Orbiter
//
//  You will use this source file (and include file) as a starting point
//  to implement assignment 5  (Parts I and II)
//
//  Please do not modify any of the keymappings.  I would like
//  the input interface to be the same for each student's
//  work.  Please also add your name/date below.

//  Please document/comment all of your work !
//  Have Fun !!
//

#include "ofApp.h"
#include "Util.h"

#include <vector>
#include <map>

//--------------------------------------------------------------
// setup scene, lighting, state and load geometry
//
void ofApp::setup(){
    
    bShaded = true;
    bWireframe = false;
    bDisplayPoints = false;
    bAltKeyDown = false;
    bCtrlKeyDown = false;
    bRoverLoaded = false;
    bTerrainSelected = true;
    bHide = false;
    bPointSelectedOctree = false;
    //    ofSetWindowShape(1024, 768);
    
    cam.setDistance(10);
    cam.setNearClip(.1);
    cam.setFov(65.5);   // approx equivalent to 28mm in 35mm format
    ofSetVerticalSync(true);
    cam.disableMouseInput();
    ofEnableSmoothing();
    
    // setup rudimentary lighting
    //
    initLightingAndMaterials();
    
    mars.loadModel("geo/mars-low-v2.obj");
    mars.setScaleNormalization(false);
    marsMesh = mars.getMesh(0);
    
    boundingBox = meshBounds(marsMesh);
    
    // generate octree
    octreeHighestDepth = 0;
    generateTree(boundingBox, marsMesh, octreeMaxDepth, octree);
    
    gui.setup();
    gui.add(sliderOctreeDepth.setup("Octree display depth", 0, 0, octreeHighestDepth));
    gui.add(gravity.setup("Gravity", 0.1, 0, 2));
    
    //  setup emitter (for engine)
    //
    engine.setRate(20);
    engine.setParticleRadius(.050);
    engine.visible = false;
    
    // create our one lonely particle
    //
    ship.lifespan = 10000;
    ship.position.set(0, 2.5, 0);
    sys.add(ship);
    
    sys.addForce(&thruster);
    //sys.addForce(new TurbulenceForce(ofVec3f(-0.02, -0.01, -0.03), ofVec3f(0.01, 0.02, 0.05)));
    sys.addForce(new GravityForce(ofVec3f(0, -gravity, 0)));
}

//--------------------------------------------------------------
// incrementally update scene (animation)
//
void ofApp::update() {
    
    sys.update();
    engine.update();
    engine.setPosition(sys.particles[0].position); // this updates the thruster position to "ship" which is the 1st particle
}

//--------------------------------------------------------------
void ofApp::draw() {
    //    ofBackgroundGradient(ofColor(20), ofColor(0));   // pick your own backgroujnd
    ofBackground(ofColor::black);
    //    cout << ofGetFrameRate() << endl;
    
    ofEnableDepthTest();
    cam.begin();
    
    // draw particle system
    //
    sys.draw();
    
    // draw engine output
    //
    engine.setPosition(sys.particles[0].position);
    engine.draw();
    
    ofPushMatrix();
    if (bWireframe) {                    // wireframe mode  (include axis)
        ofDisableLighting();
        ofSetColor(ofColor::slateGray);
        mars.drawWireframe();
        if (bRoverLoaded) {
            rover.drawWireframe();
            if (!bTerrainSelected) drawAxis(rover.getPosition());
        }
        if (bTerrainSelected) drawAxis(ofVec3f(0, 0, 0));
    }
    else {
        ofEnableLighting();              // shaded mode
        mars.drawFaces();
        
        if (bRoverLoaded) {
            rover.drawFaces();
            if (!bTerrainSelected) drawAxis(rover.getPosition());
        }
        if (bTerrainSelected) drawAxis(ofVec3f(0, 0, 0));
    }
    
    
    if (bDisplayPoints) {                // display points as an option
        glPointSize(3);
        ofSetColor(ofColor::green);
        mars.drawVertices();
    }
    
    // highlight selected point (draw sphere around selected point)
    //
    if (bPointSelected && !bPointSelectedOctree) {
        ofSetColor(ofColor::blue);
        ofDrawSphere(selectedPoint, .1);
    }
    
    
    ofNoFill();
    ofSetColor(ofColor::white);
    drawBox(boundingBox);
    
    // draw octree
    if (bPointSelectedOctree) {
        drawOctree(octree.root, true);
    } else {
        drawOctree(octree.root);
    }
    
    ofPopMatrix();
    cam.end();
    ofDisableDepthTest();

    if (!bHide) {
        gui.draw();
    }
}

//

// Draw an XYZ axis in RGB at world (0,0,0) for reference.
//
void ofApp::drawAxis(ofVec3f location) {
    
    ofPushMatrix();
    ofTranslate(location);
    
    ofSetLineWidth(1.0);
    
    // X Axis
    ofSetColor(ofColor(255, 0, 0));
    ofDrawLine(ofPoint(0, 0, 0), ofPoint(1, 0, 0));
    
    // Y Axis
    ofSetColor(ofColor(0, 255, 0));
    ofDrawLine(ofPoint(0, 0, 0), ofPoint(0, 1, 0));
    
    // Z Axis
    ofSetColor(ofColor(0, 0, 255));
    ofDrawLine(ofPoint(0, 0, 0), ofPoint(0, 0, 1));
    
    ofPopMatrix();
}

// Draws the octree
//
void ofApp::drawOctree(const Box &node, const bool onlySelectedVertexTree) {
    if (sliderOctreeDepth == 0) return;
    
    if ((!onlySelectedVertexTree && sliderOctreeDepth >= node.level) ||
        (onlySelectedVertexTree && node.containsSelectedVertex)) {
        switch (node.level % 9) {
            case 0:
                ofSetColor(ofColor::white);
                break;
            case 1:
                ofSetColor(ofColor::red);
                break;
            case 2:
                ofSetColor(ofColor::blue);
                break;
            case 3:
                ofSetColor(ofColor::green);
                break;
            case 4:
                ofSetColor(ofColor::orange);
                break;
            case 5:
                ofSetColor(ofColor::violet);
                break;
            case 6:
                ofSetColor(ofColor::turquoise);
                break;
            case 7:
                ofSetColor(ofColor::fuchsia);
                break;
            case 8:
                ofSetColor(ofColor::salmon);
                break;
        }

        drawBox(node);
        
        for (const Box &b : node.children) {
            drawOctree(b, onlySelectedVertexTree);
        }
    }
}

void ofApp::keyPressed(int key) {
    
    switch (key) {
        case 'C':
        case 'c':
            if (cam.getMouseInputEnabled()) cam.disableMouseInput();
            else cam.enableMouseInput();
            break;
        case 'F':
        case 'f':
            ofToggleFullscreen();
            break;
            
        // have LEFT/RIGHT move in the X coordinate plane
        // have UP/DOWN move in the Z coordinate plane
        // have SPACEBAR be the only way to thruster up
        case OF_KEY_DOWN:
            thruster.add(ofVec3f(0, 0, 0.5)); // was 0, 0.5, 0
            engine.setVelocity(ofVec3f(0, 0, -5)); // was 0, -5, 0
            engine.start();
            break;
        case ' ': // to move the ship upwards
           // playSound();
            thruster.add(ofVec3f(0, .5, 0));
            engine.setVelocity(ofVec3f(0, -5, 0));
            engine.start();
            break;
        case OF_KEY_UP:
           // playSound();
            thruster.add(ofVec3f(0, 0, -0.5)); // was 0, -0.5, 0
            engine.setVelocity(ofVec3f(0, 0, 5)); // was 0, 5, 0
            engine.start();
            break;
            
        case OF_KEY_LEFT:
            //playSound();
            thruster.add(ofVec3f(-.5, 0, 0));
            engine.setVelocity(ofVec3f(5, 0, 0));
            engine.start();
            break;
        case OF_KEY_RIGHT:
          //  playSound();
            thruster.add(ofVec3f(.5, 0, 0));
            engine.setVelocity(ofVec3f(-5, 0, 0));
            engine.start();
            break;
        case 'H':
        case 'h':
            bHide = !bHide; // toggle gui
            break;
        case 'r':
            cam.reset();
            break;
        case 's':
            savePicture();
            break;
        case 'S':
            toggleShadedMode();
            break;
        case 't':
            setCameraTarget();
            break;
        case 'u':
            break;
        case 'v':
            togglePointsDisplay();
            break;
        case 'V':
            togglePointSelectedOctree();
            break;
        case 'w':
            toggleWireframeMode();
            break;
        case OF_KEY_ALT:
            cam.enableMouseInput();
            bAltKeyDown = true;
            break;
        case OF_KEY_CONTROL:
            bCtrlKeyDown = true;
            break;
        case OF_KEY_SHIFT:
            break;
        case OF_KEY_DEL:
            break;
        default:
            break;
    }
}

void ofApp::toggleShadedMode() {
    bShaded = !bShaded;
}

void ofApp::toggleWireframeMode() {
    bWireframe = !bWireframe;
}

void ofApp::toggleSelectTerrain() {
    bTerrainSelected = !bTerrainSelected;
}

void ofApp::togglePointsDisplay() {
    bDisplayPoints = !bDisplayPoints;
}

void ofApp::togglePointSelectedOctree() {
    bPointSelectedOctree = !bPointSelectedOctree;
}

void ofApp::keyReleased(int key) {
    switch (key) {
        case ' ':
        case OF_KEY_RIGHT:
        case OF_KEY_LEFT:
        case OF_KEY_UP:
        case OF_KEY_DOWN:
           // soundPlayer.stop();
            engine.stop();
            thruster.set(ofVec3f(0, 0, 0));
            break;
        case OF_KEY_ALT:
            cam.disableMouseInput();
            bAltKeyDown = false;
            break;
        case OF_KEY_CONTROL:
            bCtrlKeyDown = false;
            break;
        case OF_KEY_SHIFT:
            break;
        default:
            break;
    }
}



//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){
}


//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
    auto start = ofGetElapsedTimeMicros();
    
    // create ray based on camera to screenspace
    ofVec3f mouse(mouseX, mouseY);
    ofVec3f rayPoint = cam.screenToWorld(mouse);
    ofVec3f rayDir = rayPoint - cam.getPosition();
    rayDir.normalize();
    Ray ray = Ray(Vector3(rayPoint.x, rayPoint.y, rayPoint.z),
                  Vector3(rayDir.x, rayDir.y, rayDir.z));
    
    // get list of vertex indices that intersect ray
    vector<int> selectedVertices = getIntersectingVertices(octree.root, ray);
    
    if (selectedVertices.size() != 0) {
        // Find the closest vertex
        int closestVertex = selectedVertices[0];
        for (int i : selectedVertices) {
            if ( marsMesh.getVertex(i).squareDistance((cam.getPosition())) < marsMesh.getVertex(closestVertex).squareDistance((cam.getPosition())) )
                closestVertex = i;
        }
        
        selectedPoint = marsMesh.getVertex(closestVertex);
        bPointSelected = true;
        
        // Calculate vertex selection time
        auto end = ofGetElapsedTimeMicros();
        auto duration = end - start;
        
        cout << "Selected point " << closestVertex << " at " << selectedPoint << " in "
            << duration << " microseconds" << endl;
    } else {
        bPointSelected = false;
    }
}

// Checks which vetices intersect with ray; returns vertex indices of intersection,
// empty vector if no leaf found with vertices.
vector<int> ofApp::getIntersectingVertices(Box &box, const Ray &ray) {
    if (box.intersect(ray, 0, 100)) {
        // if only one vertex in leaf, we've found the lowest depth
        if (box.vertexIndices.size() == 1) return { box.vertexIndices[0] };
        
        // accumulate growing vector of intersected vertices
        vector<int> selectedVertices;
        for (Box &child : box.children) {
            vector<int> vertices = getIntersectingVertices(child, ray);
            for (int vertex : vertices){
                selectedVertices.push_back(vertex);
            }
        }
        if (selectedVertices.size() == 0) return { };

        box.containsSelectedVertex = true;

        return selectedVertices;
    } else {
        box.containsSelectedVertex = false;
        return { };
    }
}


//draw a box from a "Box" class
//
void ofApp::drawBox(const Box &box) {
    Vector3 min = box.parameters[0];
    Vector3 max = box.parameters[1];
    Vector3 size = max - min;
    Vector3 center = size / 2 + min;
    ofVec3f p = ofVec3f(center.x(), center.y(), center.z());
    float w = size.x();
    float h = size.y();
    float d = size.z();
    ofDrawBox(p, w, h, d);
}

// return a Mesh Bounding Box for the entire Mesh
//
Box ofApp::meshBounds(const ofMesh & mesh) {
    int n = mesh.getNumVertices();
    ofVec3f v = mesh.getVertex(0);
    ofVec3f max = v;
    ofVec3f min = v;
    for (int i = 1; i < n; i++) {
        ofVec3f v = mesh.getVertex(i);
        
        if (v.x > max.x) max.x = v.x;
        else if (v.x < min.x) min.x = v.x;
        
        if (v.y > max.y) max.y = v.y;
        else if (v.y < min.y) min.y = v.y;
        
        if (v.z > max.z) max.z = v.z;
        else if (v.z < min.z) min.z = v.z;
    }
    return Box(Vector3(min.x, min.y, min.z), Vector3(max.x, max.y, max.z));
}

//  Subdivide a Box into eight(8) equal size boxes, return them in boxList;
//
void ofApp::subDivideBox8(const Box &box, vector<Box> & boxList) {
    Vector3 min = box.parameters[0];
    Vector3 max = box.parameters[1];
    Vector3 size = max - min;
    Vector3 center = size / 2 + min;
    float xdist = (max.x() - min.x()) / 2;
    float ydist = (max.y() - min.y()) / 2;
    float zdist = (max.z() - min.z()) / 2;
    Vector3 h = Vector3(0, ydist, 0);
    
    //  generate ground floor
    //
    Box b[8];
    b[0] = Box(min, center);
    b[1] = Box(b[0].min() + Vector3(xdist, 0, 0), b[0].max() + Vector3(xdist, 0, 0));
    b[2] = Box(b[1].min() + Vector3(0, 0, zdist), b[1].max() + Vector3(0, 0, zdist));
    b[3] = Box(b[2].min() + Vector3(-xdist, 0, 0), b[2].max() + Vector3(-xdist, 0, 0));
    
    boxList.clear();
    for (int i = 0; i < 4; i++)
        boxList.push_back(b[i]);
    
    // generate second story
    //
    for (int i = 4; i < 8; i++) {
        b[i] = Box(b[i - 4].min() + h, b[i - 4].max() + h);
        boxList.push_back(b[i]);
    }
}

// Recursive function for ofApp:generateTree(), generates an octreeq
//
void ofApp::generateTreeNodes(Box &node, const map<int, ofVec3f> &vertexList, int currentDepth, int maxDepth) {
    // Set node level
    node.level = currentDepth;
    if (currentDepth > octreeHighestDepth)
        octreeHighestDepth = currentDepth;
    
    // divide current node into 8 leaves
    subDivideBox8(node, node.children);
    
    for (int i = 0; i < node.children.size(); i++) {
        node.children[i].level = currentDepth + 1;
        
        map<int, ofVec3f> innerVertices;
        for (auto &pair : vertexList) {
            if (node.children[i].contains(pair.second)) {
                // if current leaf contains a vertex, add it to vertex index list
                node.children[i].vertexIndices.push_back(pair.first);
                // also add vertex to inner vector from which leaves will search through
                innerVertices.insert(pair);
            }
        }
        // delete any leaves with no vertices
        if (innerVertices.size() == 0) {
            node.children.erase(node.children.begin() + i);
            i--;
        }
        // recursively generate more leaves if more than one inner vertex
        else if (currentDepth < maxDepth && innerVertices.size() > 1)
            generateTreeNodes(node.children[i], innerVertices, node.children[i].level, maxDepth);
    }
}

// Given a bounding box & a mesh, generate an octree of a specified depth
//
void ofApp::generateTree(const Box &boundingBox, const ofMesh &mesh, int maxDepth, Octree &octree) {
    // Initialize list of vertices and corresponding indexes
    map<int, ofVec3f> vertexList;
    vector<ofVec3f> vertices = mesh.getVertices();
    for (int i = 0; i < vertices.size(); i++) {
        vertexList.emplace(i, vertices[i]);
    }
    
    octree.root = boundingBox;
    
    generateTreeNodes(octree.root, vertexList, 0, maxDepth);
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {
    
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {
    
}


//
//  Select Target Point on Terrain by comparing distance of mouse to
//  vertice points projected onto screenspace.
//  if a point is selected, return true, else return false;
//
bool ofApp::doPointSelection() {
    
    ofMesh mesh = mars.getMesh(0);
    int n = mesh.getNumVertices();
    float nearestDistance = 0;
    int nearestIndex = 0;
    
    bPointSelected = false;
    
    ofVec2f mouse(mouseX, mouseY);
    vector<ofVec3f> selection;
    
    // We check through the mesh vertices to see which ones
    // are "close" to the mouse point in screen space.  If we find
    // points that are close, we store them in a vector (dynamic array)
    //
    for (int i = 0; i < n; i++) {
        ofVec3f vert = mesh.getVertex(i);
        ofVec3f posScreen = cam.worldToScreen(vert);
        float distance = posScreen.distance(mouse);
        if (distance < selectionRange) {
            selection.push_back(vert);
            bPointSelected = true;
        }
    }
    
    //  if we found selected points, we need to determine which
    //  one is closest to the eye (camera). That one is our selected target.
    //
    if (bPointSelected) {
        float distance = 0;
        for (int i = 0; i < selection.size(); i++) {
            ofVec3f point =  cam.worldToCamera(selection[i]);
            
            // In camera space, the camera is at (0,0,0), so distance from
            // the camera is simply the length of the point vector
            //
            float curDist = point.length();
            
            if (i == 0 || curDist < distance) {
                distance = curDist;
                selectedPoint = selection[i];
            }
        }
    }
    return bPointSelected;
}

// Set the camera to use the selected point as it's new target
//
void ofApp::setCameraTarget() {
    
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
// setup basic ambient lighting in GL  (for now, enable just 1 light)
//
void ofApp::initLightingAndMaterials() {
    
    static float ambient[] =
    { .5f, .5f, .5, 1.0f };
    static float diffuse[] =
    { 1.0f, 1.0f, 1.0f, 1.0f };
    
    static float position[] =
    {5.0, 5.0, 5.0, 0.0 };
    
    static float lmodel_ambient[] =
    { 1.0f, 1.0f, 1.0f, 1.0f };
    
    static float lmodel_twoside[] =
    { GL_TRUE };
    
    
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT0, GL_POSITION, position);
    
    glLightfv(GL_LIGHT1, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT1, GL_POSITION, position);
    
    
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
    glLightModelfv(GL_LIGHT_MODEL_TWO_SIDE, lmodel_twoside);
    
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    //    glEnable(GL_LIGHT1);
    glShadeModel(GL_SMOOTH);
}

void ofApp::savePicture() {
    ofImage picture;
    picture.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
    picture.save("screenshot.png");
    cout << "picture saved" << endl;
}

//--------------------------------------------------------------
//
// support drag-and-drop of model (.obj) file loading.  when
// model is dropped in viewport, place origin under cursor
//
void ofApp::dragEvent(ofDragInfo dragInfo) {
    
    ofVec3f point;
    mouseIntersectPlane(ofVec3f(0, 0, 0), cam.getZAxis(), point);
    
    if (rover.loadModel(dragInfo.files[0])) {
        rover.setScaleNormalization(false);
        rover.setScale(.005, .005, .005);
        rover.setPosition(point.x, point.y, point.z);
        bRoverLoaded = true;
    }
    else cout << "Error: Can't load model" << dragInfo.files[0] << endl;
}

bool ofApp::mouseIntersectPlane(ofVec3f planePoint, ofVec3f planeNorm, ofVec3f &point) {
    ofVec2f mouse(mouseX, mouseY);
    ofVec3f rayPoint = cam.screenToWorld(mouse);
    ofVec3f rayDir = rayPoint - cam.getPosition();
    rayDir.normalize();
    return (rayIntersectPlane(rayPoint, rayDir, planePoint, planeNorm, point));
}

