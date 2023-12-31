/**************************************************
Scene.inl contains the definition of the scene graph
*****************************************************/
#include "RTScene.h"
#include "RTCube.h"
#include "RTObj.h"

using namespace glm;
void RTScene::init(void) {
    // Create a geometry palette
    geometry["cube"] = new RTCube;
    geometry["teapot"] = new RTObj;
    geometry["bunny"] = new RTObj;
    geometry["cube"]->init();
    geometry["teapot"]->init("models/teapot.obj");
    geometry["bunny"]->init("models/bunny.obj");

    // Create a material palette
    material["wood"] = new Material;
    material["wood"]->ambient = vec4(0.1f, 0.1f, 0.1f, 1.0f);
    material["wood"]->diffuse = vec4(0.3f, 0.15f, 0.1f, 1.0f);
    material["wood"]->specular = vec4(0.3f, 0.15f, 0.1f, 1.0f);
    material["wood"]->shininess = 100.0f;

    material["ceramic"] = new Material;
    material["ceramic"]->ambient = vec4(0.02f, 0.07f, 0.2f, 1.0f);
    material["ceramic"]->diffuse = vec4(0.1f, 0.25f, 0.7f, 1.0f);
    material["ceramic"]->specular = vec4(0.9f, 0.9f, 0.9f, 1.0f);
    material["ceramic"]->shininess = 150.0f;

    material["silver"] = new Material;
    material["silver"]->ambient = vec4(0.1f, 0.1f, 0.1f, 1.0f);
    material["silver"]->diffuse = vec4(0.2f, 0.2f, 0.2f, 1.0f);
    material["silver"]->specular = vec4(0.9f, 0.9f, 0.9f, 1.0f);
    material["silver"]->shininess = 50.0f;

    material["turquoise"] = new Material;
    material["turquoise"]->ambient = vec4(0.1f, 0.2f, 0.17f, 1.0f);
    material["turquoise"]->diffuse = vec4(0.2f, 0.375f, 0.35f, 1.0f);
    material["turquoise"]->specular = vec4(0.3f, 0.3f, 0.3f, 1.0f);
    material["turquoise"]->shininess = 100.0f;

    material["white"] = new Material;
    material["white"]->ambient = vec4(0.31f, -0.02f, 0.017f, 1.0f);
    material["white"]->diffuse = vec4(0.2f, 0.375f, 0.35f, 1.0f);
    material["white"]->specular = vec4(0.7f, 0.7f, 0.7f, 1.0f);
    material["white"]->shininess = 100.0f;

    material["black"] = new Material;
    material["black"]->ambient = vec4(.01f, 0.09f, .12f, 1.0f);
    material["black"]->diffuse = vec4(0.2f, 0.375f, 0.35f, 1.0f);
    material["black"]->specular = vec4(0.3f, 0.3f, 0.3f, 1.0f);
    material["black"]->shininess = 100.0f;

    material["brown"] = new Material;
    material["brown"]->ambient = vec4(-0.11f, .30f, 0.17f, 1.0f);
    material["brown"]->diffuse = vec4(0.2f, 0.375f, 0.35f, 1.0f);
    material["brown"]->specular = vec4(0.3f, 0.3f, 0.3f, 1.0f);
    material["brown"]->shininess = 100.0f;

    material["grey"] = new Material;
    material["grey"]->ambient = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    material["grey"]->diffuse = vec4(0.2f, 0.375f, 0.35f, 1.0f);
    material["grey"]->specular = vec4(0.3f, 0.3f, 0.3f, 1.0f);
    material["grey"]->shininess = 25.0f;


    material["bulb"] = new Material;
    material["bulb"]->ambient = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    material["bulb"]->diffuse = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    material["bulb"]->specular = vec4(1.0f, 1.0f, 1.0f, 1.0f);
    material["bulb"]->emision = vec4(1.0f, 0.2f, 0.1f, 1.0f);
    material["bulb"]->shininess = 200.0f;

    // Create a model palette
    model["teapot2"] = new RTModel;
    model["teapot2"]->geometry = geometry["teapot"];
    model["teapot2"]->material = material["ceramic"];
    model["table piece"] = new RTModel;
    model["table piece"]->geometry = geometry["cube"];
    model["table piece"]->material = material["wood"];
    model["bunny"] = new RTModel;
    model["bunny"]->geometry = geometry["bunny"];
    model["bunny"]->material = material["turquoise"];

    model["concrete"] = new RTModel;
    model["concrete"]->geometry = geometry["cube"];
    model["concrete"]->material = material["ceramic"];

    model["bunny2"] = new RTModel;
    model["bunny2"]->geometry = geometry["bunny"];
    model["bunny2"]->material = material["black"];
    model["bunny3"] = new RTModel;
    model["bunny3"]->geometry = geometry["bunny"];
    model["bunny3"]->material = material["white"];

    model["bunny4"] = new RTModel;
    model["bunny4"]->geometry = geometry["bunny"];
    model["bunny4"]->material = material["brown"];

    model["bulb"] = new RTModel;
    model["bulb"]->geometry = geometry["cube"];
    model["bulb"]->material = material["bulb"];

    // Create a light palette
    light["sun"] = new Light;
    light["sun"]->position = vec4(3.0f, 2.0f, 1.0f, 0.0f);
    light["sun"]->color = 1.0f * vec4(1.0f, 1.0f, 1.0f, 1.0f);

    light["bulb"] = new Light;
    light["bulb"]->position = vec4(0.0f, 2.0f, 0.0f, 0.0f);
    light["bulb"]->color = 1.5f * vec4(1.0f, 0.2f, 0.1f, 1.0f);

    // Build the scene graph
    node["table"] = new RTNode;
    node["table top"] = new RTNode;
    node["table leg"] = new RTNode;
    node["teapot2"] = new RTNode;
    node["bunny"] = new RTNode;
    node["bunny2"] = new RTNode;
    node["bunny3"] = new RTNode;
    node["bunny4"] = new RTNode;
    node["floor"] = new RTNode;
    node["concrete"] = new RTNode;


    node["table"]->childnodes.push_back(node["table top"]);
    node["table"]->childtransforms.push_back(translate(vec3(0.0f, 0.75f, 0.0f)));
    node["table"]->childnodes.push_back(node["table leg"]);
    node["table"]->childtransforms.push_back(translate(vec3(-0.9f, 0.0f, -0.4f)));
    node["table"]->childnodes.push_back(node["table leg"]);
    node["table"]->childtransforms.push_back(translate(vec3(-0.9f, 0.0f, 0.4f)));
    node["table"]->childnodes.push_back(node["table leg"]);
    node["table"]->childtransforms.push_back(translate(vec3(0.9f, 0.0f, 0.4f)));
    node["table"]->childnodes.push_back(node["table leg"]);
    node["table"]->childtransforms.push_back(translate(vec3(0.9f, 0.0f, -0.4f)));

    node["floor"]->models.push_back(model["concrete"]);
    node["floor"]->modeltransforms.push_back(translate(vec3(0.0f, -0.0f, 0.0f)) * scale(vec3(8.0f, .1f, 8.0f)));

    node["floor"]->models.push_back(model["concrete"]);
    node["floor"]->modeltransforms.push_back(translate(vec3(0.0f, -0.0f, 0.0f)) * scale(vec3(5.0f, .1f, 5.0f)));

    node["table leg"]->models.push_back(model["table piece"]);
    node["table leg"]->modeltransforms.push_back(translate(vec3(0.0f, 0.35f, 0.0f)) * scale(vec3(0.2f, 0.6f, 0.2f)));
    //
    node["table top"]->models.push_back(model["table piece"]);
    node["table top"]->modeltransforms.push_back(translate(vec3(0.0f, -0.1f, 0.0f)) * scale(vec3(2.0f, 0.2f, 1.0f)));
    /*node["table top"]->childnodes.push_back(node["teapot2"]);
    node["table top"]->childtransforms.push_back(translate(vec3(0.0f, 0.0f, 0.0f)) * rotate(-120.0f * float(M_PI) / 180.0f, vec3(0.0f, 1.0f, 0.0f)));*/
    //
    //node["teapot2"]->models.push_back(model["teapot2"]);
    //node["teapot2"]->modeltransforms.push_back(scale(vec3(1.0f, 1.5f, 1.0f)) * scale(vec3(0.5f)));
    //
    //node["bunny"]->models.push_back(model["bunny"]);
    //node["bunny"]->modeltransforms.push_back(scale(vec3(0.8f)) * translate(vec3(0.0f, 1.0f, 0.0f)));
    //
    //node["bunny2"]->models.push_back(model["bunny2"]);
    //node["bunny2"]->modeltransforms.push_back(scale(vec3(0.8f)) * translate(vec3(1.8f, 1.0f, 2.125f)));
    //
    //node["bunny3"]->models.push_back(model["bunny3"]);
    //node["bunny3"]->modeltransforms.push_back(scale(vec3(0.8f)) * translate(vec3(0.0f, 1.0f, -4.25f)));
    //
    //node["bunny4"]->models.push_back(model["bunny4"]);
    //node["bunny4"]->modeltransforms.push_back(scale(vec3(0.8f)) * translate(vec3(1.8f, 1.0f, -2.125f)));

    node["world"]->childnodes.push_back(node["table"]);
    node["world"]->childtransforms.push_back(mat4(1.0f));
    node["world"]->childnodes.push_back(node["floor"]);
    node["world"]->childtransforms.push_back(translate(vec3(0.0f)));
    //node["world"]->childnodes.push_back(node["bunny"]);
    //node["world"]->childtransforms.push_back(translate(vec3(-1.8f, 0.0f, 0.0f)) * rotate(90.0f * float(M_PI) / 180.0f, vec3(0.0f, 1.0f, 0.0f)));
    //node["world"]->childnodes.push_back(node["bunny2"]);
    //node["world"]->childtransforms.push_back(translate(vec3(-1.8f, 0.0f, 0.0f)) * rotate(90.0f * float(M_PI) / 180.0f, vec3(0.0f, 1.0f, 0.0f)));
    //node["world"]->childnodes.push_back(node["bunny3"]);
    //node["world"]->childtransforms.push_back(translate(vec3(-1.8f, 0.0f, 0.0f)) * rotate(-90.0f * float(M_PI) / 180.0f, vec3(0.0f, 1.0f, 0.0f)));
    //node["world"]->childnodes.push_back(node["bunny3"]);
    //node["world"]->childtransforms.push_back(translate(vec3(-1.8f, 0.0f, 0.0f)) * rotate(-90.0f * float(M_PI) / 180.0f, vec3(0.0f, 1.0f, 0.0f)));
    //node["world"]->childnodes.push_back(node["bunny4"]);
    //node["world"]->childtransforms.push_back(translate(vec3(-1.8f, 0.0f, 0.0f)) * rotate(-90.0f * float(M_PI) / 180.0f, vec3(0.0f, 1.0f, 0.0f)));
    //node["world"]->models.push_back(model["bulb"]);
    //node["world"]->modeltransforms.push_back(translate(vec3(0.0f, 2.0f, 0.0f)) * scale(vec3(0.1f)));

     // Put a camera
    camera = new Camera;
    camera->target_default = vec3(0.0f, 1.0f, 0.0f);
    camera->eye_default = vec3(0.0f, 2.0f, 8.0f);
    camera->up_default = vec3(0.0f, 1.0f, 0.0f);
    camera->reset();

    // Initialize shader
    shader = new SurfaceShader;
    shader->read_source("shaders/projective.vert", "shaders/lighting.frag");
    shader->compile();
    glUseProgram(shader->program);
    shader->initUniforms();
}