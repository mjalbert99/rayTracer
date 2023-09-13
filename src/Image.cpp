#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <GL/glut.h>
#include<Image.h>

Image::Image() {
    this->width = 0;
    this->height = 0;
    this->fbo = 0;
    this->tbo = 0;
}

Image::Image(int width, int height) {
    this->width = width;
    this->height = height;
    this->fbo = 0;
    this->tbo = 0;
    Initialize();
}

Image::~Image() { this->pixels.clear(); }

void Image::Initialize() {
    int n = this->height * this->width;
    for (int i = 0; i < n; i++) {
        glm::vec3 temp = glm::vec3(0.0f, 0.0f, 0.0f);
        this->pixels.push_back(temp);
    }
}

void Image::init() {
    glGenFramebuffers(1, &fbo);
    glGenTextures(1, &tbo);
}

void Image::draw() {
    glBindTexture(GL_TEXTURE_2D, tbo);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_FLOAT, &pixels[0][0]);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo);
    glFramebufferTexture2D(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tbo, 0);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
}