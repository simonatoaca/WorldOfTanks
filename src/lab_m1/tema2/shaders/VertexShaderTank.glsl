#version 330

// Input
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec3 v_tex_coords;
layout(location = 3) in vec3 v_color;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform vec3 Color;
uniform int HP;

// Output
out vec3 frag_position;
out vec3 frag_normal;
out vec3 frag_tex_coords;
out vec3 frag_color;
flat out int frag_HP;

void main()
{
    vec3 new_pos = v_position + sin(3.14 / 4 * (3 - HP)) * (vec3(1, 1, 1) - v_normal) / 10; // DECENT

    frag_position =  new_pos;
    frag_normal = v_normal;
    frag_tex_coords = v_tex_coords;
    frag_color = Color;
    frag_HP = HP;

    gl_Position = Projection * View * Model * vec4(new_pos, 1.0);
}
