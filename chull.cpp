#include<bits/stdc++.h>
using namespace std;

// struct for storing a point in 3-d space
struct Point {
    long double x,y,z;
    //constructor
    Point(long double x=0,long double y=0,long double z=0) {
        this->x =x ;
        this->y =y ;
        this->z =z ;
    }
    // operator overloading for the subtraction operator to perform subtraction of two points
    Point operator-(const Point &p) const {
        return Point(x - p.x, y - p.y, z - p.z);
    }
    // function for finding the cross product of two points(p is the point passed in the function)
    Point cross(const Point &p) const {
        return Point(y * p.z - z * p.y, z * p.x - x * p.z, x * p.y - y * p.x);
    }
    // function for finding the dot product of two points(p is the point passed in the function)
    long double dot(const Point &p) const {
        return x * p.x + y * p.y + z * p.z;
    }
};

// face is always considered a triangle
struct Face {
    // indices of the three vertices of the triangle in the points array
    int a,b,c;
    // outward facing normal of the triangle
    Point n;
};

// function for adding a face to the vector of faces
void addFace(vector<Face> &f,int a,int b,int c,vector<vector<bool>> &horizon,const vector<Point> &p){
    f.push_back({a,b,c,(p[b]-p[a]).cross(p[c]-p[a])});
    //edges part of a face are marked false in a counter-clockwise direction
    horizon[a][b] = false;
    horizon[b][c] = false;
    horizon[c][a] = false;
}

// returns the convex hull of a set of points
vector<Face> convexHull(const vector<Point> &p){
    int n = p.size();
    vector<Face> faces;
    // for determining the edges which are part of the horizon
    vector<vector<bool>> horizon(n,vector<bool>(n,true));

    // initially the convex hull of three points is considered to be two triangles of opposite orientations 
    addFace(faces,0,1,2,horizon,p);
    addFace(faces,0,2,1,horizon,p);

    for(int i=3;i<n;i++){
        vector<Face> invisibleFaces;
        for(Face &face: faces){
            if((p[i]-p[face.a]).dot(face.n) > 0){
                //face is visible to point p[i], hence mark the edges as true
                horizon[face.a][face.b] = horizon[face.b][face.c] = horizon[face.c][face.a] = true;
            }
            else{
                invisibleFaces.push_back(face);
            }
        }

        //reconstruct convex hull
        // We get to know the edges which are a part of the horizon because the visible edges have been marked true
        // All the edges of the visible faces would have been marked true
        faces.clear();
        for(Face &face: invisibleFaces){
            int arr[3] = {face.a,face.b,face.c};
            for(int j=0;j<3;j++){
                int a = arr[j];
                int b = arr[(j+1)%3];
                //add new faces connecting point to edges which are part of the horizon
                // When iterating through the invisible faces, if some edge has been marked
                // true in the opposite direction, it implies that the edge is a part of a
                // visible face too and hence lies on the horizon
                if(horizon[b][a]){
                    addFace(faces,b,a,i,horizon,p);
                }
            }
        }

        //add invisible faces back to the convex hull
        faces.insert(faces.end(),invisibleFaces.begin(),invisibleFaces.end());
    }

    return faces;
}

// Remove duplicate points from a vector of points
vector<Point> removeDuplicates(vector<Point>& inputVector) {
    vector<Point> uniquePoints;

    for (const Point& p : inputVector) {
        bool isDuplicate = false;
        for (const Point& q : uniquePoints) {
            if (p.x == q.x && p.y == q.y && p.z == q.z) {
                isDuplicate = true;
                break;
            }
        }
        if (!isDuplicate) {
            uniquePoints.push_back(p);
        }
    }

    return uniquePoints;
}

int main(){
    vector<Point> points;

    //open the input file
    ifstream inputFile("convex.in");

    if (!inputFile) {
        cerr << "Failed to open input file." << endl;
        return 1;
    }

    int n;
    inputFile >> n; // Read the number of points from the first line

    for (int i = 0; i < n; ++i) {
        long double x, y, z;
        inputFile >> x >> y >> z;
        points.push_back(Point(x, y, z));
    }

    inputFile.close(); // Close the input file

    // Compute the convex hull
    vector<Face> faces = convexHull(points);
    vector<Point> s;
    for (const Face& f : faces) {
        s.push_back(points[f.a]);
        s.push_back(points[f.b]);
        s.push_back(points[f.c]);
    }

    vector<Point> chull = removeDuplicates(s);

    // Open the output file for writing
    ofstream outputFile("convex.out");

    if (!outputFile) {
        cerr << "Failed to open output file." << endl;
        return 1;
    }

    int k = chull.size(); // Number of points in the convex hull

    // Write the number of points in the convex hull to the output file
    outputFile << k << endl;

    // Write the convex hull points to the output file
    for (const Point& p : chull) {
        outputFile << p.x << " " << p.y << " " << p.z << endl;
    }

    outputFile.close(); // Close the output file
    return 0;
}
