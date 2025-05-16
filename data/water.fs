#version 330

in vec2 fragTexCoord;
out vec4 finalColor;

uniform sampler2D texture0;
uniform vec2 resolution;

void main() {
    vec2 texelSize = 1.0 / resolution;
    float radius = 2.0;

    vec4 sum = vec4(0.0);
    float count = 0.0;

    for (int x = -2; x <= 2; x++) {
        for (int y = -2; y <= 2; y++) {
            vec2 offset = vec2(x, y) * texelSize * radius;
            vec4 sampleColor = texture(texture0, fragTexCoord + offset);
            
            if (sampleColor.a > 0.01) {
                sum += sampleColor;
                count += 1.0;
            }
        }
    }

    if (count > 0.0)
        finalColor = sum / count;
    else
        finalColor = vec4(0.0);
}
