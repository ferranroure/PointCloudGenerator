#include <iostream>
#include <vector>
#include <unistd.h>
#include <stdio.h>  /* defines FILENAME_MAX */
#include "math.h"
#include "string"

#ifdef WINDOWS
#include <direct.h>
    #define GetCurrentDir _getcwd
#else
#include <unistd.h>
#include <fstream>

#define GetCurrentDir getcwd
#endif

#define RANDOM 0
#define DEATHSPIRAL 1
#define LEISURESPIRAL 2
#define SLOWSPIRAL 3

# define PI           3.14159265358979323846  /* pi */

using namespace std;

struct Point{

    double x;
    double y;
    double z;

    Point(double _x, double _y, double _z){
        x = _x;
        y = _y;
        z = _z;
    }
};

vector<Point> createPointCloud(int type, int nPoints, float diag,double tightness=5){

    srand(time(NULL));
    vector<Point> res;

    // find side size
    float side = diag / sqrt(2);

    if(type == RANDOM){

        for (int i = 0; i < nPoints; ++i) {

            double y = fmod(rand(), side) - side/2;
            double z = fmod(rand(), side) - side/2;
            double x = fmod(rand(), side) - side/2;

            res.push_back(Point(x, y, z));
        }

    }
    else if(type == DEATHSPIRAL) {

        for (int i = 0; i < nPoints; ++i) {

            // (cos(t),sin(t) represents a circle t goes from zero to 2PI
            // First determine where in the circle we are, parameter tightness controls the number of points per lap, the bigger the tightness, the more points per lap
            // then, determine the part of each lap we are in
            double t = fmod(i / tightness, 2 * PI);
            // Build the first two components
            double y = t * cos(t);
            double z = t * sin(t);
            // Now create the third component
            double x = side / (i + 1);

            res.push_back(Point(x, y, z));
        }
    }
    else if(type == LEISURESPIRAL)
    {
        for (int i = 0; i < nPoints; ++i) {
                // (cos(t),sin(t) represents a circle t goes from zero to 2PI
                // First determine where in the circle we are, parameter tightness controls the number of points per lap, the bigger the tightness, the more points per lap
                // then, determine the part of each lap we are in
                double t=fmod(i/tightness, 2*PI);
                // Build the first two components
                double y = t*cos(t);
                double z = t*sin(t);
                // Now create the third component
                double x = (i<=1 ? side/(i+1) : side/sqrt(i+3));

                res.push_back(Point(x, y, z));
        }


    }
    else if(type == SLOWSPIRAL)
    {
        for (int i = 0; i < nPoints; ++i) {

            // (cos(t),sin(t) represents a circle t goes from zero to 2PI
            // First determine where in the circle we are, parameter tightness controls the number of points per lap, the bigger the tightness, the more points per lap
            // then, determine the part of each lap we are in
            double t=fmod(i/tightness, 2*PI);
            // Build the first two components
            double y = t*cos(t);
            double z = t*sin(t);
            // Now create the third component
            double x = (i<=1 ? side/(i+1) : side/log(i+6));

            res.push_back(Point(x, y, z));
        }


    }
    return res;
}

void printCloud(vector<Point> &lin){

    for(vector<Point>::iterator it=lin.begin(); it!=lin.end(); ++it){

        cout << it->x << " " << it->y << " " << it->z << endl;
    }

}

void savePly(string out_path, vector<Point> &lin){

    ofstream outfile (out_path.c_str());

    // Printing the header
    outfile << "ply" << endl;
    outfile << "format ascii 1.0 " << endl;
    outfile << "element vertex " << lin.size() << endl;
    outfile << "property float32 x" << endl;
    outfile << "property float32 y" << endl;
    outfile << "property float32 z" << endl;
    outfile << "end_header" << endl;

    for(vector<Point>::iterator it=lin.begin(); it!=lin.end(); ++it){

        outfile << it->x << " " << it->y << " " << it->z << endl;
    }

    outfile.close();
}

int main() {

    // Getting current path
    char cCurrentPath[FILENAME_MAX];

    if (!GetCurrentDir(cCurrentPath, sizeof(cCurrentPath)))
    {
        return errno;
    }

    cCurrentPath[sizeof(cCurrentPath) - 1] = '\0'; /* not really required */


    // Vars
    int type = 0;
    vector<string> typenames;
    typenames.push_back("Random");
    typenames.push_back("Death Spiral");//linear descent in x
    typenames.push_back("Leisure Spiral"); // sqrt descent in x
    typenames.push_back("Slow Spiral)"); // logarithmic descent in x

    float diag = 0;
    int nPoints = 0;
    double tightness;
    string file;

    cout << endl;
    cout << "------------------------------------------------------------------------------------" << endl;
    cout << "                               POINT CLOUD GENERATOR" << endl;
    cout << "------------------------------------------------------------------------------------" << endl;
    cout << "I will create an amazing point cloud. Give me some parameters:" << endl;
    cout << "Choose the point cloud type: " << endl;
    cout << "[0] Random" << endl;
    cout << "[1] Fast Spiral (linear)" << endl;
    cout << "[2] Medium Spiral (square root)" << endl;
    cout << "[3] Low Spiral (logarithmic)" << endl;
    cin >> type;
    if(type!=0)
    {
        cout << "Insert tightness of the spiral" << endl;
         cin >> tightness;
    }
    cout << "How many points do you need?" << endl;
    cin >> nPoints;
    cout << "Which size do you want? Give me the diagonal distance:" << endl;
    cin >> diag;
    cout << "Ok, it's almost done. Give me the name of the output file:" << endl;
    cin >> file;
    cout << endl;
    cout << "========================> Generating Point Cloud..." << endl;
    cout << " Type: " << typenames[type] << endl;
    cout << " # of points: " << nPoints << endl;
    cout << " Diagonal: " << diag << endl;
    cout << " File name: " << file << endl;
    if(type!=0)
    {
        cout << " Tightness of the spiral " <<tightness<<endl;
    }


    vector<Point> cloud = createPointCloud(type, nPoints, diag,tightness);

    cout << "========================> Saving file..." << endl;
    cout << endl;

    string path = string(cCurrentPath) + "/" + file;
    savePly(path, cloud);
    //printCloud(cloud);

    cout << "Good! Your point cloud has been created and saved in:" << endl;
    cout << cCurrentPath << "/" << file << endl;

    return 0;
}

