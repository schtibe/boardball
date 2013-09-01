#version 130

uniform vec3 ballPosition;


varying vec4 ecPosition;
varying vec4 ecBallCenter;

void main() {
	float scaleFactor = 0.3;
	mat4 scale = mat4(
			scaleFactor, 0, 0, 0,
			0, scaleFactor, 0, 0,
			0, 0, scaleFactor, 0,
			0, 0, 0, 1
	);

	mat4 translate = mat4(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		ballPosition.x, ballPosition.y, ballPosition.z, 1
	);

	vec4 vertex  = (translate * scale) * gl_Vertex;// + vec4(4, -4, 0.5, 0);
	ecBallCenter = gl_ModelViewMatrix * vec4(ballPosition, 1.0);
	ecPosition = gl_ModelViewMatrix  * vertex;

	gl_Position = gl_ModelViewProjectionMatrix * vertex;
}
