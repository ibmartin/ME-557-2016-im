#version 410 core                                                 
                                                                  
uniform sampler2D tex;
uniform sampler2D tex2;
uniform sampler2D tex3;																  
	
in vec2 pass_Tex_Coord;	
in vec3 pass_Color;                                             
out vec4 color;                                                   
void main(void)                                                   
{   
	vec4 tex_col = texture(tex, pass_Tex_Coord);
	vec4 tex2_col = texture(tex2, pass_Tex_Coord);
	vec4 tex3_col = texture(tex3, pass_Tex_Coord);
	vec4 tex_color = vec4(0.0,0.0,0.0,0.0);
	
	float px_avg = (tex_col.x + tex_col.y + tex_col.z) / 3.0;
	
	if(tex_col.x >= 0.1 || tex_col.y >= 0.1 || tex_col.z >= 0.1){
		tex_color = 1 - ((1 - tex_col) * (1 - tex2_col));
	}
	else {
		tex_color = tex_col * tex2_col;
	}
	
	tex_color = tex_color - 0.1 * tex3_col;
	//color = vec4(pass_Color, 1.0);
    color = tex_color;
} 