#version 410 core
#define MAX_LIGHTS 10                                                 

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 inverseViewMatrix;
uniform mat4 modelMatrix;

uniform sampler2D tex;
uniform sampler2D bump_tex;
uniform int mode;

uniform vec3 diffuse_color;
uniform vec3 ambient_color;
uniform vec3 specular_color;
uniform float shininess;
uniform float transparency;
uniform float cutoffAngle;

in vec3 pass_Color;
in vec2 pass_Tex_Coord;
in vec3 pass_normal;
in vec4 pass_transformedNormal;
in vec4 pass_surfacePostion;
in float pass_shininess;

out vec4 color;

uniform int num_lights;

uniform struct Light{
	vec4 pos;
    vec4 diffuse;
    vec4 ambient;
    vec4 specular;
    float coeff;
    float angle;
    vec3 direction;
	float cutoff;
}all_lights[MAX_LIGHTS];

vec4 applyLight(Light light, vec3 normal, vec4 transformedNormal, vec4 surfacePostion){

	vec3 linearColor;

	vec4 surface_to_light =   normalize( light.pos -  surfacePostion );
	float diffuse_coefficient = max( dot(transformedNormal, surface_to_light), 0.0);
	vec3 out_diffuse_color = diffuse_color  * diffuse_coefficient * (light.diffuse.xyz * light.diffuse.w);
	vec3 out_ambient_color = vec3(ambient_color) * (light.ambient.xyz * light.ambient.w);

	vec3 incidenceVector = -surface_to_light.xyz;
	vec3 reflectionVector = reflect(incidenceVector, transformedNormal.xyz);

	vec3 cameraPosition = vec3( inverseViewMatrix[3][0], inverseViewMatrix[3][1], inverseViewMatrix[3][2]);
	vec3 surfaceToCamera = normalize(cameraPosition - surfacePostion.xyz);

	float cosAngle = max(0.0, dot(surfaceToCamera, reflectionVector));
	float specular_coefficient = pow(cosAngle, shininess);
	vec3 out_specular_color = specular_color * specular_coefficient * (light.specular.xyz * light.specular.w);

	float distanceToLight = length(light.pos.xyz - surfacePostion.xyz);
	float attenuation = 1.0 / (1.0 + light.coeff * pow(distanceToLight, 2));	
	
	vec3 cone_direction_norm = normalize(light.direction);
	vec3 ray_direction = -surface_to_light.xyz;
	float light_to_surface_angle = degrees(acos(dot(ray_direction, cone_direction_norm))) ;
	if(light_to_surface_angle > light.angle){ attenuation = 0.0; }
	else if(light_to_surface_angle > light.cutoff){
		float x = (light_to_surface_angle - light.cutoff)/(light.angle - light.cutoff);
		float spot = 1.0 - (x * x * (3 - 2 * x));
		attenuation = attenuation * spot;
	}
	
	linearColor = out_ambient_color  + attenuation * ( out_diffuse_color + out_specular_color);	 
	vec3 gamma = vec3(3.0/2.2);
	
	return vec4(linearColor, 1.0);
}

void main(void)                                                   
{            
	
	vec4 finalColor = vec4(0.0, 0.0, 0.0, 0.0);
	vec4 tex_displacement = vec4(0.0, 0.0, 0.0, 0.0);
	if(mode == -2){
		tex_displacement = texture(bump_tex, pass_Tex_Coord);
	}
	
	for(int i = 0; i < num_lights; i++){
		vec4 newLight = applyLight(all_lights[i], pass_normal + tex_displacement.xyz, pass_transformedNormal + tex_displacement, pass_surfacePostion);
		//vec4 newLight = applyLight(all_lights[i], pass_normal, pass_transformedNormal, pass_surfacePostion);
		finalColor = finalColor + newLight;
	}
	
	if(mode == -1){
		vec4 tex_col = texture(tex, pass_Tex_Coord);
		color = tex_col;// + vec4(pass_Color, 1.0);
		//color = vec4(pass_Color, 1.0);
		if(tex_col[0] == 0.0 && tex_col[1] == 0.0 && tex_col[2] == 0.0 ){
			color[3] = 0.0;
		}
		else{
			//color = tex_col * vec4(pass_Color, 1.0);
			color = tex_col * finalColor;
		}
	}
	else if(mode == -2){
		vec2 disp = tex_displacement.xy;
		//color = tex_displacement + vec4(pass_Color, transparency);
		color = (1.0 * finalColor);
		color[3] = transparency;
	}
	else{
		//color = vec4(pass_Color, transparency);
		color = finalColor;
		color[3] = transparency;
	}
} 