#version 330

in vec2 fragTexCoord;
out vec4 finalColor;

uniform sampler2D ballData;
uniform sampler2D ballColors;
uniform int ballCount;
uniform vec2 resolution;

void main()
{
    vec2 p = vec2(
        fragTexCoord.x * resolution.x,
        (1.0 - fragTexCoord.y) * resolution.y
    );

    float field = 0.0;
    vec3 col = vec3(0.0);

    float radius = 10.0;
    float threshold = 1.0;
    float cutoffDist = radius * 3.0;

    for (int i = 0; i < ballCount; i++)
    {
        vec4 b = texelFetch(ballData, ivec2(i, 0), 0);
        vec4 c = texelFetch(ballColors, ivec2(i, 0), 0);
        
        float normX = ((b.r * 255.0) * 256.0 + (b.g * 255.0)) / 65535.0;
        float normY = ((b.b * 255.0) * 256.0 + (b.a * 255.0)) / 65535.0;
        vec2 pos = vec2(normX * resolution.x, normY * resolution.y);

        vec2 dv = p - pos;
        float dist = length(dv);

        // Performance cutoff - hou dit AAN!
        if (dist > cutoffDist)
            continue;

        // Smoothere falloff met betere formule
        float t = clamp(1.0 - dist / cutoffDist, 0.0, 1.0);
        float influence = t * t * (3.0 - 2.0 * t); // Smoothstep falloff
        influence *= radius;
        
        
        field += influence;   
        col += c.rgb * influence;
    }

    if (field < threshold)
        discard;

    col /= (field + 0.001);
    
    // Smoothere alpha met grotere range
    float alpha = smoothstep(threshold * 0.8, threshold * 3.0, field);
    
    finalColor = vec4(col, alpha);
}