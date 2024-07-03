#version 330 core

in vec2 UV;
out vec4 fragColour;

uniform float time;
uniform vec2 mousePos;

// Fractal function
vec2 julia(vec2 z, float time) {
    vec2 c = vec2(cos(time), sin(time)); // Constant c value that changes over time
    for (int i = 0; i < 50; i++) {
        float x = z.x * z.x - z.y * z.y + c.x;
        float y = 2.0 * z.x * z.y + c.y;
        z = vec2(x, y);
        if (length(z) > 2.0) {
            return vec2(float(i) / 50.0, 0.0);
        }
    }
    return vec2(1.0, 0.0);
}

vec3 colourToPallet(float x) {
    vec3 color = vec3(0.0);

    // Define the color palette
    vec3 palette[6] = {
        vec3(0.0, 0.0, 0.2),  // Deep blue
        vec3(0.0, 0.2, 0.4),  // Teal
        vec3(0.2, 0.4, 0.6),  // Light blue
        vec3(0.4, 0.6, 0.8),  // Pale blue
        vec3(0.6, 0.8, 1.0),  // White
        vec3(1.0, 1.0, 1.0)   // Bright white
    };

    // Map the input value x to the color palette
    x = clamp(x, 0.0, 1.0);
    int index = int(x * (float(6) - 1.0));
    color = mix(palette[index], palette[index + 1], fract(x * (float(6) - 1.0)));

    return color;
}

void main() {
    // Fractal parameters
    const int iterations = 100;
    const float scale = 1.55;
    const vec2 offset = vec2(-0.75, 0.0);
    float t = time * 5.0; // Increase the speed
    vec2 uv2 = vec2(
        sin(UV.x * 20.0 + tan(time)),
        cos(UV.y * 20.0 + sin(t))
    );
    // Calculate fractal
    vec2 result = julia(uv2, time);

    // Colorize the fractal
    fragColour = vec4(colourToPallet(result.x), 1.0);
}