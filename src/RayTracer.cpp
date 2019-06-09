#include "RayTracer.h"

#include <iostream>
#define DEBUG 1

using namespace std;

void RayTracer::solve(unsigned int img[H][W][3]) {

    Ray* ray;
    if (!rts->doPerspective) {
        ray = new Ray(view->e, view->w);
    }
    for (int j = 0; j < H; j++) {
        #if DEBUG
        std::cout << "Rendering: [";
        for (int k = 0; k < 15; k++) {
            std::cout << ((k*H < j*15)? "=" : " ");
        }
        std::cout << "] " <<  j << " / " << H << "\r" << std::flush;
        #endif

        for (int i = 0; i < W; i++) {
            int i1 = i - W/2;
            int j1 = j - H/2;

            Vector* pix;
            if (rts->doPerspective) {
                pix = getXYZ(i1, j1);
                Vector direction(*pix, view->e);
                ray = new Ray(view->e, direction);
            
            } else {
                pix = getXYZParallel(i1, j1);
                ray->origin = *pix;
            }
            
            Intersection* meeting = objects->intersect(*ray);
            
            Color* rgb;
            if (meeting == nullptr) {
                rgb = new Color(rts->BG);
            } else if (meeting->shape->isGlazed) {
                rgb = getColorRecursive(meeting, 3);
            } else {
                rgb = getColor(meeting);
            }
            
            img[j][i][0] = rgb->r << 24;
            img[j][i][1] = rgb->g << 24;
            img[j][i][2] = rgb->b << 24;

            delete meeting;
            delete rgb;
            delete pix;

            if (rts->doPerspective) {
                delete ray;
            }
        }
    }
    #if DEBUG
    std::cout << "Rendering: Done                                      \r" << std::flush;
    #endif
}

Color* RayTracer::getColorRecursive(Intersection* meeting, int depth) {
    Color* c = getColor(meeting);
    
    if (depth <= 0 || meeting == nullptr) return c;
    if (!(meeting->shape->isGlazed)) return c;
    Vector n = meeting->normal;
    Vector d = meeting->r->direction;
    Vector refDir = d.subtract(n.multiply(n.dot(d) * 2));

    Ray r(meeting->point, refDir);
    Intersection* meet2 = objects->intersect(r);
    Color* c2 = getColorRecursive(meet2, depth-1);

    Color* sum = Color::AddColors(c, c2, meeting->shape->kg);

    delete meet2;
    delete c2;
    delete c;

    return sum;
}

Color* RayTracer::getColor(Intersection* meeting) {
    if (meeting == nullptr) return new Color(rts->BG);

    double ambient = rts->AMBIENT;
    double lambert = 0.0;
    double specular = 0.0;

    for (Light* light : lights) {
        // check if another shape is blocking the intersection with this Light
        Vector lightDir = light->getDirection(meeting->point).multiply(-1);
        Ray ray(meeting->point, lightDir);
        
        Intersection* dummy = objects->intersect(ray, meeting->shape);
        bool didIntersect = (dummy != nullptr);
        delete dummy;
        
        if (!didIntersect) {
            double intensity = light->getIntensity(meeting);

            // do lambert shading
            double cosFace = meeting->normal.dot(lightDir);
            lambert += max(0.0, cosFace * intensity);

            if (!(meeting->shape->isGlazed)) {
                // do specular shading
                Vector vr = meeting->normal.multiply(2*cosFace).subtract(lightDir).normalize();
                double cosSpec = meeting->r->direction.multiply(-1).dot(vr);
                specular += intensity * pow(max(0.0, cosSpec), meeting->shape->phongExp);
            }
        }
    }

    double intensity = ambient*meeting->shape->ka 
        + lambert*meeting->shape->kl 
        + specular*meeting->shape->ks;

    if (intensity < 0) intensity = 0;

    return Color::ScaleColor(&(meeting->shape->color), intensity);
}


Vector* RayTracer::getXYZParallel(int i, int j) {
    double a = (i + 0.5) / W * rts->width;
    double b = (j + 0.5) / H * rts->height;
    double c = 1.0;

    return view->getXYZ(a, b, c);
}

Vector* RayTracer::getXYZ(int i, int j) {
    double a = (i + 0.5) / max(W, H);
    double b = (j + 0.5) / max(W, H);
    double c = 1.0;

    return view->getXYZ(a, b, c);
}
