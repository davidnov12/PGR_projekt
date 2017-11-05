#version 400

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 nor;
layout (location = 2) in vec2 uvs;
layout (location = 3) in float mid;

uniform mat4 trans;
uniform mat4 model;

out vec3 norm;
out vec3 frag;
out float mat_id;

void main(){
	mat_id = mid;
	norm = normalize(mat3(transpose(inverse(trans))) * nor);
	frag = vec3(trans * vec4(pos, 1.0));
	gl_Position = model * vec4(pos, 1.0);
}