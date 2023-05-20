#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<sstream>
#include <boost/algorithm/string/find.hpp>
#include<cctype>
#include<iomanip>

using namespace std; fet
using namespace boost;

struct Point
{
    double x;
    double y;
    int cluster_id;
    double min_dist;

    Point():
        x(0.0),
        y(0.0),
        cluster_id(-1),
        min_dist(__DBL_MAX__) {}


    Point(double x, double y):
        x(x),
        y(y),
        cluster_id(-1),
        min_dist(__DBL_MAX__) {}

    double distance(Point p)
    {
        return (p.x - x)*(p.x - x) + (p.y-  y)*(p.y - y);
    }
  
};


struct revisor
{
    double x_sum;
    double y_sum;
    int length;

    revisor():
        x_sum(0.0),
        y_sum(0.0),
        length(0) {}

};


int main()
{


    std::string line;


    vector<Point>points;
    // vector<float>point;
    const std::string file_name = "Mall_Customers.csv";
    std::ifstream file(file_name);
    float x,y;
    getline(file,line); //header line
    while( std::getline( file, line ) ) 
    {
        iterator_range<string::iterator> r1 = find_nth(line,",",2);
        iterator_range<string::iterator> r2 = find_nth(line,",",4);
        string temp2;
        stringstream temp(line.substr(std::distance(line.begin(),r1.begin() ) + 1,r2.begin()-r1.begin()-1));
        vector<string>result;
        while(!temp.eof())
        {
            getline(temp,temp2,',');
            result.push_back(temp2);
        }
       
        y = stof(result.back());
        result.pop_back();
        x = stof(result.back());
       
        points.push_back(Point(x,y));
       
    }

 

    //all points in points
    //kmeans:
    vector<Point>centroids;
    int k = 5;
    int n = 10;

    // srand(time(0));
    // for(int i=0;i<k;i++)
    // {
    //     centroids.push_back(points[rand() % n]);
    // }

    centroids = {Point(25,20),Point(25,80),Point(60,50),Point(100,20),Point(100,80)};

    for(auto it: centroids)
    {
        cout << it.x << " " <<  it.y << endl;
    }


    
    for(int i=0;i<n;i++)
    {
        //Step 1: Assign cluster ids to each point    
        for(vector<Point>::iterator c = centroids.begin(); c != centroids.end() ; c++)
        {
            int cid = c - centroids.begin();

            for(vector<Point>::iterator p = points.begin() ; p != points.end() ; p++)
            {
                double dist_to_c = (*p).distance(*c);
                if(dist_to_c < (*p).min_dist)
                {
                    (*p).min_dist = dist_to_c;
                    (*p).cluster_id = cid;
                }
            }
        }

        //Step2: Relocate centroids
        revisor centroid_relocator[k];
        for(vector<Point>::iterator p = points.begin(); p != points.end(); p++)
        {
            int cluster_ind = (*p).cluster_id;
            centroid_relocator[cluster_ind].x_sum += (*p).x;
            centroid_relocator[cluster_ind].y_sum += (*p).y;
            centroid_relocator[cluster_ind].length += 1;
            
        }
        
        for(vector<Point>::iterator c = centroids.begin(); c != centroids.end() ; c++)
        {
            double x_coord = centroid_relocator[c-centroids.begin()].x_sum/centroid_relocator[c-centroids.begin()].length;
            double y_coord = centroid_relocator[c-centroids.begin()].y_sum/centroid_relocator[c-centroids.begin()].length;
            
            centroids[c-centroids.begin()].x = x_coord;
            centroids[c-centroids.begin()].y = y_coord;
        }

    }

    //now have cluster_id of all the points
    //write out xcoord, ycooord and cluster_id of all the points to a file, then visualize in python

    std::ofstream outfile;
    outfile.open("out_file.csv");
    outfile << "x,y,c\n";

    for(vector<Point>::iterator p = points.begin(); p != points.end(); p++)
    {
        outfile << (*p).x << "," << (*p).y << "," << (*p).cluster_id << endl; 
    }

    return 0;
}