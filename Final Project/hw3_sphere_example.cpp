//
//  main.cpp
//  This example just shows how 
//
//  Created by Rafael Radkowski on 5/28/15.
//  Copyright (c) 2015 -. All rights reserved.
//

// stl include
#include <iostream>
#include <string>

// GLEW include
#include <GL/glew.h>

// GLM include files
#define GLM_FORCE_INLINE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


// glfw includes
#include <GLFW/glfw3.h>


// include local files
#include "controls.h"
#include "HCI557Common.h"
#include "CoordSystem.h"
#include "ImgLoader.h"
#include "ShaderFileUtils.h"
#include "Shaders.h"




////////////////////////////////////////////////////////////////
// Include file for the sphere functions
#include "make_sphere.h"
//#include "../gl_common/GLAppearance.h"




// this line tells the compiler to use the namespace std.
// Each object, command without a namespace is assumed to be part of std. 
using namespace std;

// Fragment shader source code. This determines the colors in the fragment generated in the shader pipeline. In this case, it colors the inside of our triangle specified by our vertex shader.                                                                \n";




/// Camera control matrices
glm::mat4 projectionMatrix; // Store the projection matrix
glm::mat4 viewMatrix; // Store the view matrix
glm::mat4 inverseViewMatrix;
glm::mat4 modelMatrix; // Store the model matrix




// The handle to the window object
GLFWwindow*         window;


// Define some of the global variables we're using for this sample
GLuint program;


//////////////////////////////////////////////////////////////////////////
// NUMBER OF VERTICES FOR A SPEHERE
int number_vertices = -1;
int number_table_vt = -1;
int number_lights = 0;
float humerus = 1.35;
float ulna = 1.10;
float shoulder = 4.675 - 3.0;

int faults = 0;
float time_count = 0.0;
int score = 0;

string light_struct = "all_lights";
string light_names[8] = { "pos", "diffuse", "ambient", "specular", "coeff", "angle", "direction", "cutoff" };


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// Fill this functions with your model code.

// USE THESE vertex array objects to define your objects
unsigned int vaoID[4];

unsigned int vboID[12];


/*!
Searches for the file in name.
Returns the new path in new_path.
*/

/*!
 ADD YOUR CODE TO CREATE THE TRIANGLE STRIP MODEL TO THIS FUNCTION
 */
glm::vec3 surface_normal(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3){
	float nx = ((y2 - y1) * (z3 - z1)) - ((z2 - z1) * (y3 - y1));
	float ny = ((z2 - z1) * (x3 - x1)) - ((x2 - x1) * (z3 - z1));
	float nz = ((x2 - x1) * (y3 - y1)) - ((y2 - y1) * (x3 - x1));
	glm::vec3 output(nx, ny, nz);

	return glm::normalize(output);
}

void mk_tri(float* pps, float* pns, int index, glm::mat3x3 data){
	pps[index + 0] = data[0][0]; pps[index + 1] = data[0][1];	pps[index + 2] = data[0][2];
	pps[index + 3] = data[1][0]; pps[index + 4] = data[1][1];	pps[index + 5] = data[1][2];
	pps[index + 6] = data[2][0]; pps[index + 7] = data[2][1];	pps[index + 8] = data[2][2];
	glm::vec3 ntemp = surface_normal(pps[index + 0], pps[index + 1], pps[index + 2], pps[index + 3], pps[index + 4], pps[index + 5], pps[index + 6], pps[index + 7], pps[index + 8]);
	pns[index + 0] = ntemp[0];	pns[index + 1] = ntemp[1];	pns[index + 2] = ntemp[2];
	pns[index + 3] = ntemp[0];	pns[index + 4] = ntemp[1];	pns[index + 5] = ntemp[2];
	pns[index + 6] = ntemp[0];	pns[index + 7] = ntemp[1];	pns[index + 8] = ntemp[2];//183
}

unsigned int createPaddleGeometry(){
	
	int num_points = 408;
	//#NPTS

	float* pps = new float[num_points];
	float* pns = new float[num_points];
	float* tex = new float[(num_points/3) * 2];

	float depth = 0.015;
	float norm = 1.0;
	float dim_size = 0.56;

	int index = 0;
	int tex_idx = 0;

	float tiersx[7] = {0.125,0.22,0.235,0.25,0.230,0.185,0.05};
	float tiersy[7] = {0.0, 0.10, 0.18, 0.28, 0.37, 0.48, 0.56};
	float center[2] = { 0.0, 0.05 };
	


	for (int rounds = 0; rounds < 2; rounds++){
		pps[index + 0] = center[0];		pps[index + 1] = center[1];		pps[index + 2] = depth;
		pns[index + 0] = 0.0;	pns[index + 1] = 0.0;	pns[index + 2] = norm;
		tex[tex_idx + 0] = (pps[index + 0] + (dim_size / 2)) / dim_size; tex[tex_idx + 1] = pps[index + 1] / dim_size;
		index += 3; tex_idx += 2;
		pps[index + 0] = tiersx[0];	pps[index + 1] = tiersy[0];	pps[index + 2] = depth;
		pns[index + 0] = 0.0;	pns[index + 1] = 0.0;	pns[index + 2] = norm;
		tex[tex_idx + 0] = (pps[index + 0] + (dim_size / 2)) / dim_size; tex[tex_idx + 1] = pps[index + 1] / dim_size;
		index += 3; tex_idx += 2;
		pps[index + 0] = tiersx[1];	pps[index + 1] = tiersy[1];	pps[index + 2] = depth;
		pns[index + 0] = 0.0;	pns[index + 1] = 0.0;	pns[index + 2] = norm;
		tex[tex_idx + 0] = (pps[index + 0] + (dim_size / 2)) / dim_size; tex[tex_idx + 1] = pps[index + 1] / dim_size;
		index += 3; tex_idx += 2;
		pps[index + 0] = tiersx[2];	pps[index + 1] = tiersy[2];	pps[index + 2] = depth;
		pns[index + 0] = 0.0;	pns[index + 1] = 0.0;	pns[index + 2] = norm;
		tex[tex_idx + 0] = (pps[index + 0] + (dim_size / 2)) / dim_size; tex[tex_idx + 1] = pps[index + 1] / dim_size;
		index += 3; tex_idx += 2;
		pps[index + 0] = tiersx[3];	pps[index + 1] = tiersy[3];	pps[index + 2] = depth;
		pns[index + 0] = 0.0;	pns[index + 1] = 0.0;	pns[index + 2] = norm;
		tex[tex_idx + 0] = (pps[index + 0] + (dim_size / 2)) / dim_size; tex[tex_idx + 1] = pps[index + 1] / dim_size;
		index += 3; tex_idx += 2;
		pps[index + 0] = tiersx[4];	pps[index + 1] = tiersy[4];	pps[index + 2] = depth;
		pns[index + 0] = 0.0;	pns[index + 1] = 0.0;	pns[index + 2] = norm;
		tex[tex_idx + 0] = (pps[index + 0] + (dim_size / 2)) / dim_size; tex[tex_idx + 1] = pps[index + 1] / dim_size;
		index += 3; tex_idx += 2;
		pps[index + 0] = tiersx[5];	pps[index + 1] = tiersy[5];	pps[index + 2] = depth;
		pns[index + 0] = 0.0;	pns[index + 1] = 0.0;	pns[index + 2] = norm;
		tex[tex_idx + 0] = (pps[index + 0] + (dim_size / 2)) / dim_size; tex[tex_idx + 1] = pps[index + 1] / dim_size;
		index += 3; tex_idx += 2;
		pps[index + 0] = tiersx[6];	pps[index + 1] = tiersy[6];	pps[index + 2] = depth;
		pns[index + 0] = 0.0;	pns[index + 1] = 0.0;	pns[index + 2] = norm;
		tex[tex_idx + 0] = (pps[index + 0] + (dim_size / 2)) / dim_size; tex[tex_idx + 1] = pps[index + 1] / dim_size;
		index += 3; tex_idx += 2;
		pps[index + 0] = -tiersx[6];	pps[index + 1] = tiersy[6];	pps[index + 2] = depth;
		pns[index + 0] = 0.0;	pns[index + 1] = 0.0;	pns[index + 2] = norm;
		tex[tex_idx + 0] = (pps[index + 0] + (dim_size / 2)) / dim_size; tex[tex_idx + 1] = pps[index + 1] / dim_size;
		index += 3; tex_idx += 2;
		pps[index + 0] = -tiersx[5];	pps[index + 1] = tiersy[5];	pps[index + 2] = depth;
		pns[index + 0] = 0.0;	pns[index + 1] = 0.0;	pns[index + 2] = norm;
		tex[tex_idx + 0] = (pps[index + 0] + (dim_size / 2)) / dim_size; tex[tex_idx + 1] = pps[index + 1] / dim_size;
		index += 3; tex_idx += 2;
		pps[index + 0] = -tiersx[4];	pps[index + 1] = tiersy[4];	pps[index + 2] = depth;
		pns[index + 0] = 0.0;	pns[index + 1] = 0.0;	pns[index + 2] = norm;
		tex[tex_idx + 0] = (pps[index + 0] + (dim_size / 2)) / dim_size; tex[tex_idx + 1] = pps[index + 1] / dim_size;
		index += 3; tex_idx += 2;
		pps[index + 0] = -tiersx[3];	pps[index + 1] = tiersy[3];	pps[index + 2] = depth;
		pns[index + 0] = 0.0;	pns[index + 1] = 0.0;	pns[index + 2] = norm;
		tex[tex_idx + 0] = (pps[index + 0] + (dim_size / 2)) / dim_size; tex[tex_idx + 1] = pps[index + 1] / dim_size;
		index += 3; tex_idx += 2;
		pps[index + 0] = -tiersx[2];	pps[index + 1] = tiersy[2];	pps[index + 2] = depth;
		pns[index + 0] = 0.0;	pns[index + 1] = 0.0;	pns[index + 2] = norm;
		tex[tex_idx + 0] = (pps[index + 0] + (dim_size / 2)) / dim_size; tex[tex_idx + 1] = pps[index + 1] / dim_size;
		index += 3; tex_idx += 2;
		pps[index + 0] = -tiersx[1];	pps[index + 1] = tiersy[1];	pps[index + 2] = depth;
		pns[index + 0] = 0.0;	pns[index + 1] = 0.0;	pns[index + 2] = norm;
		tex[tex_idx + 0] = (pps[index + 0] + (dim_size / 2)) / dim_size; tex[tex_idx + 1] = pps[index + 1] / dim_size;
		index += 3; tex_idx += 2;
		pps[index + 0] = -tiersx[0];	pps[index + 1] = tiersy[0];	pps[index + 2] = depth;
		pns[index + 0] = 0.0;	pns[index + 1] = 0.0;	pns[index + 2] = norm;
		tex[tex_idx + 0] = (pps[index + 0] + (dim_size / 2)) / dim_size; tex[tex_idx + 1] = pps[index + 1] / dim_size;
		index += 3; tex_idx += 2;
		depth *= -1;
		norm *= -1;
	}
	//90
	float mid[2] = { 0.0, 0.28 };

	pps[index + 0] = tiersx[0];	pps[index + 1] = tiersy[0];	pps[index + 2] = depth;
	pns[index + 0] = tiersx[0];	pns[index + 1] = (tiersy[0] - mid[1]);	pns[index + 2] = 0.0;
	index += 3;
	pps[index + 0] = tiersx[0];	pps[index + 1] = tiersy[0];	pps[index + 2] = -depth;
	pns[index + 0] = tiersx[0];	pns[index + 1] = (tiersy[0] - mid[1]);	pns[index + 2] = 0.0;
	index += 3;
	pps[index + 0] = tiersx[1];	pps[index + 1] = tiersy[1];	pps[index + 2] = depth;
	pns[index + 0] = tiersx[1];	pns[index + 1] = (tiersy[1] - mid[1]);	pns[index + 2] = 0.0;
	index += 3;
	pps[index + 0] = tiersx[1];	pps[index + 1] = tiersy[1];	pps[index + 2] = -depth;
	pns[index + 0] = tiersx[1];	pns[index + 1] = (tiersy[1] - mid[1]);	pns[index + 2] = 0.0;
	index += 3;
	pps[index + 0] = tiersx[2];	pps[index + 1] = tiersy[2];	pps[index + 2] = depth;
	pns[index + 0] = tiersx[2];	pns[index + 1] = (tiersy[2] - mid[1]);	pns[index + 2] = 0.0;
	index += 3;
	pps[index + 0] = tiersx[2];	pps[index + 1] = tiersy[2];	pps[index + 2] = -depth;
	pns[index + 0] = tiersx[2];	pns[index + 1] = (tiersy[2] - mid[1]);	pns[index + 2] = 0.0;
	index += 3;
	pps[index + 0] = tiersx[3];	pps[index + 1] = tiersy[3];	pps[index + 2] = depth;
	pns[index + 0] = tiersx[3];	pns[index + 1] = (tiersy[3] - mid[1]);	pns[index + 2] = 0.0;
	index += 3;
	pps[index + 0] = tiersx[3];	pps[index + 1] = tiersy[3];	pps[index + 2] = -depth;
	pns[index + 0] = tiersx[3];	pns[index + 1] = (tiersy[3] - mid[1]);	pns[index + 2] = 0.0;
	index += 3;
	pps[index + 0] = tiersx[4];	pps[index + 1] = tiersy[4];	pps[index + 2] = depth;
	pns[index + 0] = tiersx[4];	pns[index + 1] = (tiersy[4] - mid[1]);	pns[index + 2] = 0.0;
	index += 3;
	pps[index + 0] = tiersx[4];	pps[index + 1] = tiersy[4];	pps[index + 2] = -depth;
	pns[index + 0] = tiersx[4];	pns[index + 1] = (tiersy[4] - mid[1]);	pns[index + 2] = 0.0;
	index += 3;
	pps[index + 0] = tiersx[5];	pps[index + 1] = tiersy[5];	pps[index + 2] = depth;
	pns[index + 0] = tiersx[5];	pns[index + 1] = (tiersy[5] - mid[1]);	pns[index + 2] = 0.0;
	index += 3;
	pps[index + 0] = tiersx[5];	pps[index + 1] = tiersy[5];	pps[index + 2] = -depth;
	pns[index + 0] = tiersx[5];	pns[index + 1] = (tiersy[5] - mid[1]);	pns[index + 2] = 0.0;
	index += 3;
	pps[index + 0] = tiersx[6];	pps[index + 1] = tiersy[6];	pps[index + 2] = depth;
	pns[index + 0] = tiersx[6];	pns[index + 1] = (tiersy[6] - mid[1]);	pns[index + 2] = 0.0;
	index += 3;
	pps[index + 0] = tiersx[6];	pps[index + 1] = tiersy[6];	pps[index + 2] = -depth;
	pns[index + 0] = tiersx[6];	pns[index + 1] = (tiersy[6] - mid[1]);	pns[index + 2] = 0.0;
	index += 3;
	pps[index + 0] = -tiersx[6];	pps[index + 1] = tiersy[6];	pps[index + 2] = depth;
	pns[index + 0] = -tiersx[6];	pns[index + 1] = (tiersy[6] - mid[1]);	pns[index + 2] = 0.0;
	index += 3;
	pps[index + 0] = -tiersx[6];	pps[index + 1] = tiersy[6];	pps[index + 2] = -depth;
	pns[index + 0] = -tiersx[6];	pns[index + 1] = (tiersy[6] - mid[1]);	pns[index + 2] = 0.0;
	index += 3;
	pps[index + 0] = -tiersx[5];	pps[index + 1] = tiersy[5];	pps[index + 2] = depth;
	pns[index + 0] = -tiersx[5];	pns[index + 1] = (tiersy[5] - mid[1]);	pns[index + 2] = 0.0;
	index += 3;
	pps[index + 0] = -tiersx[5];	pps[index + 1] = tiersy[5];	pps[index + 2] = -depth;
	pns[index + 0] = -tiersx[5];	pns[index + 1] = (tiersy[5] - mid[1]);	pns[index + 2] = 0.0;
	index += 3;
	pps[index + 0] = -tiersx[4];	pps[index + 1] = tiersy[4];	pps[index + 2] = depth;
	pns[index + 0] = -tiersx[4];	pns[index + 1] = (tiersy[4] - mid[1]);	pns[index + 2] = 0.0;
	index += 3;
	pps[index + 0] = -tiersx[4];	pps[index + 1] = tiersy[4];	pps[index + 2] = -depth;
	pns[index + 0] = -tiersx[4];	pns[index + 1] = (tiersy[4] - mid[1]);	pns[index + 2] = 0.0;
	index += 3;
	pps[index + 0] = -tiersx[3];	pps[index + 1] = tiersy[3];	pps[index + 2] = depth;
	pns[index + 0] = -tiersx[3];	pns[index + 1] = (tiersy[3] - mid[1]);	pns[index + 2] = 0.0;
	index += 3;
	pps[index + 0] = -tiersx[3];	pps[index + 1] = tiersy[3];	pps[index + 2] = -depth;
	pns[index + 0] = -tiersx[3];	pns[index + 1] = (tiersy[3] - mid[1]);	pns[index + 2] = 0.0;
	index += 3;
	pps[index + 0] = -tiersx[2];	pps[index + 1] = tiersy[2];	pps[index + 2] = depth;
	pns[index + 0] = -tiersx[2];	pns[index + 1] = (tiersy[2] - mid[1]);	pns[index + 2] = 0.0;
	index += 3;
	pps[index + 0] = -tiersx[2];	pps[index + 1] = tiersy[2];	pps[index + 2] = -depth;
	pns[index + 0] = -tiersx[2];	pns[index + 1] = (tiersy[2] - mid[1]);	pns[index + 2] = 0.0;
	index += 3;
	pps[index + 0] = -tiersx[1];	pps[index + 1] = tiersy[1];	pps[index + 2] = depth;
	pns[index + 0] = -tiersx[1];	pns[index + 1] = (tiersy[1] - mid[1]);	pns[index + 2] = 0.0;
	index += 3;
	pps[index + 0] = -tiersx[1];	pps[index + 1] = tiersy[1];	pps[index + 2] = -depth;
	pns[index + 0] = -tiersx[1];	pns[index + 1] = (tiersy[1] - mid[1]);	pns[index + 2] = 0.0;
	index += 3;
	pps[index + 0] = -tiersx[0];	pps[index + 1] = tiersy[0];	pps[index + 2] = depth;
	pns[index + 0] = -tiersx[0];	pns[index + 1] = (tiersy[0] - mid[1]);	pns[index + 2] = 0.0;
	index += 3;
	pps[index + 0] = -tiersx[0];	pps[index + 1] = tiersy[0];	pps[index + 2] = -depth;
	pns[index + 0] = -tiersx[0];	pns[index + 1] = (tiersy[0] - mid[1]);	pns[index + 2] = 0.0;
	index += 3;//174

	glm::vec3 ntemp;	//MTRI

	mk_tri(pps, pns, index, glm::mat3x3(
		-tiersx[0], tiersy[0], depth,
		tiersx[0], tiersy[0], -depth,
		tiersx[0], tiersy[0], depth));
	index += 9;

	mk_tri(pps, pns, index, glm::mat3x3(	//Bottom of Paddle wood
		-tiersx[0], tiersy[0], depth,
		-tiersx[0], tiersy[0], -depth,
		tiersx[0], tiersy[0], -depth));
	index += 9;	

	mk_tri(pps, pns, index, glm::mat3x3(	//Front paddle triangle
		tiersx[0],	tiersy[0],	depth,
		0.0,		0.05,		depth,
		-tiersx[0],	tiersy[0],	depth));
	index += 9;

	mk_tri(pps, pns, index, glm::mat3x3(	//Front handle triangle
		3 * depth, -0.01, 2 * depth,
		center[0], center[1], depth,
		-3 * depth, -0.01, 2 * depth));
	index += 9;

	mk_tri(pps, pns, index, glm::mat3x3(	//Front handle 1
		-3 * depth, -0.31, 2 * depth,
		3 * depth, -0.01, 2 * depth,
		-3 * depth, -0.01, 2 * depth));
	index += 9;

	mk_tri(pps, pns, index, glm::mat3x3(	//Front handle 2
		-3 * depth, -0.31, 2 * depth,
		3 * depth, -0.31, 2 * depth,
		3 * depth, -0.01, 2 * depth));
	index += 9;	//210

	mk_tri(pps, pns, index, glm::mat3x3(	//Front left handle triangle
		-4 * depth, 0.0, depth,
		-3 * depth, -0.01, 2 * depth,
		center[0], center[1], depth));
	index += 9;	//219

	mk_tri(pps, pns, index, glm::mat3x3(	//Front right handle triangle
		center[0], center[1], depth,
		3 * depth, -0.01, 2 * depth,
		4 * depth, 0.0, depth));
	index += 9;	//228

	mk_tri(pps, pns, index, glm::mat3x3(	//Back handle triangle
		-3 * depth, -0.01, -2 * depth,
		center[0], center[1], -depth,
		3 * depth, -0.01, -2 * depth));
	index += 9;	//264

	mk_tri(pps, pns, index, glm::mat3x3(	//Front left handle 1
		-3 * depth, -0.31, 2 * depth,
		-3 * depth, -0.01, 2 * depth,
		-4 * depth, 0.0, depth));
	index += 9;	//273 

	mk_tri(pps, pns, index, glm::mat3x3(	//Front left handle 2
		-4 * depth, 0.0, depth,
		-4 * depth, -0.31, depth,
		-3 * depth, -0.31, 2 * depth));
	index += 9;	//282

	mk_tri(pps, pns, index, glm::mat3x3(	//Front right handle 1
		3 * depth, -0.01, 2 * depth,
		3 * depth, -0.31, 2 * depth,
		4 * depth, 0.0, depth));
	index += 9;	//291 

	mk_tri(pps, pns, index, glm::mat3x3(	//Front right handle 2
		4 * depth, -0.31, depth,
		4 * depth, 0.0, depth,
		3 * depth, -0.31, 2 * depth));
	index += 9;

	mk_tri(pps, pns, index, glm::mat3x3(	//Back handle 1
		-3 * depth, -0.01, -2 * depth,
		3 * depth, -0.01, -2 * depth,
		-3 * depth, -0.31, -2 * depth));
	index += 9;

	mk_tri(pps, pns, index, glm::mat3x3(	//Back handle 2
		3 * depth, -0.01, -2 * depth,
		3 * depth, -0.31, -2 * depth,
		-3 * depth, -0.31, -2 * depth));
	index += 9;

	mk_tri(pps, pns, index, glm::mat3x3(	//Back paddle triangle
		-tiersx[0], tiersy[0], -depth,
		0.0, 0.05, -depth,
		tiersx[0], tiersy[0], -depth));
	index += 9;	//318

	mk_tri(pps, pns, index, glm::mat3x3(	//Back left handle triangle
		-4 * depth, 0.0, -depth,
		-3 * depth, -0.01, -2 * depth,
		center[0], center[1], -depth));
	index += 9;	//219

	mk_tri(pps, pns, index, glm::mat3x3(	//Back right handle triangle
		4 * depth, 0.0, -depth,
		3 * depth, -0.01, 2 * -depth,
		center[0], center[1], -depth));
	index += 9;	//336

	mk_tri(pps, pns, index, glm::mat3x3(	//Back left handle 1
		-3 * depth, -0.01, -2 * depth,
		-3 * depth, -0.31, -2 * depth,
		-4 * depth, 0.0, -depth));
	index += 9;	//273 

	mk_tri(pps, pns, index, glm::mat3x3(	//Back left handle 2
		-4 * depth, -0.31, -depth,
		-4 * depth, 0.0, -depth,
		-3 * depth, -0.31, -2 * depth));
	index += 9;	//282

	mk_tri(pps, pns, index, glm::mat3x3(	//Back right handle 1
		3 * depth, -0.31, -2 * depth,
		3 * depth, -0.01, -2 * depth,
		4 * depth, 0.0, -depth));
	index += 9;	//291 

	mk_tri(pps, pns, index, glm::mat3x3(	//Back right handle 2
		4 * depth, 0.0, -depth,
		4 * depth, -0.31, -depth,
		3 * depth, -0.31, -2 * depth));
	index += 9;	//372

	mk_tri(pps, pns, index, glm::mat3x3(	//Left handle 1
		-4 * depth, 0.0, depth,
		-4 * depth, 0.0, -depth,
		-4 * depth, -0.31, depth));
	index += 9;	//381

	mk_tri(pps, pns, index, glm::mat3x3(	//Left handle 2
		-4 * depth, 0.0, -depth,
		-4 * depth, -0.31, -depth,
		-4 * depth, -0.31, depth));
	index += 9;	//390

	mk_tri(pps, pns, index, glm::mat3x3(	//Right handle 1
		4 * depth, -0.31, depth,
		4 * depth, 0.0, -depth,
		4 * depth, 0.0, depth));
	index += 9;	//399

	mk_tri(pps, pns, index, glm::mat3x3(	//Right handle 2
		4 * depth, -0.31, depth,
		4 * depth, -0.31, -depth,
		4 * depth, 0.0, -depth));
	index += 9;	//408
	
	glGenVertexArrays(1, &vaoID[2]);
	glBindVertexArray(vaoID[2]); // Bind our Vertex Array Object so we can use it

	glGenBuffers(3, vboID);

	glBindBuffer(GL_ARRAY_BUFFER, vboID[0]); // Bind our Vertex Buffer Object
	glBufferData(GL_ARRAY_BUFFER, num_points * sizeof(GLfloat), pps, GL_STATIC_DRAW);

	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0); // Set up our vertex attributes pointer
	glEnableVertexAttribArray(0); // Disable our Vertex Array Object

	glBindBuffer(GL_ARRAY_BUFFER, vboID[1]); // Bind our second Vertex Buffer Object
	glBufferData(GL_ARRAY_BUFFER, num_points * sizeof(GLfloat), pns, GL_STATIC_DRAW); // Set the size and data of our VBO and set it to STATIC_DRAW

	glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, 0, 0); // Set up our vertex attributes pointer
	glEnableVertexAttribArray(1); // Enable the second vertex attribute array

	glBindBuffer(GL_ARRAY_BUFFER, vboID[2]); // Bind our second Vertex Buffer Object
	glBufferData(GL_ARRAY_BUFFER, (num_points / 3) * 2 * sizeof(GLfloat), tex, GL_STATIC_DRAW); // Set the size and data of our VBO and set it to STATIC_DRAW

	glVertexAttribPointer((GLuint)2, 2, GL_FLOAT, GL_FALSE, 0, 0); // Set up our vertex attributes pointer
	glEnableVertexAttribArray(2); // Enable the second vertex attribute array

	glBindVertexArray(0); // Disable our Vertex Buffer Object

	GLuint texbmp;
	glActiveTexture(GL_TEXTURE1);
	unsigned char* texture2 = ImgLoader::Load("valve.bmp");

	glGenTextures(1, &texbmp);

	glBindTexture(GL_TEXTURE_2D, texbmp);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 256, 256, 0, GL_BGR, GL_UNSIGNED_BYTE, texture2);

	//free(texture);
	delete[] texture2;

	int texbmp_idx = glGetUniformLocation(program, "bump_tex");
	glUniform1i(texbmp_idx, 1);

	glBindTexture(GL_TEXTURE_2D, texbmp);

	return 1;
}

unsigned int createTableGeometry(){

	int groups = 8;

	float left = -2.5, right = 2.5, front = 4.5, back = -4.5;
	float texl = 0.0, texr = 0.5566, texf = 0.0, texb = 1.0;

	float blockw = (right - left) / (float)groups, tblockw = texr / (float)groups;
	float blockl = (front - back) / (float)groups, tblockl = texb / (float)groups;

	number_table_vt = groups * groups * 6;

	float* table_points = new float[90 + (number_table_vt * 3)];
	float* table_normals = new float[90 + (number_table_vt * 3)];
	float* table_texc = new float[60 + (number_table_vt * 2)];

	int index = 0, texidx = 0;
	for (int i = 0; i < groups; i++){
		for (int j = 0; j < groups; j++){

			//Table
			//table_points[index + 0] = -2.5;	table_points[index + 1] = 0; table_points[index + 2] = -4.5;	//Top Left
			table_points[index + 0] = left + ((j + 0) * blockw);	table_points[index + 1] = 0; table_points[index + 2] = back + ((i + 0) * blockl);	//Top Left
			//table_points[index + 3] = -2.5;	table_points[index + 4] = 0; table_points[index + 5] = 4.5;		//Bottom Left
			table_points[index + 3] = left + ((j + 0) * blockw);	table_points[index + 4] = 0; table_points[index + 5] = back + ((i + 1) * blockl);		//Bottom Left
			//table_points[index + 6] = 2.5;	table_points[index + 7] = 0; table_points[index + 8] = 4.5;		//Bottom Right
			table_points[index + 6] = left + ((j + 1) * blockw);	table_points[index + 7] = 0; table_points[index + 8] = back + ((i + 1) * blockl);		//Bottom Right
			table_normals[index + 0] = 0.0;	table_normals[index + 1] = 1.0; table_normals[index + 2] = 0.0;
			table_normals[index + 3] = 0.0;	table_normals[index + 4] = 1.0; table_normals[index + 5] = 0.0;
			table_normals[index + 6] = 0.0;	table_normals[index + 7] = 1.0; table_normals[index + 8] = 0.0;
			//table_texc[texidx + 0] = 0.0;	table_texc[texidx + 1] = 1.0;	//Top Left
			table_texc[texidx + 0] = texl + ((j + 0) * tblockw);	table_texc[texidx + 1] = texb - ((i + 0) * tblockl);	//Top Left
			//table_texc[texidx + 2] = 0.0;	table_texc[texidx + 3] = 0.0;	//Bottom Left
			table_texc[texidx + 2] = texl + ((j + 0) * tblockw);	table_texc[texidx + 3] = texb - ((i + 1) * tblockl);	//Bottom Left
			//table_texc[texidx + 4] = 0.5566;	table_texc[texidx + 5] = 0.0;	//Bottom Right
			table_texc[texidx + 4] = texl + ((j + 1) * tblockw);	table_texc[texidx + 5] = texb - ((i + 1) * tblockl);	//Bottom Right
			index += 9; texidx += 6;
			//table_points[index + 0] = -2.5;	table_points[index + 1] = 0; table_points[index + 2] = -4.5;	//Top Left
			table_points[index + 0] = left + ((j + 0) * blockw);	table_points[index + 1] = 0; table_points[index + 2] = back + ((i + 0) * blockl);	//Top Left
			//table_points[index + 3] = 2.5;	table_points[index + 4] = 0; table_points[index + 5] = -4.5;	//Top Right
			table_points[index + 3] = left + ((j + 1) * blockw);	table_points[index + 4] = 0; table_points[index + 5] = back + ((i + 0) * blockl);	//Top Right
			//table_points[index + 6] = 2.5;	table_points[index + 7] = 0; table_points[index + 8] = 4.5;		//Bottom Right
			table_points[index + 6] = left + ((j + 1) * blockw);	table_points[index + 7] = 0; table_points[index + 8] = back + ((i + 1) * blockl);		//Bottom Right
			table_normals[index + 0] = 0.0;	table_normals[index + 1] = 1.0; table_normals[index + 2] = 0.0;
			table_normals[index + 3] = 0.0;	table_normals[index + 4] = 1.0; table_normals[index + 5] = 0.0;
			table_normals[index + 6] = 0.0;	table_normals[index + 7] = 1.0; table_normals[index + 8] = 0.0;
			//table_texc[texidx + 0] = 0.0;	table_texc[texidx + 1] = 1.0;	//Top Left
			table_texc[texidx + 0] = texl + ((j + 0) * tblockw);	table_texc[texidx + 1] = texb - ((i + 0) * tblockl);	//Top Left
			//table_texc[texidx + 2] = 0.5566;	table_texc[texidx + 3] = 1.0;	//Top Right
			table_texc[texidx + 2] = texl + ((j + 1) * tblockw);	table_texc[texidx + 3] = texb - ((i + 0) * tblockl);	//Top Right
			//table_texc[texidx + 4] = 0.5566;	table_texc[texidx + 5] = 0.0;	//Bottom Right
			table_texc[texidx + 4] = texl + ((j + 1) * tblockw);	table_texc[texidx + 5] = texb - ((i + 1) * tblockl);	//Bottom Right
			index += 9; texidx += 6;//18

		}
	}

	/*//Table
	table_points[index + 0] = -2.5;	table_points[index + 1] = 0; table_points[index + 2] = -4.5;	//Top Left
	table_points[index + 3] = -2.5;	table_points[index + 4] = 0; table_points[index + 5] = 4.5;		//Bottom Left
	table_points[index + 6] = 2.5;	table_points[index + 7] = 0; table_points[index + 8] = 4.5;		//Bottom Right
	table_normals[index + 0] = 0.0;	table_normals[index + 1] = 1.0; table_normals[index + 2] = 0.0;
	table_normals[index + 3] = 0.0;	table_normals[index + 4] = 1.0; table_normals[index + 5] = 0.0;
	table_normals[index + 6] = 0.0;	table_normals[index + 7] = 1.0; table_normals[index + 8] = 0.0;
	table_texc[texidx + 0] = 0.0;	table_texc[texidx + 1] = 1.0;
	table_texc[texidx + 2] = 0.0;	table_texc[texidx + 3] = 0.0;
	table_texc[texidx + 4] = 0.5566;	table_texc[texidx + 5] = 0.0;
	index += 9; texidx += 6;
	table_points[index + 0] = -2.5;	table_points[index + 1] = 0; table_points[index + 2] = -4.5;	//Top Left
	table_points[index + 3] = 2.5;	table_points[index + 4] = 0; table_points[index + 5] = -4.5;	//Top Right
	table_points[index + 6] = 2.5;	table_points[index + 7] = 0; table_points[index + 8] = 4.5;		//Bottom Right
	table_normals[index + 0] = 0.0;	table_normals[index + 1] = 1.0; table_normals[index + 2] = 0.0;
	table_normals[index + 3] = 0.0;	table_normals[index + 4] = 1.0; table_normals[index + 5] = 0.0;
	table_normals[index + 6] = 0.0;	table_normals[index + 7] = 1.0; table_normals[index + 8] = 0.0;
	table_texc[texidx + 0] = 0.0;	table_texc[texidx + 1] = 1.0;
	table_texc[texidx + 2] = 0.5566;	table_texc[texidx + 3] = 1.0;
	table_texc[texidx + 4] = 0.5566;	table_texc[texidx + 5] = 0.0;
	index += 9; texidx += 6;//18*/
	
	//Front edge of table
	table_points[index + 0] = -2.5;	table_points[index + 1] = 0; table_points[index + 2] = 4.5;
	table_points[index + 3] = -2.5;	table_points[index + 4] = -0.2; table_points[index + 5] = 4.5;
	table_points[index + 6] = 2.5;	table_points[index + 7] = 0; table_points[index + 8] = 4.5;
	table_normals[index + 0] = 0.0;	table_normals[index + 1] = 0.0; table_normals[index + 2] = 1.0;
	table_normals[index + 3] = 0.0;	table_normals[index + 4] = 0.0; table_normals[index + 5] = 1.0;
	table_normals[index + 6] = 0.0;	table_normals[index + 7] = 0.0; table_normals[index + 8] = 1.0;
	table_texc[texidx + 0] = 0.2;	table_texc[texidx + 1] = 0.2;
	table_texc[texidx + 2] = 0.2;	table_texc[texidx + 3] = 0.2;
	table_texc[texidx + 4] = 0.2;	table_texc[texidx + 5] = 0.2;
	index += 9;  texidx += 6;
	table_points[index + 0] = -2.5;	table_points[index + 1] = -0.2; table_points[index + 2] = 4.5;
	table_points[index + 3] = 2.5;	table_points[index + 4] = -0.2; table_points[index + 5] = 4.5;
	table_points[index + 6] = 2.5;	table_points[index + 7] = 0; table_points[index + 8] = 4.5;
	table_normals[index + 0] = 0.0;	table_normals[index + 1] = 0.0; table_normals[index + 2] = 1.0;
	table_normals[index + 3] = 0.0;	table_normals[index + 4] = 0.0; table_normals[index + 5] = 1.0;
	table_normals[index + 6] = 0.0;	table_normals[index + 7] = 0.0; table_normals[index + 8] = 1.0;
	table_texc[texidx + 0] = 0.2;	table_texc[texidx + 1] = 0.2;
	table_texc[texidx + 2] = 0.2;	table_texc[texidx + 3] = 0.2;
	table_texc[texidx + 4] = 0.2;	table_texc[texidx + 5] = 0.2;

	index += 9; texidx += 6;//36
	//Back edge of table
	table_points[index + 0] = -2.5;	table_points[index + 1] = 0; table_points[index + 2] = -4.5;
	table_points[index + 3] = -2.5;	table_points[index + 4] = -0.2; table_points[index + 5] = -4.5;
	table_points[index + 6] = 2.5;	table_points[index + 7] = 0; table_points[index + 8] = -4.5;
	table_normals[index + 0] = 0.0;	table_normals[index + 1] = 0.0; table_normals[index + 2] = -1.0;
	table_normals[index + 3] = 0.0;	table_normals[index + 4] = 0.0; table_normals[index + 5] = -1.0;
	table_normals[index + 6] = 0.0;	table_normals[index + 7] = 0.0; table_normals[index + 8] = -1.0;
	table_texc[texidx + 0] = 0.2;	table_texc[texidx + 1] = 0.2;
	table_texc[texidx + 2] = 0.2;	table_texc[texidx + 3] = 0.2;
	table_texc[texidx + 4] = 0.2;	table_texc[texidx + 5] = 0.2;
	index += 9;  texidx += 6;
	table_points[index + 0] = -2.5;	table_points[index + 1] = -0.2; table_points[index + 2] = -4.5;
	table_points[index + 3] = 2.5;	table_points[index + 4] = -0.2; table_points[index + 5] = -4.5;
	table_points[index + 6] = 2.5;	table_points[index + 7] = 0; table_points[index + 8] = -4.5;
	table_normals[index + 0] = 0.0;	table_normals[index + 1] = 0.0; table_normals[index + 2] = -1.0;
	table_normals[index + 3] = 0.0;	table_normals[index + 4] = 0.0; table_normals[index + 5] = -1.0;
	table_normals[index + 6] = 0.0;	table_normals[index + 7] = 0.0; table_normals[index + 8] = -1.0;
	table_texc[texidx + 0] = 0.2;	table_texc[texidx + 1] = 0.2;
	table_texc[texidx + 2] = 0.2;	table_texc[texidx + 3] = 0.2;
	table_texc[texidx + 4] = 0.2;	table_texc[texidx + 5] = 0.2;
	
	index += 9;  texidx += 6;//54
	//Right edge of table
	table_points[index + 0] = 2.5;	table_points[index + 1] = 0; table_points[index + 2] = 4.5;
	table_points[index + 3] = 2.5;	table_points[index + 4] = -0.2; table_points[index + 5] = 4.5;
	table_points[index + 6] = 2.5;	table_points[index + 7] = 0; table_points[index + 8] = -4.5;
	table_normals[index + 0] = 1.0;	table_normals[index + 1] = 0.0; table_normals[index + 2] = 0.0;
	table_normals[index + 3] = 1.0;	table_normals[index + 4] = 0.0; table_normals[index + 5] = 0.0;
	table_normals[index + 6] = 1.0;	table_normals[index + 7] = 0.0; table_normals[index + 8] = 0.0;
	table_texc[texidx + 0] = 0.2;	table_texc[texidx + 1] = 0.2;
	table_texc[texidx + 2] = 0.2;	table_texc[texidx + 3] = 0.2;
	table_texc[texidx + 4] = 0.2;	table_texc[texidx + 5] = 0.2;
	index += 9;  texidx += 6;
	table_points[index + 0] = 2.5;	table_points[index + 1] = -0.2; table_points[index + 2] = 4.5;
	table_points[index + 3] = 2.5;	table_points[index + 4] = 0.0; table_points[index + 5] = -4.5;
	table_points[index + 6] = 2.5;	table_points[index + 7] = -0.2; table_points[index + 8] = -4.5;
	table_normals[index + 0] = 1.0;	table_normals[index + 1] = 0.0; table_normals[index + 2] = 0.0;
	table_normals[index + 3] = 1.0;	table_normals[index + 4] = 0.0; table_normals[index + 5] = 0.0;
	table_normals[index + 6] = 1.0;	table_normals[index + 7] = 0.0; table_normals[index + 8] = 0.0;
	table_texc[texidx + 0] = 0.2;	table_texc[texidx + 1] = 0.2;
	table_texc[texidx + 2] = 0.2;	table_texc[texidx + 3] = 0.2;
	table_texc[texidx + 4] = 0.2;	table_texc[texidx + 5] = 0.2;
	
	index += 9;  texidx += 6;//72
	//Left edge of table
	table_points[index + 0] = -2.5;	table_points[index + 1] = 0; table_points[index + 2] = 4.5;
	table_points[index + 3] = -2.5;	table_points[index + 4] = -0.2; table_points[index + 5] = 4.5;
	table_points[index + 6] = -2.5;	table_points[index + 7] = 0; table_points[index + 8] = -4.5;
	table_normals[index + 0] = -1.0;	table_normals[index + 1] = 0.0; table_normals[index + 2] = 0.0;
	table_normals[index + 3] = -1.0;	table_normals[index + 4] = 0.0; table_normals[index + 5] = 0.0;
	table_normals[index + 6] = -1.0;	table_normals[index + 7] = 0.0; table_normals[index + 8] = 0.0;
	table_texc[texidx + 0] = 0.2;	table_texc[texidx + 1] = 0.2;
	table_texc[texidx + 2] = 0.2;	table_texc[texidx + 3] = 0.2;
	table_texc[texidx + 4] = 0.2;	table_texc[texidx + 5] = 0.2;
	index += 9;  texidx += 6;
	table_points[index + 0] = -2.5;	table_points[index + 1] = -0.2; table_points[index + 2] = 4.5;
	table_points[index + 3] = -2.5;	table_points[index + 4] = 0.0; table_points[index + 5] = -4.5;
	table_points[index + 6] = -2.5;	table_points[index + 7] = -0.2; table_points[index + 8] = -4.5;
	table_normals[index + 0] = -1.0;	table_normals[index + 1] = 0.0; table_normals[index + 2] = 0.0;
	table_normals[index + 3] = -1.0;	table_normals[index + 4] = 0.0; table_normals[index + 5] = 0.0;
	table_normals[index + 6] = -1.0;	table_normals[index + 7] = 0.0; table_normals[index + 8] = 0.0;
	table_texc[texidx + 0] = 0.2;	table_texc[texidx + 1] = 0.2;
	table_texc[texidx + 2] = 0.2;	table_texc[texidx + 3] = 0.2;
	table_texc[texidx + 4] = 0.2;	table_texc[texidx + 5] = 0.2;
	
	index += 9;  texidx += 6;//90
	//Net
	table_points[index + 0] = -3.0;	table_points[index + 1] = 0; table_points[index + 2] = 0.0;		//Bottom Left
	table_points[index + 3] = -3.0;	table_points[index + 4] = 0.5; table_points[index + 5] = 0.0;	//Top Left
	table_points[index + 6] = 3.0;	table_points[index + 7] = 0; table_points[index + 8] = 0.0;		//Bottom Right
	table_normals[index + 0] = 0.0;	table_normals[index + 1] = 0.0; table_normals[index + 2] = 1.0;
	table_normals[index + 3] = 0.0;	table_normals[index + 4] = 0.0; table_normals[index + 5] = 1.0;
	table_normals[index + 6] = 0.0;	table_normals[index + 7] = 0.0; table_normals[index + 8] = 1.0;
	table_texc[texidx + 0] = 0.5566;	table_texc[texidx + 1] = 1.0;
	table_texc[texidx + 2] = 0.61;	table_texc[texidx + 3] = 1.0;
	table_texc[texidx + 4] = 0.5566;	table_texc[texidx + 5] = 0.330;
	index += 9;  texidx += 6;
	table_points[index + 0] = 3.0;	table_points[index + 1] = 0.0; table_points[index + 2] = 0.0;	//Bottom Right
	table_points[index + 3] = 3.0;	table_points[index + 4] = 0.5; table_points[index + 5] = 0.0;	//Top Right
	table_points[index + 6] = -3.0;	table_points[index + 7] = 0.5; table_points[index + 8] = 0.0;	//Top Left
	table_normals[index + 0] = 0.0;	table_normals[index + 1] = 0.0; table_normals[index + 2] = 1.0;
	table_normals[index + 3] = 0.0;	table_normals[index + 4] = 0.0; table_normals[index + 5] = 1.0;
	table_normals[index + 6] = 0.0;	table_normals[index + 7] = 0.0; table_normals[index + 8] = 1.0;
	table_texc[texidx + 0] = 0.5566;	table_texc[texidx + 1] = 0.335;
	table_texc[texidx + 2] = 0.61;	table_texc[texidx + 3] = 0.335;
	table_texc[texidx + 4] = 0.61;	table_texc[texidx + 5] = 1.0;
	index += 9;  texidx += 6; //108

	// Create our Vertex Array Object
	glGenVertexArrays(1, &vaoID[1]);
	glBindVertexArray(vaoID[1]); // Bind our Vertex Array Object so we can use it

	glGenBuffers(4, vboID);

	glBindBuffer(GL_ARRAY_BUFFER, vboID[0]); // Bind our Vertex Buffer Object
	glBufferData(GL_ARRAY_BUFFER, (90 + (number_table_vt * 3)) * sizeof(GLfloat), table_points, GL_STATIC_DRAW);

	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0); // Set up our vertex attributes pointer
	glEnableVertexAttribArray(0); // Disable our Vertex Array Object

	glBindBuffer(GL_ARRAY_BUFFER, vboID[1]); // Bind our second Vertex Buffer Object
	glBufferData(GL_ARRAY_BUFFER, (90 + (number_table_vt * 3)) * sizeof(GLfloat), table_normals, GL_STATIC_DRAW); // Set the size and data of our VBO and set it to STATIC_DRAW

	glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, 0, 0); // Set up our vertex attributes pointer
	glEnableVertexAttribArray(1); // Enable the second vertex attribute array


	glBindBuffer(GL_ARRAY_BUFFER, vboID[2]); // Bind our second Vertex Buffer Object
	glBufferData(GL_ARRAY_BUFFER, (60 + (number_table_vt * 2)) * sizeof(GLfloat), table_texc, GL_STATIC_DRAW); // Set the size and data of our VBO and set it to STATIC_DRAW

	glVertexAttribPointer((GLuint)2, 2, GL_FLOAT, GL_FALSE, 0, 0); // Set up our vertex attributes pointer
	glEnableVertexAttribArray(2); // Enable the second vertex attribute array

	glBindVertexArray(0); // Disable our Vertex Buffer Object

	delete[] table_points;
	delete[] table_normals;
	delete[] table_texc;

	GLuint tex;
	glActiveTexture(GL_TEXTURE0);
	unsigned char* texture = ImgLoader::Load("table_texture.bmp");

	glGenTextures(1, &tex);

	glBindTexture(GL_TEXTURE_2D, tex);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 512, 512, 0, GL_BGR, GL_UNSIGNED_BYTE, texture);

	//free(texture);
	delete[] texture;

	int tex_idx = glGetUniformLocation(program, "tex");
	glUniform1i(tex_idx, 0);

	glBindTexture(GL_TEXTURE_2D, tex);

	return 1;
}





unsigned int createTriangleStripSphere(int fish)
{
    // use the vertex array object vaoID[0] for this model representation
    
	// Set the center and the radius
	float center[3] = {0.0, 0.0, 0.0};
	float radius = 0.09;

	if (fish == 3){
		radius = 0.15;
	}

	
	// Set the number of segment elements and rows of the sphere. 
	// This will affect the number of triangles in the model
	int segments = 25;
	int rows = 25;

	
	// Compute the number of array elements necessary to store all coordinates.
	int  N = NumVec3ArrayElements(rows, segments);


	// create memory for the vertices and the normals. 
	float* points = new float[N];
	float* normals = new float[N];
	float* colors = new float[N];


    // Create the points and the normal vectors
	number_vertices = Make_Sphere(rows, segments, center, radius, points, normals, colors);
    
	// Create our Vertex Array Object
    glGenVertexArrays(1, &vaoID[fish]); 
    glBindVertexArray(vaoID[fish]); // Bind our Vertex Array Object so we can use it
    
     // Generate our Vertex Buffer Object
    glGenBuffers(3, vboID);
    
    // vertices
    glBindBuffer(GL_ARRAY_BUFFER, vboID[0]); // Bind our Vertex Buffer Object
    glBufferData(GL_ARRAY_BUFFER, N * sizeof(GLfloat), points, GL_STATIC_DRAW); // Set the size and data of our VBO and set it to STATIC_DRAW
    
    glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0); // Set up our vertex attributes pointer
    glEnableVertexAttribArray(0); // Disable our Vertex Array Object
    
    
    // normal vectors
    glBindBuffer(GL_ARRAY_BUFFER, vboID[1]); // Bind our second Vertex Buffer Object
    glBufferData(GL_ARRAY_BUFFER, N * sizeof(GLfloat), normals, GL_STATIC_DRAW); // Set the size and data of our VBO and set it to STATIC_DRAW
    
    glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, 0, 0); // Set up our vertex attributes pointer
    glEnableVertexAttribArray(1); // Enable the second vertex attribute array

	// colors
	/*glBindBuffer(GL_ARRAY_BUFFER, vboID[1]); // Bind our second Vertex Buffer Object
	glBufferData(GL_ARRAY_BUFFER, N * sizeof(GLfloat), colors, GL_STATIC_DRAW); // Set the size and data of our VBO and set it to STATIC_DRAW

	glVertexAttribPointer((GLuint)2, 3, GL_FLOAT, GL_FALSE, 0, 0); // Set up our vertex attributes pointer
	glEnableVertexAttribArray(2); // Enable the second vertex attribute array*/
    
    glBindVertexArray(0); // Disable our Vertex Buffer Object
    
    return 1;
}

void makeMaterial(int i){
	glm::vec3 diffuse_material = glm::vec3(0.4, 0.4, 0.4);
	glm::vec3 ambient_material = glm::vec3(0.01, 0.01, 0.01);
	glm::vec3 specular_material = glm::vec3(0.2, 0.2, 0.2);
	int mode = 1;
	//float cone_angle = 70.0;
	//float cutoff_angle = 70.0;
	float shininess = 10.0;
	float transparency = 1.0;

	if (i == -1){
		mode = -1;
		glm::vec3 diffuse_material = glm::vec3(0.5, 0.5, 0.5);
		glm::vec3 ambient_material = glm::vec3(0.01, 0.01, 0.01);
		glm::vec3 specular_material = glm::vec3(0.2, 0.2, 0.2);
		shininess = 1.0;
	}
	else if (i == 1){
		glm::vec3 diffuse_material = glm::vec3(0.1, 0.1, 0.1);
		glm::vec3 ambient_material = glm::vec3(0.01, 0.01, 0.01);
		glm::vec3 specular_material = glm::vec3(0.2, 0.2, 0.2);
	}
	else if (i == -2){
		mode = -2;
		diffuse_material = glm::vec3(0.2, 0.0, 0.0);
		ambient_material = glm::vec3(0.05, 0.0, 0.0);
		specular_material = glm::vec3(0.01, 0.0, 0.0);
		shininess = 0.5;
	}
	else if (i == 9){
		diffuse_material = glm::vec3(0.2, 0.0, 0.0);
		ambient_material = glm::vec3(0.05, 0.0, 0.0);
		specular_material = glm::vec3(0.5, 0.5, 0.5);
	}
	else if (i == 10){
		shininess = 10.0;
		specular_material = glm::vec3(0.1, 0.1, 0.1);
		diffuse_material = glm::vec3(0.57, 0.42, 0.25);
		ambient_material = glm::vec3(0.114, 0.084, 0.05);
	}
	else if (i == 20){
		specular_material = glm::vec3(0.5, 0.5, 1.0);
		diffuse_material = glm::vec3(0.5, 0.5, 1.0);
		ambient_material = glm::vec3(0.5, 0.5, 0.5);
		transparency = 0.3;
	}
	else if (i == 21){
		specular_material = glm::vec3(1.0, 0.3, 0.0);
		diffuse_material = glm::vec3(1.0, 0.3, 0.0);
		ambient_material = glm::vec3(1.0, 0.3, 0.0);
		transparency = 0.3;
	}

	int ambientColorPos = glGetUniformLocation(program, "ambient_color");
	int diffuseColorPos = glGetUniformLocation(program, "diffuse_color");
	int specularColorPos = glGetUniformLocation(program, "specular_color");
	//int cone_angleIdx = glGetUniformLocation(program, "cone_angle");
	//int cutoffAngleIdx = glGetUniformLocation(program, "cutoffAngle");
	int shininessIdx = glGetUniformLocation(program, "shininess");
	int transparencyIdx = glGetUniformLocation(program, "transparency");
	int modeIdx = glGetUniformLocation(program, "mode");

	glUniform3fv(ambientColorPos, 1, &ambient_material[0]);
	glUniform3fv(diffuseColorPos, 1, &diffuse_material[0]);
	glUniform3fv(specularColorPos, 1, &specular_material[0]);
	//glUniform1f(cone_angleIdx, cone_angle);
	//glUniform1f(cutoffAngleIdx, cutoff_angle);
	glUniform1f(shininessIdx, shininess);
	glUniform1f(transparencyIdx, transparency);
	glUniform1i(modeIdx, mode);
}

/*!
 This function renders the triangle strip sphere
 */
void renderTriangleStripSphere(int fish)
{

    // Bind the buffer and switch it to an active buffer
	if (fish == 1){
		fish = 0;
		makeMaterial(21);
	}
    glBindVertexArray(vaoID[fish]);
        

	
	if (fish == 3){
		makeMaterial(20);
	}
    // Draw the triangle strip model
    glDrawArrays(GL_TRIANGLE_STRIP, 0, number_vertices );

    // Unbind our Vertex Array Object
    glBindVertexArray(0);
}

void renderTable(void){
	glBindVertexArray(vaoID[1]);

	// Draw the triangle strip model
	makeMaterial(-1);
	
	glDrawArrays(GL_TRIANGLES, 0, number_table_vt);

	makeMaterial(1);

	glDrawArrays(GL_TRIANGLES, number_table_vt, 24);

	makeMaterial(-1);

	glDrawArrays(GL_TRIANGLES, number_table_vt + 24, 6);

	makeMaterial(0);

	// Unbind our Vertex Array Object
	glBindVertexArray(0);

}

void renderPaddle(void){
	// #RPAD
	// Bind the buffer and switch it to an active buffer
	glBindVertexArray(vaoID[2]);

	// Draw the triangle strip model
	makeMaterial(-2);

	glDrawArrays(GL_TRIANGLE_FAN, 0, 15);

	glDrawArrays(GL_TRIANGLE_FAN, 15, 15);

	makeMaterial(9);

	glDrawArrays(GL_TRIANGLE_STRIP, 30, 28);

	makeMaterial(10);

	glDrawArrays(GL_TRIANGLES, 58, 78);

	// Unbind our Vertex Array Object
	glBindVertexArray(0);
}



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*!
 This function creates the two models
 */
void setupScene(void) {
    
    createTriangleStripSphere(0);
	createTriangleStripSphere(3);
	createTableGeometry();
	createPaddleGeometry();
    
}

bool boundsCheck(glm::vec3 ballPos, glm::vec3 ballVel){
	return ballPos[1] <= 0.13 && ballPos[0] <= 2.5 && ballPos[0] >= -2.5
		&& ballPos[2] <= 4.5 && ballPos[2] >= -4.5 || ballPos[1] <= -2.5;
}

bool paddleCheck(glm::vec3 ballPos, glm::vec3 ballVel, glm::vec3 paddPos){
	float cenx = paddPos[0], ceny = paddPos[1] + 0.28, cenz = paddPos[2];
	float boundx = 0.25, boundy = 0.28;

	return	ballPos[0] >= cenx - boundx && ballPos[0] <= cenx + boundx &&
			ballPos[1] >= ceny - boundy && ballPos[1] <= ceny + boundy &&
			ballPos[2] - cenz <= 0.13 && ballPos[2] - cenz >= 0.0;

}

bool netCheck(glm::vec3 ballPos, glm::vec3 ballVel){
	return ((ballPos[1] < 0.58	&& ballPos[1] > 0) &&
		(ballPos[0] >= -3.0	&& ballPos[0] <= 3.0) &&
		abs(ballPos[2]) < 0.13);
}

void collisionCheck(glm::vec3& ballPos, glm::vec3& ballVel, glm::vec3 paddPos, glm::vec3 paddVel, double time_diff){
	float tableFactor = 0.77;

	bool mitig = false;

	if (ballPos[1] <= -2.0){
		if (ballPos[2] < 0 && faults < 1){
			faults = -2;
		}
		else if (ballPos[2] < 0 && faults == 1){
			faults = 2;
		}
		else if (ballPos[2] > 0 && faults > -1){
			faults = 2;
		}
		else{
			faults = -2;
		}
		return;
	}

	if (boundsCheck(ballPos,ballVel)
		&& ballVel[1] < 0){
		ballPos[1] = 0.13;
		ballVel[1] *= -1;
		ballVel *= tableFactor;
		mitig = true;
		//if (ballVel[1] < 0.02)
			//ballVel[1] = 0;
		if (ballPos[2] > 0){
			if (faults < 0) faults = 1;
			else faults++;
		}
		else if (ballPos[2] < 0){
			if (faults > 0) faults = -1;
			else faults--;
		}

	}

	if (paddleCheck(ballPos, ballVel, paddPos)){
		ballPos[2] = paddPos[2] + 0.13;
		ballVel[2] *= -2.5;
		ballVel[1] += 1.0;

		ballVel += paddVel;
	}

	if (netCheck(ballPos, ballVel)){
		if (ballPos[2] < 0)
			ballPos[2] = -0.09;
		else
			ballPos[2] = 0.09;
		ballVel[2] *= -1.0;
		if (!mitig){
			ballVel *= 0.4;
		}
		else{
			ballVel *= 0.8;
		}

		if (ballPos[2] > 0){
			faults = 2;
		}
	}
}


int main(int argc, const char * argv[])
{
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //// Init glfw, create a window, and init glew
    
    // Init the GLFW Window
    window = initWindow();
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	float dimx = 1920, dimy = 1080;
    
    
    // Init the glew api
    initGlew();
    
	// Prepares some defaults
	CoordSystemRenderer* coordinate_system_renderer = new CoordSystemRenderer(10.0);

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //// The Shader Program starts here
    
    // Vertex shader source code. This draws the vertices in our window. We have 3 vertices since we're drawing an triangle.
    // Each vertex is represented by a vector of size 4 (x, y, z, w) coordinates.
    //static const string vertex_code = vs_string;
	static const string vertex_code = ShaderFileUtils::LoadFromFile("project_shader.vs");
    static const char * vs_source = vertex_code.c_str();
    
    // Fragment shader source code. This determines the colors in the fragment generated in the shader pipeline. In this case, it colors the inside of our triangle specified by our vertex shader.
    //static const string fragment_code = fs_string;
	static const string fragment_code = ShaderFileUtils::LoadFromFile("project_shader.fs");
    static const char * fs_source = fragment_code.c_str();
    
    // This next section we'll generate the OpenGL program and attach the shaders to it so that we can render our triangle.
    program = glCreateProgram();
    
    // We create a shader with our fragment shader source code and compile it.
    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fs_source, NULL);
    glCompileShader(fs);
    
    // We create a shader with our vertex shader source code and compile it.
    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vs_source, NULL);
    glCompileShader(vs);
    
    // We'll attach our two compiled shaders to the OpenGL program.
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    
    glLinkProgram(program);
    
    // We'll specify that we want to use this program that we've attached the shaders to.
    glUseProgram(program);
    
    //// The Shader Program ends here
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    
    /// IGNORE THE NEXT PART OF THIS CODE
    /// IGNORE THE NEXT PART OF THIS CODE
    /// IGNORE THE NEXT PART OF THIS CODE
    // It controls the virtual camera
    
    // Set up our green background color
    static const GLfloat clear_color[] = { 0.6f, 0.7f, 1.0f, 1.0f };
    static const GLfloat clear_depth[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    
    
    projectionMatrix = glm::perspective(1.1f, (float)dimx / (float)dimy, 0.1f, 100.f);
    modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)); // Create our model matrix which will halve the size of our model
    viewMatrix = glm::lookAt(glm::vec3(0.0f, 2.0f, 6.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	//viewMatrix = glm::lookAt(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	inverseViewMatrix = glm::inverse(viewMatrix);
    
    int projectionMatrixLocation = glGetUniformLocation(program, "projectionMatrix"); // Get the location of our projection matrix in the shader
    int viewMatrixLocation = glGetUniformLocation(program, "viewMatrix"); // Get the location of our view matrix in the shader
    int modelMatrixLocation = glGetUniformLocation(program, "modelMatrix"); // Get the location of our model matrix in the shader
	int invViewMatrixLocation = glGetUniformLocation(program, "inverseViewMatrix");
    
    glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]); // Send our projection matrix to the shader
    glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]); // Send our view matrix to the shader
	glUniformMatrix4fv(invViewMatrixLocation, 1, GL_FALSE, &inverseViewMatrix[0][0]);
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]); // Send our model matrix to the shader

	//Light

	glm::vec4 lightPos = glm::vec4(0.0, 5.0, 0.0, 1.0);
	float ambient_intensity = 0.0;
	float specular_intensity = 6.0;
	float diffuse_intensity = 6.0;
	float attenuation_coeff = 0.01;
	float cutoff_angle = 5.0;
	float cone_angle = 10.0;
	glm::vec3 cone_direction = glm::vec3(0.0, -1.0, 0.0);

	glm::vec4 ambient_l = glm::vec4(0.0, 0.0, 0.0, ambient_intensity);
	glm::vec4 diffuse_l = glm::vec4(1.0, 1.0, 1.0, diffuse_intensity);
	glm::vec4 specula_l = glm::vec4(1.0, 1.0, 1.0, specular_intensity);

	number_lights++;
	int num_lightsIdx = glGetUniformLocation(program, "num_lights");
	glUniform1i(num_lightsIdx, number_lights);

	int lightPoslIdx = glGetUniformLocation(program, GetVariableName(light_struct, light_names[0], number_lights - 1).c_str());
	int diffuselIdx = glGetUniformLocation(program, GetVariableName(light_struct, light_names[1], number_lights - 1).c_str());
	int ambientlIdx = glGetUniformLocation(program, GetVariableName(light_struct, light_names[2], number_lights - 1).c_str());
	int speculalIdx = glGetUniformLocation(program, GetVariableName(light_struct, light_names[3], number_lights - 1).c_str());
	int coefflIdx = glGetUniformLocation(program, GetVariableName(light_struct, light_names[4], number_lights - 1).c_str());
	int coneanglelIdx = glGetUniformLocation(program, GetVariableName(light_struct, light_names[5], number_lights - 1).c_str());
	int conedirlIdx = glGetUniformLocation(program, GetVariableName(light_struct, light_names[6], number_lights - 1).c_str());
	int cutofflIdx = glGetUniformLocation(program, GetVariableName(light_struct, light_names[7], number_lights - 1).c_str());

	glUniform4fv(ambientlIdx, 1, &ambient_l[0]);
	glUniform4fv(diffuselIdx, 1, &diffuse_l[0]);
	glUniform4fv(speculalIdx, 1, &specula_l[0]);
	glUniform1f(coefflIdx, attenuation_coeff);
	glUniform4fv(lightPoslIdx, 1, &lightPos[0]);
	glUniform1f(coneanglelIdx, cone_angle);
	glUniform3fv(conedirlIdx, 1, &cone_direction[0]);
	glUniform1f(cutofflIdx, cutoff_angle);



	glm::vec4 lightPos2 = glm::vec4(0.0, 10.0, 0.0, 1.0);
	float ambient_intensity2 = 0.0;
	float specular_intensity2 = 0.5;
	float diffuse_intensity2 = 0.5;
	float attenuation_coeff2 = 0.01;
	float cutoff_angle2 = 360.0;
	float cone_angle2 = 360.0;
	glm::vec3 cone_direction2 = glm::vec3(0.0, -1.0, 0.0);

	glm::vec4 ambient_l2 = glm::vec4(0.0, 0.0, 0.0, ambient_intensity);
	glm::vec4 diffuse_l2 = glm::vec4(1.0, 1.0, 1.0, diffuse_intensity);
	glm::vec4 specula_l2 = glm::vec4(1.0, 1.0, 1.0, specular_intensity);

	number_lights++;
	int num_lightsIdx2 = glGetUniformLocation(program, "num_lights");
	glUniform1i(num_lightsIdx2, number_lights);

	int lightPoslIdx2 = glGetUniformLocation(program, GetVariableName(light_struct, light_names[0], number_lights - 1).c_str());
	int diffuselIdx2 = glGetUniformLocation(program, GetVariableName(light_struct, light_names[1], number_lights - 1).c_str());
	int ambientlIdx2 = glGetUniformLocation(program, GetVariableName(light_struct, light_names[2], number_lights - 1).c_str());
	int speculalIdx2 = glGetUniformLocation(program, GetVariableName(light_struct, light_names[3], number_lights - 1).c_str());
	int coefflIdx2 = glGetUniformLocation(program, GetVariableName(light_struct, light_names[4], number_lights - 1).c_str());
	int coneanglelIdx2 = glGetUniformLocation(program, GetVariableName(light_struct, light_names[5], number_lights - 1).c_str());
	int conedirlIdx2 = glGetUniformLocation(program, GetVariableName(light_struct, light_names[6], number_lights - 1).c_str());
	int cutofflIdx2 = glGetUniformLocation(program, GetVariableName(light_struct, light_names[7], number_lights - 1).c_str());

	glUniform4fv(ambientlIdx2, 1, &ambient_l2[0]);
	glUniform4fv(diffuselIdx2, 1, &diffuse_l2[0]);
	glUniform4fv(speculalIdx2, 1, &specula_l2[0]);
	glUniform1f(coefflIdx2, attenuation_coeff2);
	glUniform4fv(lightPoslIdx2, 1, &lightPos2[0]);
	glUniform1f(coneanglelIdx2, cone_angle2);
	glUniform3fv(conedirlIdx2, 1, &cone_direction2[0]);
	glUniform1f(cutofflIdx2, cutoff_angle2);

	number_lights++;

	glm::vec4 lightPos3 = glm::vec4(0.0, 10.0, 0.0, 1.0);
	float ambient_intensity3 = 0.0;
	float specular_intensity3 = 0.0;
	float diffuse_intensity3 = 0.0;
	float attenuation_coeff3 = 0.5;
	float cutoff_angle3 = 360.0;
	float cone_angle3 = 360.0;
	glm::vec3 cone_direction3 = glm::vec3(0.0, -1.0, 0.0);

	glm::vec4 ambient_l3 = glm::vec4(0.0, 0.0, 0.0, ambient_intensity);
	glm::vec4 diffuse_l3 = glm::vec4(0.0, 0.0, 0.0, diffuse_intensity);
	glm::vec4 specula_l3 = glm::vec4(0.0, 0.0, 0.0, specular_intensity);

	number_lights++;
	int num_lightsIdx3 = glGetUniformLocation(program, "num_lights");
	glUniform1i(num_lightsIdx3, number_lights);

	int lightPoslIdx3 = glGetUniformLocation(program, GetVariableName(light_struct, light_names[0], number_lights - 1).c_str());
	int diffuselIdx3 = glGetUniformLocation(program, GetVariableName(light_struct, light_names[1], number_lights - 1).c_str());
	int ambientlIdx3 = glGetUniformLocation(program, GetVariableName(light_struct, light_names[2], number_lights - 1).c_str());
	int speculalIdx3 = glGetUniformLocation(program, GetVariableName(light_struct, light_names[3], number_lights - 1).c_str());
	int coefflIdx3 = glGetUniformLocation(program, GetVariableName(light_struct, light_names[4], number_lights - 1).c_str());
	int coneanglelIdx3 = glGetUniformLocation(program, GetVariableName(light_struct, light_names[5], number_lights - 1).c_str());
	int conedirlIdx3 = glGetUniformLocation(program, GetVariableName(light_struct, light_names[6], number_lights - 1).c_str());
	int cutofflIdx3 = glGetUniformLocation(program, GetVariableName(light_struct, light_names[7], number_lights - 1).c_str());

	glUniform4fv(ambientlIdx3, 1, &ambient_l3[0]);
	glUniform4fv(diffuselIdx3, 1, &diffuse_l3[0]);
	glUniform4fv(speculalIdx3, 1, &specula_l3[0]);
	glUniform1f(coefflIdx3, attenuation_coeff3);
	glUniform4fv(lightPoslIdx3, 1, &lightPos3[0]);
	glUniform1f(coneanglelIdx3, cone_angle3);
	glUniform3fv(conedirlIdx3, 1, &cone_direction3[0]);
	glUniform1f(cutofflIdx3, cutoff_angle3);

	// create two uniform variables
    glBindAttribLocation(program, 0, "in_Position");
    glBindAttribLocation(program, 1, "in_Normal");
	glBindAttribLocation(program, 2, "in_Tex_Coord");
    
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
	 //// The Shader Program ends here
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    
    // this creates the scene
    setupScene();
    
    int i=0;

    // Enable depth test
    // ignore this line, it allows us to keep the distance value after we proejct each object to a 2d canvas.
    glEnable(GL_DEPTH_TEST);

	glm::vec3 ballPosInit(0.0f, 3.0f, -4.0f);
	glm::vec3 ballVelInit(-0.5f, 0.0f, 8.0f);

	glm::vec3 gravity(0.0f, -32.2f, 0.0f);
	glm::vec3 ballPos(0.0f, 3.0f, -4.0f);
	glm::vec3 ballVel(-0.5f, 0.0f, 8.0f);
	glm::vec3 paddPos(0.0f, 0.5f, 4.5f);
	glm::vec3 paddPrev = paddPos;
	//glm::vec3 paddPos(0.0f, 0.0f, 0.0f);

	float ballRad = 0.13;
	bool ball_in_play = true;
	double last_time = glfwGetTime();
	int cameraMode = 0;
	bool camSentinel = true;

	bool lightSwitch = false;
    
    // This is our render loop. As long as our window remains open (ESC is not pressed), we'll continue to render things.
    while(!glfwWindowShouldClose(window))
    {
        
		double curr_time = glfwGetTime();
		
		double time_diff = curr_time - last_time;

		if (time_count > 0.0){
			time_count -= time_diff;

			if (time_count <= 0.0){
				time_count = 0.0;
				score = 0;
				lightSwitch = true;
			}
		}

		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		xpos = fmin(fmax(xpos, 0.0), dimx);
		ypos = fmin(fmax(ypos, 0.0), dimy);

		float relx = (6.0 * xpos / dimx) - 3.0;
		float rely = ((dimy - ypos) / dimy) * 3.0;

		paddPos = glm::vec3(relx, rely, 4.5f);

		int keyEventCode = getKeyEvent();
		if (keyEventCode == -1){
			ball_in_play = true;
			ballPos = ballPosInit;
			ballVel = ballVelInit;
		}
		else if (keyEventCode == 1 || keyEventCode == 2 || keyEventCode == 3){
			cameraMode = keyEventCode - 1;
			camSentinel = true;
		}

		if (ball_in_play == true){
			ballPos += ballVel * glm::float32(time_diff);

			ballVel += gravity * glm::float32(time_diff);

			collisionCheck(ballPos, ballVel, paddPos, (paddPos - paddPrev)/glm::float32(time_diff), time_diff);
		}

		if (faults == 2 || faults == -2){
			ball_in_play = false;
			score = faults / 2;
			time_count = 2.0;
			faults = 0;
		}
		
		last_time = curr_time;

		/*cone_direction = (glm::vec3(lightPos[0], -lightPos[1], lightPos[2]) - ballPos);
		conedirlIdx = glGetUniformLocation(program, GetVariableName(light_struct, light_names[6], 0).c_str());
		glUniform3fv(conedirlIdx, 1, &cone_direction[0]);*/

		if (cameraMode == 0 && camSentinel){
			viewMatrix = glm::lookAt(glm::vec3(0.0f, 2.0f, 7.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			inverseViewMatrix = glm::inverse(viewMatrix);
			glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]); // Send our view matrix to the shader
			glUniformMatrix4fv(invViewMatrixLocation, 1, GL_FALSE, &inverseViewMatrix[0][0]);
			camSentinel = false;
		}
		else if (cameraMode == 1){
			viewMatrix = glm::lookAt(glm::vec3(4.5f, 3.0f, 0.5f), ballPos, glm::vec3(0.0f, 1.0f, 0.0f));
			inverseViewMatrix = glm::inverse(viewMatrix);
			glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]); // Send our view matrix to the shader
			glUniformMatrix4fv(invViewMatrixLocation, 1, GL_FALSE, &inverseViewMatrix[0][0]);
		}
		else if (cameraMode == 2){
			glm::vec3 raised(0.0, 0.1, 0.0);
			glm::vec3 ballCamAng = ballPos + (glm::float32(-0.1) * ballVel);
			ballCamAng[1] = ballPos[1] + 0.1;
			//glm::vec3 ballTarget(ballPos[0], ballCamAng[1], ballPos[2]);
			viewMatrix = glm::lookAt(ballCamAng, ballPos + raised, glm::vec3(0.0f, 1.0f, 0.0f));
			inverseViewMatrix = glm::inverse(viewMatrix);
			glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]); // Send our view matrix to the shader
			glUniformMatrix4fv(invViewMatrixLocation, 1, GL_FALSE, &inverseViewMatrix[0][0]);
		}

		//printf("X: %f, Y: %f\n", xpos, ypos);

        // Clear the entire buffer with our green color (sets the background to be green).
        glClearBufferfv(GL_COLOR , 0, clear_color);
        glClearBufferfv(GL_DEPTH , 0, clear_depth);
        
        // this draws the coordinate system
		//coordinate_system_renderer->draw();
        
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //// This generate the object
        // Enable the shader program
        glUseProgram(program);
        
        // this changes the camera location
        glm::mat4 rotated_view = viewMatrix * GetRotationMatrix();
        glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &rotated_view[0][0]); // send the view matrix to our shader

		glm::mat4 inv_rotated_view = glm::inverse(rotated_view);
		glUniformMatrix4fv(invViewMatrixLocation, 1, GL_FALSE, &inv_rotated_view[0][0]);

        // This moves the model to the right
        modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]); // Send our model matrix to the shader

		//lightPos = glm::vec4(-0.0, 6.0, 6.0, 1.0);
		//glUniform4fv(lightPoslIdx, 1, &lightPos[0]);
		cone_direction = (ballPos - glm::vec3(lightPos[0], lightPos[1], lightPos[2]));
		conedirlIdx = glGetUniformLocation(program, GetVariableName(light_struct, light_names[6], 0).c_str());
		glUniform3fv(conedirlIdx, 1, &cone_direction[0]);


		makeMaterial(0);
		renderTable();

		modelMatrix = glm::translate(glm::mat4(1.0f), paddPos);
		modelMatrix = glm::rotate(modelMatrix, 3.1415f, glm::vec3(0.0, 1.0, 0.0));
		glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]); // Send our model matrix to the shader
		makeMaterial(0);
		renderPaddle();

		modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0, - 0.3, 0.0));
		glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]);
		renderTriangleStripSphere(3);

		glm::vec3 shoulder_pos(paddPos[0] + 0.3 * ulna, shoulder, paddPos[2]);

		//float shoulder_dist = abs(glm::distance(shoulder_pos, paddPos));

		float shoulder_dist = sqrtf(powf(paddPos[0] - shoulder_pos[0], 2) + powf(paddPos[1] - 0.3 - shoulder_pos[1], 2));

		float a_val = (((humerus*humerus) - (ulna*ulna)) + (shoulder_dist*shoulder_dist)) / (2 * shoulder_dist);
		float h_val = humerus * sin( acosf(a_val/humerus) );
		glm::vec3 p2 = (paddPos - glm::vec3(0.0, 0.3, 0.0)) + a_val * (shoulder_pos - (paddPos - glm::vec3(0.0, 0.3, 0.0))) / shoulder_dist;
		float x3 = p2[0] - h_val * ((paddPos[1] - 0.3) - shoulder_pos[1]) / shoulder_dist;
		float y3 = p2[1] + h_val * (paddPos[0] - shoulder_pos[0]) / shoulder_dist;
		glm::vec3 joint3(x3, y3, shoulder_pos[2]);

		printf("X: %f; Y: %f, A: %f, Dist: %f, check %f\n", x3, y3, a_val, shoulder_dist, abs(humerus - ulna));

		modelMatrix = glm::translate(glm::mat4(1.0f), shoulder_pos);
		glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]); // Send our model matrix to the shader
		renderTriangleStripSphere(3);

		modelMatrix = glm::translate(glm::mat4(1.0f), joint3);
		glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]); // Send our model matrix to the shader
		renderTriangleStripSphere(3);

		

		modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.5f, -4.5f));
		modelMatrix = glm::rotate(modelMatrix, 1.2f, glm::vec3(0.0, 1.0, 1.0));
		glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]); // Send our model matrix to the shader
		makeMaterial(0);
		renderPaddle();

		modelMatrix = glm::translate(glm::mat4(1.0f), ballPos);
		glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]); // Send our model matrix to the shader
		
		// This line renders your triangle strip model
		if (ball_in_play){
			renderTriangleStripSphere(0);
		}

		
		if (score != 0){
			
			diffuse_l3 = glm::vec4(1.0, 0.3, 0.0, 5.0);
			specula_l3 = glm::vec4(1.0, 0.3, 0.0, 5.0);
			diffuselIdx3 = glGetUniformLocation(program, GetVariableName(light_struct, light_names[1], number_lights - 1).c_str());
			speculalIdx3 = glGetUniformLocation(program, GetVariableName(light_struct, light_names[3], number_lights - 1).c_str());
			glUniform4fv(diffuselIdx3, 1, &diffuse_l3[0]);
			glUniform4fv(speculalIdx3, 1, &specula_l3[0]);

			glm::vec3 score_pos = (glm::vec3(2.2f, 0.0f, 4.0f) * glm::float32(-score)) + glm::vec3(0.0, 0.3, 0.0);
			modelMatrix = glm::translate(glm::mat4(1.0f), score_pos);
			glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]); // Send our model matrix to the shader
			lightPos3 = glm::vec4(score_pos, 1.0);
			lightPoslIdx3 = glGetUniformLocation(program, GetVariableName(light_struct, light_names[0], number_lights - 1).c_str());
			glUniform4fv(lightPoslIdx3, 1, &lightPos3[0]); // Send our model matrix to the shader
			renderTriangleStripSphere(1);
		}
		if (lightSwitch){
			diffuse_l3 = glm::vec4(0.0, 0.0, 0.0, 0.0);
			specula_l3 = glm::vec4(0.0, 0.0, 0.0, 0.0);
			diffuselIdx3 = glGetUniformLocation(program, GetVariableName(light_struct, light_names[1], number_lights - 1).c_str());
			speculalIdx3 = glGetUniformLocation(program, GetVariableName(light_struct, light_names[3], number_lights - 1).c_str());
			glUniform4fv(diffuselIdx3, 1, &diffuse_l3[0]);
			glUniform4fv(speculalIdx3, 1, &specula_l3[0]);
			lightSwitch = false;
		}
        
		paddPrev = paddPos;
        
		// disable the shader program
        glUseProgram(0);


        //// This generate the object
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        
        
        // Swap the buffers so that what we drew will appear on the screen.
        glfwSwapBuffers(window);
        glfwPollEvents();
        
    }
    
	// delete the coordinate system object
	delete coordinate_system_renderer;

    // Program clean up when the window gets closed.
    glDeleteVertexArrays(3, vaoID);
    glDeleteProgram(program);
}

