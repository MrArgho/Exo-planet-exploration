// sphere.h

#ifndef sphere_h
#define sphere_h
#define PI 3.1416
#include <glad/glad.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.h"
#include <cmath>

using namespace std;

class Sphere {
public:
    // Material properties
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    // Texture properties
    float TXmin = 0.0f;
    float TXmax = 1.0f;
    float TYmin = 0.0f;
    float TYmax = 1.0f;
    unsigned int diffuseMap;
    unsigned int specularMap;

    float shininess;

    Sphere() {
        setDefaults();
        setUpSphereVertexDataAndConfigureVertexAttribute();
    }

    Sphere(float radius, int sectorCount, int stackCount, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, float shiny,
        unsigned int dMap, unsigned int sMap, float textureXmin, float textureYmin, float textureXmax, float textureYmax) {
        set(radius, sectorCount, stackCount, amb, diff, spec, shiny, dMap, sMap, textureXmin, textureYmin, textureXmax, textureYmax);
        setUpSphereVertexDataAndConfigureVertexAttribute();
    }

    ~Sphere() {
        glDeleteVertexArrays(1, &sphereTexVAO);
        glDeleteBuffers(1, &sphereVBO);
        glDeleteBuffers(1, &sphereEBO);
    }

    void set(float radius, int sectorCount, int stackCount, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, float shiny,
        unsigned int dMap, unsigned int sMap, float textureXmin, float textureYmin, float textureXmax, float textureYmax) {
        setDefaults();
        this->radius = radius;
        this->sectorCount = sectorCount;
        this->stackCount = stackCount;
        this->ambient = amb;
        this->diffuse = diff;
        this->specular = spec;
        this->shininess = shiny;

        this->diffuseMap = dMap;
        this->specularMap = sMap;
        this->TXmin = textureXmin;
        this->TXmax = textureXmax;
        this->TYmin = textureYmin;
        this->TYmax = textureYmax;
    }

    void drawSphereWithTexture(Shader& lightingShaderWithTexture, glm::mat4 model = glm::mat4(1.0f)) {
        lightingShaderWithTexture.use();

        lightingShaderWithTexture.setVec3("material.ambient", this->ambient);
        lightingShaderWithTexture.setVec3("material.diffuse", this->diffuse);
        lightingShaderWithTexture.setVec3("material.specular", this->specular);
        lightingShaderWithTexture.setFloat("material.shininess", this->shininess);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, this->diffuseMap);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, this->specularMap);

        lightingShaderWithTexture.setMat4("model", model);

        glBindVertexArray(sphereTexVAO);
        glDrawElements(GL_TRIANGLES, getIndexCount(), GL_UNSIGNED_INT, 0);
    }

private:
    unsigned int sphereTexVAO;
    unsigned int sphereVBO;
    unsigned int sphereEBO;

    float radius;
    int sectorCount;
    int stackCount;
    int verticesCount;

    vector<float> vertices;
    vector<unsigned int> indices;
    vector<float> normals;
    vector<float> texCoords;

    void setDefaults() {
        this->radius = 1.0f;
        this->sectorCount = 36;
        this->stackCount = 18;
        this->ambient = glm::vec3(1.0f, 0.0f, 0.0f);
        this->diffuse = glm::vec3(1.0f, 0.0f, 0.0f);
        this->specular = glm::vec3(0.5f, 0.5f, 0.5f);
        this->shininess = 32.0f;
        this->diffuseMap = 0;
        this->specularMap = 0;
        this->TXmin = 0.0f;
        this->TXmax = 1.0f;
        this->TYmin = 0.0f;
        this->TYmax = 1.0f;
    }

void generateSphereData() {
    // Calculate vertex, normal, and texture coordinate data for the sphere
    // ...

    verticesCount = (stackCount + 1) * (sectorCount + 1);
    vertices.reserve(verticesCount * 8);
    normals.reserve(verticesCount * 8);
    texCoords.reserve(verticesCount * 2);

    for (int i = 0; i <= stackCount; ++i) {
        float stackAngle = PI / 2.0f - (PI * i / stackCount);
        float xy = radius * cos(stackAngle);
        float z = radius * sin(stackAngle);

        for (int j = 0; j <= sectorCount; ++j) {
            float sectorAngle = 2.0f * PI * j / sectorCount;
            float x = xy * cos(sectorAngle);
            float y = xy * sin(sectorAngle);

            // Convert to spherical coordinates
            float theta = atan2(-z, x);
            float phi = acos(-y / radius);

            // Remap spherical coordinates to [0, 1)
            float u = (float)j / sectorCount;
            float v = (float)i / stackCount;

            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);
            normals.push_back(x);
            normals.push_back(y);
            normals.push_back(z);
            texCoords.push_back(u);
            texCoords.push_back(v);
        }
    }

    // Generate sphere indices
    indices.reserve(stackCount * sectorCount * 6);
    for (int i = 0; i < stackCount; ++i) {
        int k1 = i * (sectorCount + 1);
        int k2 = (i + 1) * (sectorCount + 1);

        for (int j = 0; j < sectorCount; ++j, ++k1, ++k2) {
            // 2 triangles per sector excluding first and last stacks
            // k1 => k2 => k1+1
            indices.push_back(k1);
            indices.push_back(k2);
            indices.push_back(k1 + 1);
            // k1+1 => k2 => k2+1
            indices.push_back(k1 + 1);
            indices.push_back(k2);
            indices.push_back(k2 + 1);
        }
    }
}



    void setUpSphereVertexDataAndConfigureVertexAttribute() {
        generateSphereData();

        glGenVertexArrays(1, &sphereTexVAO);
        glGenBuffers(1, &sphereVBO);
        glGenBuffers(1, &sphereEBO);

        glBindVertexArray(sphereTexVAO);
        glBindBuffer(GL_ARRAY_BUFFER, sphereVBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphereEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 24, (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 24, (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 24, (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);
    }

    int getIndexCount() const {
        return indices.size();
    }
};

#endif /* sphere_h */

