#include "Scene.h"


//From STL
#include <fstream>
#include <sstream>
#include <cmath>

//From QT
#include <QVector3D>


using namespace  std;

Scene::Scene()
{

}

Scene::~Scene()
{


}

/*
 * Read *.obj and *.mtl together to set the objects
*/
void Scene::loadObjects(string filePath)
{
    string mtlLib;
    ifstream objFile(filePath);
    QVector <QVector3D> vertexPositions;// The container of the vertex position (v)
    QVector <QVector3D> vertexNormals;// The container of the vertex normal (vn)

    if (objFile.is_open())// Check if the file is open
    {
        string keyString;// The key string of the line
        string line;

        Material *currenMaterial = nullptr;

        while (objFile)
        {

            getline(objFile, line);
            stringstream lineStream(line);
            lineStream >> keyString;

            if (keyString == "v") // v - vertex coordinate
            {

                float x, y, z;
                lineStream >> x >> y >> z;
                QVector3D vertPos(x, y, z);
                vertexPositions.push_back(vertPos);
            }
            else if (keyString == "vn") // vn - vertex normal
            {
                float x, y, z;
                lineStream >> x >> y >> z;
                QVector3D vertNorm(x, y, z);
                vertexNormals.push_back(vertNorm);
            }
            else if (keyString == "f") // f - faces(v/vt/vn)
            {
                string substring;
                int num = 0;// Number of the vertices on the face
                QVector3D triangleVertexPostions[3];
                QVector3D triangleVertexNormals[3];

                while (lineStream >> substring) // While not at the end of the line
                {
                    int positionId;// Index of the positions
                    int normalId;// Index of the normals

                    positionId = atoi(substring.substr(0, substring.find_first_of('/')+1).c_str());
                    normalId = atoi(substring.substr(substring.find_last_of('/') + 1, substring.length() - substring.find_last_of('/')).c_str());
                    if (num <3)
                    {
                        triangleVertexPostions[num] = vertexPositions[positionId-1];
                        triangleVertexNormals[num] = vertexNormals[normalId-1];
                    }
                    else//The face has at least three vertices
                    {
                        triangleVertexPostions[1] = triangleVertexPostions[2];
                        triangleVertexNormals[1] = triangleVertexNormals[2];
                        triangleVertexPostions[2] = vertexPositions[positionId -1];
                        triangleVertexNormals[2] = vertexNormals[normalId-1];
                    }
                    num ++;
                    if (num>=3)
                    {
                        Triangle *triangle = new Triangle(triangleVertexPostions, triangleVertexNormals, currenMaterial);
                        triangle->updateAABBox();
                        this->triangles.push_back(triangle);
                    }

                }
            }
            else if (keyString == "mtllib")
            {
                string mtllibName;// Name of the material library
                lineStream >> mtllibName;
                string mtllibPath = filePath.substr(0, filePath.find_last_of('/') + 1) + mtllibName;
                this->loadMaterials(mtllibPath);

            }
            else if (keyString == "usemtl")
            {

                string mtlName;
                lineStream >> mtlName;
                currenMaterial = &this->materials[mtlName];
            }

        }

        this->updateAABBox();
        this->constructKdTree();
    }

}

void Scene::loadMaterials(string filePath)
{
    ifstream mtlFile(filePath);
    string currentMaterialName;

    if (mtlFile.is_open())
    {
        string keyString;
        string line;
        while (mtlFile)
        {
            getline(mtlFile, line);
            stringstream linestream(line);
            linestream >> keyString;
            if (keyString == "newmtl") // Create a new meterial
            {
                linestream >> currentMaterialName;

            }
            else if (keyString == "Ka") // Ka - ambient reflectivity
            {
                float r, g, b;
                linestream >> r >> g >> b;
                QVector3D ka (r, g, b);

                this->materials[currentMaterialName].Ka = ka;
            }
            else if (keyString == "Kd") // Kd - diffuse reflectivitys
            {
                float r, g, b;
                linestream >> r >> g >> b;
                QVector3D kd (r, g, b);
                this->materials[currentMaterialName].Kd = kd;


            }
            else if (keyString == "Ks") // Ks - specular reflectivity
            {
                float r, g, b;
                linestream >> r >> g >> b;
                QVector3D ks (r, g, b);
                this->materials[currentMaterialName].Ks = ks;

            }
            else if (keyString == "Tf") // Tf - transmission filter
            {
                float r, g, b;
                linestream >> r >> g >> b;
                QVector3D tf(r, g, b);
                this->materials[currentMaterialName].Tf = tf;
            }
            else if (keyString == "Ni") // Ni - optical density for the surface.
            {

                linestream >> this->materials[currentMaterialName].Ni;
            }
            else if (keyString == "Ns")// Ns - specular exponent for the current material
            {
                linestream >> this->materials[currentMaterialName].Ns;
            }

        }
    }
}


/*
 * Check whether or not the ray intersects the triangles
 * return with the t (distance of the ray) and the normal(the normal at the  intersect point)
*/
bool Scene::intersect(const Ray &ray, Intersection &intersection)
{

    if (this->kdTree.intersect(ray,intersection))
    {
        return true;
    }

    return false;
}

/*
 *  Construct a KD tree
*/
void Scene::constructKdTree()
{
    this->kdTree.construct(this->triangles, this->aabbox, 0);
}

bool Scene::seeLightSource(const Ray &ray, const float length)
{
    return  this->kdTree.seeLightSource(ray,length);
}

/*
* Update AABBox after loading objects
*/
void Scene::updateAABBox()
{
    int nSize = this->triangles.size();// The size of the triangles

    //Set the first triangle's bottom-left veretx and top-right vertex as the AABBox's bottom-left veretx and top-rightvertex,respectively.
    this->aabbox.initialize(this->triangles[0]->aabbox.bottom_left, this->triangles[0]->aabbox.top_right);

    for (int i=1; i<nSize; i++)
    {
        this->aabbox.expand(this->triangles[i]->aabbox);
    }
}
