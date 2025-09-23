#version 330

in vec2 fragTexCoord;
out vec4 finalColor;

uniform sampler2D texture0;
uniform float time;
uniform vec2 resolution;

// Metaball function voor vloeiende vloeistof
float metaball(vec2 pos, vec2 center, float radius) {
    float dist = distance(pos, center);
    return radius / (dist * dist + 0.1);
}

// Verbeterde blur voor vloeistof effect
vec4 fluidBlur(vec2 uv) {
    vec2 offset = vec2(1.0) / resolution;
    vec4 sum = vec4(0.0);
    float totalWeight = 0.0;
    
    // Gaussian blur weights
    float weights[9] = float[](
        0.05, 0.09, 0.05,
        0.09, 0.20, 0.09,
        0.05, 0.09, 0.05
    );
    
    int index = 0;
    for (int x = -1; x <= 1; x++) {
        for (int y = -1; y <= 1; y++) {
            vec2 sampleUV = uv + vec2(x, y) * offset * 8.0;
            vec4 sampleColor = texture(texture0, sampleUV);
            
            if (sampleColor.a > 0.0) {
                sum += sampleColor * weights[index];
                totalWeight += weights[index];
            }
            index++;
        }
    }
    
    if (totalWeight > 0.0) {
        return sum / totalWeight;
    }
    return vec4(0.0);
}

void main() {
    vec2 uv = fragTexCoord;
    vec4 texColor = texture(texture0, uv);
    
    if (texColor.a < 0.01) {
        // Gebruik metaball effect voor vloeistof
        vec4 fluidColor = fluidBlur(uv);
        
        // Voeg lichte beweging toe
        float wave = sin(time * 2.0 + uv.x * 10.0) * 0.01;
        uv.y += wave;
        
        // Gradient effect voor diepte
        float depth = 1.0 - uv.y;
        fluidColor.rgb *= (0.7 + depth * 0.3);
        
        // Specular highlights
        float highlight = pow(max(0.0, sin(time + uv.x * 15.0) * sin(time + uv.y * 15.0)), 20.0);
        fluidColor.rgb += vec3(highlight * 0.3);
        
        finalColor = fluidColor;
    } else {
        // Bestaande particle met lichte transparantie
        finalColor = vec4(texColor.rgb, texColor.a * 0.8);
    }
    
    // Refractie effect
    vec2 distortion = vec2(
        sin(time * 0.5 + uv.y * 20.0) * 0.002,
        cos(time * 0.3 + uv.x * 15.0) * 0.002
    );
    finalColor.rgb += texture(texture0, uv + distortion).rgb * 0.1;

    // finalColor = texColor;
}