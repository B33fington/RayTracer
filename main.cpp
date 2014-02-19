#include "precompiled.h"

using namespace std;

struct RGBType
{
    double r;
    double g;
    double b;
};

void savebmp(const char* filename, int w, int h, int dpi, RGBType *data)
{
    FILE *f;
    int k = w*h;
    int s = 4*k;
    int filesize = 54 + s;

    double factor = 39.375f;
    int m = static_cast<int>(factor);

    int ppm = dpi*m;
    unsigned char bmpfileheader[14] = {'B','M', 0,0,0,0, 0,0,0,0, 54,0,0,0};
    unsigned char bmpinfoheader[40] = {40,0,0,0, 0,0,0,0, 0,0,0,0, 1,0,24,0};
    
    bmpfileheader[2]  = (unsigned char)(filesize);
    bmpfileheader[3]  = (unsigned char)(filesize>>8);
    bmpfileheader[4]  = (unsigned char)(filesize>>16);
    bmpfileheader[5]  = (unsigned char)(filesize>>24);

    bmpinfoheader[4]  = (unsigned char)(w);
    bmpinfoheader[5]  = (unsigned char)(w>>8);
    bmpinfoheader[6]  = (unsigned char)(w>>16);
    bmpinfoheader[7]  = (unsigned char)(w>>24);

    bmpinfoheader[8]  = (unsigned char)(h);
    bmpinfoheader[9]  = (unsigned char)(h>>8);
    bmpinfoheader[10] = (unsigned char)(h>>16);
    bmpinfoheader[11] = (unsigned char)(h>>24);

    bmpinfoheader[21] = (unsigned char)(s);
    bmpinfoheader[22] = (unsigned char)(s>>8);
    bmpinfoheader[23] = (unsigned char)(s>>16);
    bmpinfoheader[24] = (unsigned char)(s>>24);

    bmpinfoheader[25] = (unsigned char)(ppm);
    bmpinfoheader[26] = (unsigned char)(ppm>>8);
    bmpinfoheader[27] = (unsigned char)(ppm>>16);
    bmpinfoheader[28] = (unsigned char)(ppm>>24);

    bmpinfoheader[29] = (unsigned char)(ppm);
    bmpinfoheader[30] = (unsigned char)(ppm>>8);
    bmpinfoheader[31] = (unsigned char)(ppm>>16);
    bmpinfoheader[32] = (unsigned char)(ppm>>24);

    f = fopen(filename,"wb");
    fwrite(bmpfileheader,1,14,f);
    fwrite(bmpinfoheader,1,40,f);

    for(int i = 0; i < k; i++)
    {
        //RGBType rgb = data[i];

        double red = (data[i].r)*255;
        double green = (data[i].g)*255;
        double blue = (data[i].b)*255;

        unsigned char color[3] = {(unsigned char)floor(blue),(unsigned char)floor(green),(unsigned char)floor(red)};

        fwrite(color,1,3,f);
    }
    fclose(f);
}

int closestObject(vector<double> object_intersections){
    // returns the index with the closest value
    int minIndex;
    //prevent unnecessary calculations
    if(object_intersections.size() == 0){
        //no intersections
        return -1; 
    } 
    else if (object_intersections.size() == 1) {
        // only one intersection
        if(object_intersections.at(0) > 0){
            // if that intersection is greater than zero, then its the minIndex
            return 0;
        }else {
            return -1;
        } 
    } 
    else {
        //otherwise there is more than one intersection
        // first find the max value in the vector

        double max = 0;
        for(size_t i = 0; i < object_intersections.size(); i++){
            if(max < object_intersections.at(i)){
                max = object_intersections.at(i);
            }
        }
        //then starting from the max value find the minimum position
        if(max > 0){
            //we only want positive intersections
            for (size_t index = 0; index < object_intersections.size(); index++){
                if (object_intersections.at(index) > 0 && object_intersections.at(index) <= max){
                    max = object_intersections.at(index);
                    minIndex = index;
                }
            }
            return minIndex;
        }
        else{
            // all the intersections were negative
            return -1;
        }  
    }
}

Color getColorAt(vec3 intersectionPosition, vec3 intersectingRayDirection, vector<Object*> sceneObjects, int closest, vector<Source*> lightSources, double ambientLight){
    
    Color closestObjectColor = sceneObjects.at(closest)->getColor();
    vec3 closestObjectNormal = sceneObjects.at(closest)->getNormalAt(intersectionPosition); 

    if(closestObjectColor.getSpec() == 2) {
        // checkered floor pattern

        int square = (int)floor(intersectionPosition.getX()) + (int)floor(intersectionPosition.getZ());
        
                if(square % 2 == 0){
                    closestObjectColor.setRed(0.1);
                    closestObjectColor.setGreen(0.1);
                    closestObjectColor.setBlue(0.1);
                }
                else{
                    closestObjectColor.setRed(0.8);
                    closestObjectColor.setGreen(0.8);
                    closestObjectColor.setBlue(0.8);
                }
    }

    Color finalColor = closestObjectColor.colorScalar(ambientLight);

    if(closestObjectColor.getSpec() > 0 && closestObjectColor.getSpec() <= 1){
        //reflection from objects with specular value
        double dot1 = closestObjectNormal.dot(intersectingRayDirection.invert());
        vec3 scalar1 = closestObjectNormal.mult(dot1);
        vec3 add1 = scalar1.add(intersectingRayDirection);
        vec3 scalar2 = add1.mult(2);
        vec3 add2 = intersectingRayDirection.invert().add(scalar2);
        vec3 reflectionDirection = add2.normalize();

        Ray reflectionRay (intersectionPosition, reflectionDirection);

        //determine what the ray intersects with

        vector<double> reflectionIntersections;
        reflectionIntersections.erase(reflectionIntersections.begin(),reflectionIntersections.begin()+reflectionIntersections.size());

        for(size_t reflectionIndex = 0; reflectionIndex < sceneObjects.size(); reflectionIndex++){
            reflectionIntersections.push_back(sceneObjects.at(reflectionIndex)->findIntersection(reflectionRay));
        }

        int reflectedObjectIndex = closestObject(reflectionIntersections);

        if(reflectedObjectIndex != -1){
            //reflection ray missed all objects
            if(reflectionIntersections.at(reflectedObjectIndex) > ACCURACY){
                //determine the position and direction at the point of intersection with the reflection ray
                //the ray only affects the color if it reflected off something

                vec3 reflectionIntersectionPosition = intersectionPosition.add(reflectionDirection.mult(reflectionIntersections.at(reflectedObjectIndex)));
                vec3 reflectionIntersectionRayDirection = reflectionDirection;

                Color reflectionIntersectionColor = getColorAt(reflectionIntersectionPosition,reflectionIntersectionRayDirection, sceneObjects, reflectedObjectIndex, lightSources, ambientLight);

                finalColor = finalColor.colorAdd(reflectionIntersectionColor.colorScalar(closestObjectColor.getSpec()));                
            }
        }

    }

    for(size_t lightIndex = 0; lightIndex < lightSources.size(); lightIndex++){
        vec3 lightDirection = lightSources.at(lightIndex)->getLightPosition().add( intersectionPosition.invert()).normalize();

        double cosAngle = closestObjectNormal.dot(lightDirection);

        if(cosAngle > 0){
            //test for shadows

            bool shadowed = false;
            vec3 distanceToLight = lightSources.at(lightIndex)->getLightPosition().add(intersectionPosition.invert()).normalize();
            double distanceToLightMagnitude = distanceToLight.magnitude();

            Ray shadowRay (intersectionPosition, lightSources.at(lightIndex)->getLightPosition().add(intersectionPosition.invert()).normalize());

            vector<double> secondaryIntersections;
            secondaryIntersections.erase(secondaryIntersections.begin(),secondaryIntersections.begin()+secondaryIntersections.size());

            for(size_t objectIndex = 0; objectIndex < sceneObjects.size() && shadowed == false; objectIndex++){
                secondaryIntersections.push_back(sceneObjects.at(objectIndex)->findIntersection(shadowRay));
            }

            for(size_t a = 0; a < secondaryIntersections.size(); a++){
                if(secondaryIntersections.at(a) > ACCURACY){
                    if(secondaryIntersections.at(a) <= distanceToLightMagnitude){
                        shadowed = true;
                    }
                    break;
                }
            }

            if(shadowed == false){
                finalColor = finalColor.colorAdd(closestObjectColor.colorMult(lightSources.at(lightIndex)->getLightColor()).colorScalar(cosAngle));

                if(closestObjectColor.getSpec() > 0 && closestObjectColor.getSpec() <= 1){
                    //specularity [0 to 1]
                    double dot1 = closestObjectNormal.dot(intersectingRayDirection.invert());
                    vec3 scalar1 = closestObjectNormal.mult(dot1);
                    vec3 add1 = scalar1.add(intersectingRayDirection);
                    vec3 scalar2 = add1.mult(2);
                    vec3 add2 = intersectingRayDirection.invert().add(scalar2);
                    vec3 reflectionDirection = add2.normalize();
                    double specularity = reflectionDirection.dot(lightDirection);
                    if(specularity > 0){
                        specularity = pow(specularity, 10);
                        finalColor = finalColor.colorAdd(lightSources.at(lightIndex)->getLightColor().colorScalar(specularity*closestObjectColor.getSpec()));
                    }

                }
            }
        }
    }

    return finalColor.clip();
}

int main(int argc, char *argv[])
{
	cout << "rendering ..." << endl;
    clock_t t1, t2;
    t1 = clock();

    int linearCoordinate = 0;

    //image variables
    
    int dpi = 72;
    int width = 640;
    int height = 480;
    int n = width*height;
    int aaDepth = 5;

    //double aaThreshold = 0.1;
    double aspectRatio = (double)width/(double)height;
    double ambientLight = 0.05;
    RGBType *pixels = new RGBType[n];

    //axis vectors
    vec3 O (0,0,0);
    vec3 X (1,0,0);
    vec3 Y (0,1,0);
    vec3 Z (0,0,1);

    //camera variables

    vec3 _camPos (0, 1.5, -3.3);
    vec3 view (0, 0, 0);
    vec3 difference (_camPos.getX() - view.getX(), _camPos.getY() - view.getY(), _camPos.getZ() - view.getZ());

    vec3 _camDir = difference.invert().normalize();
    vec3 _camRight = Y.cross(_camDir).normalize();
    vec3 _camDown = _camRight.cross(_camDir);

    Camera sceneCam (_camPos, _camDir, _camRight, _camDown);

    //color initializations
    Color white_light (1.0f, 1.0f, 1.0f, 0.0f);
    Color Green (0.1f, 1.0f, 0.1f, 0.35f);
    Color Red (0.9f, 0.1f, 0.1f, 0.35f);
    Color Blue (0.1f, 0.1f, 0.9f, 0.35f);    
    Color yellow (0.9f, 0.9f, 0.1f, 0.35f);
    Color black (0.0f, 0.0f, 0.0f, 0.35f);
    Color testColor(0, 0, 0.4, 0.0);
    Color floorPlane(0.7f, 0.2f, 0.0f, 2);
    Color backPlane (0.05f, 0.3f, 0.05f, 0.5f);

    // lighting vectors
    vec3 lightPos (-7.0f, 10.0f, -8.0f);
    vec3 lightPos2 (7.0f, 10.0f, -8.0f);
    Light sceneLight (lightPos, white_light);
    Light sceneLight2 (lightPos2, white_light);
    vector<Source*> lightSources;
    lightSources.push_back(dynamic_cast<Source*>(&sceneLight));
    lightSources.push_back(dynamic_cast<Source*>(&sceneLight2));

    //scene objects
    Sphere sceneSphere (O, 1.0f, Red);
    Sphere sceneSphere2 (vec3(-2.5f, 0.0f, 2.0f), 1.0f, Blue);
    Sphere sceneSphere3 (vec3(2.5f, 0.0f, 2.0f), 1.0f, yellow);
    Sphere sceneSphere4 (vec3(0.0f, 0.0f, 4.0f), 1.0f, black);
    Plane scenePlane (Y, -1, floorPlane);
    Plane scenePlane2 (Z.invert(), -10, backPlane);

    vector<Object*> sceneObjects;
    sceneObjects.push_back(dynamic_cast<Object*>(&sceneSphere));
    sceneObjects.push_back(dynamic_cast<Object*>(&sceneSphere2));
    sceneObjects.push_back(dynamic_cast<Object*>(&sceneSphere3));
    sceneObjects.push_back(dynamic_cast<Object*>(&sceneSphere4));
    sceneObjects.push_back(dynamic_cast<Object*>(&scenePlane));
    sceneObjects.push_back(dynamic_cast<Object*>(&scenePlane2));

    int aaIndex;
    double xAmt, yAmt;
    //double tempRed, tempGreen, tempBlue;

	for(int x = 0; x < width; x++)
    {
        for(int y = 0; y < height; y++)
        {
            linearCoordinate = y*width + x;

            
            //start with a blank pixel
            double tempRed[aaDepth*aaDepth]; 
            double tempGreen[aaDepth*aaDepth];
            double tempBlue[aaDepth*aaDepth];



            for(int aax = 0; aax < aaDepth; aax++){
                for(int aay = 0; aay < aaDepth; aay++){

                    aaIndex = aay*aaDepth + aax;
                    srand(time(0));

                    //create ray from the camera to this pixel
                    if(aaDepth == 1){

                        if(width > height) {
                            //the image is wider than it is tall
                            xAmt = ((x+0.5)/width)*aspectRatio - (((width-height)/(double)height)/2);
                            yAmt = ((height - y) + 0.5)/height;
                        } else if (height > width) {
                            //the images is taller than it is wide
                            xAmt = (x + 0.5)/width;
                            yAmt = (((height-y) + 0.5)/height)/aspectRatio - (((height - width)/(double)width)/2);
                        } else {
                            //the image is square
                            xAmt = (x + 0.5)/width;
                            yAmt = ((height - y) + 0.5)/height;
                        }
                    }
                    else {
                        //anti-aliasing
                        if(width > height) {
                            //the image is wider than it is tall
                            xAmt = ((x + (double)aax/((double)aaDepth-1))/width)*aspectRatio - (((width-height)/(double)height)/2);
                            yAmt = ((height - y) + (double)aax/((double)aaDepth-1))/height;
                        } else if (height > width) {
                            //the images is taller than it is wide
                            xAmt = (x + (double)aax/((double)aaDepth-1))/width;
                            yAmt = (((height-y) + (double)aax/((double)aaDepth-1))/height)/aspectRatio - (((height - width)/(double)width)/2);
                        } else {
                            //the image is square
                            xAmt = (x + (double)aax/((double)aaDepth-1))/width;
                            yAmt = ((height - y) + (double)aax/((double)aaDepth-1))/height;
                        }
                    }

                    vec3 camRayOrigin = sceneCam.getCameraPosition();
                    vec3 camRayDirection = _camDir.add(_camRight.mult(xAmt - 0.5).add(_camDown.mult(yAmt - 0.5))).normalize();
                    
                    Ray camRay (camRayOrigin, camRayDirection);

                    vector<double> intersections;
                    for (size_t index = 0; index < sceneObjects.size(); index++){
                        intersections.push_back(sceneObjects.at(index)->findIntersection(camRay));
                    }

                    int closest = closestObject(intersections);

                    if(closest == -1)
                    {
                        tempRed[aaIndex] = 0.0f;
                        tempGreen[aaIndex] = 0.0f;
                        tempBlue[aaIndex] = 0.0f;
                    }
                    else{
                        
                        if(intersections.at(closest) > ACCURACY) {
                            //determine the position and direction vectors at the 
                            //point of intersection

                            vec3 intersectionPosition = camRayOrigin.add(camRayDirection.mult(intersections.at(closest)));
                            vec3 intersectingRayDirection = camRayDirection;

                            Color intersectionColor = getColorAt(intersectionPosition, intersectingRayDirection, sceneObjects, closest, lightSources, ambientLight);

                            tempRed[aaIndex] = intersectionColor.getRed();
                            tempGreen[aaIndex] = intersectionColor.getGreen();
                            tempBlue[aaIndex] = intersectionColor.getBlue();
                        }
                    }
                }
            }
            //average the pixel color
            double totalRed = 0;
            double totalGreen = 0;
            double totalBlue = 0;
            for(int aai = 0; aai < aaDepth*aaDepth; aai++){
                totalRed += tempRed[aai];
                totalGreen += tempGreen[aai];
                totalBlue += tempBlue[aai];
            }
            double avgRed = totalRed/(aaDepth*aaDepth);
            double avgGreen = totalGreen/(aaDepth*aaDepth);
            double avgBlue = totalBlue/(aaDepth*aaDepth);

            pixels[linearCoordinate].r = avgRed;
            pixels[linearCoordinate].g = avgGreen;
            pixels[linearCoordinate].b = avgBlue;
        }
    }

    savebmp("sceneAA5.bmp",width,height,dpi,pixels);

    delete pixels; 
    t2 = clock();
    float diff = ((float)t2 - (float)t1)/1000;

    cout << diff << " seconds" << endl;

	return 0;
}