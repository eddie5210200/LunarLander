#ifndef _BOX_H_
#define _BOX_H_

#include <assert.h>
#include "vector3.h"
#include "ray.h"
#include "ofMain.h"

#include <vector>

/*
 * Axis-aligned bounding box class, for use with the optimized ray-box
 * intersection test described in:
 *
 *      Amy Williams, Steve Barrus, R. Keith Morley, and Peter Shirley
 *      "An Efficient and Robust Ray-Box Intersection Algorithm"
 *      Journal of graphics tools, 10(1):49-54, 2005
 *
 */

class Box {
  public:
    Box() { }
    Box(const Vector3 &min, const Vector3 &max) {
      assert(min < max);
      parameters[0] = min;
      parameters[1] = max;
      containsSelectedVertex = false;
    }
    Box(const Vector3 &min, const Vector3 &max, int level) {
        assert(min < max);
        parameters[0] = min;
        parameters[1] = max;
        this->level = level;
        containsSelectedVertex = false;
    }
    // (t0, t1) is the interval for valid hits
    bool intersect(const Ray &, float t0, float t1) const;
    // returns true if the box contains the point
    bool contains(const ofVec3f &point) const;
    
    // corners
    Vector3 parameters[2];
	Vector3 min() { return parameters[0]; }
	Vector3 max() { return parameters[1]; }
    
    // octree children
    std::vector<Box> children;
    std::vector<int> vertexIndices;
    int level;
    bool containsSelectedVertex;
};

#endif // _BOX_H_
