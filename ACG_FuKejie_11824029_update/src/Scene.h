#ifndef SCENE_H
#define SCENE_H

// From STL
#include <string>
//#include <map>

// From QT
#include <QVector>


#include "Material.h"
#include "Ray.h"
#include "Material.h"
#include "KdTree.h"
#include "Triangle.h"
#include "AABBox.h"
#include "LightSource.h"
#include "Intersection.h"
#include "Camera.h"
using namespace std;

/*
 *
*/
class Scene
{
public:
    Scene();

    ~Scene();


    /*
     * Read *.obj and *.mtl together to set the objects
    */
    void loadObjects(string filePath);

    /*
     *
    */
    void loadMaterials(string filePath);

    /*
     * Check whether or not current ray intersects the triangles, further find the nearest intersections
    */
    bool intersect(const Ray &ray, Intersection &intersection);

    /*
     * Construct KdTree
    */
    void constructKdTree();

    /*
     * Check whether or not the point can see the light source by a sample ray from sampling from the light source
    */
    bool seeLightSource(const Ray &ray, const float length);

    /*
     * Update AABBox
    */
    void updateAABBox();

public:
    QVector <Triangle*> triangles;
    QVector <LightSource*> lights;
    map<std::string, Material> materials;//The materials used in this scene ( map<material name, material attributes>)
    Camera camera;
    treeNode kdTree;
    AABBox aabbox;
};


#endif // SCENE_H
