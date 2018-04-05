#include <iostream>
#include <cmath>
#include "Object.h"
#include "Sphere.h"
#include "Plane.h"
#include "lodepng.h"

const float M_PI = 3.141592654;
const int MAX_DEPTH = 1;
Vec3 followRay(Vec3 &origin, Vec3 &raydir,std::vector<Object*> &shapes, int &depth);
bool shadowRay(Vec3 &hitPoint, Vec3 &pointToLight, Vec3 &normal, std::vector<Object*> &shapes);

struct Light{
    Vec3 position;
    Vec3 color;
};

std::vector<Light*>light;

//encodes the scene into png format
void encodeOneStep(const char* filename, std::vector<unsigned char>& image, unsigned width, unsigned height)
{
  //Encode the image
  unsigned error = lodepng::encode(filename, image, width, height);

  //if there's an error, display it
  if(error) std::cout << "encoder error " << error << ": "<< lodepng_error_text(error) << std::endl;
}//encodeOneStep

//renders the scene
void renderScene(std::vector<Object*> &shapes)
{
    unsigned width = 640, height = 480;
    int depth = 0;
    Vec3 *image = new Vec3[width * height];
    Vec3 *pixel = image;
    Vec3 rayOrigin(0.0, 1.0, 0.0);
    float invWidth = 1 / float(width);
    float  invHeight = 1 / float(height);
    float fov = 30;
    float  aspectratio = width / float(height);
    float angle = tan(M_PI * 0.5 * fov / 180.0);

    for (unsigned y = 0; y < height; ++y)
        {
        for (unsigned x = 0; x < width; ++x, ++pixel)
        {
            float viewSpaceCoordX = (2 * ((x + 0.5) * invWidth) - 1) * angle * aspectratio;
            float viewSpaceCoordY = (1 - 2 * ((y + 0.5) * invHeight)) * angle;
            Vec3 raydir(viewSpaceCoordX, viewSpaceCoordY, -1);
            raydir.normalize();
            depth = 0;
            *pixel = followRay(rayOrigin, raydir, shapes, depth);
        }//for
    }//for

    std::vector<unsigned char> image1;
    image1.resize(width * height * 4);
    for(unsigned y = 0; y < height; y++)
    {
        for(unsigned x = 0; x < width; x++)
        {
            int temp = y * width + x;
            image1[4 * width * y + 4 * x + 0] = image[temp].x * 255;
            image1[4 * width * y + 4 * x + 1] = image[temp].y * 255;
            image1[4 * width * y + 4 * x + 2] = image[temp].z * 255;
            image1[4 * width * y + 4 * x + 3] = 255;
        }//for
   }//for
   encodeOneStep("test9.png", image1, width, height);
}//renderScene

//follows the ray in the scene and determines which objects are visible
Vec3 followRay(Vec3 &origin,Vec3 &rayDirection, std::vector<Object*> &shapes, int &depth)
{
    Vec3 color(0.0, 0.0,0.0);
    float epsilon = 0.001;
    float minDistance = INFINITY;
    Object *winningObject = nullptr;

    //determine if there is an intersection between ray and objects in the scene
    for(unsigned i = 0; i < shapes.size(); i++){
        float t0 = INFINITY,  t1 = INFINITY;
        if( shapes[i] -> intersection(origin, rayDirection, t0, t1 )){
            if(t0 < 0) t0 = t1;
            if( minDistance > t0)
            {
                minDistance = t0;
                winningObject = shapes[i];
            }//if
        }//if
    }//for

    if(winningObject != nullptr){
        //Vec3 lightColor(1.0, 1.0, 1.0);
         //ambient contribution
        float ambientStrength = 0.0025;

        //intersection of ray and surface
        Vec3 hitPoint = origin + (rayDirection * minDistance);
        //normal to the ssurface
        Vec3 normal = winningObject -> getNormal(hitPoint);
        normal.normalize();
        //add each light source's contribution to final color
        for(unsigned i = 0; i < light.size(); i++){
            //direction from interscetion point to light source
            Vec3 pointToLight = light[i]->position - hitPoint;
            pointToLight.normalize();
            //ambient contribution
            Vec3 ambientResult = light[i]-> color * ambientStrength;
            color = (color * 0.5) + (ambientResult * winningObject->getColor()* 0.5);
            //check forshadows
            Vec3 biasPoint = hitPoint + (pointToLight * epsilon);
            bool isShadow = shadowRay(biasPoint , pointToLight, normal, shapes);
            if(!isShadow){
                //diffuse contribution
                float diff = std::max(normal.dot(pointToLight), 0.0f);
                Vec3 diffuse = light[i]->color * diff;
                //specular contribution
                float specStrength = 0.5f;
                Vec3 halfWay = origin - hitPoint;
                halfWay.normalize();
                float spec = std::pow(std::max(normal.dot(halfWay), 0.0f), 32);
                Vec3 specularResult = light[i]->color * specStrength * spec;
                color = (color * 0.4) + ((diffuse + specularResult) * winningObject->getColor() *0.6);
            }//if
        }//for
        //if depth < MAX_DEPTH, compute reflective and refractive ray contribution to color
       if(depth < MAX_DEPTH){
            //compute reflection vector
            Vec3 intersRayDir = rayDirection * -1;
            float dot1 = normal.dot(intersRayDir);
            Vec3 CI = normal * dot1;
            Vec3 SI = CI + rayDirection;
            Vec3 reflection = CI + SI;
            reflection.normalize();
            depth +=1;
            //add reflection color to pixel color
            Vec3 biasPoint2 = hitPoint + (reflection * epsilon);
            color =  ((color * (1 - winningObject->specular)) +
                      (followRay(biasPoint2, reflection, shapes, depth) * winningObject->specular)) * winningObject->getColor() ;
            //check if  ray hit inside or outside of sphere
            float inorOut = normal.dot(intersRayDir);
            //compute refractive ray
            float ratio = 0.658 ;
            if(inorOut > 0)ratio = 0.658;
            else ratio = 1.0 / ratio;
            Vec3 ST = CI * ratio;
            float exp = 1 - (ST.dot(ST));
            float exp2 = std::sqrt(exp);
            Vec3 negNormal = normal * -1;
            Vec3 CT = negNormal * exp2;
            Vec3 refraction = CT + ST;
            refraction.normalize();
            //add refractive color to pixel color
            Vec3 biasPoint3 = hitPoint + (refraction * epsilon);
            color = ((color * (1- winningObject->refractive)) +
                     (followRay(biasPoint3, refraction, shapes, depth) * winningObject->refractive)) * winningObject->getColor() ;
        }//if
    }//if
    return color;
}//followRay

bool shadowRay(Vec3 &hitPoint, Vec3 &pointToLight, Vec3 &normal, std::vector<Object*> &shapes)
{
    float epsilon1 = 0.001     ;
    bool isShadow = false;
    float cosine = normal.dot(pointToLight);
    //check that object is not between camera and light source
    if(cosine > 0){
        float pointToLight_Magnitude = pointToLight.length();
        float t = INFINITY;
        for(unsigned i = 0; i < shapes.size(); i++){
            float t00 = INFINITY ,  t11 = INFINITY;
            if(shapes[i] -> intersection(hitPoint , pointToLight, t00, t11))
            {
                if(t00 < 0) t00 = t11;
                if( t > t00) t = t00;
                if( t > epsilon1){
                    if( t <= pointToLight_Magnitude)
                    {
                        isShadow = true;
                        break;
                    }//if
                }//if
            }//if
        }//for
    }//for
    return isShadow;
}//shadowRay


int main(int argc, char **argv)
{
    //scene lights
    //light 1
    Light light1;
    light1.position = Vec3(55, 55, 70);
    light1.color = Vec3(1.0, 1.0, 1.0);
    light.push_back(&light1);
    //light2
    Light light2;
    light2.position = Vec3( 1e-16, 1.2e19, 1e16);
    light2.color = Vec3(1.0, 1.0, 1.0);
    light.push_back(&light2);
    //light3
    Light light3;
    light3.position = Vec3(-1e13, 6e16, 2e13);
    light3.color = Vec3(1.0, 1.0, 1.0);
    light.push_back(&light3);


    //scene objects;
    Plane _plane;
    Sphere ball(Vec3( 1.0, 0.5, -6.0), 0.5, Vec3(0.0, 1.0, 0.0), 0, 0);
    Sphere ball2(Vec3( -1, 1.5 , -8), 1.5, Vec3(1.0, 1.0, 1.0 ), 0.3, 0.3);
    Sphere ball3(Vec3( -2, 2 , -11 ), 2, Vec3(0.0, 0.0, 1.0 ), 0, 0);

    std::vector<Object*> shapes;
    shapes.push_back(dynamic_cast<Object*>(& _plane));
    shapes.push_back(dynamic_cast<Object*>(&ball));
    shapes.push_back(dynamic_cast<Object*>(&ball2));
    shapes.push_back(dynamic_cast<Object*>(&ball3));
    renderScene(shapes);

    return 0;
}//main
