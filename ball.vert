#version 130

uniform vec3 ballPosition;

varying float LightIntensity;

const float specularContribution = 0.1;
const float diffuseContribution = 1.0 - specularContribution;

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
		0, 0, 0, 1
	);

	vec4 vertex  = (translate * scale) * gl_Vertex;// + vec4(4, -4, 0.5, 0);

	vec3 normal = gl_Vertex.xyz - ballPosition;
	vec3 ecPosition3 = vec3(gl_ModelViewMatrix  * vertex);

	vec3 LightPosition = vec3(gl_ModelViewMatrix * gl_LightSource[0].position);

	vec3 lightVec   = normalize(LightPosition - ecPosition3);
	vec3 reflectVec = reflect(-lightVec, normal);
	vec3 viewVec    = normalize(-ecPosition3);
	float spec      = clamp(dot(reflectVec, viewVec), 0.0, 1.0);
	spec            = pow(spec, 16.0);

	LightIntensity = diffuseContribution * max(dot(lightVec, normal), 0.0)
		+ specularContribution * spec;

	gl_Position = gl_ModelViewProjectionMatrix * vertex;
}
