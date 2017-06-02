#include <algorithm>
#include <iostream>
#include "BVHNode.h"

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

Eigen::Vector3f BVHNode::minVector(const Eigen::Vector3f &a, const Eigen::Vector3f &b) {
    Vector3f minVec;
    minVec.x() = std::min(a.x(), b.x());
    minVec.y() = std::min(a.y(), b.y());
    minVec.z() = std::min(a.z(), b.z());
    return minVec;
}

Eigen::Vector3f BVHNode::maxVector(const Eigen::Vector3f &a, const Eigen::Vector3f &b) {
    Vector3f maxVec;
    maxVec.x() = std::max(a.x(), b.x());
    maxVec.y() = std::max(a.y(), b.y());
    maxVec.z() = std::max(a.z(), b.z());
    return maxVec;
}

void BVHNode::calculateBoundingBox() {
    if (left != NULL) {
        Vector3f leftMin = left->getMin();
        Vector3f leftMax = left->getMax();

        min = minVector(min, leftMin);
        max = maxVector(max, leftMax);
    }

    if (right != NULL) {
        Vector3f rightMin = right->getMin();
        Vector3f rightMax = right->getMax();

        min = minVector(min, rightMin);
        max = maxVector(max, rightMax);
    }
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

bool BVHNode::hitsNode(const Ray &r) {
    float tgmin = -INFINITY, tgmax = INFINITY;
    Vector3f d = r.direction();
    float dx = d.x();
    float dy = d.y();
    float dz = d.z();
    if (dx == 0 || dy == 0 || dz == 0) {
        return false;
    }

    Vector3f origin = r.origin();

    float t1 = (min.x() - origin.x()) / dx;
    float t2 = (max.x() - origin.x()) / dx;

    if (t1 > t2) {
        swap(t1, t2);
    }
    if (t1 > tgmin) {
        tgmin = t1;
    }
    if (t2 < tgmax) {
        tgmax = t2;
    }

    t1 = (min.y() - origin.y()) / dy;
    t2 = (max.y() - origin.y()) / dy;
    if (t1 > t2) {
        swap(t1, t2);
    }
    if (t1 > tgmin) {
        tgmin = t1;
    }
    if (t2 < tgmax) {
        tgmax = t2;
    }

    t1 = (min.z() - origin.z()) / dz;
    t2 = (max.z() - origin.z()) / dz;
    if (t1 > t2) {
        swap(t1, t2);
    }
    if (t1 > tgmin) {
        tgmin = t1;
    }
    if (t2 < tgmax) {
        tgmax = t2;
    }

    if (tgmin > tgmax || tgmax < 0) {
        return false;
    }

    return true;
}

std::shared_ptr<Geometry> BVHNode::firstHit(const Ray &r, floatOptional &t) {
    shared_ptr<Geometry> hitObj = NULL;
    t = {false, INFINITY};
    // this isn't a leaf node
    if (object == NULL) {
        if (hitsNode(r)) {
            floatOptional leftT, rightT;
            auto leftObj = left->firstHit(r, leftT);
            auto rightObj = right->firstHit(r, rightT);

            if (leftT.valid && leftT.value > 0.001f) {
                t = leftT;
                hitObj = leftObj;
            }
            if (rightT.valid && rightT.value < t.value && rightT.value > 0.001f) {
                t = rightT;
                hitObj = rightObj;
            }
        }
    }
    // this is a leaf node, check if ray hits object
    else {
        t = object->intersect(r);
        if (t.valid && t.value > 0.001f) {
            hitObj = object;
        }
    }
    return hitObj;
}

void BVHNode::print(int depth) {
    string indent = "";
    for (size_t i = 0; i < depth; i++) {
        indent += "  ";
    }
    if (object) {
        cout << indent << object->type();
        cout << ": (" << min.x() << ", " << min.y() << ", " << min.z() << ")";
        cout << " - " << "(" << max.x() << ", " << max.y() << ", " << max.z() << ")" << endl;
    }
    else {
        cout << indent << "(" << min.x() << ", " << min.y() << ", " << min.z() << ")";
        cout << " - " << "(" << max.x() << ", " << max.y() << ", " << max.z() << ")" << endl;
    }
    if (left) {
        left->print(depth + 1);
    }
    if (right) {
        right->print(depth + 1);
    }
}
