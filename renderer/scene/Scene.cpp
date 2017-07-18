#include "Scene.h"

#include <cmath>
#include <limits>
#include <fstream>
#include <cstring>
#include <iostream>
#include <algorithm>

namespace renderer
{

glm::vec3 Scene::ComputePhongIllumination(const glm::vec3 & f_pos, const glm::vec3 & n,
    const glm::vec3 & Kd, const glm::vec3 & Ks, float alpha) const
{
    glm::vec3 Id(0.0f);
    glm::vec3 Is(0.0f);

    glm::vec3 r = glm::normalize(glm::reflect(f_pos, n));

    for (auto& light : mLights)
    {
        glm::vec3 l = glm::normalize(light.pos - f_pos);  // Vector from frag to light.
        glm::vec3 intersection, temp;
        float t_triangle, t_sphere;

        int nearestTriangleIndex = Scene::NearestTriangle(l, f_pos, intersection, temp, t_triangle);
        int nearestSphereIndex = Scene::NearestSphere(l, f_pos, intersection, temp, t_sphere);

        if (((nearestTriangleIndex == -1) || (t_triangle > glm::length(light.pos - f_pos) + kEpsilon))
            && ((nearestSphereIndex == -1) || (t_sphere > glm::length(light.pos - f_pos) + kEpsilon)))
        {
            float dot_l_n = glm::dot(l, n);
            float dot_l_r = glm::dot(l, r);

            // Basic lighting - Phong model.
            Id += light.col * Kd * std::max(dot_l_n, 0.0f);
            //Is += light.col * Ks * std::pow(std::max(dot_l_r, 0.0f), alpha);
        }
    }

    return (mAmbLight + (Id + Is));
}

int Scene::NearestTriangle(const glm::vec3 & r, const glm::vec3 & O,
    glm::vec3 & intersection, glm::vec3 & baryCoord, float & t) const
{
    float nearest_t = std::numeric_limits<float>::max();
    int nearestTriangleIndex = -1;

    glm::vec3 I;  // Intersection point.
    glm::vec3 B;  // Barycentric coordinates.
    float t_test;

    // Force brute - search over all triangles.
    for (int i = 0; i < mTriangles.size(); i++)
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

int Scene::NearestSphere(const glm::vec3 & r, const glm::vec3 & O,
    glm::vec3 & intersection, glm::vec3 & n, float & t) const
{
    float nearest_t = std::numeric_limits<float>::max();
    int nearestSphereIndex = -1;

    glm::vec3 I;  // Intersection point.
    glm::vec3 normal;
    float t_test;

    // Force brute - search over all triangles.
    for (int i = 0; i < mSpheres.size(); i++)
    {
        // Test if sphere i has an intersection with input ray.
        if (IntersectionRaySphere(mSpheres[i], r, O, I, normal, t_test))
        {
            if (t_test < nearest_t)  // Found a better triangle!
            {
                nearestSphereIndex = i;
                nearest_t = t_test;

                intersection = I;
                n = normal;

                t = t_test;
            }
        }
    }

    return nearestSphereIndex;
}

// ============================================================================================= //
// FILE LOADING METHODS

// Parse 3-tuple attributes, such as position, color and so on.
bool ParseAttribute(std::ifstream & file, const std::string & expectedName, 
    glm::vec3 & v, std::string & outputMessage)
{
    std::string name;
    file >> name;
    std::transform(name.begin(), name.end(), name.begin(), ::tolower);

    if (file.fail() || (name != expectedName))
    {
        outputMessage = "Scene file parsing error - expected attribute " + expectedName + ".\n";
        return false;
    }

    file >> v[0] >> v[1] >> v[2];

    if (file.fail())
    {
        outputMessage = "Scene file parsing error - incorrect attribute " + expectedName + ".\n";
        return false;
    }

    return true;
}

// Parse a single attribute, such as shininess.
bool ParseAttribute(std::ifstream & file, const std::string & expectedName, 
    float & a, std::string & outputMessage)
{
    std::string name;
    file >> name;
    std::transform(name.begin(), name.end(), name.begin(), ::tolower);

    if (file.fail() || (name != expectedName))
    {
        outputMessage = "Scene file parsing error - expected attribute " + expectedName + ".\n";
        return false;
    }

    file >> a;

    if (file.fail())
    {
        outputMessage = "Scene file parsing error - incorrect attribute " + expectedName + ".\n";
        return false;
    }

    return true;
}

void Scene::Log(std::ostream & stream)
{
    stream << "SCENE LOG" << std::endl;
    stream << "Number of light sources ... " << mLights.size() << ";" << std::endl;
    stream << "Number of triangles ....... " << mTriangles.size() << ";" << std::endl;
    stream << "Number of spheres ......... " << mSpheres.size() << ";" << std::endl;
    stream << "TOTAL ..................... " << mTriangles.size() + mSpheres.size() + mLights.size() << ";" << std::endl;
}

bool Scene::Load(const std::string & filePath, std::string & outputMessage)
{
    // Open input file.
    std::ifstream file(filePath, std::ifstream::in);

    if (!file.is_open())  // Failure - not good.
    {
        outputMessage = "Scene file at '" + filePath + "' couldn't be opened.\n";
        return false;
    }

    float n_refr = 1.00f;

    // Read ambient light.
    if (!ParseAttribute(file, "amb:", mAmbLight, outputMessage))
    {
        return false;
    }

    // Read each object data.
    while (true)
    {
        // Read type of object.
        std::string type;
        file >> type;
        std::transform(type.begin(), type.end(), type.begin(), ::tolower);

        if (file.fail())
        {
            break;
        }

        if (type == "triangle")  // Triangle.
        {
            Triangle triangle;
            TriangleAttrib attrib;

            for (int i = 0; i < 3; i++) // Read all vertices.
            {
                glm::vec3 Kd, Ks, n;
                float shininess;

                if (!(ParseAttribute(file, "pos:", triangle.v[i], outputMessage) &&
                      ParseAttribute(file, "nor:", n, outputMessage) &&
                      ParseAttribute(file, "dif:", Kd, outputMessage) &&
                      ParseAttribute(file, "spe:", Ks, outputMessage) &&
                      ParseAttribute(file, "shi:", shininess, outputMessage)
                    ))   // Failure, no new triangle.
                {
                    std::cout << "\tRefer to triangle number " << mTriangles.size() << ".\n";
                    return false;
                }

                attrib.Kd[i] = Kd;
                attrib.Ks[i] = Ks;
                attrib.n[i] = n;
                attrib.shininess[i] = shininess;
                attrib.n_refr = n_refr;
            }

            mTriangles.push_back(triangle);
            mTriangleAttribList.push_back(attrib);
        }
        else if (type == "sphere")  // Sphere.
        {
            Sphere sphere;
            SphereAttrib attrib;

            if (ParseAttribute(file, "pos:", sphere.pos, outputMessage) &&
                ParseAttribute(file, "rad:", sphere.radius, outputMessage) &&
                ParseAttribute(file, "dif:", attrib.Kd, outputMessage) &&
                ParseAttribute(file, "spe:", attrib.Ks, outputMessage) &&
                ParseAttribute(file, "shi:", attrib.shininess, outputMessage)
                )   // Success - new sphere.
            {
                attrib.n_refr = n_refr;
                mSpheres.push_back(sphere);
                mSphereAttribList.push_back(attrib);
            }
            else  // Failure, no new sphere.
            {
                //outputMessage += "\tRefer to sphere number " << std::string(mSpheres.size()) << ".\n";
                return false;
            }
        }
        else if (type == "light")  // Light Source.
        {
            Light light;

            if (ParseAttribute(file, "pos:", light.pos, outputMessage) &&
                ParseAttribute(file, "col:", light.col, outputMessage)
                )  // Success - new light source.
            {
                mLights.push_back(light);
            }
            else
            {
                //std::cout << "\tRefer to light number " << mLights.size() << ".\n";
                return false;
            }
        }
        else if (type == "obj")  // 3D Obj.
        {
            // DISABLED!. 
            std::string path;
            if (file >> path)
            {
                //Scene::LoadObj(path);
            }
        }
        else if (type == "n_refr")  // Specify new index of refraction.
        {
            if (!(file >> n_refr))
            {
                outputMessage = "Scene file parsing error - expected n_refr value.\n";
            }
        }
        else if (type == "camera")
        {
            glm::vec3 pos, rot;
            float scale = 1.0f;

            if (ParseAttribute(file, "pos:", pos, outputMessage) &&
                ParseAttribute(file, "rot:", rot, outputMessage) &&
                ParseAttribute(file, "scale:", scale, outputMessage)
                )  // Success - new camera configuration.
            {
                mCamera.SetPosition(pos);
                mCamera.SetRotation(rot);
                mCamera.SetScale(scale);
            }
        }
        else if (type == "background-color")
        {
            glm::vec3 rgb;
            float alpha = 1.0f;

            if (ParseAttribute(file, "rgb:", rgb, outputMessage)
             && ParseAttribute(file, "alpha:", alpha, outputMessage))
            {
                mBackgroundColor = glm::vec4(rgb, alpha);
            }
        }
        // INSERT HERE NEW TYPES OF OBJECT.
        else  // Invalid object.
        {
            outputMessage = "Scene file parsing error - invalid object type: '" + type + "'.\n";
            return false;
        }
    }

    return true;
}

// ============================================================================================= //

/*
bool Scene::LoadObj(const std::string & objFilepath)
{
    FILE* inFile = fopen(objFilepath.c_str(), "r");

    // Successfully opened the file.
    if (inFile != nullptr)
    {
        char line[256];  // Line buffer.
        char token[32];  // Token buffer.
        char name[100];  // Group name buffer.
        const int kNumIndices = 100;
        std::vector<float> positions;  // List of all positions (x, y, z).
        std::vector<float> texCoords;  // List of all texture (u, v).
        std::vector<float> normals;    // List of all normals (nx, ny, nz).
        bool buildingGroup = false;

        // Read until get EOF.
        while (fgets(line, sizeof(line), inFile) != nullptr)
        {
            // Exclude repeated spaces.
            int i = 0;
            while (line[i] != '\0')
            {
                if (line[i] == ' ' && line[i + 1] == ' ')
                {
                    for (int j = i; line[j] != '\0'; j++)
                    {
                        line[j] = line[j + 1];
                    }
                }
                i++;
            }

            // Read first token.
            sscanf(line, "%s", token);

            if (strcmp(token, "g") == 0)  // New group.
            {

            }
            else if (strcmp(token, "v") == 0)   // New vertex.
            {
                float x, y, z;
                sscanf(line, "%s %f %f %f", token, &x, &y, &z);
                positions.push_back(x);
                positions.push_back(y);
                positions.push_back(z);
            }
            else if (strcmp(token, "vt") == 0)  // New texture coordinate.
            {
                float u, v;
                sscanf(line, "%s %f %f", token, &u, &v);
                // texCoords.push_back(u);
                // texCoords.push_back(v);
            }
            else if (strcmp(token, "vn") == 0)  // New normal.
            {
                float nx, ny, nz;
                sscanf(line, "%s %f %f %f", token, &nx, &ny, &nz);
                normals.push_back(nx);
                normals.push_back(ny);
                normals.push_back(nz);
            }
            else if (strcmp(token, "f") == 0)  // New face (triangle).
            {
                int a_v, a_vt, a_vn;
                int b_v, b_vt, b_vn;
                int c_v, c_vt, c_vn;

                if (sscanf(line, "%s %d//%d %d//%d %d//%d", token, &a_v, &a_vn,
                    &b_v, &b_vn,
                    &c_v, &c_vn) == 7)
                {
                    // No texture coordinates
                }
                else if (sscanf(line, "%s %d/%d/%d %d/%d/%d %d/%d/%d", token, &a_v, &a_vt, &a_vn,
                    &b_v, &b_vt, &b_vn,
                    &c_v, &c_vt, &c_vn) == 9)
                {
                    // // Has texture coordinates.
                    // a_vt--;
                    // b_vt--;
                    // c_vt--;
                }

                a_v--; a_vn--;
                b_v--; b_vn--;
                c_v--; c_vn--;

                Triangle triangle;
                TriangleAttrib attrib;

                attrib.Ks = glm::mat3(glm::vec3(.7), glm::vec3(.7), glm::vec3(.7));
                attrib.Kd = glm::mat3(glm::vec3(1.0, 0.0, 0.0),
                    glm::vec3(1.0, 0.0, 0.0),
                    glm::vec3(1.0, 0.0, 0.0));
                attrib.shininess = glm::vec3(200);
                attrib.n_refr = 1.0f;

                // Vertex A.
                triangle.v[0][0] = positions[3 * a_v + 0];
                triangle.v[0][1] = positions[3 * a_v + 1];
                triangle.v[0][2] = positions[3 * a_v + 2];
                attrib.n[0][0] = normals[3 * a_vn + 0];
                attrib.n[0][1] = normals[3 * a_vn + 1];
                attrib.n[0][2] = normals[3 * a_vn + 2];

                // Vertex B.
                triangle.v[1][0] = positions[3 * b_v + 0];
                triangle.v[1][1] = positions[3 * b_v + 1];
                triangle.v[1][2] = positions[3 * b_v + 2];
                attrib.n[1][0] = normals[3 * b_vn + 0];
                attrib.n[1][1] = normals[3 * b_vn + 1];
                attrib.n[1][2] = normals[3 * b_vn + 2];

                // Vertex C.
                triangle.v[2][0] = positions[3 * c_v + 0];
                triangle.v[2][1] = positions[3 * c_v + 1];
                triangle.v[2][2] = positions[3 * c_v + 2];
                attrib.n[2][0] = normals[3 * c_vn + 0];
                attrib.n[2][1] = normals[3 * c_vn + 1];
                attrib.n[2][2] = normals[3 * c_vn + 2];

                mTriangles.push_back(triangle);
                mTriangleAttribList.push_back(attrib);
            }
            else
            {
                // Ignore anything else
            }
        }

        fclose(inFile);
        return true;
    }
    else  // Failed.
    {
        printf("ERROR Couldn't load file at %s\n", objFilepath.c_str());
        return false;
    }
}

*/
}  // namespace renderer.