#version 130

uniform vec3 ballPosition;

varying float LightIntensity;

const float specularContribution = 0.1;
const float diffuseContribution = 1.0 - specularContribution;

void main() {
	vec3 normal = gl_Vertex.xyz - ballPosition;
	vec3 ecPosition3 = vec3(gl_ModelViewMatrix  * gl_Vertex);

	vec3 LightPosition = vec3(gl_ModelViewMatrix * gl_LightSource[0].position);

	vec3 lightVec   = normalize(LightPosition - ecPosition3);
	vec3 reflectVec = reflect(-lightVec, normal);
	vec3 viewVec    = normalize(-ecPosition3);
	float spec      = clamp(dot(reflectVec, viewVec), 0.0, 1.0);
	spec            = pow(spec, 16.0);

	LightIntensity = diffuseContribution * max(dot(lightVec, normal), 0.0)
		+ specularContribution * spec;

	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}
