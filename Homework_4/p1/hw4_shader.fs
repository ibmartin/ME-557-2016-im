#version 410 core                                                 
                                                                  
uniform sampler2D tex;																  
	
in vec2 pass_Tex_Coord;	
in vec3 pass_Color;                                             
out vec4 color;                                                   
void main(void)                                                   
{   
	vec4 tex_color = texture(tex, pass_Tex_Coord);
	//color = vec4(pass_Color, 1.0);
    color = tex_color;
} 