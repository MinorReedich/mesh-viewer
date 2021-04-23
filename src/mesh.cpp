
#include "mesh.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;
using namespace glm;
using namespace agl;

Mesh::Mesh() 
{
}

Mesh::~Mesh()
{
   delete[] normal;
   delete[] position;
   delete[] vertices;
   delete[] index;
}

bool Mesh::loadPLY(const std::string& filename)
{
   string trash;
   string ply;
   
   ifstream file(filename);
   if (!file) // true if the file is valid
   {
	   cout << "Cannot load file: " << filename << endl;
	   return false;
   }
   file >> ply;
   if(ply!="ply") return false; 

   // skip initial comments etc.
   for(int i = 0; i<17; i++){
      file >> trash;
   }
   file >> numVertex;

   // 3 property lines and descriptor for numPolygons
   for(int i = 0; i<(9+2); i++){
      file >> trash;
   }
   file >> numPolygons;

   // skip property line and end_header
   for(int i = 0; i<(5+1); i++){
      file >> trash;
   } 

   vertices = new float[numVertex*3];
   normal = new float[numPolygons*3*3];
   for(int i = 0; i<numVertex*3; i+=3){
      int x,y,z,nx,ny,nz;
      file >> x >> y >> z;
      file >> nx >> ny >> nz;
      if(x<minX) minX = x;
      else if(x>maxX) maxX = x;
      if(y<minY) minY = y;
      else if(y>maxY) maxY = y;
      if(z<minZ) minZ = z;
      else if(z>maxZ) maxZ = z;
      vertices[i]=x;
      vertices[i+1]=y;
      vertices[i+2]=z;
      normal[i]=nx;
      normal[i+1]=ny;
      normal[i+2]=nz;
   }  
   index = new unsigned int[numPolygons];
   for(int i = 0; i<numPolygons; i++){
      index[i] = i;
   }
   position = new float[numPolygons*3*3];
   for(int i = 0; i<numPolygons*3*3; i+=9){
      file >> trash;
      int v1,v2,v3;
      file >> v1 >> v2 >> v3;
      position[i] = vertices[v1*3];
      position[i+1] = vertices[v1*3+1];
      position[i+2] = vertices[v1*3+2];
      position[i+3] = vertices[v2*3]; 
      position[i+4] = vertices[v2*3+1];
      position[i+5] = vertices[v2*3+2];
      position[i+6] = vertices[v3*3];
      position[i+7] = vertices[v3*3+1];
      position[i+8] = vertices[v3*3+2];
   }
   
   /*for(int i = 0; i<numPolygons*3*3; i+=9){
      vec3 p1 = vec3(position[i], position[i+1], position[i+2]);
      vec3 p2 = vec3(position[i+3], position[i+4], position[i+5]);
      vec3 p3 = vec3(position[i+6], position[i+7], position[i+8]);
      vec3 cross = crossProduct(p1,p2,p3); 
      normal[i]=cross.x;
      normal[i+1]=cross.y;
      normal[i+2]=cross.z;
      normal[i+3]=cross.x;
      normal[i+4]=cross.y;
      normal[i+5]=cross.z;
      normal[i+6]=cross.x;
      normal[i+7]=cross.y;
      normal[i+8]=cross.z; 
   }*/
   return true;
}

glm::vec3 Mesh::crossProduct(vec3 p1, vec3 p2, vec3 p3){
   vec3 a = vec3(p1.x-p2.x, p1.y-p2.y, p1.z-p2.z);
   vec3 b = vec3(p1.x-p3.x, p1.y-p3.y, p1.z-p3.z); 
   float x = a.y*b.z - a.z*b.y;
   float y = a.z*b.x - a.x*b.z;
   float z = a.x*b.y - a.y*b.x;
   return vec3(x,y,z);
}

glm::vec3 Mesh::getMinBounds() const
{
  return vec3(minX, minY, minZ);
}

glm::vec3 Mesh::getMaxBounds() const
{
  return vec3(maxX, maxY, maxZ);
}

int Mesh::numVertices() const
{
   return numVertex;
}

int Mesh::numTriangles() const
{
   return numPolygons;
}

float* Mesh::positions() const
{
   return position;
}

float* Mesh::normals() const
{
   return normal;
}

unsigned int* Mesh::indices() const
{
   return index;
}

void Mesh::setVertex(float v, int index) const
{
   position[index] = v;
}