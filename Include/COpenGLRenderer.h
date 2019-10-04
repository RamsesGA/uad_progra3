#pragma once

#ifndef _OPENGL_RENDERER_H
#define _OPENGL_RENDERER_H

// include glad *before* glfw
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "MathHelper.h"
#include "COpenGLShaderProgram.h"

#include <map>
#include <vector>
using namespace std;

#define BUFFER_OFFSET(a) ((void*)(a))
#define MIN_CAMERA_DISTANCE 5.0f
#define MAX_CAMERA_DISTANCE 1000.0f
#define MOVE_CAMERA_DELTA 1.5f
#define RENDERER_DEFAULT_FB_WIDTH 800
#define RENDERER_DEFAULT_FB_HEIGHT 600

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* ONLY MODIFY THIS FILE IF YOU KNOW WHAT YOU'RE DOING...
* AND EVEN THEN... BE CAREFUL...
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// =========================================================================================================
// Class for a simple OpenGL renderer targeted for OpenGL 4.3
// UAD lab PCs support it
// *NOTE: This code has been tested to work fine on NVidia cards. Radeon cards seem to behave differently...
// =========================================================================================================

class COpenGLRenderer
{
public:
	enum EPRIMITIVE_MODE
	{
		POINTS = 0,
		LINES,
		LINE_STRIP,
		LINE_LOOP,
		TRIANGLES,
		TRIANGLE_STRIP,
		TRIANGLE_FAN
	};

	enum OPENGL_BLEND_MODE
	{
		BLEND_ZERO = 0, 
		BLEND_ONE, 
		BLEND_SRC_COLOR, 
		BLEND_ONE_MINUS_SRC_COLOR, 
		BLEND_DST_COLOR, 
		BLEND_ONE_MINUS_DST_COLOR, 
		BLEND_SRC_ALPHA, 
		BLEND_ONE_MINUS_SRC_ALPHA, 
		BLEND_DST_ALPHA, 
		BLEND_ONE_MINUS_DST_ALPHA,
		BLEND_CONSTANT_COLOR, 
		BLEND_ONE_MINUS_CONSTANT_COLOR, 
		BLEND_CONSTANT_ALPHA, 
		BLEND_ONE_MINUS_CONSTANT_ALPHA
	};

private:
	int m_frameBufferWidth;
	int m_frameBufferHeight;
	bool m_OpenGLError;

	std::vector<std::string> m_knownShaderProgramNames;
	std::vector<std::string> m_knownShaderProgramVertexShaders;
	std::vector<std::string> m_knownShaderProgramFragmentShaders;
	std::map<std::string, unsigned int> m_knownShaders;
	std::map<unsigned int, COpenGLShaderProgram*> m_shaderProgramWrappers;
	std::vector<std::string> m_expectedUniformsInShader;
	std::vector<std::string> m_expectedAttributesInShader;

	float m_cameraDistance; // Distance from camera view point to target point, expressed in OpenGL units

	const unsigned int *m_activeShaderProgram;
	const COpenGLShaderProgram *m_activeShaderProgramWrapper;

	// TEST OBJECT VARS
	// When no 3D object is loaded, we display a test object (spinning cube)
	// ===========================
	GLuint m_testCubeShaderProgramID;
	GLuint m_testCubeVAOID;

	GLuint m_mCCubeShaderProgramID;
	GLuint m_mCCubeVAOID;
	// ===========================

	//
	void setCurrentVertexAttribPointer(int vertexShaderAttribPos, int numComponents, int type) const;

	// This method is internally called by allocateGraphicsMemoryForObject(), (that's why it's a private method).
	//
	// There's no easy way to render multi-indexed arrays (one index for vertex, one index for normal, one index for UV coords... but that's how the info
	// is saved in the .obj format), 
	// so this method duplicates some vertices in order to render correctly.
	// The geometry created by this method is used to allocate the graphics card memory.
	// The caller can discard the geometry if it's not going to be used for anything else...
	bool generateRenderGeometry(
		GLfloat *vertices, int numVertices,
		GLfloat *normals, int numNormals,
		GLfloat *UVcoords, int numUVCoords,
		int numIndicesVert,
		unsigned short *indicesVertices,
		unsigned short *indicesNormals, 
		unsigned short *indicesUVCoords, 
		GLfloat *finalVertices,
		GLfloat *finalNormals,
		GLfloat *finalUVCoords,
		int *numTriangles
	);

	//
	GLuint generateVertexArrayObjectID() const;

	//
	GLuint generateBufferObjectID(GLfloat * const vertices, int numVertices) const;

	//
	void deleteBufferObject(GLuint *id);

	//
	void deleteVertexArrayObject(GLuint *id);

	//
	GLenum primitiveModeToGLEnum(EPRIMITIVE_MODE mode) const;

	//
	COpenGLShaderProgram* getShaderProgramWrapper(unsigned int id);

	//
	bool deleteShaderProgram(unsigned int *shaderProgramId);

	//
	GLenum translateBlendMode(OPENGL_BLEND_MODE factor);

	//
	float GET_ABS(float x) { return x > 0.0f ? x : -x; }

	//
	void getLineParameters(
		float *x1, float *y1, float *x2, float *y2, // coordinates of the line
		float *Cr, float *Cg, float *Cb, float *Br,
		float w,
		bool alphablend,
		float *A,
		float *tx, float *ty,     // core thinkness of a line
		float *Rx, float *Ry,     // fading edge of a line
		float *cx, float *cy      // cap of a line
	);

public:
	// Constructor and Destructor
	COpenGLRenderer();
	~COpenGLRenderer();

	// =================================================================
	// Initialize known shaders
	// Must be called after creating the window
	// =================================================================
	bool initialize();

	// =================================================================
	// Allocates graphics memory for a given 3D object 
	// Note: shader program must be already created
	// =================================================================
	bool allocateGraphicsMemoryForObject(
		unsigned int *shaderProgramId,
		unsigned int *vertexArrayObjectID, 
		GLfloat *vertices, int numVertices,
		GLfloat *normals, int numNormals,
		GLfloat *UVcoords, int numUVCoords,
		unsigned short *indicesVertices, int numIndicesVert,
		unsigned short *indicesNormals, int numIndicesNormals,
		unsigned short *indicesUVCoords, int numIndicesUVCoords);

	// =================================================================
	// Allocates graphics memory for a given 3D object
	// Note: shader program must be already created
	// =================================================================
	bool allocateGraphicsMemoryForObject(
		const unsigned int * const shaderProgramId,
		unsigned int *vertexArrayObjectID,
		GLfloat *vertices, int numVertices,
		unsigned short *indicesVertices, int numIndicesVert);

	// =================================================================
	// Free graphics memory for a given 3D object 
	// =================================================================
	bool freeGraphicsMemoryForObject(
		//unsigned int *shaderProgramId, 
		unsigned int *vertexArrayObjectID);

	// =================================================================
	// Allocates graphics memory for a menu item (a quad)
	// We don't allocate the memory for all menu items at once because
	// we change the color (GLSL uniform) for the currently selected menu item
	// =================================================================
	bool allocateGraphicsMemoryForMenuItem(
		float topX, float topY, float menuItemHeight,
		float *uvCoords,
		unsigned int *shaderProgramId,
		unsigned int *vertexArrayObjectID
	);

	//
	bool createShaderProgram(
		std::string shaderProgramName,
		unsigned int *shaderProgramId,
		const char *vertexShader,
		const char *fragmentShader);

	//
	unsigned int getShaderProgramID(std::string shaderProgramName);

	//
	bool createTextureObject(
		unsigned int *textureObjectId, 
		unsigned char *textureData,
		int width, 
		int height);

	// 
	bool renderWireframeObject(
		unsigned int *shaderProgramId,
		unsigned int *vertexArrayObjectId,
		int numFaces,
		GLfloat *objectColor,
		MathHelper::Matrix4 *objectTransformation);

	// 
	bool renderObject(
		unsigned int *shaderProgramId,
		unsigned int *vertexArrayObjectId,
		unsigned int *textureObjectId,
		int numFaces,
		GLfloat *objectColor,
		MathHelper::Matrix4 *objectTransformation = NULL,
		EPRIMITIVE_MODE mode = TRIANGLES,
		bool drawIndexedPrimitives = false);

	// 
	bool renderObject(
		unsigned int *shaderProgramId,
		unsigned int *vertexArrayObjectId,
		unsigned int *textureObjectId,
		int numFaces,
		GLfloat *objectColor,
		MathHelper::Matrix4 *modelMatrix,
		MathHelper::Matrix4 *viewMatrix,
		MathHelper::Matrix4 *projectionMatrix,
		EPRIMITIVE_MODE mode = TRIANGLES,
		bool drawIndexedPrimitives = false);

	// 
	bool renderObjectNew(
		int numFaces,
		MathHelper::Matrix4 *modelMatrix,
		EPRIMITIVE_MODE mode = TRIANGLES,
		bool drawIndexedPrimitives = false);

	//
	bool useShaderProgram(const unsigned int * const shaderProgramId);

	//
	void setCurrentVertexArrayObjectID(unsigned int *vertexArrayObjectId);

	//
	void setCurrentShaderObjectColor(GLfloat *objectColor);

	//
	void setCurrentShaderViewMatrix(MathHelper::Matrix4 *viewMatrix);

	// 
	void setCurrentShaderProjectionMatrix(MathHelper::Matrix4 *projectionMatrix);

	//
	void setCurrentShaderTexture(unsigned int *textureObjectId);

	//
	void setCurrentShaderAmbientLight(GLfloat red, GLfloat green, GLfloat blue, GLfloat ambientIntensity);

	/*
	 * With vertex attributes, each run of the vertex shader will cause GLSL to retrieve the next set of vertex attributes that belong to the current vertex. 
	 * When defining a vertex attribute as an instanced array however, the vertex shader only updates the content of the vertex attribute per instance instead of per vertex. 
	 * This allows us to use the standard vertex attributes for data per vertex and use the instanced array for storing data that is unique per instance.
	 */
	//
	bool setInstancedOffsetPositions(unsigned int *shaderProgramId);

	bool setInstancedModelViewMatrices(unsigned int *shaderProgramId);

	/*
	https://learnopengl.com/Advanced-OpenGL/Instancing
	https://learnopengl.com/code_viewer_gh.php?code=src/4.advanced_opengl/10.1.instancing_quads/instancing_quads.cpp
	http://www.opengl-tutorial.org/intermediate-tutorials/billboards-particles/particles-instancing/
	https://learnopengl.com/code_viewer_gh.php?code=includes/learnopengl/shader.h
	no longer use gl_InstanceID and can directly use the offset attribute without first indexing into a large uniform array.

Because an instanced array is a vertex attribute, just like the position and color variables, we also need to store its content in a vertex buffer object and configure its attribute pointer. We're first going to store the translations array (from the previous section) in a new buffer object:


unsigned int instanceVBO;
glGenBuffers(1, &instanceVBO);
glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * 100, &translations[0], GL_STATIC_DRAW);
glBindBuffer(GL_ARRAY_BUFFER, 0); 
Then we also need to set its vertex attribute pointer and enable the vertex attribute:


glEnableVertexAttribArray(2);
glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
glBindBuffer(GL_ARRAY_BUFFER, 0);	
glVertexAttribDivisor(2, 1);  
What makes this code interesting is the last line where we call glVertexAttribDivisor. This function tells OpenGL when to update the content of a vertex attribute to the next element. Its first parameter is the vertex attribute in question and the second parameter the attribute divisor. By default the attribute divisor is 0 which tells OpenGL to update the content of the vertex attribute each iteration of the vertex shader. By setting this attribute to 1 we're telling OpenGL that we want to update the content of the vertex attribute when we start to render a new instance. By setting it to 2 we'd update the content every 2 instances and so on. By setting the attribute divisor to 1 we're effectively telling OpenGL that the vertex attribute at attribute location 2 is an instanced array.

If we now were to render the quads again using glDrawArraysInstanced we'd get the following outpu
	*/

	//
	bool renderMenuItem(
		unsigned int *shaderProgramId, 
		unsigned int *textureObjectId,
		unsigned int *vertexArrayObjectId,
		GLfloat *menuItemColor);

	//
	void setFramebufferWidth(int width) { m_frameBufferWidth = width; }
	void setFramebufferHeight(int height) { m_frameBufferHeight = height; }
	int getFramebufferWidth() const { return m_frameBufferWidth; }
	int getFramebufferHeight() const { return m_frameBufferHeight; }

	//
	void initializeColorCube();
	//
	void renderColorCube(MathHelper::Matrix4 *objectTransformation = NULL);
	void renderColorCube(
		MathHelper::Matrix4 *modelMatrix, 
		MathHelper::Matrix4 *viewMatrix, 
		MathHelper::Matrix4 *projectionMatrix );
	
	//
	void initializeTexturedCube();
	//
	void renderTexturedCube(unsigned int cubeTextureID, MathHelper::Matrix4 *objectTransformation = NULL);
	void renderTexturedCube(
		unsigned int cubeTextureID, 
		MathHelper::Matrix4 *modelMatrix,
		MathHelper::Matrix4 *viewMatrix,
		MathHelper::Matrix4 *projectionMatrix);

	//
	bool checkOpenGLError(char *operationAttempted);

	//
	void setClearScreenColor(float r, float g, float b) { glClearColor(r, g, b, 1.0f); }

	//
	void deleteTexture(unsigned int *id);

	//
	float getCameraDistance() const { return m_cameraDistance; }
	void setCameraDistance(float d) { m_cameraDistance = d; }

	//
	void simpleCameraZoom(float direction);

	//
	void enableDepthTest() { glEnable(GL_DEPTH_TEST); }

	//
	void setViewport(int width, int height) { glViewport(0, 0, width, height); }

	//
	const GLubyte * getOpenGLString(GLenum id) { return glGetString(id); }

	//
	void clearScreen() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }

	//
	void drawString(unsigned int *textureObjectId, std::string &text, float x, float y, CVector3 &color);

	//
	bool isDebugContextEnabled() const;

	//
	void activateOpenGLDebugging();

	// 
	void setWireframePolygonMode() { glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);	}

	// 
	void setFillPolygonMode() { glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); }

	//
	void startProfiling();

	//
	void endProfiling();

	//
	GLuint64 getProfilingTime();

	//
	void enableBlending(bool enable) { if (enable)glEnable(GL_BLEND); else glDisable(GL_BLEND); }

	//
	void setBlendingMode(OPENGL_BLEND_MODE sourceFactor, OPENGL_BLEND_MODE destinationFactor);

	// Based off of
	// https://www.codeproject.com/Articles/199525/Drawing-nearly-perfect-D-line-segments-in-OpenGL
	// http://artgrammer.blogspot.com/2011/05/drawing-nearly-perfect-2d-line-segments.html
	// http://www.codeproject.com/KB/openGL/gllinedraw.aspx
	// By.Chris Tsang.
	//
	void allocateGraphicsMemoryForLines(
		const unsigned int * const shaderProgramId,
		unsigned int *vertexArrayObjectID,
		GLfloat *lineVertices, int numLines,    // Coordinates (x1, y1, z1), (x2, y2, z2) of each line. 6 floats per line
		float lineWidth,                        // Width/thickness of the line in pixel
		float red, float green, float blue,     // RGB color components
		float backR, float backG, float backB,  // Color of background when alphablend=false,  Br=alpha of color when alphablend=true
		float cAlpha,                           // Alpha 
		bool alphablend);                       // Alpha blend?

	//
	static void APIENTRY debugOutputCallback(
		GLenum source, 
		GLenum type, 
		GLuint id, 
		GLenum severity,
		GLsizei length, 
		const GLchar *message, 
		const GLvoid *userParam);

};

#endif // !_OPENGL_RENDERER_H

// https://www.khronos.org/opengl/wiki/Common_Mistakes