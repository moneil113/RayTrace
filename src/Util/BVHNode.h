#pragma once
#ifndef BVHNODE_H
#define BVHNODE_H

#include <memory>
#include <vector>
#include <Eigen/Dense>
#include "../Geometry/Geometry.h"

class BVHNode {
private:
    std::shared_ptr<BVHNode> left;
    std::shared_ptr<BVHNode> right;

    std::shared_ptr<Geometry> object;

    Eigen::Vector3f min;
    Eigen::Vector3f max;

    void recursiveBuild(const std::vector<std::shared_ptr<Geometry>> &objects,
                        const std::vector<std::shared_ptr<Geometry>>::iterator start,
                        const std::vector<std::shared_ptr<Geometry>>::iterator end,
                        int axis);
    void calculateBoundingBox();
    void sortOnAxis(std::vector<std::shared_ptr<Geometry>>::iterator start,
                    std::vector<std::shared_ptr<Geometry>>::iterator end,
                    int axis);

    bool hitsNode(const Ray &r);

    void print(int depth);

public:
    BVHNode();

    void build(std::vector<std::shared_ptr<Geometry>> &objects);
    const std::shared_ptr<BVHNode> &getLeft() const { return left; }
    const std::shared_ptr<BVHNode> &getRight() const { return right; }
    const Eigen::Vector3f &getMin() const { return min; }
    const Eigen::Vector3f &getMax() const { return max; }

    std::shared_ptr<Geometry> firstHit(const Ray &r, floatOptional &t);
};
#endif
