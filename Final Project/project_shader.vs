#version 410 core
#define MAX_LIGHTS 10

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 inverseViewMatrix;
uniform mat4 modelMatrix;

in vec3 in_Position;
in vec3 in_Normal;
//in vec3 in_Color;
in vec2 in_Tex_Coord;

uniform vec3 diffuse_color;
uniform vec3 ambient_color;
uniform vec3 specular_color;
uniform float shininess;
uniform float transparency;

uniform vec4 light_position;
uniform float cone_angle;
uniform vec3 cone_direction;
uniform float diffuse_intensity;
uniform float ambient_intensity;
uniform float specular_intensity;

uniform float attenuationCoefficient;
uniform float cutoffAngle;

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

out vec3 pass_Color;
out vec2 pass_Tex_Coord;
out vec3 pass_normal;
out vec4 pass_transformedNormal;
out vec4 pass_surfacePostion;
out float pass_shininess;

void main(void)
{
	vec3 normal = normalize(in_Normal);
	vec4 transformedNormal = normalize(transpose(inverse( modelMatrix)) * vec4( normal, 1.0 ));
	vec4 surfacePostion = modelMatrix * vec4(in_Position, 1.0);
	//vec4 surface_to_light =   normalize( light_position -  surfacePostion );
	 
	vec3 finalColor = vec3(0.0, 0.0, 0.0);
	 
	for(int i = 0; i < num_lights; i++){
	 
		vec4 surface_to_light =   normalize( all_lights[i].pos -  surfacePostion );
		float diffuse_coefficient = max( dot(transformedNormal, surface_to_light), 0.0);
		//vec3 out_diffuse_color = diffuse_color  * diffuse_coefficient * diffuse_intensity;
		vec3 out_diffuse_color = diffuse_color  * diffuse_coefficient * (all_lights[i].diffuse.xyz * all_lights[i].diffuse.w);
		//vec3 out_ambient_color = vec3(ambient_color) * ambient_intensity;
		vec3 out_ambient_color = vec3(ambient_color) * (all_lights[i].ambient.xyz * all_lights[i].ambient.w);

		vec3 incidenceVector = -surface_to_light.xyz;
		vec3 reflectionVector = reflect(incidenceVector, transformedNormal.xyz);

		vec3 cameraPosition = vec3( inverseViewMatrix[3][0], inverseViewMatrix[3][1], inverseViewMatrix[3][2]);
		vec3 surfaceToCamera = normalize(cameraPosition - surfacePostion.xyz);

		float cosAngle = max(0.0, dot(surfaceToCamera, reflectionVector));
		float specular_coefficient = pow(cosAngle, shininess);
		//vec3 out_specular_color = specular_color * specular_coefficient * specular_intensity;
		vec3 out_specular_color = specular_color * specular_coefficient * (all_lights[i].specular.xyz * all_lights[i].specular.w);

		//float distanceToLight = length(light_position.xyz - surfacePostion.xyz);
		float distanceToLight = length(all_lights[i].pos.xyz - surfacePostion.xyz);
		float attenuation = 1.0 / (1.0 + all_lights[i].coeff * pow(distanceToLight, 2));	
		//float attenuation = 1.0 / (1.0 + attenuationCoefficient * pow(distanceToLight, 2));
		
		vec3 cone_direction_norm = normalize(all_lights[i].direction);
		vec3 ray_direction = -surface_to_light.xyz;
		float light_to_surface_angle = degrees(acos(dot(ray_direction, cone_direction_norm))) ;
		//if(light_to_surface_angle > cone_angle){ attenuation = 0.0; }
		if(light_to_surface_angle > all_lights[i].angle){ attenuation = 0.0; }
		else if(light_to_surface_angle > cutoffAngle){
			//float x = (light_to_surface_angle - cutoffAngle)/(cone_angle - cutoffAngle);
			float x = (light_to_surface_angle - cutoffAngle)/(all_lights[i].angle - cutoffAngle);
			float spot = 1.0 - (x * x * (3 - 2 * x));
			attenuation = attenuation * spot;
		}
	
		vec3 linearColor = out_ambient_color  + attenuation * ( out_diffuse_color + out_specular_color);	 
		vec3 gamma = vec3(3.0/2.2);
		finalColor = finalColor + pow(linearColor, gamma);
	}

    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(in_Position, 1.0);
	pass_Tex_Coord = in_Tex_Coord;
    pass_Color = finalColor;
	pass_normal = normal;
	pass_transformedNormal = transformedNormal;
	pass_shininess = shininess;
	pass_surfacePostion = surfacePostion;
}