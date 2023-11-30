

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "camera.h"
#include "basic_camera.h"
#include "pointLight.h"
#include "cube.h"
#include "curve.h"
#include "Sphere.h"
#include "Sphere2.h"
#include "stb_image.h"


#include <iostream>

using namespace std;
float M_PI = 3.1416;
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
//void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void trees(Shader& lightingShader, glm::mat4 alTogether, Curve& head, Curve& body);
void ponds(Shader& lightingShader, glm::mat4 alTogether, Curve& pond, Curve& pond_water);
void Jupiter(Shader& lightingShaderWithTexture, glm::mat4 altogether, Sphere2& jupiter, Curve& Pond, Curve& Pond_water, Curve& tree_base, Curve& tree_leaf, Curve& hill, Curve& special_tree);
void make_special_tree(Shader& lightingShader, glm::mat4 alTogether, Curve& special_tree, glm::vec3 position, float angle, glm::vec3 angle_axis);
void make_tree(Shader& lightingShaderWithTexture, glm::mat4 alTogether, Curve& tree_base, Curve& tree_leaf, glm::vec3 position, float angle, glm::vec3 angle_axis);
void make_ponds(Shader& lightingShaderWithTexture, glm::mat4 alTogether, Curve& Pond, Curve& Pond_water, glm::vec3 position, float angle, glm::vec3 angle_axis);
void make_hill(Shader& lightingShader, glm::mat4 alTogether, Curve& hill, glm::vec3 position, float angle, glm::vec3 angle_axis);
void processInput(GLFWwindow* window);
unsigned int loadTexture(char const* path, GLenum textureWrappingModeS, GLenum textureWrappingModeT, GLenum textureFilteringModeMin, GLenum textureFilteringModeMax);
float counter = 1.0;
int trigger = 0;
int split = 0;
// settings
const unsigned int SCR_WIDTH = 1000;
const unsigned int SCR_HEIGHT = 700;

// modelling transform
float rotateAngle_X = 0.0;
float rotateAngle_Y = 0.0;
float rotateAngle_Z = 0.0;
float rotateAxis_X = 0.0;
float rotateAxis_Y = 0.0;
float rotateAxis_Z = 1.0;
float translate_X = 0.0;
float translate_Y = 0.0;
float translate_Z = 0.0;
float scale_X = 1.0;
float scale_Y = 1.0;
float scale_Z = 1.0;
float spacex = 0.0;
float spacey = 0.0;
float spacez = 0.0;
float spin_of_jupiter = 0.0;
int countt = 0;
float currentAngleUfo = 90.0f;
float currentAngleUfoPhi = 90.0f;
float spot_list_pos_x = 5.0;
float spot_list_pos_y = 4.0;
float spot_list_pos_z = 5.0;

// camera
Camera camera(glm::vec3(0.0f, 1.1f, 10.2f));
//Camera camera(glm::vec3(20.0f, 20.0f, 20.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float eyeX = 0.0, eyeY = 1.0, eyeZ = 3.0;
float lookAtX = 0.0, lookAtY = 0.0, lookAtZ = 0.0;
glm::vec3 V = glm::vec3(0.0f, 1.0f, 0.0f);
BasicCamera basic_camera(eyeX, eyeY, eyeZ, lookAtX, lookAtY, lookAtZ, V);

// lights
// positions of the point lights
// positions of the point lights
glm::vec3 pointLightPositions[] = {
    /*glm::vec3(1.50f,  1.50f,  0.0f),
    glm::vec3(1.5f,  -1.5f,  0.0f),
    glm::vec3(-1.5f,  1.5f,  0.0f)*/

    //glm::vec3(3.0f,  3.0f,  0.0f), //spotlight
    glm::vec3(spot_list_pos_x,  spot_list_pos_y,  spot_list_pos_z), //spotlight
    glm::vec3(-3.0f,  -3.0f,  0.0f),  //pointlight
    glm::vec3(0.0f,  0.0f,  0.0f), //directional light

};

vector<float>Fish = {
-0.0100, 1.9950, 5.1000,
-0.0550, 1.9800, 5.1000,
-0.0950, 1.9350, 5.1000,
-0.1500, 1.8250, 5.1000,
-0.2250, 1.5900, 5.1000,
-0.2550, 1.3450, 5.1000,
-0.2050, 1.1700, 5.1000,
-0.1400, 1.0050, 5.1000,
-0.0400, 0.8600, 5.1000,
0.0400, 0.7300, 5.1000,
0.1300, 0.6350, 5.1000,
0.2400, 0.5050, 5.1000,
};
std::vector<float> Mountain = {

    // Body of the spaceship
    0, 1, 0,
    1, 1, 0,
    1, 0, 0,
    0, 0, 0,
    0, 1, 0,

    // Wing 1
    1, 0.5, 0,
    1.5, 0.5, 0,
    1.25, 0.25, 0,
    1, 0.5, 0,

    // Wing 2
    0, 0.5, 0,
    -0.5, 0.5, 0,
    -0.25, 0.25, 0,
    0, 0.5, 0
};

std::vector<float> Cylinder = {
    // Base of the cylinder
0.2, 0, 0,
0.2, 1, 0,
0.1732, 1, 0,
0.1732, 0, 0,
0.1732, -1, 0,
0.2, -1, 0,
0.2, 0, 0,

// Top of the cylinder
0.2, 0, 1,
0.2, 1, 1,
0.1732, 1, 1,
0.1732, 0, 1,
0.1732, -1, 1,
0.2, -1, 1,
0.2, 0, 1

};

std::vector<float> special_tree = {
    // Base of the cylinder
   0.2, 0, 0,
   0.2, 1, 0,
   0.1732, 1, 0,
   0.1732, 0, 0,
   0.1732, -1, 0,
   0.2, -1, 0,
   0.2, 0, 0,

   // Top of the cylinder
   0.2, 0, 1,
   0.2, 1, 1,
   0.1732, 1, 1,
   0.1732, 0, 1,
   0.1732, -1, 1,
   0.2, -1, 1,
   0.2, 0, 1




};

std::vector<float> tree_head = {
    -0.0050, 2.5650, 5.1000,
-0.2250, 2.4400, 5.1000,
-0.6300, 2.3150, 5.1000,
-0.9750, 2.1900, 5.1000,
-1.3150, 1.9650, 5.1000,
-1.1650, 1.8850, 5.1000,
-0.9550, 1.8900, 5.1000,
-0.5800, 1.8900, 5.1000,
-0.1550, 1.8700, 5.1000,
-0.0600, 1.8300, 5.1000,
-0.4650, 1.5600, 5.1000,
-0.8200, 1.4200, 5.1000,
-1.2250, 1.2050, 5.1000,
-1.5500, 0.9450, 5.1000,
-1.6750, 0.7250, 5.1000,
-1.7050, 0.5350, 5.1000,
-1.6850, 0.4050, 5.1000,
-1.4500, 0.3000, 5.1000,
-1.2300, 0.3000, 5.1000,
-0.8050, 0.3000, 5.1000,
};

std::vector<float> pond = {
-1.4800, 0.9000, 5.1000,
-1.6500, 0.6950, 5.1000,
-1.7550, 0.5950, 5.1000,
-1.8650, 0.4600, 5.1000,
-1.9400, 0.3350, 5.1000,
-1.9650, 0.2250, 5.1000,
-1.9650, 0.1150, 5.1000,
-1.9350, 0.0700, 5.1000,
-1.9000, 0.0650, 5.1000,
};
std::vector<float> water = {

        -0.0050, 1.7650, 5.1000,
    -1.8150, 1.5700, 5.1000,
    0.0100, 1.5750, 5.1000,
};
std::vector<float> hill = {
-0.1700, 2.5650, 5.1000,
-0.3050, 2.4050, 5.1000,
-0.4900, 2.1700, 5.1000,
-0.7150, 2.0850, 5.1000,
-0.8550, 1.7050, 5.1000,
-0.9150, 1.4200, 5.1000,
-1.3400, 1.0350, 5.1000,
-1.5600, 0.6750, 5.1000,
-1.8650, 0.2000, 5.1000,
-1.9450, -0.1300, 5.1000,
-1.9600, -0.1950, 5.1000,
};


PointLight pointlight1(//spotlight

    pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z,  // position
    0.1f, 0.1f, 0.1f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    //0.0f, 0.0f, 0.0f,     // ambient
    //0.0f, 0.0f, 0.0f,     // diffuse
    //0.0f, 0.0f, 0.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    1    ,   // light number
    0, 0 , 0
);
PointLight pointlight2(

    pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z,  // position
    //0.05f, 0.05f, 0.05f,     // ambient
    ////0.8f, 0.8f, 0.8f,     // ambient
    //0.8f, 0.8f, 0.8f,     // diffuse
    //1.0f, 1.0f, 1.0f,        // specular
    0.00f, 0.00f, 0.00f,     // ambient
    0.0f, 0.0f, 0.0f,     // diffuse
    0.0f, 0.0f, 0.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    2       // light number
);
PointLight pointlight3(//directional light

    pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z,  // position
    //0.05f, 0.05f, 0.05f,     // ambient
    0.6f, 0.6f, 0.6f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    100      // light number
);


// light settings
bool dirLightOn = true;
bool pointLightOn = true;
bool spotLightOn = true;
bool ambientToggle = true;
bool diffuseToggle = true;
bool specularToggle = true;


// timing
float deltaTime = 0.0f;    // time between current frame and last frame
float lastFrame = 0.0f;


std::vector<glm::vec3> ellipsePoints;

void calculateEllipsePoints() {
    const int numPoints = 20000;
    const float semiMajorAxis = 15.0f; // Adjust the major axis as needed
    const float semiMinorAxis = 15.0f; // Adjust the minor axis as needed

    for (int i = 0; i < numPoints; ++i) {
        float angle = (2 * 3.1416 * i) / numPoints;
        float x = semiMajorAxis * cos(angle);
        float z = semiMinorAxis * sin(angle);
        glm::vec3 point(x, 0.0f, z);
        ellipsePoints.push_back(point);
    }
}
void printEllipsePoints() {
    /*for (const glm::vec3& point : ellipsePoints) {
        std::cout << "x: " << point.x << ", y: " << point.y << ", z: " << point.z << std::endl;
    }*/
}

// Calculate the Euclidean distance between two points in 3D space
float calculateDistance(glm::vec3 point1, glm::vec3 point2) {
    return glm::distance(point1, point2);
}

float catch_the_ufo = 0.0;
float ufo_land_x = 0.0f;
float ufo_land_y = 4.2f;
float ufo_land_z = 2.5f;
float ufo_land_call_trigger = 0.0;
float little_ufo_on = 0.0;
float Inspection = 0.0;
glm::mat4 static_view;

//void ufo_landing();
int main()
{



    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CSE 4208: Computer Graphics Laboratory", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    /*   glfwSetKeyCallback(window, key_callback);*/
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile our shader zprogram
    // ------------------------------------

    Shader lightingShaderWithTexture("vertexShaderForPhongShadingWithTexture.vs", "fragmentShaderForPhongShadingWithTexture.fs");
    Shader ourShader("vertexShader.vs", "fragmentShader.fs");


    /////////////////////////////////////////


    //sun
    string diffuseMapPath = "13913_Sun_diff.jpg";
    string specularMapPath = "13913_Sun_diff.jpg";

    unsigned int diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap = loadTexture(specularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);

    Sphere2 sun = Sphere2(2.0f, 144, 72, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.5f, 0.5f, 0.5f), 32.0f, diffMap, specMap, 0.0f, 0.0f, 1.0f, 1.0f);

    //earth
    string diffuseMapPath_j = "Saturn2.jpg";
    string specularMapPath_j = "Saturn2.jpg";

    unsigned int diffMap_j = loadTexture(diffuseMapPath_j.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap_j = loadTexture(specularMapPath_j.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);
    // set(radius, sectorCount, stackCount, ambient, diffuse, specular, shininess, diffuseMap, specularMap, TYmax, TYmin, TXmax, TYmax);
    Sphere2 jupiter = Sphere2(1.0f, 144, 72, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.5f, 0.5f, 0.5f), 32.0f, diffMap_j, specMap_j, 0.0f, 0.0f, 1.0f, 1.0f);


    diffuseMapPath = "space.jpg";
    specularMapPath = "space.jpg";

    diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specMap = loadTexture(specularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);
    //amb,  diff,  spec, shiny
    Cube cube = Cube(diffMap, specMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    Cube cube_test = Cube(diffMap_j, specMap_j, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);



    //UFO

    string diffuseHillPath = "ufo.jpg";
    string specularHillPath = "ufo.jpg";
    unsigned int diffuseHill = loadTexture(diffuseHillPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specularHill = loadTexture(specularHillPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);
    Curve  UFO = Curve(Mountain, diffuseHill, specularHill, 5);

    string diffuseHillPath2 = "ufo2.jpg";
    string specularHillPath2 = "ufo2.jpg";
    unsigned int diffuseHill2 = loadTexture(diffuseHillPath2.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specularHill2 = loadTexture(specularHillPath2.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);
    Curve  UFO2 = Curve(Mountain, diffuseHill2, specularHill2, 5);


    string ufo2_diffuse = "ufo2.jpg";
    string ufo2_specular = "ufo2.jpg";

    unsigned int ufo2_diff = loadTexture(ufo2_diffuse.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int ufo2_spec = loadTexture(ufo2_specular.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);
    // set(radius, sectorCount, stackCount, ambient, diffuse, specular, shininess, diffuseMap, specularMap, TYmax, TYmin, TXmax, TYmax);
    Sphere2 ufo2 = Sphere2(.5f, 144, 72, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.5f, 0.5f, 0.5f), 32.0f, ufo2_diff, ufo2_spec, 0.0f, 0.0f, 1.0f, 1.0f);



    //tree
    string diffuse_tree_Path = "tree.jpg";
    string specular_tree_Path = "tree.jpg";
    unsigned int diffuse_tree = loadTexture(diffuse_tree_Path.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specular_tree = loadTexture(specular_tree_Path.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);
    Curve  tree_base = Curve(Cylinder, diffuse_tree, specular_tree, 5);

    diffuse_tree_Path = "leaf.jpg";
    specular_tree_Path = "leaf.jpg";
    diffuse_tree = loadTexture(diffuse_tree_Path.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specular_tree = loadTexture(specular_tree_Path.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);
    Curve  tree_leaf = Curve(tree_head, diffuse_tree, specular_tree, 5);



    //pond
    string diffuse_pond_Path = "Saturn2.jpg";
    string specular_pond_Path = "Saturn2.jpg";
    unsigned int diffuse_pond = loadTexture(diffuse_pond_Path.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specular_pond = loadTexture(specular_pond_Path.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);
    Curve  Pond = Curve(pond, diffuse_pond, specular_pond, 5);

    diffuse_pond_Path = "water6.jpg";
    specular_pond_Path = "water6.jpg";
    diffuse_pond = loadTexture(diffuse_pond_Path.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specular_pond = loadTexture(specular_pond_Path.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);
    Curve  Pond_water = Curve(water, diffuse_pond, specular_pond, 5);

    //hill
    string diffuse_hill_Path = "stone2.jpg";
    string specular_hill_Path = "stone.jpg";
    unsigned int diffuse_hill = loadTexture(diffuse_hill_Path.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specular_hill = loadTexture(specular_hill_Path.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);
    Curve  Hill = Curve(hill, diffuse_hill, specular_hill, 5);

    //special_tree
    string diffuse_special_tree_Path = "tree2.jpg";
    string specular_special_tree_Path = "stone.jpg";
    unsigned int diffuse_special_tree = loadTexture(diffuse_special_tree_Path.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specular_special_tree = loadTexture(specular_special_tree_Path.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);
    Curve  Special_tree = Curve(special_tree, diffuse_special_tree, specular_special_tree, 5);

    //

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    ///////////////////////////////////////////////////





    // render loop
    // -----------

    calculateEllipsePoints();
    //printEllipsePoints();
    static_view = camera.GetViewMatrix();
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // be sure to activate shader when setting uniforms/drawing objects
        lightingShaderWithTexture.use();
        lightingShaderWithTexture.setVec3("viewPos", camera.Position);

        // pass projection matrix to shader (note that in this case it could change every frame)
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        //glm::mat4 projection = glm::ortho(-2.0f, +2.0f, -1.5f, +1.5f, 0.1f, 100.0f);
        lightingShaderWithTexture.setMat4("projection", projection);

        // camera/view transformation
        glm::mat4 view = camera.GetViewMatrix();
        
        //glm::mat4 view = basic_camera.createViewMatrix();

        //if(little_ufo_on != 1.0f)
        if(Inspection==0.0)
            lightingShaderWithTexture.setMat4("view", view);

        // Modelling Transformation
        glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;
        translateMatrix = glm::translate(identityMatrix, glm::vec3(translate_X, translate_Y, translate_Z));
        rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_X), glm::vec3(1.0f, 0.0f, 0.0f));
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X, scale_Y, scale_Z));
        model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;


        lightingShaderWithTexture.use();
        // point light 1
        pointlight1.setUpPointLight(lightingShaderWithTexture);
        // point light 2
        pointlight2.setUpPointLight(lightingShaderWithTexture);
        // point light 3
        pointlight3.setUpPointLight(lightingShaderWithTexture);
        // point light 4


        //space
        glm::mat4 model_for_space = glm::mat4(1.0f);
        model_for_space = glm::translate(model_for_space, glm::vec3(-35, -35, -35));
        model_for_space = glm::scale(model_for_space, glm::vec3(70, 70, 70));
        //model_for_space = glm::scale(model_for_space, glm::vec3(100, 100, 00));
        cube.drawCubeWithTexture(lightingShaderWithTexture, model_for_space);


        //sun
        glm::mat4 modelMatrixForContainer2 = glm::mat4(1.0f);
        //modelMatrixForContainer2 = glm::translate(model, glm::vec3(-0.45f, -0.4f, -2.8f));
        modelMatrixForContainer2 = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        sun.drawSphereWithTexture(lightingShaderWithTexture, modelMatrixForContainer2);


        









        glm::vec3 currentPoint = ellipsePoints[countt];
        if (countt == 19999)
            countt = 0;
        else
            countt = countt + 1;
        //currentPoint = glm::vec3(2.0f, 1.0f, 4.0f);
        spin_of_jupiter += .2f;
        translateMatrix = glm::translate(identityMatrix, currentPoint);
        glm::mat4 rotateY = glm::rotate(identityMatrix, glm::radians(spin_of_jupiter), glm::vec3(0.0f, 1.0f, 0.0f));/////////////////////////
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X, scale_Y, scale_Z));
        model = translateMatrix * rotateY * scaleMatrix;
        Jupiter(lightingShaderWithTexture, model, jupiter, Pond, Pond_water, tree_base, tree_leaf,Hill,Special_tree);





        //UFO
        if (catch_the_ufo == 0.0) {
            //pointLightPositions[0] =glm::vec3(spot_list_pos_x + spacex, spot_list_pos_y + spacey, spot_list_pos_z + spacez); //spotlight
            pointlight1.position = glm::vec3(spot_list_pos_x + spacex, spot_list_pos_y + spacey, spot_list_pos_z + spacez); //spotlight
           pointlight1.spotpos[1] = glm::vec3(spot_list_pos_x + spacex, spot_list_pos_y + spacey-4.0f, spot_list_pos_z + spacez); //spotlight
            glm::mat4 UFOmodel = glm::mat4(1.0f);
            UFOmodel = glm::translate(UFOmodel, glm::vec3(5.0f + spacex, 4.0f + spacey, 5.0f + spacez));
            //UFOmodel = glm::translate(UFOmodel, glm::vec3(camera.Position[0], camera.Position[1]-.7f, camera.Position[2]-.8f));
            UFOmodel = glm::rotate(UFOmodel, glm::radians(15.0f), glm::vec3(1.0f, 0.0f, 0.0f));
            UFOmodel = glm::scale(UFOmodel, glm::vec3(0.5f));
            UFO.draw(lightingShaderWithTexture, UFOmodel, glm::vec3(5.0f, 0.0f, 5.0f));

            glm::vec3 ufoPosition = glm::vec3(UFOmodel[3]);
            glm::vec3 jupiterPosition = currentPoint;
            float distance = calculateDistance(ufoPosition, jupiterPosition);
            //cout << "distance = " << distance << endl;
            if (distance < 5)
            {
                catch_the_ufo = 1.0;
                spacex = 0.0f;
                spacey = 5.0f;
                spacez = 0.0f;
            }

            //test cube
            glm::mat4 modelMat = glm::mat4(1.0f);
            modelMat = glm::translate(modelMat, glm::vec3(5.0f + spacex, 4.2f + spacey, 5.0f + spacez));
            modelMat = glm::rotate(modelMat, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
            modelMat = glm::scale(modelMat, glm::vec3(0.2f));
            //cube_test.drawCubeWithTexture(lightingShaderWithTexture, modelMat);
            //UFO2.draw(lightingShaderWithTexture, modelMat, glm::vec3(5.0f, 0.0f, 5.0f));
            ufo2.drawSphereWithTexture(lightingShaderWithTexture, modelMat);
        }
        else
        {
            pointlight1.position = currentPoint + glm::vec3(0.0f, 4.0f, 1.5f); //spotlight
            pointlight1.spotpos[1] = currentPoint;

            if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS)
            {
                if(ufo_land_y>0.0f)
                    ufo_land_y -= .01f;
                if (ufo_land_y <= 0.0f) {
                    little_ufo_on = 1.0;
                    //cout << "little_ufo_on" << little_ufo_on << endl;
                }
                    
            }

            glm::mat4 UFOmodel = glm::mat4(1.0f);
            UFOmodel = glm::translate(UFOmodel, currentPoint + glm::vec3(0.0f, 4.0f, 2.5f));
            //UFOmodel = glm::translate(UFOmodel, glm::vec3(camera.Position[0], camera.Position[1]-.7f, camera.Position[2]-.8f));
            UFOmodel = glm::rotate(UFOmodel, glm::radians(15.0f), glm::vec3(1.0f, 0.0f, 0.0f));
            UFOmodel = glm::scale(UFOmodel, glm::vec3(0.5f));
            UFO.draw(lightingShaderWithTexture, UFOmodel, glm::vec3(5.0f, 0.0f, 5.0f));
            //ufo2.drawSphereWithTexture(lightingShaderWithTexture, UFOmodel);

            if (little_ufo_on == 0.0) {
                glm::mat4 modelMat = glm::mat4(1.0f);
                //modelMat = glm::translate(modelMat, currentPoint + glm::vec3(ufo_land_x + spacex, ufo_land_y + spacey, ufo_land_z + spacez));
                modelMat = glm::translate(modelMat, currentPoint + glm::vec3(ufo_land_x, ufo_land_y, ufo_land_z));
                modelMat = glm::rotate(modelMat, glm::radians(currentAngleUfo - 90.0f), glm::vec3(.0f, .0f, 1.0f));
                //modelMat = glm::rotate(modelMat, glm::radians(currentAngleUfo), glm::vec3(.0f, .0f, 1.0f));
                modelMat = glm::scale(modelMat, glm::vec3(0.2f));
                //cube_test.drawCubeWithTexture(lightingShaderWithTexture, modelMat);
                //UFO2.draw(lightingShaderWithTexture, modelMat, glm::vec3(5.0f, 0.0f, 5.0f));
                ufo2.drawSphereWithTexture(lightingShaderWithTexture, modelMat);
            }
            else if (little_ufo_on == 1.0) {
                float xc = 2.5 * glm::sin(glm::radians(float(currentAngleUfo))) * glm::cos(glm::radians(float(currentAngleUfoPhi)));
                float yc = 2.5 * glm::cos(glm::radians(float(currentAngleUfo)));
                float zc = 2.5 * glm::sin(glm::radians(float(currentAngleUfo))) * glm::sin(glm::radians(float(currentAngleUfoPhi)));

                //cout << "xc: " << xc << " yc: " << yc << " zc:" << zc << endl;

                //float xc = 1 * glm::cos(glm::radians(float(currentAngleUfo)));
                //float yc = 1 * glm::sin(glm::radians(float(currentAngleUfo)));

                //test cube
                glm::mat4 modelMat = glm::mat4(1.0f);
                glm::mat4 dami = glm::mat4(1.0f);
                //modelMat = glm::translate(modelMat, currentPoint + glm::vec3(ufo_land_x + spacex, ufo_land_y + spacey, ufo_land_z + spacez));
                modelMat = glm::translate(modelMat, currentPoint + glm::vec3(xc ,yc, zc));
                dami = modelMat;
                modelMat = glm::rotate(modelMat, glm::radians(0.0f), glm::vec3(.0f, .0f, 1.0f));
                //modelMat = glm::rotate(modelMat, glm::radians(currentAngleUfo), glm::vec3(.0f, .0f, 1.0f));
                modelMat = glm::scale(modelMat, glm::vec3(0.2f));
                //cube_test.drawCubeWithTexture(lightingShaderWithTexture, modelMat);
                //UFO2.draw(lightingShaderWithTexture, modelMat, glm::vec3(5.0f, 0.0f, 5.0f));
                //lightingShaderWithTexture.setVec3("viewPos",  glm::vec3(-10.0f, 0.0f, -10.0f));
                ufo2.drawSphereWithTexture(lightingShaderWithTexture, modelMat);
                if (glfwGetKey(window, GLFW_KEY_BACKSPACE) == GLFW_PRESS) {
                    if (Inspection == 0.0) {
                        view = static_view;
                        Inspection = 1.0;
                    }
                        
                    else
                        Inspection = 0.0;
                }
                if(Inspection==1.0)
                    lightingShaderWithTexture.setMat4("view", view * glm::inverse(dami) * glm::translate(glm::mat4(1.0f), glm::vec3(.0f, 0.0f, 10.0f)));
            }
            
        }



        // also draw the lamp object(s)
        ourShader.use();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);

        // we now draw as many light bulbs as we have point lights.
        for (unsigned int i = 0; i < 3; i++)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, pointLightPositions[i]);
            model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
           // cube.drawCube(ourShader, model, 0.8f, 0.8f, 0.8f);
        }

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------


    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}




// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.ProcessKeyboard(FORWARD, 2*deltaTime);
        //spacez = spacez - .005;
        //cout << "delta = " << deltaTime << endl;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.ProcessKeyboard(BACKWARD, 2 * deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.ProcessKeyboard(LEFT, 2 * deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.ProcessKeyboard(RIGHT, 2 * deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
        camera.ProcessKeyboard(DOWN, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
        camera.ProcessKeyboard(P_UP, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
        camera.ProcessKeyboard(P_DOWN, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS) {
        camera.ProcessKeyboard(Y_LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS) {
        camera.ProcessKeyboard(Y_RIGHT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
        camera.ProcessKeyboard(R_LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        camera.ProcessKeyboard(R_RIGHT, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
    {
        eyeX += 2.5 * deltaTime;
        basic_camera.changeEye(eyeX, eyeY, eyeZ);
    }
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
    {
        eyeX -= 2.5 * deltaTime;
        basic_camera.changeEye(eyeX, eyeY, eyeZ);
    }
    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
    {
        eyeZ += 2.5 * deltaTime;
        basic_camera.changeEye(eyeX, eyeY, eyeZ);
    }
    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
    {
        eyeZ -= 2.5 * deltaTime;
        basic_camera.changeEye(eyeX, eyeY, eyeZ);
    }
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
    {
        trigger = 1 - trigger;
        //cout << trigger << endl;
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    {
        eyeY -= 2.5 * deltaTime;
        basic_camera.changeEye(eyeX, eyeY, eyeZ);
    }
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
    {
        pointlight2.turnOn();

    }
    if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)
    {
        pointlight2.turnOff();


    }
    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
    {
        pointlight3.turnOn();

    }
    if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
    {
        pointlight3.turnOff();


    }
    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
    {
        pointlight1.turnOn();


    }
    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
    {
        currentAngleUfo += .2f;
    }
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
    {
        currentAngleUfo -= .2f;
    }
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
    {
        currentAngleUfoPhi += .2f;
    }
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
    {
        currentAngleUfoPhi -= .2f;
    }

    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
    {
        pointlight1.turnOff();


    }
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
    {
        /*if (diffuseToggle)
        {*/
        if (pointlight1.isOn())
            pointlight1.turnAmbientOn();
        if (pointlight2.isOn())
            pointlight2.turnAmbientOn();
        if (pointlight3.isOn())
            pointlight3.turnAmbientOn();

        //diffuseToggle = !diffuseToggle;
    //}
    }
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
    {
        /*if (diffuseToggle)
        {*/
        if (pointlight1.isOn())
            pointlight1.turnAmbientOff();
        if (pointlight2.isOn())
            pointlight2.turnAmbientOff();
        if (pointlight3.isOn())
            pointlight3.turnAmbientOff();
        //pointlight4.turnDiffuseOff();
        //diffuseToggle = !diffuseToggle;
    //}
    }

    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
    {
        /*if (diffuseToggle)
        {*/
        if (pointlight1.isOn())
            pointlight1.turnDiffuseOn();
        if (pointlight2.isOn())
            pointlight2.turnDiffuseOn();
        if (pointlight3.isOn())
            pointlight3.turnDiffuseOn();
        //pointlight4.turnAmbientOn();
        //diffuseToggle = !diffuseToggle;
        //}
    }
    if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
    {
        /*if (diffuseToggle)
        {*/
        if (pointlight1.isOn())
            pointlight1.turnDiffuseOff();
        if (pointlight2.isOn())
            pointlight2.turnDiffuseOff();
        if (pointlight3.isOn())
            pointlight3.turnDiffuseOff();
        //diffuseToggle = !diffuseToggle;
        //}
    }


    if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
    {
        /*if (diffuseToggle)
        {*/
        if (pointlight1.isOn())
            pointlight1.turnSpecularOn();
        if (pointlight2.isOn())
            pointlight2.turnSpecularOn();
        if (pointlight3.isOn())
            pointlight3.turnSpecularOn();
        //pointlight4.turnSpecularOn();
        //diffuseToggle = !diffuseToggle;
        //}
    }
    if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS)
    {
        /*if (diffuseToggle)
        {*/
        /*cout << "1 " << pointlight1.isOn() << endl;
        cout << pointlight2.isOn() << endl;
        cout << pointlight3.isOn() << endl;*/
        if (pointlight1.isOn())
            pointlight1.turnSpecularOff();
        if (pointlight2.isOn())
            pointlight2.turnSpecularOff();
        if (pointlight3.isOn())
            pointlight3.turnSpecularOff();
        //pointlight4.turnSpecularOff();
        //diffuseToggle = !diffuseToggle;
        //}
    }
    if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS)
    {
        split++;
    }

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
        split++;
    }

    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        spacez = spacez + .01;
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        spacez = spacez - .01;
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        spacex = spacex + .01;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        spacex = spacex - .01;
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)
    {
        spacey = spacey + .01;
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS)
    {
        spacey = spacey - .01;
    }



}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_1 && action == GLFW_PRESS)
    {
        if (pointLightOn)
        {
            pointlight1.turnOff();
            pointlight2.turnOff();
            pointlight3.turnOff();

            pointLightOn = !pointLightOn;
        }
        else
        {
            pointlight1.turnOn();
            pointlight2.turnOn();
            pointlight3.turnOn();

            pointLightOn = !pointLightOn;
        }
    }

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

unsigned int loadTexture(char const* path, GLenum textureWrappingModeS, GLenum textureWrappingModeT, GLenum textureFilteringModeMin, GLenum textureFilteringModeMax)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, textureWrappingModeS);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, textureWrappingModeT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, textureFilteringModeMin);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, textureFilteringModeMax);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}


void Jupiter(Shader& lightingShaderWithTexture, glm::mat4 altogether, Sphere2& jupiter, Curve& Pond, Curve& Pond_water, Curve& tree_base, Curve& tree_leaf, Curve& hill, Curve& special_tree)
{
    //jupiter
    glm::mat4 modelMatrixForContainer_j = glm::mat4(1.0f);
    modelMatrixForContainer_j = glm::translate(modelMatrixForContainer_j, glm::vec3(0.0f, 0.0f, 0.0f));
    //modelMatrixForContainer_j = glm::rotate(modelMatrixForContainer_j, glm::radians(spin_of_earth), glm::vec3(.0f, 1.0f, 0.0f));
    modelMatrixForContainer_j = altogether * modelMatrixForContainer_j;
    jupiter.drawSphereWithTexture(lightingShaderWithTexture, modelMatrixForContainer_j);

    //pond
    make_ponds(lightingShaderWithTexture, altogether, Pond, Pond_water, glm::vec3(1.0f, .0f, .0f), -90.0f, glm::vec3(.0f, .0f, 1.0f));
    make_ponds(lightingShaderWithTexture, altogether, Pond, Pond_water, glm::vec3(-1.0f, .2f, .0f), 90.0f, glm::vec3(.0f, .0f, 1.0f));
    make_ponds(lightingShaderWithTexture, altogether, Pond, Pond_water, glm::vec3(.0f, -1.0f, .0f), 180.0f, glm::vec3(1.0f, .0f, .0f));
    make_ponds(lightingShaderWithTexture, altogether, Pond, Pond_water, glm::vec3(.98f, .0f, .15f), -90.0f, glm::vec3(.0f, .0f, 1.0f));
    make_ponds(lightingShaderWithTexture, altogether, Pond, Pond_water, glm::vec3(-1.0f, .0f, .0f), 90.0f, glm::vec3(.0f, .0f, 1.0f));
    make_ponds(lightingShaderWithTexture, altogether, Pond, Pond_water, glm::vec3(-.8f, .51f, -.3f), 60.0f, glm::vec3(.0f, .0f, 1.0f));
    make_ponds(lightingShaderWithTexture, altogether, Pond, Pond_water, glm::vec3(.32f, -.2f, .92f), 90.0f, glm::vec3(1.0f, .0f, .0f));
    make_ponds(lightingShaderWithTexture, altogether, Pond, Pond_water, glm::vec3(-.32f, -.2f, .92f), 110.0f, glm::vec3(1.0f, .0f, 1.0f));
    make_ponds(lightingShaderWithTexture, altogether, Pond, Pond_water, glm::vec3(.57f, -.57f, -.57f), -120.0f, glm::vec3(1.0f, .0f, 1.0f));



    //tree
    make_tree(lightingShaderWithTexture, altogether, tree_base, tree_leaf, glm::vec3(.0f, .0f, 1.0f), 90.0f, glm::vec3(1.0f, .0f, .0f));
    make_tree(lightingShaderWithTexture, altogether, tree_base, tree_leaf, glm::vec3(.0f, 1.0f, .0f), 90.0f, glm::vec3(.0f, 1.0f, .0f));
    make_tree(lightingShaderWithTexture, altogether, tree_base, tree_leaf, glm::vec3(0.1f, -.99f, 0.0f), -2.0f, glm::vec3(.0f, -1.0f, .0f));
    make_tree(lightingShaderWithTexture, altogether, tree_base, tree_leaf, glm::vec3(-0.1f, .99f, 0.0f), 2.0f, glm::vec3(.0f, 1.0f, .0f));
    make_tree(lightingShaderWithTexture, altogether, tree_base, tree_leaf, glm::vec3(00.0f, .99f, 0.1f), 0.0f, glm::vec3(.0f, 1.0f, .0f));

    make_tree(lightingShaderWithTexture, altogether, tree_base, tree_leaf, glm::vec3(0.f, -.99f, -.1f), 0.0f, glm::vec3(.0f, -1.0f, .0f));
    make_tree(lightingShaderWithTexture, altogether, tree_base, tree_leaf, glm::vec3(-0.1f, .99f, -0.1f), (2.0f), glm::vec3(.0f, 1.0f, .0f));
    make_tree(lightingShaderWithTexture, altogether, tree_base, tree_leaf, glm::vec3(-0.1f, -.99f, 0.1f), (2.0f), glm::vec3(.0f, -1.0f, .0f));
    make_tree(lightingShaderWithTexture, altogether, tree_base, tree_leaf, glm::vec3(0.1f, .99f, -0.1f), (2.0f), glm::vec3(.0f, 1.0f, .0f));
    make_tree(lightingShaderWithTexture, altogether, tree_base, tree_leaf, glm::vec3(0.1f, -.99f, 0.1f), 2.0f, glm::vec3(.0f, -1.0f, .0f));


    make_tree(lightingShaderWithTexture, altogether, tree_base, tree_leaf, glm::vec3(.5f, .5f, .7f), 80.0f, glm::vec3(1.0f, 1.0f, .0f));
    make_tree(lightingShaderWithTexture, altogether, tree_base, tree_leaf, glm::vec3(.5f, .7f, .5f), 60.0f, glm::vec3(1.0f, 1.0f, .0f));
    make_tree(lightingShaderWithTexture, altogether, tree_base, tree_leaf, glm::vec3(.3f, .5f, .81f), 80.0f, glm::vec3(1.0f, 1.0f, .0f));
    make_tree(lightingShaderWithTexture, altogether, tree_base, tree_leaf, glm::vec3(.9f, .4f, .17f), -70.0f, glm::vec3(.0f, .0f, 1.0f)); 
    make_tree(lightingShaderWithTexture, altogether, tree_base, tree_leaf, glm::vec3(.9f, .173f, .4f), -80.0f, glm::vec3(.0f, .0f, 1.0f));
    make_tree(lightingShaderWithTexture, altogether, tree_base, tree_leaf, glm::vec3(.173f, .4f, .9f), 75.0f, glm::vec3(1.0f, 1.0f, .0f));
    make_tree(lightingShaderWithTexture, altogether, tree_base, tree_leaf, glm::vec3(.173f, .9f, .4f), 45.0f, glm::vec3(1.0f, 1.0f, .0f));
    make_tree(lightingShaderWithTexture, altogether, tree_base, tree_leaf, glm::vec3(.173f, .9f, .3f), 45.0f, glm::vec3(1.0f, 1.0f, .0f));
    make_tree(lightingShaderWithTexture, altogether, tree_base, tree_leaf, glm::vec3(-.173f, .9f, .4f), 20.0f, glm::vec3(1.0f, .0f, .0f));
    make_tree(lightingShaderWithTexture, altogether, tree_base, tree_leaf, glm::vec3(.173f, .9f, -.4f), -20.0f, glm::vec3(1.0f, .0f, .0f));
    make_tree(lightingShaderWithTexture, altogether, tree_base, tree_leaf, glm::vec3(.173f, -.9f, .4f), 190.0f, glm::vec3(.0f, .0f, 1.0f));
    make_tree(lightingShaderWithTexture, altogether, tree_base, tree_leaf, glm::vec3(.173f, -.9f, -.4f), 200.0f, glm::vec3(1.0f, .0f, .0f));
    make_tree(lightingShaderWithTexture, altogether, tree_base, tree_leaf, glm::vec3(-.173f, .9f, -.4f), -20.0f, glm::vec3(1.0f, .0f, -1.0f));
    make_tree(lightingShaderWithTexture, altogether, tree_base, tree_leaf, glm::vec3(-.173f, -.9f, .4f), 170.0f, glm::vec3(1.0f, .0f, .0f));
    make_tree(lightingShaderWithTexture, altogether, tree_base, tree_leaf, glm::vec3(-.173f, -.9f, -.4f), -150.0f, glm::vec3(1.0f, .0f, .0f));
    make_tree(lightingShaderWithTexture, altogether, tree_base, tree_leaf, glm::vec3(.5f, .0f, .866f), 105.0f, glm::vec3(1.0f, 1.0f, .0f));
    make_tree(lightingShaderWithTexture, altogether, tree_base, tree_leaf, glm::vec3(.866f, .0f, .5f), -120.0f, glm::vec3(.0f, 1.0f, 1.0f));
    make_tree(lightingShaderWithTexture, altogether, tree_base, tree_leaf, glm::vec3(-.866f, .0f, .5f), 90.0f, glm::vec3(.0f, .0f, 1.0f));
    make_tree(lightingShaderWithTexture, altogether, tree_base, tree_leaf, glm::vec3(.866f, .0f, -.5f), -90.0f, glm::vec3(1.0f, .0f, 1.0f));
    make_tree(lightingShaderWithTexture, altogether, tree_base, tree_leaf, glm::vec3(.3f, .3f, .91f), 110.0f, glm::vec3(1.0f, 1.0f, .0f));
    make_tree(lightingShaderWithTexture, altogether, tree_base, tree_leaf, glm::vec3(.3f, .91f, .3f), .0f, glm::vec3(1.0f, .0f, .0f));
    make_tree(lightingShaderWithTexture, altogether, tree_base, tree_leaf, glm::vec3(.91f, .3f, .3f), -70.0f, glm::vec3(.0f, .0f, 1.0f));
    make_tree(lightingShaderWithTexture, altogether, tree_base, tree_leaf, glm::vec3(-.91f, .3f, .3f), 70.0f, glm::vec3(.0f, .0f, 1.0f));
    make_tree(lightingShaderWithTexture, altogether, tree_base, tree_leaf, glm::vec3(.91f, -.3f, .3f), -110.0f, glm::vec3(.0f, .0f, 1.0f));
    make_tree(lightingShaderWithTexture, altogether, tree_base, tree_leaf, glm::vec3(.91f, .3f, -.3f), -70.0f, glm::vec3(.0f, .0f, 1.0f));
    make_tree(lightingShaderWithTexture, altogether, tree_base, tree_leaf, glm::vec3(-.3f, .91f, .3f), .0f, glm::vec3(1.0f, .0f, .0f));
    make_tree(lightingShaderWithTexture, altogether, tree_base, tree_leaf, glm::vec3(.3f, -.91f, .3f), 190.0f, glm::vec3(.0f, .0f, 1.0f));
    make_tree(lightingShaderWithTexture, altogether, tree_base, tree_leaf, glm::vec3(.3f, .91f, -.3f), .0f, glm::vec3(.0f, .0f, 1.0f));
    make_tree(lightingShaderWithTexture, altogether, tree_base, tree_leaf, glm::vec3(.3f, -.91f, -.3f), 200.0f, glm::vec3(.0f, .0f, 1.0f));
    make_tree(lightingShaderWithTexture, altogether, tree_base, tree_leaf, glm::vec3(-.3f, -.91f, .3f), 180.0f, glm::vec3(.0f, .0f, 1.0f));
    make_tree(lightingShaderWithTexture, altogether, tree_base, tree_leaf, glm::vec3(-.2f, -.91f, -.3f), 210.0f, glm::vec3(1.0f, .0f, .0f));
    make_tree(lightingShaderWithTexture, altogether, tree_base, tree_leaf, glm::vec3(-.3f, -.8f, -.3f), 200.0f, glm::vec3(1.0f, .0f, .0f));
    make_tree(lightingShaderWithTexture, altogether, tree_base, tree_leaf, glm::vec3(.4f, .8f, .4f), .0f, glm::vec3(1.0f, .0f, .0f));
    make_tree(lightingShaderWithTexture, altogether, tree_base, tree_leaf, glm::vec3(.5f, .0f, .8f), 105.0f, glm::vec3(1.0f, 1.0f, .0f));
    make_tree(lightingShaderWithTexture, altogether, tree_base, tree_leaf, glm::vec3(-.6f, .0f, .8f), 90.0f, glm::vec3(1.0f, .0f, .0f));
    make_tree(lightingShaderWithTexture, altogether, tree_base, tree_leaf, glm::vec3(.5f, .0f, -.866f), -90.0f, glm::vec3(1.0f, .0f, .0f));
    make_tree(lightingShaderWithTexture, altogether, tree_base, tree_leaf, glm::vec3(.5f, .2f, -.8f), -90.0f, glm::vec3(1.0f, .0f, .0f));
    make_tree(lightingShaderWithTexture, altogether, tree_base, tree_leaf, glm::vec3(.5f, .33f, -.8f), -90.0f, glm::vec3(1.0f, .0f, .0f));
    make_tree(lightingShaderWithTexture, altogether, tree_base, tree_leaf, glm::vec3(.5f, .25f, -.8f), -50.0f, glm::vec3(1.0f, .0f, .0f));
    make_tree(lightingShaderWithTexture, altogether, tree_base, tree_leaf, glm::vec3(-.87f, .4f, .3f), 70.0f, glm::vec3(.0f, .0f, 1.0f));
    make_tree(lightingShaderWithTexture, altogether, tree_base, tree_leaf, glm::vec3(-.87f, .3f, .4f), 70.0f, glm::vec3(.0f, .0f, 1.0f));
    make_tree(lightingShaderWithTexture, altogether, tree_base, tree_leaf, glm::vec3(-.3f, -.91f, -.3f), 210.0f, glm::vec3(1.0f, .0f, .0f));
    make_tree(lightingShaderWithTexture, altogether, tree_base, tree_leaf, glm::vec3(.5f, .45f, -.72f), 270.0f, glm::vec3(1.0f, .0f, .0f));
    make_tree(lightingShaderWithTexture, altogether, tree_base, tree_leaf, glm::vec3(.5f, .6f, -.6f), 290.0f, glm::vec3(1.0f, .0f, .0f));




    //hill
    make_hill(lightingShaderWithTexture, altogether, hill, glm::vec3(.0f, .0f, -1.0f), -90.0f, glm::vec3(1.0f, .0f, .0f));
    make_hill(lightingShaderWithTexture, altogether, hill, glm::vec3(.0f, .2f, -1.0f), -90.0f, glm::vec3(1.0f, .0f, .0f));
    make_hill(lightingShaderWithTexture, altogether, hill, glm::vec3(.57f, .57f, .57f), 90.0f, glm::vec3(1.0f, 1.0f, .0f));
    make_hill(lightingShaderWithTexture, altogether, hill, glm::vec3(-.57f, .57f, .57f), 90.0f, glm::vec3(.0f, 1.0f, 1.0f));
    make_hill(lightingShaderWithTexture, altogether, hill, glm::vec3(-.57f, .57f, -.57f), -90.0f, glm::vec3(1.0f, -1.0f, .0f));

    //special_tree
    make_special_tree(lightingShaderWithTexture, altogether, special_tree, glm::vec3(1.0f, .0f, .0f), 90.0f, glm::vec3(1.0f, .0f, .0f));
    make_special_tree(lightingShaderWithTexture, altogether, special_tree, glm::vec3(-.5f, .4f, .7f), 0.0f, glm::vec3(1.0f, .0f, .0f));
    make_special_tree(lightingShaderWithTexture, altogether, special_tree, glm::vec3(-.5f, .4f, -.7f), 0.0f, glm::vec3(1.0f, .0f, .0f));
    make_special_tree(lightingShaderWithTexture, altogether, special_tree, glm::vec3(-.5f, -.4f, -.7f), 90.0f, glm::vec3(.0f, .0f, 1.0f));


}

void trees(Shader& lightingShader, glm::mat4 alTogether, Curve& head, Curve& body) {

    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 0.05f, 0.0f));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f));
    model = alTogether * translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;

    head.draw(lightingShader, model, glm::vec3(1.0f, 0.0f, 0.0f));


    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 0.07f, 0.0f));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.05f));
    model = alTogether * translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;

    body.draw(lightingShader, model, glm::vec3(1.0f, 0.0f, 0.0f));
}

void ponds(Shader& lightingShader, glm::mat4 alTogether, Curve& pond, Curve& pond_water) {

    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, -.35f, 0.0f));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f,.5f,.1f));
    model = alTogether * translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;

    pond.draw(lightingShader, model, glm::vec3(1.0f, 0.0f, 0.0f));


    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, -.21f, 0.0f));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(.169f));
    model = alTogether * translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;

    pond_water.draw(lightingShader, model, glm::vec3(1.0f, 0.0f, 0.0f));
}

void make_tree(Shader& lightingShaderWithTexture, glm::mat4 alTogether, Curve& tree_base, Curve& tree_leaf, glm::vec3 position, float angle, glm::vec3 angle_axis)
{
    glm::mat4 model_for_tree = glm::mat4(1.0f);
    model_for_tree = glm::translate(model_for_tree, position);
    model_for_tree = glm::rotate(model_for_tree, glm::radians( angle), angle_axis );
    model_for_tree = glm::scale(model_for_tree, glm::vec3(.7f,.7f, .7f));
    model_for_tree = alTogether * model_for_tree;
    trees(lightingShaderWithTexture, model_for_tree, tree_base, tree_leaf);
}
void make_ponds(Shader& lightingShaderWithTexture, glm::mat4 alTogether, Curve& Pond, Curve& Pond_water, glm::vec3 position, float angle, glm::vec3 angle_axis)
{
    glm::mat4 model_for_pond = glm::mat4(1.0f);
    model_for_pond = glm::translate(model_for_pond, position);
    model_for_pond = glm::rotate(model_for_pond, glm::radians(angle), angle_axis);
    model_for_pond = glm::scale(model_for_pond, glm::vec3(.5f, .5f, .5f));
    model_for_pond = alTogether * model_for_pond;
    ponds(lightingShaderWithTexture, model_for_pond, Pond, Pond_water);
}
void make_hill(Shader& lightingShader, glm::mat4 alTogether, Curve& hill, glm::vec3 position, float angle, glm::vec3 angle_axis)
{
    glm::mat4 model_for_hill = glm::mat4(1.0f);
    model_for_hill = glm::translate(model_for_hill, position);
    model_for_hill = glm::rotate(model_for_hill, glm::radians(angle), angle_axis);
    model_for_hill = glm::scale(model_for_hill, glm::vec3(.07f, .08f, .07f));
    model_for_hill = alTogether * model_for_hill;
    hill.draw(lightingShader, model_for_hill, glm::vec3(1.0f, 0.0f, 0.0f));
}
void make_special_tree(Shader& lightingShader, glm::mat4 alTogether, Curve& special_tree, glm::vec3 position, float angle, glm::vec3 angle_axis)
{
    glm::mat4 model_for_special_tree = glm::mat4(1.0f);
    model_for_special_tree = glm::translate(model_for_special_tree, position);
    model_for_special_tree = glm::rotate(model_for_special_tree, glm::radians(angle), angle_axis);
    model_for_special_tree = glm::scale(model_for_special_tree, glm::vec3(1.0f, .1f, 1.0f));
    model_for_special_tree = alTogether * model_for_special_tree;
    special_tree.draw(lightingShader, model_for_special_tree, glm::vec3(1.0f, 0.0f, 0.0f));
}
