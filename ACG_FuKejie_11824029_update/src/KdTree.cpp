#include "KdTree.h"

#include <QDebug>
#include <cmath>

/*
 *  Construct the treeNode
 *
 *  Use the SAH (Surface Area Heuristic) to evaluate split candidates:
 *      SAHCost = Tt * N_NonLeafNode + Ti * (Area_Left * N_Left + Area_Right *N_Right)/ Area_Total
 *  or
 *      SAHCost = Tt * N_NonLeafNode + Ti * (Length_Left * N_Left + Length_Right *N_Right)/ Length_Total
 *      Here:
 *          Tt: Traversal time for traversing 1 child node
 *          Ti: Intersection time
 *          Area_Left: area of the left child / Area_Right: area of the right child
 *          Length_Left: length of the split axis of the left child
 *          Length_Right: length of the split axis of the right child
 *          N_Left: number of the tirangles whose AABBoxes at the left
 *          N_Right: number of the tirangles whose AABBoxes at the right
 *          N_NonLeafNode: number of node which is not a leaf
 *
 *  Specially, the SAH cost for the tree node without split (as leaf node):
 *      SAHCost = Ti * N
 *      Hera:
 *          N: number of the tirangles
 *  For each father node we sample (2*Num_Triangle - 1) split candidates, and compare with the nosplit case to find the least cost
*/
void treeNode::construct(QVector<Triangle *> &tris, AABBox &box, int depth)
{
    this->aabbox.initialize(box.bottom_left, box.top_right);
    this->nTrianglesIn = tris.size();

    // If the node depth>10 or the tris in this node less than 10, set as the leaf node
    if (depth > 10 || tris.size()< 2)
    {
        this->isLeaf = true;
        for (int i=0; i<tris.size(); i++)
        {
            this->triangles.push_back(tris[i]);
        }
        return;

    }

    float Ti = 80.0f;
    float Tt = 1.0f;
    int nSplits = 10 * 2;// Split the longest axis into nSplit sections
    QVector <Triangle *> leftTriangles;
    QVector <Triangle *> rightTriangles;

    // Calculate the SAH cost without split, set as the Minimum SAH cost at first
    float EmptySAHCost = Ti * tris.size();

    float MinSAHCost = EmptySAHCost;

    // Calculate total area of the AABBox
    float axisLength[3];
    for (int i=0; i<3; i++)
    {
        axisLength[i] = this->aabbox.top_right[i] - this->aabbox.bottom_left[i];
    }
    float totalArea = 2 * (axisLength[0] * axisLength[1] + axisLength[1] * axisLength[2] + axisLength[2] * axisLength[0]);

    int bestSplitAxis = 0;
    int bestSpilts = 0;

    for (int t=0; t<3;t++)
    {
        // Calculate the SAH cost for the sample splits

        float steplength = axisLength[t] / nSplits; //Length of every step
        float sideArea = 2 * axisLength[(t+1)%3] * axisLength[(t+2)%3];
        float unitSplitArea = 2 * (axisLength[(t+1)%3] + axisLength[(t+2)%3]);


        for (int i=1; i<nSplits; i++)
        {
            float Length_left = steplength * i;
            float leftArea = sideArea + Length_left * unitSplitArea;
            float rightArea = totalArea - Length_left * unitSplitArea;

            int  N_left = 0;
            int  N_right = 0;

            for (int j=0; j<tris.size(); j++)
            {
                if (tris[j]->aabbox.bottom_left[t] <= box.bottom_left[t] + Length_left)
                {
                    N_left ++;
                }

                if (tris[j]->aabbox.top_right[t] >= box.bottom_left[t] + Length_left)
                {
                    N_right ++;
                }
            }
            int minus=0;
            if (N_left==0|| N_right==0)
            {
                minus--;
            }
            float sampleCost = Tt * (2 + minus) + Ti *(leftArea * N_left + rightArea * N_right) /totalArea;
            if (sampleCost < MinSAHCost)
            {
                MinSAHCost = sampleCost;
                bestSpilts = i;
                bestSplitAxis = t;
            }
        }
    }
    this->splitAxis = bestSplitAxis;


    if (bestSpilts == 0) // Don't need split
    {
        this->isLeaf = true;
        for (int i=0; i<tris.size(); i++)
        {
            this->triangles.push_back(tris[i]);
        }
        return;
    }
    else
    {

        float Length_left = axisLength[this->splitAxis] / nSplits * bestSpilts;
        float splitCandidate = box.bottom_left[this->splitAxis] + Length_left;

        for (int i=0; i<tris.size(); i++)
        {
            if (tris[i]->aabbox.bottom_left[this->splitAxis] <= splitCandidate)
            {
                leftTriangles.push_back(tris[i]);
            }

            if (tris[i]->aabbox.top_right[this->splitAxis] >= splitCandidate)
            {
                rightTriangles.push_back(tris[i]);
            }
        }

        if (leftTriangles.size() > 0)
        {

            this->left = new treeNode;
            AABBox leftBox;
            QVector3D tr = box.top_right;// Top-right vertex of the left box
            tr[this->splitAxis] =  splitCandidate;
            leftBox.initialize(box.bottom_left, tr);
            this->left->construct(leftTriangles, leftBox, depth+1);
        }
        else
        {
            this->left = nullptr;
        }

        if (rightTriangles.size() > 0)
        {

            this->right = new treeNode;
            AABBox rightBox;
            QVector3D bl = box.bottom_left;// Bottom-left vertex of the right box
            bl[this->splitAxis] =  splitCandidate;
            rightBox.initialize(bl, box.top_right);
            this->right->construct(rightTriangles, rightBox, depth+1);
        }
        else
        {
            this->right = nullptr;
        }
    }
}


/*
 * Check whether or not the ray intersects the triangles
 * return with the t (distance of the ray) and normal(the normal at the  intersect point)
*/
bool treeNode::intersect(const Ray &r, Intersection &intersection)
{
    // If the ray hit the AABBox of the node
    if (this->aabbox.isHit(r))
    {

        bool isIntersect = false;//The ray has not hit the triangle , set false; The ray has hit a triangle, set true

        // If this node is the leaf node
        if (this->isLeaf)
        {

            for (int i=0; i<this->triangles.size(); i++)
            {
                if (this->triangles[i]->aabbox.isHit(r))
                {
                    Intersection temp_intersection;
                    if (this->triangles[i]->intersect(r, temp_intersection))// If the ray intersect with the triangle
                    {
                        if (isIntersect == false)
                        {
                           isIntersect = true;
                           intersection = temp_intersection;
                        }
                        else if (intersection.t > temp_intersection.t)
                        {

                            intersection = temp_intersection;
                        }
                    }// if (this->triangles[i]->intersect(r, temp_t, temp_normal))

                }// if (this->triangles[i]->aabbox.isHit(r))

            }// for (int i=0; i<this->triangles.size(); i++)

            if (isIntersect == true)
            {
                return true;
            }
            else// The ray hit the node's AABBox but not hit any tirangle
            {
                return  false;
            }

        }// if (this->isLeaf)
        else// Else this node is not the leaf node
        {

            Intersection left_isect;
            Intersection right_isect;

            if (this->left!=nullptr && this->left->intersect(r, left_isect))
            {
                isIntersect = true;
                intersection = left_isect;
            }

            if (this->right!=nullptr && this->right->intersect(r, right_isect))
            {
                if (isIntersect == false)
                {
                    isIntersect = true;
                    intersection = right_isect;
                }
                else if ( right_isect.t < left_isect.t)
                {
                    intersection = right_isect;
                }
            }

            if (isIntersect == true)
            {
                return  true;
            }
            else
            {
                return  false;
            }
        }// Else this node is not the leaf node

    }
    else// Else the ray not hit the AABBox of the node
    {
        return false;
    }
}



/*
 * Check whether or not the point of the object can see the light source
 * (That means between the point and the light source there are no objects)
*/
bool treeNode::seeLightSource(const Ray &r,const float length)
{
    // If the ray hit the AABBox of the node
    if (this->aabbox.isHit(r))
    {

        // If this node is the leaf node
        if (this->isLeaf)
        {

            for (int i=0; i<this->triangles.size(); i++)
            {
                if (this->triangles[i]->aabbox.isHit(r))
                {
                    Intersection temp_intersection;
                    if (this->triangles[i]->intersect(r, temp_intersection))// If the ray intersect with the triangle
                    {
                        if (temp_intersection.t < length) //The ray hit a triangle first
                        {
                            return false;
                        }
                    }// if (this->triangles[i]->intersect(r, temp_t, temp_normal))

                }// if (this->triangles[i]->aabbox.isHit(r))

            }// for (int i=0; i<this->triangles.size(); i++)
            return true;

        }// if (this->isLeaf)
        else// Else this node is not the leaf node
        {

            if (this->left!=nullptr && this->left->seeLightSource(r, length) ==false) //The ray hit a triangle at the left node
            {
                return false;
            }

            if (this->right!=nullptr && this->right->seeLightSource(r, length) == false)//The ray hit a trianle at the right node
            {
                return  false;
            }

            return true;
        }// Else this node is not the leaf node

    }
    else// Else the ray not hit the AABBox of the node
    {
        return true;
    }
}
