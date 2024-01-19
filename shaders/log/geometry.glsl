#version 450

#define VERTICES 50

layout(triangles) in;
layout(triangle_strip, max_vertices = VERTICES) out;

uniform float log_depth;
uniform float log_width;

in vec3 tesPosition[];

out vec3 gNormal;
out vec3 gPosition;
out vec3 gColor;

uniform mat4 view_proj;
uniform mat4 model;

uniform vec3 log_center;

float rand(vec2 co){
    return fract(sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453);
}

void emit(vec3 pos) {
    vec4 world = model * vec4(pos, 1.);
    gl_Position = view_proj * world;
    gPosition = world.xyz;
    gColor = vec3(rand(pos.xy), rand(pos.xz), rand(pos.yz));
    EmitVertex();
}

void main(void)
{
    int center_index;
    for (center_index = 0; center_index < 3; center_index++)
        if (length(gl_in[center_index].gl_Position.xyz - log_center) < 1e-3)
            break;
    
    // Center
    vec3 pc = gl_in[center_index].gl_Position.xyz;

    // Left
    vec3 pl = gl_in[int(mod(center_index + 1, 3))].gl_Position.xyz;
    pl = normalize(pl - pc) * log_width;

    // Right
    vec3 pr = gl_in[int(mod(center_index + 2, 3))].gl_Position.xyz;
    pr = normalize(pr - pc) * log_width;

    vec3 baseNormal = normalize(cross(pr - pc, pl - pc));
    gNormal = normalize(model * vec4((mix(pl, pr, 0.5) - pc), 1.)).xyz;

    emit(pc);

    // -2 to account for start/endcap center vertices
    for (int i = 0; i < VERTICES - 2; i += 2)
    {
        // Cross: (i / (VERTICES - 2)) X (? / log_depth)
        float factor = log_depth * float(i) / (VERTICES - 2);

        emit(pl + factor * baseNormal);
        emit(pr + factor * baseNormal);
    }

    emit(pc + baseNormal * log_depth);

    EndPrimitive();
}
