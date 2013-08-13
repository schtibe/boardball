#ifndef __SHADER_HPP

#define __SHADER_HPP

#include <string>
#include <GL/gl.h>
#include <stdexcept>

using std::string;
using std::logic_error;

class Shader {
	public:
		static GLuint loadShaders(string vertexShaderFile, string fragmentShaderFile);

	private:
		static string loadShaderFile(string file);
		static void compileShader(GLuint shaderID, string shaderCode) throw (logic_error);
		static void checkShader(GLuint) throw (logic_error);
		static void checkProgram(GLuint) throw (logic_error);
};

#endif /* end of include guard: __SHADER_HPP */
