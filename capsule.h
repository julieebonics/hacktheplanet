
// Ensure the header file is included only once in multi-file projects
#ifndef CAPSULE_H
#define CAPSULE_H

// Include needed header files
# include <vector>
# include <gsim/gs_color.h>
# include <gsim/gs_vec.h>
# include "ogl_tools.h"
# include "light.h"
# include "material.h"
# include <gsim/gs_image.h>


// Scene objects should be implemented in their own classes; and
// here is an example of how to organize a scene object in a class.
// Scene object axis:
class capsule : public GlObjects
{
private:
	std::vector<GsVec>   P; // coordinates
	std::vector<GsVec2> T; // texture
	std::vector<GsVec> N; //normals
	int _numpoints;   // saves the number of points
	gsuint _texid, _texid2;
	
public:
	capsule();
	void init(const GlProgram& prog);
	void build(float len, float rt, float rb, int nfaces, bool smooth);
	void draw(const GsMat& tr, const GsMat& pr, Light& l,  Material& m);
	bool instead = true;
	float dr = .3;
};


#endif // SO_AXIS_H
#pragma once
