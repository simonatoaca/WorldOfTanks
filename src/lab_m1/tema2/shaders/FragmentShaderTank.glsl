#version 330

// Input
in vec3 frag_position;
in vec3 frag_normal;
in vec3 frag_coordinate;
in vec3 frag_color;
flat in int frag_HP;

// Output
layout(location = 0) out vec4 out_color;

void main()
{
    vec3 new_color = frag_color - (3 - frag_HP) * vec3(0.08, 0.08, 0.08);
    out_color = vec4(new_color, 1);
}
