#version 430
out vec4 fragColor;

in vec3 frag_position; // Fragment position in world-space
in vec3 frag_normal;   // Fragment normal in world-space
in vec2 uv;

uniform vec3 u_light_position;
uniform sampler2D u_sampler0;

void main()
{
    vec4 tex_col = texture(u_sampler0, uv);

    // FROM fragment TO light (AB = B - A)
    vec3 L = normalize(u_light_position - frag_position);
    vec3 N = normalize(frag_normal);
    float dotNL = dot(N, L);

    fragColor = vec4(tex_col * dotNL);
}
