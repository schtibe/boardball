#version 130

varying vec4 ecPosition;
varying vec4 ecBallCenter;

void main() {

	vec4 specularColor = vec4(0.4, 0.4, 0.4, 6.0);
	vec4 surfColor = vec4(0.5, 0.5, 0.5, 1.0);
	vec4 normal = normalize(vec4(ecPosition.xyz - ecBallCenter.xyz, 1.0));

	float intensity = 0.2;
	vec4 lightPos = gl_ModelViewMatrix * gl_LightSource[0].position;
	vec4 halfVector = gl_LightSource[0].halfVector;
	vec3 lightDir = (lightPos - ecPosition).xyz;

	//intensity += 0.2 * clamp(dot(LightDir.xyz, normal.xyz), 0.0, 1.0);
	intensity += 0.2 * dot(lightDir, normal.xyz);
	surfColor *= intensity;

	intensity = 2 * dot(halfVector, normal) - 1;
	intensity = pow(intensity, specularColor.a);
	surfColor += specularColor * intensity * 0.1;

	gl_FragColor = vec4(surfColor.xyz, 1.0);
}
