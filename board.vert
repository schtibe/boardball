#version 130

#pragma debug(on)

attribute vec3 vertex_normal;
attribute vec2 texel;

uniform sampler2D tex0;

uniform mat4 transformation;
varying float LightIntensity;

varying vec2 texCoord;

const float specularContribution = 0.1;
const float diffuseContribution = 1.0 - specularContribution;

void DirectionalLight(in int i,
		in vec3 normal,
		inout vec4 ambient,
		inout vec4 diffuse,
		inout vec4 specular)
{
	float nDotVP; // normal . light direction
	float nDotHV; // normal . light half vector
	float pf; // power factor
	nDotVP = max(0.0, dot(normal, normalize(vec3(gl_ModelViewMatrix * gl_LightSource[i].position))));
	nDotHV = max(0.0, dot(normal, vec3(gl_LightSource[i].halfVector)));
	if (nDotVP == 0.0)
		pf = 0.0;
	else
		pf = pow(nDotHV, gl_FrontMaterial.shininess);

	ambient  += gl_LightSource[i].ambient;
	diffuse  += gl_LightSource[i].diffuse * nDotVP;
	specular += gl_LightSource[i].specular * pf;
}

void PointLight(in int i,
		in vec3 eye,
		in vec3 ecPosition33,
		in vec3 normal,
		inout vec4 ambient,
		inout vec4 diffuse,
		inout vec4 specular)
{
	float nDotVP; // normal . light direction
	float nDotHV; // normal . light half vector
	float pf; // power factor
	float attenuation; // computed attenuation factor
	float d; // distance from surface to light source
	vec3 VP; // direction from surface to light position
	vec3 halfVector; // direction of maximum highlights
	// Compute vector from surface to light position
	VP = vec3(gl_ModelViewMatrix * gl_LightSource[i].position) - ecPosition33;
	// Compute distance between surface and light position
	d = length(VP);
	// Normalize the vector from surface to light position
	VP = normalize(VP);
	// Compute attenuation
	attenuation = 1.0 / (gl_LightSource[i].constantAttenuation +
			gl_LightSource[i].linearAttenuation * d +
			gl_LightSource[i].quadraticAttenuation * d * d);
	halfVector = normalize(VP + eye);

	nDotVP = max(0.0, dot(normal, VP));
	nDotHV = max(0.0, dot(normal, halfVector));
	if (nDotVP == 0.0)
		pf = 0.0;
	else
		pf = pow(nDotHV, gl_FrontMaterial.shininess);
	ambient += gl_LightSource[i].ambient * attenuation;
	diffuse += gl_LightSource[i].diffuse * nDotVP * attenuation;
	specular += gl_LightSource[i].specular * pf * attenuation;
}

void SpotLight(in int i,
		in vec3 eye,
		in vec3 ecPosition33,
		in vec3 normal,
		inout vec4 ambient,
		inout vec4 diffuse,
		inout vec4 specular)
{
	float nDotVP; // normal . light direction
	float nDotHV; // normal . light half vector
	float pf; // power factor
	float spotDot; // cosine of angle between spotlight
	float spotAttenuation; // spotlight attenuation factor
	float attenuation; // computed attenuation factor
	float d; // distance from surface to light source
	vec3 VP; // direction from surface to light position
	vec3 halfVector; // direction of maximum highlights
	// Compute vector from surface to light position
	VP = vec3(gl_ModelViewMatrix * gl_LightSource[i].position) - ecPosition33;
	// Compute distance between surface and light position
	d = length(VP);
	// Normalize the vector from surface to light position
	VP = normalize(VP);
	// Compute attenuation
	attenuation = 1.0 / (gl_LightSource[i].constantAttenuation +
			gl_LightSource[i].linearAttenuation * d +
			gl_LightSource[i].quadraticAttenuation * d * d);
	// See if point on surface is inside cone of illumination
	spotDot = dot(-VP, normalize(gl_LightSource[i].spotDirection));
	if (spotDot < gl_LightSource[i].spotCosCutoff)
		spotAttenuation = 0.0; // light adds no contribution
	else
		spotAttenuation = pow(spotDot, gl_LightSource[i].spotExponent);
	// Combine the spotlight and distance attenuation.
	attenuation *= spotAttenuation;
	halfVector = normalize(VP + eye);
	nDotVP = max(0.0, dot(normal, VP));
	nDotHV = max(0.0, dot(normal, halfVector));
	if (nDotVP == 0.0)
		pf = 0.0;
	else
		pf = pow(nDotHV, gl_FrontMaterial.shininess);

	ambient += gl_LightSource[i].ambient * attenuation;
	diffuse += gl_LightSource[i].diffuse * nDotVP * attenuation;
	specular += gl_LightSource[i].specular * pf * attenuation;
}

const float pi = 3.14159;
const float angle = -(pi / 2);
mat4 rotate = mat4(
				   1.0  , 0.0         , 0.0        , 0.0 ,
				   0.0  , cos(angle)  , sin(angle) , 0   ,
				   0    , -sin(angle) , cos(angle) , 0   ,
			       0    , 0           , 0          , 1   
				   );

void main() {
	vec3 ecPosition3 = vec3(gl_ModelViewMatrix *(transformation * gl_Vertex));

	vec4 transformedNormal = transformation * vec4(vertex_normal, 0.0);
	vec3 normal = normalize(
			vec3(gl_ModelViewMatrix * transformedNormal)
	);

	vec4 ambient = vec4(0.0);
	vec4 diffuse = vec4(0.0);
	vec4 specular = vec4(0.0);

	DirectionalLight(0, normal, ambient, diffuse, specular);
	PointLight(0, vec3(0.0, 0.0, 1.0) , ecPosition3, normal, ambient, diffuse, specular);
	SpotLight(0, vec3(0.0, 0.0, 1.0), ecPosition3, normal, ambient, diffuse, specular);

	vec4 color = gl_FrontLightModelProduct.sceneColor +
		ambient * gl_FrontMaterial.ambient +
		diffuse * gl_FrontMaterial.diffuse;

	gl_FrontColor = color;
	gl_FrontSecondaryColor = specular * gl_FrontMaterial.specular;


	/*
	vec3 LightPosition = vec3(gl_ModelViewMatrix * gl_LightSource[0].position);


	vec3 lightVec   = normalize(LightPosition - ecPosition3);
	vec3 reflectVec = reflect(-lightVec, normal);
	vec3 viewVec    = normalize(-ecPosition3);
	float spec      = clamp(dot(reflectVec, viewVec), 0.0, 1.0);
	spec            = pow(spec, 16.0);

	LightIntensity = diffuseContribution * max(dot(lightVec, normal), 0.0)
		+ specularContribution * spec;
	*/

	texCoord = texel;

	gl_Position = gl_ModelViewProjectionMatrix * (transformation * gl_Vertex);
}


