#include <GL/glut.h>

#include "Settings.h"

unsigned int buf[H][W][3];

#if TREE_STATS
extern int totalDepth, totalTris, leafCount;
#endif

#define PI 3.141592

#include "RayTracer.h"
#include "Ray.h"
#include "Shape.h"
#include "Triangle.h"
#include "Color.h"
#include "Light.h"
#include "Vector.h"

#include "kd.h"
#include "bvh.h"
#include "ShapeContainer.h"
#include "util.h"

#include "glm.h"

#include <unistd.h>
#include <vector>
#include <sys/time.h>
#include <stdio.h>
#include "math.h"

using namespace std;
enum storageMethods {
    naive,
    kdtree,
    bvh
};
int whatMethodToUse = bvh;


GLMmodel* model;
bool doPerspective = true, drawEnable = false;
bool useColors = false;
double theta = 0.0, phi = 0.0, depth = 200.0;
std::string model_name;

Light* l[] = {
    new Light(Vector(0,0,0), Vector(0, -0.5, -1), Light::PARALLEL, 0.6),
    new Light(Vector(0,0,0), Vector(0, 1, 1), Light::PARALLEL, 0.6),
    new Light(Vector(0,0,0), Vector(1, 0.5, 0.5), Light::PARALLEL, 0.6)
};

vector<Shape*> raw_shapes;
Intersectable* shapes;

void init() {
    model = glmReadOBJ((char*)model_name.c_str());

    std::cout << "Building container." << std::endl;
    std::cout << "Loading model." << std::endl;
    
    int numTris = model->numtriangles;

    GLMtriangle* triangles = model->triangles;
    GLMgroup* group = model->groups;
    while (group) {
        GLMmaterial* material = 0;
        if (group->material) {
            material = &model->materials[group->material];
        }
        
        for (int i = 0; i < group->numtriangles; i++) {

            GLMtriangle* triangle = &model->triangles[group->triangles[i]];
            GLfloat* v1 = &model->vertices[3 * triangle->vindices[0]];
            GLfloat* v2 = &model->vertices[3 * triangle->vindices[1]];
            GLfloat* v3 = &model->vertices[3 * triangle->vindices[2]];

            Vector a(-v1[2], -v1[0], v1[1]);
            Vector b(-v2[2], -v2[0], v2[1]);
            Vector c(-v3[2], -v3[0], v3[1]);
            
            Color col = Color::WHITE;
            if (material && useColors) {
                Color col2(
                    material->diffuse[0] * 255, 
                    material->diffuse[1] * 255, 
                    material->diffuse[2] * 255
                );
                col = col2;
            }
            
            Triangle* tri = new Triangle(a, b, c, col);
            raw_shapes.push_back(tri);
        }
        group = group->next;
    }
    
    struct timeval start, end;
    gettimeofday(&start, NULL);

    switch (whatMethodToUse) {
    case naive:
        std::cout << "Using naive storage" << std::endl;
        shapes = new ShapeContainer(raw_shapes);
        break;
    case kdtree:
        std::cout << "Building k-d tree" << std::endl;
        shapes = KD::ConstructKD(raw_shapes);
        break;
    case bvh:
        std::cout << "Building bvh" << std::endl;
        shapes = BVH::ConstructBVH(raw_shapes);
        break;
    }

    gettimeofday(&end, NULL);

    long secs_diff = (end.tv_sec - start.tv_sec);
    long diff = (end.tv_usec - start.tv_usec)/1000 + secs_diff*1000;

    std::cout << "Built container in " << diff << "ms." << std::endl;
    #if TREE_STATS
    std::cout << "Avg. Leaf depth: " << totalDepth / leafCount << std::endl;
    std::cout << "Avg. Tris/Leaf: " << totalTris / leafCount << std::endl;
    #endif
    std::cout << raw_shapes.size() << " triangles" << std::endl;

}

void mouseCallback(int button, int state, int x, int y) {
    bool doRedisplay = false;
    if (state == GLUT_DOWN) {
        doRedisplay = true;

        if (x < W/4 && y < H/4) {
            depth *= 0.5;
        } else if (x < W/4 && y > 3*H/4) {
            depth /= 0.5;
        } else if (x < W/4) {
            theta += PI / 6.0;
        } else if (x > 3*W/4) {
            theta -= PI / 6.0;
        } else if (y > 3*H/4) {
            phi -= PI / 6.0;
            if (phi < -PI/2.0) phi = -PI/2.0; 
        } else if (y < H/4) {
            phi += PI / 6.0;
            if (phi > PI/2.0) phi = PI/2.0;
        } else {
            doRedisplay = false;
        }
        
        doRedisplay = doRedisplay || !drawEnable;
        drawEnable = true;
    }
    
    if (doRedisplay) glutPostRedisplay();
}

void display() {
    struct timeval start, end;
    gettimeofday(&start, NULL);

    if (drawEnable) {
        vector<Light*> lights(l, l+sizeof(l)/sizeof(l[0]));
    
        Vector e(cos(theta) * cos(phi) * depth, sin(theta) * cos(phi) * depth, sin(phi) * depth);
        Vector w(Vector::zero, e);
        w = w.normalize();
        Vector u(sin(theta), -cos(theta), 0);
        Vector v = w.cross(u);
        v = v.normalize();

        RayTracer r(shapes, lights, new View(e, u, v, w), new RayTracerSettings(&Color::WHITE, 0.4, doPerspective));
        r.solve(buf);
    }
    
    gettimeofday(&end, NULL);

    glRasterPos3f(-1.0f, -1.0f, 1.0f);
    glDrawPixels(W, H, GL_RGB, GL_UNSIGNED_INT, buf);
    glColor3d(1.0f, 1.0f, 1.0f);


    glRasterPos3f(-0.9f, -0.9f, 1.0f);
    long secs_diff = (end.tv_sec - start.tv_sec);
    long diff = (end.tv_usec - start.tv_usec)/1000 + secs_diff*1000;
    std::string test = std::string("Draw time: ") + std::to_string(diff) + std::string(" ms");
    for (int i = 0; i < test.size(); i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, test[i]);
    }

    glutSwapBuffers();
}

int main(int argc, char** argv) {

    std::string arg2;
    if (argc < 3) {
        std::cout << "Using default model and rendering mode." << std::endl;
        model_name = "data/shark.obj";
        useColors = false;
    } else { 
        model_name = argv[1];
        std::string arg2 = argv[2];
        useColors = arg2 == "true";
    }

    if (argc > 3) {
        std::string arg = argv[3];
        if (arg == "naive") {
            whatMethodToUse = naive;
        } else if (arg == "kdtree") {
            whatMethodToUse = kdtree;
        } else if (arg == "bvh") {
            whatMethodToUse = bvh;
        }
    }

    init();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB);
    glutInitWindowSize(W, H);
    glutCreateWindow("Scene");
    glutDisplayFunc(display);
    glutMouseFunc(mouseCallback);
    glutMainLoop();

    delete shapes;

    return 0;
}
