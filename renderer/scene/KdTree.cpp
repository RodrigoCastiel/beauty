#include "KdTree.h"

#include "../geometries/MathFunctions.h"

namespace renderer
{

KdTree::KdTree(const std::vector<Triangle> & triangles, int capacity)
: mCapacity(capacity), mRoot(nullptr), mTriangles(triangles)
{

}

KdTree::~KdTree()
{

}

bool KdTree::BuildFromListOfTriangles()
{
    // Precompute center of mass of all triangles and scene bounding box.
    mCenterOfMassList.resize(mTriangles.size());
    BoundingBox sceneBB;
    for (int i = 0; i < mTriangles.size(); i++) {
        mCenterOfMassList[i] = (mTriangles[i].v[0] 
                              + mTriangles[i].v[1]
                              + mTriangles[i].v[2]) / 3.0f;

        for (int j = 0; j < 3; j++) {
            sceneBB.mMin[0] = std::min(sceneBB.mMin.x, mTriangles[i].v[j].x);
            sceneBB.mMin[1] = std::min(sceneBB.mMin.y, mTriangles[i].v[j].y);
            sceneBB.mMin[2] = std::min(sceneBB.mMin.z, mTriangles[i].v[j].z);

            sceneBB.mMax[0] = std::max(sceneBB.mMax.x, mTriangles[i].v[j].x);
            sceneBB.mMax[1] = std::max(sceneBB.mMax.y, mTriangles[i].v[j].y);
            sceneBB.mMax[2] = std::max(sceneBB.mMax.z, mTriangles[i].v[j].z);
        }
    }

    // Build initial list of indices.
    std::vector<int> indices(mTriangles.size());
    for (int i = 0; i < indices.size(); i++) {
        indices[i] = i;
    }

    // Recursively build tree.
    mRoot = this->Build(indices, sceneBB);

    return true;
}

int KdTree::NearestTriangleInSubset(const std::vector<int> & subset, const glm::vec3 & r, 
    const glm::vec3 & O, glm::vec3 & intersection, glm::vec3 & baryCoord, float & t) const
{
    float nearest_t = std::numeric_limits<float>::max();
    int nearestTriangleIndex = -1;

    glm::vec3 I;  // Intersection point.
    glm::vec3 B;  // Barycentric coordinates.
    float t_test;

    // Test intersection against all triangles in subset.
    for (int i : subset)
    {
        // Test if triangle i has an intersection with input ray.
        if (IntersectionRayTriangle(mTriangles[i], r, O, I, B, t_test))
        {
            if (t_test < nearest_t)  // Found a better triangle!
            {
                nearestTriangleIndex = i;
                nearest_t = t_test;

                intersection = I;
                baryCoord = B;
                t = t_test;
            }
        }
    }

    return nearestTriangleIndex;
}

void KdTree::Print() const
{
    this->PrintPreorder(mRoot);
}

int KdTree::NearestTriangleOpt(KdTreeNode* root, const glm::vec3 & r, const glm::vec3 & O,
                               glm::vec3 & intersection,  glm::vec3 & baryCoord, float & t) const
{
    // Check ray-bb intersection against current bb.
    if ((root == nullptr) || !IntersectionRayBB(r, O, root->mBox))
        return -1;  // base case: no intersection.

    // Test intersection against current triangles and lft/rgt subtrees.
    float t_curr = 0.0;
    int nearest_i_curr = -1;
    glm::vec3 I_curr, B_curr;
    nearest_i_curr = this->NearestTriangleInSubset(root->mData, r, O, I_curr, B_curr, t_curr);

    // Test intersection against lft child triangles.
    float t_lft = 0.0;
    int nearest_i_lft = -1;
    glm::vec3 I_lft, B_lft;
    nearest_i_lft = this->NearestTriangleOpt(root->mLft, r, O, I_lft, B_lft, t_lft);

    // Test intersection against rght child triangles.
    float t_rgt = 0.0;
    int nearest_i_rgt = -1;
    glm::vec3 I_rgt, B_rgt;
    nearest_i_rgt = this->NearestTriangleOpt(root->mRgt, r, O, I_rgt, B_rgt, t_rgt);

    if (nearest_i_curr < 0)
        t_curr = std::numeric_limits<float>::max();
    if (nearest_i_lft < 0)
        t_lft = std::numeric_limits<float>::max();
    if (nearest_i_rgt < 0)
        t_rgt = std::numeric_limits<float>::max();

    // Choose intersection with smallest t value.
    if (t_curr < std::min(t_lft, t_rgt))
    {
        intersection = I_curr;
        baryCoord = B_curr;
        t = t_curr;
        return nearest_i_curr;
    }
    else if (t_lft < std::min(t_curr, t_rgt))
    {
        intersection = I_lft;
        baryCoord = B_lft;
        t = t_lft;
        return nearest_i_lft;
    }
    else
    {
        intersection = I_rgt;
        baryCoord = B_rgt;
        t = t_rgt;
        return nearest_i_rgt;
    }
}

KdTreeNode* KdTree::Build(std::vector<int> & indices,
                          const BoundingBox & bb, int parent_axis) const
{
    if (indices.size() == 0)
        return nullptr;

    // Create node.
    KdTreeNode* root = new KdTreeNode();
    root->mBox = bb;
    int numIndices = static_cast<int>(indices.size());

    // Check if it is out of capacity.
    if (numIndices <= mCapacity)  // Can be a leaf node.
    {
        // Copy indices into node data.
        for (int i = 0; i < numIndices; i++) {
            root->mData.push_back(indices[i]);
        }
    }
    else  // Split triangles into different sub-regions.
    {
        // Pick axis with largest variance.
        /*glm::vec3 var = SubsetVariance(indices, mCenterOfMassList);
        int component = 2;
        if (var.x > std::max(var.y, var.z))
            component = 0;
        else if (var.y > std::max(var.x, var.z))
            component = 1;

        if (component == parent_axis)*/
            int component = (parent_axis+1)%3;

        // Sort by component with largest variance and split using median.
        struct LessThanCriteria
        {
            LessThanCriteria(const std::vector<glm::vec3> & points, int axis) 
            : mAxis(axis), mPoints(points)
            { }

            const std::vector<glm::vec3> & mPoints;
            const int mAxis;

            bool operator()(int lft, int rgt) {
                float value_lft = mPoints[lft][mAxis];
                float value_rgt = mPoints[rgt][mAxis];
                return value_lft < value_rgt;
            }
        };

        std::sort(indices.begin(), indices.end(), LessThanCriteria(mCenterOfMassList, component));
        int median = indices[indices.size()/2];
        root->mSplitValue = mCenterOfMassList[median][component];
        root->mAxis = static_cast<SplitAxis>(component);

        // Distribute triangles to half-spaces.
        std::vector<int> less_than_indices;
        std::vector<int> greater_than_indices;
        //printf("\n\n");

        for (int i = 0; i < indices.size(); i++) {
            // Test intersection against less-than/greater-than half-spaces.
            bool intersects_lft = 
                IntersectionHalfSpaceTriangle(mTriangles[indices[i]], root->mSplitValue, component, -1);
            bool intersects_rgt = 
                IntersectionHalfSpaceTriangle(mTriangles[indices[i]], root->mSplitValue, component, +1);

            //printf(" %4.2f\n", mCenterOfMassList[indices[i]][component]);
            //bool intersects_lft = mCenterOfMassList[indices[i]][component] <  root->mSplitValue;
            //bool intersects_rgt = mCenterOfMassList[indices[i]][component] >= root->mSplitValue;

            if (intersects_lft && !intersects_rgt)
                less_than_indices.push_back(indices[i]);
            else if (!intersects_lft && intersects_rgt)
                greater_than_indices.push_back(indices[i]);
            else if (intersects_lft && intersects_rgt) {
                if (root->mData.size() < mCapacity) {
                    root->mData.push_back(indices[i]);
                }
                else {
                    less_than_indices.push_back(indices[i]);
                    greater_than_indices.push_back(indices[i]);
                }
            }
        }

        // Clear current indices to avoid useless memory consumption.
        indices.clear();

        // Build subtrees.
        BoundingBox lft = bb.Split(root->mSplitValue, component, -1);
        BoundingBox rgt = bb.Split(root->mSplitValue, component, +1);
        root->mLft = Build(less_than_indices, lft, component);
        root->mRgt = Build(greater_than_indices, rgt, component);
    }

    return root;
}

void PrintSpaces(int n)
{
    for (int i = 0; i < n; i++)
        printf(" ");
}

void KdTree::PrintPreorder(KdTreeNode* root, int depth) const
{
    if (root) 
    {
        if (root->mAxis == k_no_split)  // Leaf node!
        {
            PrintSpaces(depth);
            printf("<Leaf Node>\n");
        }
        else  // Inner node.
        {
            PrintSpaces(depth);
            printf("<Inner Node> \t axis(%d), value(%4.2f)\n", root->mAxis, root->mSplitValue);
        }

        PrintSpaces(depth);
        root->mBox.Print();

        for (int i = 0; i < root->mData.size(); i++) {
            int j = root->mData[i];
            PrintSpaces(depth);
            printf("| %d ", j);

            for (int k = 0; k < 3; k++)
                printf("  [%4.2f, %4.2f, %4.2f]", mTriangles[j].v[k].x,
                    mTriangles[j].v[k].y,
                    mTriangles[j].v[k].z);
            printf("\n");
        }

        if (root->mLft) 
        {
            PrintSpaces(depth);
            printf("LFT:\n");
            this->PrintPreorder(root->mLft, depth+4);
        }
        if (root->mRgt) 
        {
            PrintSpaces(depth);
            printf("RGT:\n");
            this->PrintPreorder(root->mRgt, depth+4);
        }
    }
    else 
    {
        PrintSpaces(depth);
        printf("<null>\n");
    }
}


}  // namespace renderer.