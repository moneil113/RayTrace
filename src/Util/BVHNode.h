#include <memory>
#include <vector>
#include <Eigen/Dense>
#include "../Geometry/Geometry.h"

class BVHNode {
private:
    std::shared_ptr<BVHNode> left;
    std::shared_ptr<BVHNode> right;

    Eigen::Vector3f min;
    Eigen::Vector3f max;

    std::vector<std::shared_ptr<Geometry>>::iterator start;
    std::vector<std::shared_ptr<Geometry>>::iterator end;

    void recursiveBuild(std::shared_ptr<std::vector<std::shared_ptr<Geometry>>> objects,
                        std::vector<std::shared_ptr<Geometry>>::iterator start,
                        std::vector<std::shared_ptr<Geometry>>::iterator end,
                        int axis);
    void calculateBoundingBox();
    void sortOnAxis(int axis);

public:
    BVHNode();

    void build(std::shared_ptr<std::vector<std::shared_ptr<Geometry>>> objects);
    const std::shared_ptr<BVHNode> &getLeft() const { return left; }
    const std::shared_ptr<BVHNode> &getRight() const { return right; }
    const Eigen::Vector3f &getMin() const { return min; }
    const Eigen::Vector3f &getMax() const { return max; }
};
