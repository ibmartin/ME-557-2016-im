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




////////////////////////////////////////////////////////////////
// Include file for the sphere functions
#include "make_sphere.h"
//#include "../gl_common/GLAppearance.h"




// this line tells the compiler to use the namespace std.
// Each object, command without a namespace is assumed to be part of std. 
using namespace std;




static const string vs_string =
"#version 410 core                                                 \n"
"                                                                   \n"
"uniform mat4 projectionMatrix;                                    \n"
"uniform mat4 viewMatrix;                                           \n"
"uniform mat4 inverseViewMatrix;                                           \n"
"uniform mat4 modelMatrix;                                          \n"
"\n"
"in vec3 in_Position;                                               \n"
"in vec3 in_Normal;                                                  \n"
"in vec3 in_Color;													\n"
"\n"
"uniform vec3 diffuse_color;	\n"
"uniform vec3 ambient_color;	\n"
"uniform vec3 specular_color;	\n"
"\n"
"uniform vec4 light_position;	\n"
"uniform float cone_angle;	\n"
"uniform vec3 cone_direction;	\n"
"\n"
"uniform float diffuse_intensity;	\n"
"uniform float ambient_intensity;	\n"
"uniform float specular_intensity;	\n"
"uniform float shininess;	\n"
"uniform float attenuationCoefficient; 	\n"
"uniform float cutoffAngle;	\n"
"                                                                   \n"
"out vec3 pass_Color;                                               \n"
"                                                                  \n"
"void main(void)                                                   \n"
"{                                                                 \n"
"	 vec3 normal = normalize(in_Normal);	\n"
"	 vec4 transformedNormal = normalize(transpose(inverse( modelMatrix)) * vec4( normal, 1.0 ));\n"
"\n"
"	 vec4 surfacePostion = modelMatrix * vec4(in_Position, 1.0);	\n"
"	 vec4 surface_to_light =   normalize( light_position -  surfacePostion );	\n"
"\n"
"	 float diffuse_coefficient = max( dot(transformedNormal, surface_to_light), 0.0);	\n"
"	 vec3 out_diffuse_color = diffuse_color  * diffuse_coefficient * diffuse_intensity;	\n"
"	 vec3 out_ambient_color = vec3(ambient_color) * ambient_intensity;	\n"
"\n"
"	 vec3 incidenceVector = -surface_to_light.xyz;	\n"
"	 vec3 reflectionVector = reflect(incidenceVector, normal.xyz);	\n"
"\n"
"	 vec3 cameraPosition = vec3( inverseViewMatrix[3][0], inverseViewMatrix[3][1], inverseViewMatrix[3][2]); \n"
"	 vec3 surfaceToCamera = normalize(cameraPosition - surfacePostion.xyz); \n"
"\n"
"	 float cosAngle = max(0.0, dot(surfaceToCamera, reflectionVector)); \n"
"	 float specular_coefficient = pow(cosAngle, shininess); \n"
"	 vec3 out_specular_color = specular_color * specular_coefficient * specular_intensity;	\n"
"\n"
"	 float distanceToLight = length(light_position.xyz - surfacePostion.xyz);	\n"
"	 float attenuation = 1.0 / (1.0 + attenuationCoefficient * pow(distanceToLight, 2));	\n"
"\n"
"	 vec3 cone_direction_norm = normalize(cone_direction);	\n"
"	 vec3 ray_direction = -surface_to_light.xyz;			\n"
"	 float light_to_surface_angle = degrees(acos(dot(ray_direction, cone_direction_norm))) ;	\n"
"	 if(light_to_surface_angle > cone_angle){ attenuation = 0.0; }\n"
"	 else if(light_to_surface_angle > cutoffAngle){\n"
"		float x = (light_to_surface_angle - cutoffAngle)/(cone_angle - cutoffAngle);\n"
"		float spot = 1.0 - (x * x * (3 - 2 * x));\n"
"		attenuation = attenuation * spot;\n"
"	 }\n"
"\n"
"	 vec3 linearColor = out_ambient_color  + attenuation * ( out_diffuse_color + out_specular_color);	 \n"
"	 vec3 gamma = vec3(1.0/2.2);	 \n"
"	 vec3 finalColor = pow(linearColor, gamma);	 \n"
"\n"
"    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(in_Position, 1.0);  \n"
"    pass_Color = finalColor;                                         \n"
"}                                                                 \n";

// Fragment shader source code. This determines the colors in the fragment generated in the shader pipeline. In this case, it colors the inside of our triangle specified by our vertex shader.
static const string fs_string  =
"#version 410 core                                                 \n"
"                                                                  \n"
"in vec3 pass_Color;                                                 \n"
"out vec4 color;                                                    \n"
"void main(void)                                                   \n"
"{                                                                 \n"
"    color = vec4(pass_Color, 1.0);                               \n"
"}                                                                 \n";




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



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// Fill this functions with your model code.

// USE THESE vertex array objects to define your objects
unsigned int vaoID[2];

unsigned int vboID[6];


/*!
Searches for the file in name.
Returns the new path in new_path.
*/

/*!
 ADD YOUR CODE TO CREATE THE TRIANGLE STRIP MODEL TO THIS FUNCTION
 */
unsigned int createTriangleStripSphere(void)
{
    // use the vertex array object vaoID[0] for this model representation
    
	// Set the center and the radius
	float center[3] = {0.0, 0.0, 0.0};
	float radius = 2.0;

	
	// Set the number of segment elements and rows of the sphere. 
	// This will affect the number of triangles in the model
	int segments = 200;
	int rows = 200;

	
	// Compute the number of array elements necessary to store all coordinates.
	int  N = NumVec3ArrayElements(rows, segments);


	// create memory for the vertices and the normals. 
	float* points = new float[N];
	float* normals = new float[N];
	float* colors = new float[N];


    // Create the points and the normal vectors
	number_vertices = Make_Sphere(rows, segments, center, radius, points, normals, colors);
    
	// Create our Vertex Array Object
    glGenVertexArrays(1, &vaoID[0]); 
    glBindVertexArray(vaoID[0]); // Bind our Vertex Array Object so we can use it
    
     // Generate our Vertex Buffer Object
    glGenBuffers(3, vboID);
    
    // vertices
    glBindBuffer(GL_ARRAY_BUFFER, vboID[0]); // Bind our Vertex Buffer Object
    glBufferData(GL_ARRAY_BUFFER, N * sizeof(GLfloat), points, GL_STATIC_DRAW); // Set the size and data of our VBO and set it to STATIC_DRAW
    
    glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0); // Set up our vertex attributes pointer
    glEnableVertexAttribArray(0); // Disable our Vertex Array Object
    
    
    // normal vectors
    glBindBuffer(GL_ARRAY_BUFFER, vboID[2]); // Bind our second Vertex Buffer Object
    glBufferData(GL_ARRAY_BUFFER, N * sizeof(GLfloat), normals, GL_STATIC_DRAW); // Set the size and data of our VBO and set it to STATIC_DRAW
    
    glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, 0, 0); // Set up our vertex attributes pointer
    glEnableVertexAttribArray(1); // Enable the second vertex attribute array

	// colors
	glBindBuffer(GL_ARRAY_BUFFER, vboID[1]); // Bind our second Vertex Buffer Object
	glBufferData(GL_ARRAY_BUFFER, N * sizeof(GLfloat), colors, GL_STATIC_DRAW); // Set the size and data of our VBO and set it to STATIC_DRAW

	glVertexAttribPointer((GLuint)2, 3, GL_FLOAT, GL_FALSE, 0, 0); // Set up our vertex attributes pointer
	glEnableVertexAttribArray(2); // Enable the second vertex attribute array
    
    glBindVertexArray(0); // Disable our Vertex Buffer Object
    
    return 1;
}

void makeMaterial(int i){
	glm::vec3 diffuse_material = glm::vec3(1.0, 0.0, 0.0);
	glm::vec3 ambient_material = glm::vec3(0.5, 0.0, 0.0);
	glm::vec3 specular_material = glm::vec3(1.0, 1.0, 1.0);
	float cone_angle = 70.0;
	float cutoff_angle = 70.0;
	float shininess = 60.0;

	if (i == 1){
		diffuse_material = glm::vec3(0.0, 0.0, 1.0);
		ambient_material = glm::vec3(0.0, 0.0, 0.5);
		specular_material = glm::vec3(0.0, 0.0, 0.0);
	}
	else if (i == 2){
		diffuse_material = glm::vec3(0.0, 1.0, 0.0);
		ambient_material = glm::vec3(0.0, 0.0, 0.0);
		specular_material = glm::vec3(1.0, 1.0, 1.0);
		cone_angle = 40;
		cutoff_angle = 40;
	}
	else if (i == 3){
		diffuse_material = glm::vec3(0.2, 0.2, 0);
		ambient_material = glm::vec3(0.2, 0.2, 0.0);
		specular_material = glm::vec3(0.1, 0.1, 0.1);
		cone_angle = 70;
		cutoff_angle = 30;
	}

	int ambientColorPos = glGetUniformLocation(program, "ambient_color");
	int diffuseColorPos = glGetUniformLocation(program, "diffuse_color");
	int specularColorPos = glGetUniformLocation(program, "specular_color");
	int cone_angleIdx = glGetUniformLocation(program, "cone_angle");
	int cutoffAngleIdx = glGetUniformLocation(program, "cutoffAngle");
	int shininessIdx = glGetUniformLocation(program, "shininess");

	glUniform3fv(ambientColorPos, 1, &ambient_material[0]);
	glUniform3fv(diffuseColorPos, 1, &diffuse_material[0]);
	glUniform3fv(specularColorPos, 1, &specular_material[0]);
	glUniform1f(cone_angleIdx, cone_angle);
	glUniform1f(cutoffAngleIdx, cutoff_angle);
	glUniform1f(shininessIdx, shininess);
}

/*!
 This function renders the triangle strip sphere
 */
void renderTriangleStripSphere(void)
{

    // Bind the buffer and switch it to an active buffer
    glBindVertexArray(vaoID[0]);
        
    // Draw the triangle strip model
    glDrawArrays(GL_TRIANGLE_STRIP, 0, number_vertices );

    // Unbind our Vertex Array Object
    glBindVertexArray(0);
}



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*!
 This function creates the two models
 */
void setupScene(void) {
    
    createTriangleStripSphere();
    
}


int main(int argc, const char * argv[])
{
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //// Init glfw, create a window, and init glew
    
    // Init the GLFW Window
    window = initWindow();
    
    
    // Init the glew api
    initGlew();
    
	// Prepares some defaults
	CoordSystemRenderer* coordinate_system_renderer = new CoordSystemRenderer(10.0);

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //// The Shader Program starts here
    
    // Vertex shader source code. This draws the vertices in our window. We have 3 vertices since we're drawing an triangle.
    // Each vertex is represented by a vector of size 4 (x, y, z, w) coordinates.
    static const string vertex_code = vs_string;
	//static const string vertex_code = LoadFromFile("spotlight.vs");
    static const char * vs_source = vertex_code.c_str();
    
    // Fragment shader source code. This determines the colors in the fragment generated in the shader pipeline. In this case, it colors the inside of our triangle specified by our vertex shader.
    static const string fragment_code = fs_string;
	//static const string fragment_code = LoadFromFile("spotlight.fs");
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
    
    
    projectionMatrix = glm::perspective(1.1f, (float)800 / (float)600, 0.1f, 100.f);
    modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)); // Create our model matrix which will halve the size of our model
    viewMatrix = glm::lookAt(glm::vec3(1.0f, 0.0f, 15.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
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

	glm::vec4 lightPos = glm::vec4(0.0, 4.0, 5.0, 1.0);
	float ambient_intensity = 0.3;
	float specular_intensity = 10.0;
	float diffuse_intensity = 15.5;
	float attenuation_coeff = 0.02;
	float cutoff_angle = 90.0;

	float cone_angle = 90.0;
	glm::vec3 cone_direction = glm::vec3(0.0, 0.0, -1.0);

	int ambientIdx = glGetUniformLocation(program, "ambient_intensity");
	int diffuseIdx = glGetUniformLocation(program, "diffuse_intensity");
	int specularIdx = glGetUniformLocation(program, "specular_intensity");
	int attenuation_coeffIdx = glGetUniformLocation(program, "attenuationCoefficient");
	int cutoffAngleIdx = glGetUniformLocation(program, "cutoffAngle");
	int lightPosIdx = glGetUniformLocation(program, "light_position");
	int cone_angleIdx = glGetUniformLocation(program, "cone_angle");
	int cone_directionIdx = glGetUniformLocation(program, "cone_direction");

	glUniform1f(ambientIdx, ambient_intensity);
	glUniform1f(diffuseIdx, diffuse_intensity);
	glUniform1f(specularIdx, specular_intensity);
	glUniform1f(attenuation_coeffIdx, attenuation_coeff);
	glUniform1f(cutoffAngleIdx, cutoff_angle);
	glUniform4fv(lightPosIdx, 1, &lightPos[0]);

	glUniform1f(cone_angleIdx, cone_angle);
	glUniform3fv(cone_directionIdx, 1, &cone_direction[0]);


	// create two uniform variables
    glBindAttribLocation(program, 0, "in_Position");
    glBindAttribLocation(program, 1, "in_Normal");
	glBindAttribLocation(program, 2, "in_Color");
    
	    
    
	 //// The Shader Program ends here
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    
    // this creates the scene
    setupScene();
    
    int i=0;

    // Enable depth test
    // ignore this line, it allows us to keep the distance value after we proejct each object to a 2d canvas.
    glEnable(GL_DEPTH_TEST);
    
    // This is our render loop. As long as our window remains open (ESC is not pressed), we'll continue to render things.
    while(!glfwWindowShouldClose(window))
    {
        
        // Clear the entire buffer with our green color (sets the background to be green).
        glClearBufferfv(GL_COLOR , 0, clear_color);
        glClearBufferfv(GL_DEPTH , 0, clear_depth);
        
        // this draws the coordinate system
		coordinate_system_renderer->draw();
        
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
        modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-9.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]); // Send our model matrix to the shader
		lightPos = glm::vec4(-9.0, 1.0, 5.0, 1.0);
		glUniform4fv(lightPosIdx, 1, &lightPos[0]);

		makeMaterial(0);

		// This line renders your triangle strip model
        renderTriangleStripSphere();


		// This moves the model to the left
		modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-3.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]); // Send our model matrix to the shader
		lightPos = glm::vec4(-3.0, 1.0, 5.0, 1.0);
		glUniform4fv(lightPosIdx, 1, &lightPos[0]);

		makeMaterial(1);
		// This line renders your triangle strip model
		renderTriangleStripSphere();

		modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(3.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]); // Send our model matrix to the shader
		lightPos = glm::vec4(3.0, 1.0, 3.0, 1.0);
		glUniform4fv(lightPosIdx, 1, &lightPos[0]);

		makeMaterial(2);
		// This line renders your triangle strip model
		renderTriangleStripSphere();

		modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(9.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]); // Send our model matrix to the shader
		lightPos = glm::vec4(9.0, 1.0, 3.0, 1.0);
		glUniform4fv(lightPosIdx, 1, &lightPos[0]);

		makeMaterial(3);
		// This line renders your triangle strip model
		renderTriangleStripSphere();
        
        
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
    glDeleteVertexArrays(2, vaoID);
    glDeleteProgram(program);
}

