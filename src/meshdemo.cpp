#include "AGL.h"
#include "AGLM.h"
#include "mesh.h"

using namespace std;
using namespace glm;
using namespace agl;

int main(int argc, char** argv)
{
   Mesh m;
   m.loadPLY("../models/airplane.py");
   for(int i = 0; i<200; i++){
      m.setVertex(random()%2000, round(random()%m.numVertices()));
   }
   return 0;
}


