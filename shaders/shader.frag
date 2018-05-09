#ifdef GL_ES
// define default precision for float, vec, mat.
precision highp float;
#endif

in vec4 position;

uniform vec3 reference;

void main (void) {
	vec4 pos = position;

	//distance < x = red not working
	if(distance(reference, pos.xyz) < 5.0){
		gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
	} else if(distance(reference, pos.xyz) < 10.0) {
		gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);
	} else {
		gl_FragColor = vec4(1.0, 1.0, 1.0, 0.5);
	}
    
}