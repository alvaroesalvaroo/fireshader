#pragma once

#include <iostream>
#include <ostream>

#include "glad/glad.h"

class Shader
{
public:

	Shader();
	~Shader();

	void init(const GLchar *filename);
	GLuint getID();
	void Use();
	void deleteProgram()	{ if (mAllOK) glDeleteProgram(mIDprogram); };
	bool AllOK() { return mAllOK; };


private:
	GLuint	mIDprogram;
	bool	mAllOK;

};

