#version 400

in vec3 norm;
in vec3 frag;
in float mat_id;

out vec4 color;

uniform vec3 viewPos;

void main(){

	float specStrength = 0.0;

	vec3 light = vec3(0.0, 2.0, -0.8);
	vec3 lightDir = normalize(light - frag);
	float diff = 0.9 * max(0.0, dot(norm, lightDir));
	
	vec3 shape_color = vec3(0.8, 0.8, 0.1);

	vec3 viewDir = normalize(viewPos - frag);
	vec3 reflectDir = reflect(lightDir, norm);

	if(mat_id == 1){ shape_color = vec3(1.0, 0.0, 0.3); specStrength = 0.8;}
	else if(mat_id == 6) {shape_color = vec3(0.2); specStrength = 0.99;}
	else if(mat_id == 11) shape_color = vec3(0.1);

	vec3 specular = specStrength * pow(max(0.0, dot(viewDir, reflectDir)), 32) * vec3(1.0);

	vec3 col = (0.1 + diff) * shape_color + specular; 
	
	color = vec4(col, 1.0);
}