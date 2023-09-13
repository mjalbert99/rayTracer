#include <Ray.h>
#include <limits>
#include <iostream>

using namespace glm;

Ray::Ray(glm::vec3 p, glm::vec3 d) {
    this->p0 = p;
    this->dir = d;
}

// Checked-> Looks Good: In World Coord
Ray RayTracer::RayThruPixel(Camera c, int i, int j, int wid, int hei) {
    glm::vec3 w = glm::normalize(c.eye - c.target);
    glm::vec3 u = glm::normalize(glm::cross(c.up, w));
    glm::vec3 v = -glm::cross(w, u);

    float a = c.aspect;
    float alpha = 2 * ((i + 0.5f) / (float)(wid)) - 1.0f;
    float beta = 1 - (2 * ((j + 0.5f) / (float)(hei)));
    float fovy_rad = c.fovy * M_PI / 180.0f;

    glm::vec3 p0 = c.eye;
    glm::vec3 d = glm::normalize(alpha * a * glm::tan(fovy_rad / 2) * u + beta * tan(fovy_rad / 2) * v - w);

    Ray ray = Ray(p0, d);
    return ray;
}

//Finished -> Looks ok. No issues with triangle display
Intersection RayTracer::Intersect(Ray r, Triangle& t) {
    glm::vec3 p1 = t.P.at(0);
    glm::vec3 p2 = t.P.at(1);
    glm::vec3 p3 = t.P.at(2);

    glm::mat4 M = glm::mat4(p1.x, p1.y, p1.z, 1,
        p2.x, p2.y, p2.z, 1,
        p3.x, p3.y, p3.z, 1,
        -r.dir.x, -r.dir.y, -r.dir.z, 0);

    glm::vec4 lam = glm::inverse(M) * glm::vec4(r.p0, 1.0f);
    glm::vec3 p = lam.x * p1 + lam.y * p2 + lam.z * p3;
    glm::vec3 n = glm::normalize(lam.x * t.N[0] + lam.y * t.N[1] + lam.z * t.N[2]);

    Intersection inter = Intersection();
    if (lam.x >= 0 && lam.y >= 0 && lam.z >= 0 && lam.w >= 0) {

        inter.dist = lam.w;
        inter.P = p;
        inter.N = n;
        inter.V = -r.dir; // only thing ify about this
        inter.triangle = t;
    }


    return inter;
}


// Specular lighting
glm::vec4 BlinnPhong(RTScene& s, Intersection hit) {
    vec4 position = vec4(hit.P, 1.0f);
    vec3 normal = hit.N;

    mat4 modelview = s.shader->modelview;
    mat4 view = s.shader->view;

    vec4 ambient = hit.triangle.material->ambient;
    vec4 diffuse = hit.triangle.material->diffuse;
    vec4 specular = hit.triangle.material->specular;
    vec4 emision = hit.triangle.material->emision;
    float shininess = hit.triangle.material->shininess;

    int nlights = s.light.size();
    std::vector<vec4> lightpositions = s.shader->lightpositions;
    std::vector<vec4> lightcolors = s.shader->lightcolors;
    /**************************************************************************/

    vec4 pWorld = position;            //World coord: Pworld = [M][Pmodel]
   // vec3 p3d = -normalize(vec3(pWorld)/pWorld.w);      

    vec3 nWorld = normalize(transpose(inverse(mat3(modelview))) * normal); // Normal World


    vec4 eye = inverse(view) * vec4(0, 0, 0, 1);
    vec3 newEye = normalize(vec3(-pWorld) / pWorld.w);

    vec4 fragColor = emision; //FragColor = emision + sum

    vec4 sum = vec4(0);

    for (int i = 0; i < nlights; i++) {

        vec4 qWorld = lightpositions[i];           //World coord: Qworld = light position

        //vec3 l = normalize(pWorld.w * vec3(qWorld) - qWorld.w * vec3(pWorld));

        vec3 l = normalize(pWorld.w * vec3(qWorld) - qWorld.w * vec3(pWorld));


        vec3 h = normalize(newEye + l);

        //Sum += Lj(C_ambient + R_diffuse + R_specular)

        sum += (ambient
            + (diffuse * glm::max(dot(nWorld, l), 0.0f))
            + (specular * pow(glm::max(glm::dot(nWorld, h), 0.0f), shininess)))
            * lightcolors[i];


    }
    fragColor += sum;

    return fragColor;

}

//Finished
Intersection RayTracer::Intersect(Ray r, RTScene& s) {
    float minDist = INFINITY;
    Intersection hit = Intersection();

    for (int i = 0; i < s.triangle_soup.size(); i++) {
        Intersection hit_temp = Intersect(r, s.triangle_soup.at(i));

        if (hit_temp.dist < minDist) {

            minDist = hit_temp.dist;
            hit = hit_temp;
        }
    }
    return hit;
}


/**
- Following Equation LightSeen = Sum( Cdiffuse * LightSource[i] * max(dot(n,lightpos[i]),0) ) + Cdiffuse*L(dot(n,d))
1. Initialize color to background color. Returns this if no conditions are met
2. Set Color based on light seen. Check if ray is blocking or hits -> Set colors based on light seen
3. Add Recursive Reflection
4. Set color as shadow if there's no reflection
5. Set base case depth==0 -> return color
**/
glm::vec3 RayTracer::FindColor(RTScene& s, Intersection& hit, int depth) {

    glm::vec3 color = vec3(0.1, 0.2, 0.3);

    //Base Case
    if (depth == 0) {
        return color;
    }

    // Check if it is ray hits or is blocked
    if (hit.dist < INFINITY) {

        //Set Color based on light seen
        for (auto const& l : s.shader->lightpositions) {
            //SET COLOR
            color = hit.triangle.material->ambient;     //Initialize Color
            int nLights = s.light.size();

            //Light Seen Eq = Sum( C_diffuse * LightSources * max( dot(n,lightpos[i]), 0) )
            for (int i = 0; i < nLights; i++) {
                color += vec3(hit.triangle.material->diffuse * s.shader->lightcolors[i] * max(dot(hit.N, vec3(s.shader->lightpositions[i])), 0.0f));

            }
            //color += vec3(hit.triangle.material->specular) * vec3(BlinnPhong(s, hit));    //No longer needed, Used for direct shading model for light seen

// MIRROR REFLECTION MODEL
            //Shoot Ray to Light
            Ray lightRay(hit.P + (hit.triangle.N[0] * 0.05f), normalize(vec3(l) - hit.P)); //may not neeed to normalize
            Intersection lightRayHit = Intersect(lightRay, s);

            // Check Visibility
            if (lightRayHit.dist == INFINITY) {

                //Mirror reflection direction: r=2(dot(n,v))*n - v
                Ray reflection(hit.P + hit.triangle.N[0] * .05f, 2.0f * dot(hit.N, hit.V) * hit.N - hit.V);
                Intersection reflectionHit = Intersect(reflection, s);

                color += vec3(hit.triangle.material->specular) * FindColor(s, reflectionHit, depth - 1);// color_reflect;

            }
            else {
                //SHADOW SHADING
                color = hit.triangle.material->ambient; // Softer shadow than black

            }

        }
    }
    return color;
}


//Finished -> Looks good
void RayTracer::RayTrace(Camera c, RTScene& s, Image& im) {
    int w = im.width;
    int h = im.height;
    std::cout << im.pixels.size() << std::endl;
    for (int j = 0; j < h; j++) {
        for (int i = 0; i < w; i++) {
            // std::cout << j*w+i << std::endl;
            Ray r = RayThruPixel(c, i, j, w, h);
            Intersection hit = Intersect(r, s);
            im.pixels[j * w + i] = FindColor(s, hit, 10);  // translation of pixels[i][j]
        }
    }
}
