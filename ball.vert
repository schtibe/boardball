#version 130

uniform vec3 ballPosition;
uniform float time;
uniform vec3 speed;

varying vec4 ecPosition;
varying vec4 ecBallCenter;

mat4 scaleMatrix() {
	float scaleFactor = 0.25;
	mat4 scale = mat4(
			scaleFactor, 0, 0, 0,
			0, scaleFactor, 0, 0,
			0, 0, scaleFactor, 0,
			0, 0, 0, 1
	);

	return scale;
}

mat4 translationMatrix() {
	mat4 translate = mat4(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		ballPosition.x, ballPosition.y, ballPosition.z, 1
	);

	return translate;
}

mat4 rotationMatrix() {
	float xAngle = 1;
	float yAngle = 0;
	float zAngle = 0;

	mat4 xRotation = mat4(
		1 , 0           , 0            , 0 ,
		0 , cos(xAngle) , -sin(xAngle) , 0 ,
		0 , sin(xAngle) , cos(xAngle)  , 0 ,
		0 , 0           , 0            , 1
	);

	mat4 yRotation = mat4(
		cos(yAngle)  , 0 , sin(yAngle) , 0 ,
		0            , 1 , 0           , 0 ,
		-sin(yAngle) , 0 , cos(yAngle) , 0 ,
		0            , 0 , 0           , 1
	);

	mat4 zRotation = mat4(
		cos(zAngle) , -sin(zAngle) , 0 , 0 ,
		sin(zAngle) , cos(zAngle)  , 0 , 0 ,
		0           , 0            , 1 , 0 ,
		0           , 0            , 0 , 1
	);

	return xRotation * yRotation * zRotation;
}

void main() {
	mat4 scale     = scaleMatrix();
	mat4 translate = translationMatrix();
	mat4 rotate    = rotationMatrix();

	vec4 vertex  = (translate * scale * rotate) * gl_Vertex;// + vec4(4, -4, 0.5, 0);

	ecBallCenter = gl_ModelViewMatrix * vec4(ballPosition, 1.0);
	ecPosition = gl_ModelViewMatrix  * vertex;

	gl_Position = gl_ModelViewProjectionMatrix * vertex;
}
