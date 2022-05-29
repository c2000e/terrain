#version 330 core

in vec3 frag_pos;
in vec3 frag_norm;

out vec4 frag_color;

const vec3 light_color = vec3(1.0, 1.0, 1.0);

uniform int shade_normals;
uniform vec3 view_pos;
uniform vec3 pointlight_pos;

void main()
{
    if (shade_normals == 1)
    {
        frag_color = vec4(0.5 * (frag_norm + 1.0), 1.0);
    }
    else
    {
        vec3 dx = dFdx(frag_pos);
        vec3 dy = dFdy(frag_pos);
        vec3 normal = normalize(cross(dy, dx));

        float slope = (normal.y + 1.0) * 0.5;
        normal = mix(normal, -frag_norm, 1 - slope);
        vec3 object_color = mix(vec3(0.6, 0.7, 0.5), vec3(0.5, 0.6, 0.7), slope);

        float d = length(pointlight_pos - frag_pos);
        float attenuation = 1.0 / (1.0 + 0.022 * d + 0.0019 * d * d);

        vec3 light_dir = normalize(frag_pos - pointlight_pos);
        vec3 view_dir = normalize(view_pos - frag_pos);
        vec3 reflect_dir = reflect(-light_dir, normal);

        float ambient_strength = 0.1;
        vec3 ambient = ambient_strength * light_color * attenuation;

        float diffuse_strength = max(dot(normal, light_dir), 0.0);
        vec3 diffuse = diffuse_strength * light_color * attenuation;

        float specular_strength = 0.5;
        float spec = pow(max(dot(view_dir, reflect_dir), 0.0), 32);
        vec3 specular = specular_strength * spec * light_color * attenuation;
    
        frag_color = vec4((ambient + diffuse + specular) * object_color, 1.0);
    }
}
