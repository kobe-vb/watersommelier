#version 330

in vec2 fragTexCoord;
out vec4 finalColor;

uniform sampler2D texture0;
uniform float time;

// Geoptimaliseerde water shader - minder berekeningen
void main() {
    vec2 uv = fragTexCoord;
    vec4 texColor = texture(texture0, uv);
    
    if (texColor.a > 0.01) {
        // Particle met lichte transparantie en refractie
        
        // Simpele refractie (goedkoper dan voorheen)
        vec2 distortion = vec2(
            sin(time * 2.0 + uv.y * 15.0),
            cos(time * 1.5 + uv.x * 12.0)
        ) * 0.003;
        
        vec4 refracted = texture(texture0, uv + distortion);
        
        // Mix huidige kleur met refracted voor water effect
        finalColor = mix(texColor, refracted, 0.25);
        
        // Lichte glans op basis van positie
        float highlight = pow(max(0.0, sin(time * 3.0 + uv.x * 20.0) * sin(time * 3.0 + uv.y * 20.0)), 15.0);
        finalColor.rgb += vec3(highlight * 0.15);
        
        // Behoud alpha met lichte transparantie
        finalColor.a = texColor.a * 0.85;
    } else {
        // Geen particle - transparant
        finalColor = vec4(0.0);
    }
}