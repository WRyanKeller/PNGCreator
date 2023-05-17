// -----------------------------------------------------------------
// CityEngine PNG Creator
// by Ryan Keller https://github.com/WRyanKeller
// -----------------------------------------------------------------

// -----------------------------------------------------------------
// PNG Writer Credits
// Paul Blackburn https://github.com/individual61
// Axel Huebl https://github.com/ax3l
// -----------------------------------------------------------------

// -----------------------------------------------------------------
// Uses OpenGL
// -----------------------------------------------------------------

// -----------------------------------------------------------------
// References some basic glut functions from code by Dr. Chao Peng
// -----------------------------------------------------------------

#ifdef __APPLE__
#include <GLUT/glut.h> // include glut for Mac
#else
#include <GL/freeglut.h> //include glut for Windows
#endif

#include <math.h>
#include "pngwriter.h"
#include "Landmark.h"
#include "Area.h"
#include <cmath>
#include <iostream>
#include <string>
using namespace std;
#include <ctime>
#include <cstdlib>
#include <vector>

// constants for pngs
const int PngWidth = 760;
const int PngHeight = 576;

// png references
pngwriter* areaTypes;
pngwriter* elevation;
pngwriter* obstacles;
pngwriter* skyline;
pngwriter* topology;

// landmark stuff
vector<Landmark*> landmarks;
vector<Area*> areas;
vector<Landmark*> skylines;

// menu functions
void generate();
void save();
void reference();
bool generated;

// helpers for generation
bool validBounds(double minX, double maxX, double minY, double maxY);
bool validAreaBounds(double minX, double maxX, double minY, double maxY);

// the window's width and height
int width;
int height;

/// <summary>
/// first calls
/// </summary>
/// <param name=""></param>
void init(void)
{
    // initialize the size of the window
    width = 760;
    height = 576;

    generated = false;

    // initialize vectors
    landmarks = vector<Landmark*>();
    areas = vector<Area*>();
    skylines = vector<Landmark*>();

    srand(time(0));
}

/// <summary>
/// calls on redisplay flag
/// </summary>
/// <param name=""></param>
void display(void)
{
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // draw skylines
    for (Landmark* s : skylines)
    {
        s->Draw();
    }

    // draw each landmark
    for (Landmark* l : landmarks)
    {
        l->Draw();
    }

    glutSwapBuffers();
}

/// <summary>
/// called upon window creation / resize
/// </summary>
/// <param name="w">width</param>
/// <param name="h">height</param>
void reshape(int w, int h)
{
    // update the screen dimensions
    width = w;
    height = h;

    // project onto 0-10 coords
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, 760.0, 0.0, 576.0);

    glViewport(0, 0, (GLsizei)width, (GLsizei)height);

    glutPostRedisplay();
}

/// <summary>
/// called on click/release
/// </summary>
/// <param name="button">left/right</param>
/// <param name="state">up/down</param>
/// <param name="x">x</param>
/// <param name="y">y</param>
void mouse(int button, int state, int x, int y)
{
    // example mouse command for the future
    /*if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        current.addVertex(vec2((float)x / rasterSize[0] * canvasSize[0], (float)(rasterSize[1] - y) / rasterSize[1] * canvasSize[1]));
        glutPostRedisplay();
    }*/
}

/// <summary>
/// called when mouse moves
/// </summary>
/// <param name="x">x</param>
/// <param name="y">y</param>
void motion(int x, int y)
{
    // example movement design pattern for the future
    //mousePos[0] = (float)x / rasterSize[0] * canvasSize[0];
    //mousePos[1] = (float)(rasterSize[1] - y) / rasterSize[1] * canvasSize[1];

    glutPostRedisplay();
}

/// <summary>
/// decides what to do on a menu click
/// </summary>
/// <param name="value">option selected</param>
void menu(int value)
{
    switch (value)
    {
        case 0: // generate
            generate();
            break;
        case 1: // save
            save();
            break;
        case 2: // exit
            exit(0);
        default:
            break;
    }
}

/// <summary>
/// creates the menu
/// </summary>
void createMenu()
{
    glutCreateMenu(menu);
    glutAddMenuEntry("Generate", 0);
    glutAddMenuEntry("Save", 1);
    glutAddMenuEntry("Exit", 2);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

int main(int argc, char* argv[])
{
    // initializtion calls
    init();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize((int)width, (int)height);
    glutCreateWindow("CityEngine PNG Creator");
    createMenu();

    // openGL / glut function binds
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutPassiveMotionFunc(motion);
    glutMainLoop();
    return 0;
}

/// <summary>
/// generates a new set of landmarks, areas, and skylines
/// </summary>
void generate()
{
    generated = true;
    
    // reset vectors
    landmarks.clear();
    areas.clear();
    skylines.clear();

    srand(time(0));
    short counter = 0;
    short xRange;
    short xStart;
    short yRange;
    short yStart;

    // add large water body
    xRange = rand() % 200 + 300;
    xStart = rand() % (width - xRange);
    yRange = rand() % 200 + 300;
    yStart = rand() % (width - yRange);
    landmarks.push_back(new Landmark(xStart, xStart + xRange, yStart, yStart + yRange, 0.05, 0.35, 0.7));

    // add large land
    do
    {
        counter++;
        xRange = rand() % 200 + 300;
        xStart = rand() % (width - xRange);
        yRange = rand() % 200 + 300;
        yStart = rand() % (width - yRange);
    } while (!validBounds(xStart, xStart + xRange, yStart, yStart + yRange) && counter < 6);
    landmarks.push_back(new Landmark(xStart, xStart + xRange, yStart, yStart + yRange, 0.05, 0.45, 0.15));

    // add small land
    do
    {
        counter++;
        xRange = rand() % 100 + 100;
        xStart = rand() % (width - xRange);
        yRange = rand() % 100 + 100;
        yStart = rand() % (width - yRange);
    } while (!validBounds(xStart, xStart + xRange, yStart, yStart + yRange) && counter < 15);
    landmarks.push_back(new Landmark(xStart, xStart + xRange, yStart, yStart + yRange, 0.05, 0.35, 0.7));

    // add small water
    do
    {
        counter++;
        xRange = rand() % 200 + 200;
        xStart = rand() % (width - xRange);
        yRange = rand() % 200 + 200;
        yStart = rand() % (width - yRange);
    } while (!validBounds(xStart, xStart + xRange, yStart, yStart + yRange) && counter < 30);
    landmarks.push_back(new Landmark(xStart, xStart + xRange, yStart, yStart + yRange, 0.05, 0.45, 0.15));

    // maybe add more land
    if (rand() % 2 == 1)
    {
        do
        {
            counter++;
            xRange = rand() % 400 + 100;
            xStart = rand() % (width - xRange);
            yRange = rand() % 400 + 100;
            yStart = rand() % (width - yRange);
        } while (!validBounds(xStart, xStart + xRange, yStart, yStart + yRange) && counter < 30);
        landmarks.push_back(new Landmark(xStart, xStart + xRange, yStart, yStart + yRange, 0.05, 0.35, 0.7));
    }

    // maybe add more water
    if (rand() % 2 == 1)
    {
        do
        {
            counter++;
            xRange = rand() % 200 + 200;
            xStart = rand() % (width - xRange);
            yRange = rand() % 200 + 200;
            yStart = rand() % (width - yRange);
        } while (!validBounds(xStart, xStart + xRange, yStart, yStart + yRange) && counter < 30);
        landmarks.push_back(new Landmark(xStart, xStart + xRange, yStart, yStart + yRange, 0.05, 0.45, 0.15));
    }

    // skyline time
    xRange = rand() % 100 + 50;
    xStart = rand() % (width - xRange);
    yRange = rand() % 100 + 50;
    yStart = rand() % (width - yRange);
    skylines.push_back(new Landmark(xStart, xStart + xRange, yStart, yStart + yRange, 1, 0, 0));

    xRange = rand() % 100 + 50;
    xStart = rand() % (width - xRange);
    yRange = rand() % 100 + 50;
    yStart = rand() % (width - yRange);
    skylines.push_back(new Landmark(xStart, xStart + xRange, yStart, yStart + yRange, 1, 0, 0));

    // generate areas
    xRange = rand() % 150 + 250;
    xStart = rand() % (width - xRange);
    yRange = rand() % 150 + 250;
    yStart = rand() % (width - yRange);
    areas.push_back(new Area(xStart, xStart + xRange, yStart, yStart + yRange, .1, .45, .75));

    // make absolutely CERTAIN the areas do not interfere with one another
    counter = 0;
    do
    {
        counter++;
        xRange = rand() % 150 + 250;
        xStart = rand() % (width - xRange);
        yRange = rand() % 150 + 250;
        yStart = rand() % (width - yRange);
    } while (!validAreaBounds(xStart, xStart + xRange, yStart, yStart + yRange) && counter < 100);
    areas.push_back(new Area(xStart, xStart + xRange, yStart, yStart + yRange, 0.9, 0.25, 0.2));

    glutPostRedisplay();
}

/// <summary>
/// returns false if it interferes with any other landmarks
/// </summary>
/// <param name="minX"></param>
/// <param name="maxX"></param>
/// <param name="minY"></param>
/// <param name="maxY"></param>
/// <returns></returns>
bool validBounds(double minX, double maxX, double minY, double maxY)
{
    // check each landmark
    for (Landmark* l : landmarks)
    {
        // run AABB collision detection
        if (l->GetMaxBoundX() < minX ||
            l->GetMinBoundX() > maxX ||
            l->GetMaxBoundY() < minY ||
            l->GetMinBoundY() > maxY)
        {
            continue;
        }

        return false;
    }

    return true;
}

/// <summary>
/// returns false if it interferes with any other areas
/// </summary>
/// <param name="minX"></param>
/// <param name="maxX"></param>
/// <param name="minY"></param>
/// <param name="maxY"></param>
/// <returns></returns>
bool validAreaBounds(double minX, double maxX, double minY, double maxY)
{
    // check each area
    for (Area* a : areas)
    {
        // run AABB collision detection
        if (a->GetMaxBoundX() < minX ||
            a->GetMinBoundX() > maxX ||
            a->GetMaxBoundY() < minY ||
            a->GetMinBoundY() > maxY)
        {
            continue;
        }

        return false;
    }

    return true;
}

/// <summary>
/// saves the current set of maps
/// </summary>
void save()
{
    if (!generated) { return; }

    // create all of the png writers
    areaTypes = new pngwriter(760, 576, 0, "areaTypes.png");
    elevation = new pngwriter(760, 576, 0, "elevation.png");
    obstacles = new pngwriter(760, 576, 1.0, "obstacles.png");
    skyline = new pngwriter(760, 576, 0, "skyline.png");
    topology = new pngwriter(760, 576, 0.56, "topology.png");

    // green background for area types
    areaTypes->filledsquare(0, 0, 760, 576, 0.15, 0.65, 0.25);

    // save landmarks to topology (as an obstacle)
    for (Landmark* l : landmarks)
    {
        l->Save(obstacles, topology, true);
    }

    // save skylines to skyline map (not obstacle)
    for (Landmark* s : skylines)
    {
        s->Save(obstacles, skyline, false);
    }

    // save areas to area map
    for (Area* a : areas)
    {
        a->Save(areaTypes);
    }

    // now it's elevation time

    // generate first elevation landmark
    short xRange = rand() % 50 + 100;
    short xStart = rand() % (width) - xRange / 2;
    short yRange = rand() % 50 + 100;
    short yStart = rand() % (width) - yRange / 2;
    double color = rand() % 20 / 100.0;
    Landmark* ele = new Landmark(xStart, xStart + xRange, yStart, yStart + yRange, color, color, color);

    // save to elevation map (not obstacle, is elevation)
    ele->Save(obstacles, elevation, false, true);

    // generate second elevation landmark
    xRange = rand() % 50 + 100;
    xStart = rand() % (width)-xRange / 2;
    yRange = rand() % 50 + 100;
    yStart = rand() % (width)-yRange / 2;
    color = rand() % 20 / 100.0;
    Landmark* ele2 = new Landmark(xStart, xStart + xRange, yStart, yStart + yRange, color, color, color);

    // save to elevation map (not obstacle, is elevation)
    ele2->Save(obstacles, elevation, false, true);

    // flush and close
    areaTypes->close();
    elevation->close();
    obstacles->close();
    skyline->close();
    topology->close();

    // memory management
    delete ele;
    delete ele2;
}

void reference()
{
    /* Introduction
     * This program is meant to serve as an introduction to using PNGwriter.
     * It is not the simplest possible use of it, and does not cover all
     * of its features. See the website for more examples.
     * This program will generate 4 PNG images. The first two will be
     * examples of using PNGwriter's figures.
     * The third will be an example of how to read an already existing PNG image.
     * The fourth will be an example of how to use plotHSV to generate a rainbow.
     * */

     //////////////////////////////////////////////////////////////////////

     /* one.png
      * This will be a 300x300 image with a black background, called one.png
      * */
    pngwriter one(300, 300, 0, "one.png");

    std::cout << "Generating one.png...";

    /* Purple rectangle (filled)
     * Make a purple filled retangle. Notice we are using
     * int colour levels, e.g. 65535 is the max level of green.
     * */
    one.filledsquare(30, 5, 45, 295, 65535, 0, 65535);

    /* Blue rectangle
     * Make a blue rectangle over the green one.
     * Notice that we are using colour coefficcients of type double.
     * */
    one.square(20, 10, 40, 290, 0.0, 0.0, 1.0);

    /* Yellow circle (filled) and Grey Circle
     * Using colour coefficcients of type double.
     * */
    one.filledcircle(250, 250, 40, 1.0, 1.0, 0.0);
    one.circle(250, 70, 30, 20000, 20000, 50000);

    /* Lines
     * Draw 16 lines to test all general directions of line().
     * All start from the centre and radiate outwards.
     * The colour is varied to be able to distinguish each line.
     * */
     //Top row
    one.line(150, 150, 0, 300, 0, 65535, 4000);
    one.line(150, 150, 75, 300, 0, 65535, 8000);
    one.line(150, 150, 150, 300, 0, 65535, 12000);
    one.line(150, 150, 225, 300, 0, 65535, 16000);
    one.line(150, 150, 300, 300, 0, 65535, 20000);
    //Right side
    one.line(150, 150, 300, 225, 0, 65535, 24000);
    one.line(150, 150, 300, 150, 0, 65535, 28000);
    one.line(150, 150, 300, 75, 0, 65535, 32000);
    one.line(150, 150, 300, 0, 0, 65535, 36000);
    //Bottom row
    one.line(150, 150, 225, 0, 0, 65535, 40000);
    one.line(150, 150, 150, 0, 0, 65535, 44000);
    one.line(150, 150, 75, 0, 0, 65535, 48000);
    one.line(150, 150, 0, 0, 0, 65535, 52000);
    //Left side
    one.line(150, 150, 0, 75, 0, 65535, 56000);
    one.line(150, 150, 0, 150, 0, 65535, 60000);
    one.line(150, 150, 0, 225, 0, 65535, 64000);

    /*Change the text information in the PNG header
     * */
    one.settext("one.png", "John Cleese", "Test PNG", "pngtest");

    std::cout << " done. Writing to disk...";
    one.close();
    std::cout << " done." << std::endl;


    ////////////////////////////////////////////////////////////////////////////////////
     /*  two.png
      *  This will be a 300x300 image with a black background, it will be called two.png.
      * Note that we are using 0.0 as the background colour and that we are using a string
      * type object as the filename, which can convert itself into a const char * with
      * filename.c_str().
      * */
    std::cout << "Generating two.png...";
    string filename = "two.png";
    pngwriter two(300, 300, 0.0, filename.c_str());

    /* Gradients
     * Here we will draw a few gradients, which are nothing more than drawing a filledsquare
     * or line or filledcircle repeatedly, changing the colour as we go.
     * */

     /* Draw the gradient in the lower left corner.
      * */
    for (int iter = 1;iter < 300;iter++)
    {
        two.line(1, 300, iter, 1, 65535 - int(65535 * ((double)iter) / 300.0), 0, 65535);
    }

    /* Draw the green circles.
     * */
    for (int iter2 = 0; iter2 < 8; iter2++)
    {
        two.filledcircle(280 - 25 * iter2, 250, 10, 0, 65535 - int(65535 * ((double)iter2 / 7.0)), 0);
    }

    /* Draw the other central gradient.
     * Notice that it is drawing a filled square, and moving the upper right corner down
     * and left, while changind the colour.
     * */
    for (int iter3 = 120; iter3 > 0;iter3--)
    {
        two.filledsquare(70, 70, 70 + iter3, 70 + iter3, 0, 0, 65535 - int(65535 * ((double)iter3 / 120.0)));
    }

    /* Draw the orange circle gradient.
     * */
    for (int iter4 = 0; iter4 < 31; iter4++)
    {
        two.filledcircle(250, 200, 30 - iter4, 1.0, 1.0 - double(iter4) / 30.0, 0.0);
    }


    /* Now, just as an example, we will use a lower compression on this image.
     * The default is 6 (from 0 to 9) and we will set it to 3. The lower the compression used
     * the faster the image will be close()d. Complex images will take longer to
     * close() than simple ones.
     * */
    two.setcompressionlevel(3);
    std::cout << " done. Writing to disk...";
    two.close();
    std::cout << " done." << std::endl;


    ///////////////////////////////////////////////////////////////////

    /* copiaburro.png
     * This section opens burro.png and places it in the pngwriter instance called burro.
     * The PNGwriter instance burro is 1x1 pixels in size because we will use it
     * as a container for another PNG image that we will
     * soon read in. No point in grabbing memory now, so we'll set the width and height to 1.
     * Notice that the original is not altered in any way.
     * */
    pngwriter burro(1, 1, 0, "copiaburro.png");

    /* readfromfile()
     * Now we specify the path to the file we wish to place into
     * the PNGwriter instance "burro":
     * */
    std::cout << "Opening burro.png...";
    burro.readfromfile("burro.png"); //It really is that easy.
    std::cout << " done." << std::endl;

    /* getwidth() and getheight()
     * We may not know the size of the image we just read in.
     * We find out like this:
     * */
    int burrowidth = burro.getwidth();
    int burroheight = burro.getheight();


    std::cout << "The image that has just been read from disk (burro.png) is " << burro.getheight();
    std::cout << " pixels high and " << burro.getwidth() << " pixels wide." << std::endl;
    std::cout << "Bit depth is " << burro.getbitdepth() << std::endl;
    std::cout << "Image gamma is: " << burro.getgamma() << std::endl;

    /* Cover the Donkey
     * We now draw a red circle over the donkey, just to prove that the image was read in correctly,
     * and that it was manipulated.
     * */
    burro.filledcircle((int)(burrowidth / 3.0), (int)(2.0 * burroheight / 3.0), (int)(burrowidth / 5), 65535, 0, 0);

    /* close()
     * Must close this instance to have it written to disk.
     * */
    std::cout << "Writing to disk...";
    burro.close();
    std::cout << " done." << std::endl;

    /////////////////////////////////////////////////////////////////////

    /* Rainbow
     * To demonstrate the use and usefulness of plotHSV(), we will
     * generate a rainbow.
     * We will use the function plotHSV that
     * plots a pixel at x, y with the colour given in the HSV colourspace
     * (hue, saturation, value), instead of the traditional RGB.
     * */

    std::cout << "\nCreating arcoiris.png...";
    pngwriter arcoiris(300, 300, 0, "arcoiris.png");
    std::cout << " done." << endl;
    std::cout << "Plotting...";
    for (int a = 1;a < 301;a++)
        for (int b = 1;b < 301;b++)
            arcoiris.plotHSV(a, b, double(a) / 300.0, double(b) / 300.0, 1.0);

    std::cout << " done." << endl;

    arcoiris.setgamma(0.5);
    std::cout << "Writing to disk...";
    arcoiris.close();
    std::cout << " done." << std::endl;
    /////////////////////////////////////////////////////////////////

    /* Random filled blended triangles.*/

    srand(time(NULL));

    std::cout << "\nCreating triangles.png...";
    pngwriter triangles(400, 400, 0, "triangles.png");
    std::cout << " done.";
    double opacityrange = 0.3;

    std::cout << "\nPlotting triangles...";
    for (int jj = 0; jj < 10; jj++)
    {

        //This creates the blue triangle outlines.
        triangles.triangle(
            ((int)rand() % 500) - 50,
            ((int)rand() % 500) - 50,
            ((int)rand() % 500) - 50,
            ((int)rand() % 500) - 50,
            ((int)rand() % 500) - 50,
            ((int)rand() % 500) - 50,
            0.0, 0.0, 1.0);
    }

    for (int ii = 0; ii < 20; ii++)
    {
        //This creates the red blended triangles
        triangles.filledtriangle_blend(
            ((int)rand() % 500) - 50,
            ((int)rand() % 500) - 50,
            ((int)rand() % 500) - 50,
            ((int)rand() % 500) - 50,
            ((int)rand() % 500) - 50,
            ((int)rand() % 500) - 50,
            0.4 + ((double)(rand() / RAND_MAX)) * opacityrange,
            1.0, 0.0, 0.0);
    }
    std::cout << " done.";
    std::cout << "\nWriting to disk...";
    triangles.close();
    std::cout << " done.";
    std::cout << "\n\npngtest has finished. Take a look at the PNG images that have been created!\n";
}