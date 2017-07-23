//  +-----------------------------------------------------+
//  |   File: KTree.h                                     |
//  |   Author: Rodrigo Castiel                           |
//  |   Date: July 18, 2017.                              |
//  +-----------------------------------------------------+
#pragma once

#include <vector>
#include <algorithm>

#include "../geometries/Primitives.h"

// Relaxed 3d-Tree.
// Spatial Data Structure for fast triangle intersection queries.
// It does not support dynamic insertion or removal of triangles.
// It must be initialized from a list of input triangles.

namespace renderer
{

enum SplitAxis
{
    k_split_x,
    k_split_y,
    k_split_z,
    k_no_split
};

struct KdTreeNode
{
    KdTreeNode()
    : mLft(nullptr)
    , mRgt(nullptr)
    , mAxis(k_no_split) { }

    SplitAxis mAxis;    // Split direction.
    float mSplitValue;  // Split value.
    KdTreeNode* mLft;   // < median.
    KdTreeNode* mRgt;   // >= median.
    BoundingBox mBox;   // Bounding box of current node.

    std::vector<int> mData;  // List of triangle indices (leaf nodes only).
};

class KdTree
{
public:
    // Default constructor.
    KdTree(const std::vector<Triangle> & triangles, int capacity = 8);

    // Destructor.
    ~KdTree();

    // Initializes from input triangle list.
    bool BuildFromListOfTriangles();

    int KdTree::NearestTriangle(const glm::vec3 & r, const glm::vec3 & O,
                                glm::vec3 & intersection,  glm::vec3 & baryCoord, float & t) const;

    // (Linear) Searches for nearest triangle to O along r direction.
    int NearestTriangleInSubset(const std::vector<int> & subset, const glm::vec3 & r, 
        const glm::vec3 & O, glm::vec3 & intersection, glm::vec3 & baryCoord, float & t) const;

    // Prints hierarchy of all nodes with corresponding data.
    void Print() const;

private:
    // Intermediate methods for tree construction / ray intersection.

    // Recursive building function.
    KdTreeNode* Build(std::vector<int> & indices, 
        const BoundingBox & bb, int parent_axis=-1) const;

    // Preorder transversal.
    void PrintPreorder(KdTreeNode* root, int depth=0) const;

    int NearestTriangleOpt(KdTreeNode* root,  const glm::vec3 & r, const glm::vec3 & O,
                           glm::vec3 & intersection, glm::vec3 & baryCoord, float & t) const;

    // Properties.
    int mCapacity;

    // Data and auxiliary data.
    KdTreeNode* mRoot;
    std::vector<glm::vec3> mCenterOfMassList;
    const std::vector<Triangle> & mTriangles;
};

inline
int KdTree::NearestTriangle(const glm::vec3 & r, const glm::vec3 & O,
    glm::vec3 & intersection, glm::vec3 & baryCoord, float & t) const
{
    return this->NearestTriangleOpt(mRoot, r, O, intersection, baryCoord, t);
}

}  // namespace renderer.
