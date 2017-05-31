#include "BVHNode.h"

using namespace std;
using namespace Eigen;

BVHNode::BVHNode() {
    left = NULL;
    right = NULL;
    min << INFINITY, INFINITY, INFINITY;
    max << -INFINITY, -INFINITY, -INFINITY;
}

void BVHNode::recursiveBuild(shared_ptr<vector<shared_ptr<Geometry>>> objects,
                             vector<shared_ptr<Geometry>>::iterator start,
                             vector<shared_ptr<Geometry>>::iterator end,
                             int axis) {
    if (std::distance(start, end) <= 1) {
        calculateBoundingBox();
        return;
    }

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

    min << min(leftMin.x(), rightMin.x()),
           min(leftMin.y(), rightMin.y()),
           min(leftMin.z(), rightMin.z());

    max << max(leftMax.x(), rightMax.x()),
           max(leftMax.y(), rightMax.y()),
           max(leftMax.z(), rightMax.z());
}

void BVHNode::sortOnAxis(int axis) {

}

void BVHNode::build(shared_ptr<vector<shared_ptr<Geometry>>> objects) {
    recursiveBuild(objects, objects->begin(), objects->end(), 0);
}
