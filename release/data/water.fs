#version 330

in vec2 fragTexCoord;
out vec4 finalColor;

uniform sampler2D texture0;
uniform float time;
uniform vec2 resolution; // voeg deze toe aan je shader input

// Berekent het gemiddelde van naburige pixels
vec4 getAverageColor(vec2 uv) {
    vec2 offset = vec2(1.0) / resolution; // kleine offset gebaseerd op texture grootte
    vec4 sum = vec4(0.0);
    float count = 0.0;

    // 3x3 kernel rond de huidige pixel
    for (int x = -8; x <= 8; ++x) {
        for (int y = -8; y <= 8; ++y) {
            vec2 sampleUV = uv + vec2(x, y) * offset;
            vec4 sampleColor = texture(texture0, sampleUV);
            if (sampleColor.a > 0.0) { // alleen geldige buren meetellen
                sum += sampleColor;
                count += 1.0;
            }
        }
    }

    if (count > 0.0)
        return sum / count;
    else
        return vec4(0.0); // volledig transparant fallback
}

void main() {
    vec2 uv = fragTexCoord;
    // uv.x += sin(time); // lichte beweging zoals eerder

    vec4 texColor = texture(texture0, uv);

    if (texColor.a < 0.01) {
        // Transparante pixel, vervang met gemiddelde kleur
        finalColor = getAverageColor(uv);

        // // Controleer of de kleur zwart is (alle RGB-componenten zijn 0)
        // if (finalColor.r == 0.0 && finalColor.g == 0.0 && finalColor.b == 0.0) {
        //     finalColor = vec4(0.0, 0.0, 1.0, 1.0); // fallback: blauw
        // }
    } else {
        finalColor = texColor;
    }
}
