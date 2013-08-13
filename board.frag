#version 130

uniform sampler2D tex0;

varying float LightIntensity;

varying vec2 texCoord;

void main() {

	vec3 texColor = vec3(texture2D(tex0, texCoord.st));

	//gl_FragColor = vec4(gl_Color.xyz, 1.0);
	gl_FragColor = vec4(gl_Color.xyz * texColor, 1.0);
}


