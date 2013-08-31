#version 130

varying float LightIntensity;
varying vec3 normal;

void main() {

	float intensity;
	intensity = clamp(LightIntensity, 0.5, 1.5);

	gl_FragColor = vec4(intensity * vec3(1.0, 0, 0), 1.0);

}
