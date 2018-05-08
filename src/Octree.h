//
//  Octree.h
//  myOctreeAssignment
//
//  Created by Justin Tennant on 4/17/18.
//

#ifndef Octree_h
#define Octree_h

#include "box.h"

class Octree {
    int num_levels;
public:
    Octree() { }
    
    int getNumofLevels() { return num_levels; }
    void addLevel() { num_levels++; }
    
    Box root;
};

#endif /* Octree_h */
