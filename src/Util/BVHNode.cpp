#include <algorithm>
#include "BVHNode.h"


#include <iostream>

using namespace std;
using namespace Eigen;

BVHNode::BVHNode() :
    left(NULL),
    right(NULL),
    object(NULL),
    min(INFINITY, INFINITY, INFINITY),
    max(-INFINITY, -INFINITY, -INFINITY)
{
}

void BVHNode::recursiveBuild(const vector<shared_ptr<Geometry>> &objects,
                             const vector<shared_ptr<Geometry>>::iterator start,
                             const vector<shared_ptr<Geometry>>::iterator end,
                             int axis) {
    if (distance(start, end) <= 1) {
        if (distance(start, end) == 1) {
            object = objects.at(start - objects.begin());
            object->boundingBox(min, max);
        }
        return;
    }

    sortOnAxis(start, end, axis);

    left = make_shared<BVHNode>();
    right = make_shared<BVHNode>();

    int half = std::distance(start, end) / 2;

    left->recursiveBuild(objects, start, start + half, (axis + 1) % 3);
    right->recursiveBuild(objects, start + half, end, (axis + 1) % 3);
    calculateBoundingBox();
}

void BVHNode::calculateBoundingBox() {
    Vector3f leftMin, leftMax, rightMin, rightMax;
    if (left != NULL) {
        leftMin = left->getMin();
        leftMax = left->getMax();
    }
    else {
        leftMin << min;
        leftMax << max;
    }

    if (right != NULL) {
        rightMin = right->getMin();
        rightMax = right->getMax();
    }
    else {
        rightMin << min;
        rightMax << max;
    }

    min << (leftMin.x() < rightMin.x() ? leftMin.x() : rightMin.x()),
           (leftMin.y() < rightMin.y() ? leftMin.y() : rightMin.y()),
           (leftMin.z() < rightMin.z() ? leftMin.z() : rightMin.z());

    max << (leftMax.x() > rightMax.x() ? leftMax.x() : rightMax.x()),
           (leftMax.y() > rightMax.y() ? leftMax.y() : rightMax.y()),
           (leftMax.z() > rightMax.z() ? leftMax.z() : rightMax.z());
}

bool sortOnX(shared_ptr<Geometry> a, shared_ptr<Geometry> b) {
    return a->getCenter().x() < b->getCenter().x();
}

bool sortOnY(shared_ptr<Geometry> a, shared_ptr<Geometry> b) {
    return a->getCenter().y() < b->getCenter().y();
}

bool sortOnZ(shared_ptr<Geometry> a, shared_ptr<Geometry> b) {
    return a->getCenter().z() < b->getCenter().z();
}

void BVHNode::sortOnAxis(vector<shared_ptr<Geometry>>::iterator start,
                         vector<shared_ptr<Geometry>>::iterator end,
                         int axis) {
    if (axis == 0) {
        sort(start, end, sortOnX);
    }
    else if (axis == 1) {
        sort(start, end, sortOnY);
    }
    else {
        sort(start, end, sortOnZ);
    }
}

void BVHNode::build(vector<shared_ptr<Geometry>> &objects) {
    recursiveBuild(objects, objects.begin(), objects.end(), 0);
}
