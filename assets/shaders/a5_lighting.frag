#version 430
out vec4 fragColor;

in vec3 frag_position; // Fragment position in world-space
in vec3 frag_normal;   // Fragment normal in world-space
in vec2 uv;

uniform vec3 u_light_position;
uniform vec3 u_light_color;

uniform vec3 u_view_position;
uniform sampler2D u_sampler0;

// directional light
uniform vec3 u_dir_light_direction;  // Direction *toward* the light (e.g. (-1, -1, -1))
uniform vec3 u_dir_light_color;

uniform float u_ambient_strength = 0.1;
uniform float u_specular_strength = 0.3;
uniform float u_shininess = 32.0;

void main()
{
    vec3 tex_col = texture(u_sampler0, uv).rgb;

    // Normalize once
    vec3 N = normalize(frag_normal);
    vec3 V = normalize(u_view_position - frag_position);

    // Point
    vec3 Lp = normalize(u_light_position - frag_position);
    float diffP = max(dot(N, Lp), 0.0);
    vec3 Rp = reflect(-Lp, N);
    float specP = pow(max(dot(V, Rp), 0.0), u_shininess);
    vec3 ambientP = u_ambient_strength * u_light_color;
    vec3 diffuseP = diffP * u_light_color;
    vec3 specularP = specP * u_specular_strength * u_light_color;

    // Directional
    // Light comes *from* -u_dir_light_direction
    vec3 Ld = normalize(-u_dir_light_direction);
    float diffD = max(dot(N, Ld), 0.0);
    vec3 Rd = reflect(-Ld, N);
    float specD = pow(max(dot(V, Rd), 0.0), u_shininess);
    vec3 ambientD = u_ambient_strength * u_dir_light_color;
    vec3 diffuseD = diffD * u_dir_light_color;
    vec3 specularD = specD * u_specular_strength * u_dir_light_color;

    // Combine lights
    vec3 lighting = (ambientP + diffuseP + specularP) +
                    (ambientD + diffuseD + specularD);

    fragColor = vec4(tex_col * lighting, 1.0);
}