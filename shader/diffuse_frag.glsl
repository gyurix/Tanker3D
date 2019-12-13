#version 330
// A texture is expected as program attribute
uniform sampler2D Texture;

uniform int LightCount[1];
uniform vec3 Light[100];
uniform vec3 LightColor[100];

uniform mat4 ProjectionMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ModelMatrix;

// (optional) Texture offset
uniform vec2 TextureOffset;

// The vertex shader will feed this input
in vec2 texCoord;

// Wordspace normal passed from vertex shader
in vec4 normal;

in vec3 pos;

// The final color
out vec4 FragmentColor;

void main() {
    // Compute diffuse lighting
    FragmentColor = texture(Texture, vec2(texCoord.x, 1.0 - texCoord.y) + TextureOffset);

    if (FragmentColor.a == 1.0f && LightCount[0] > 0){
        float diffuseR = 0.15f;
        float diffuseG = 0.15f;
        float diffuseB = 0.15f;
        for (int i=0; i<LightCount[0]; ++i){
            float dist = distance(pos, Light[i]);
            float value=dist >2 ? 1.4f - 0.2f * dist : 3.0f - dist;
            diffuseR = max(value*LightColor[i].x, diffuseR);
            diffuseG = max(value*LightColor[i].y, diffuseG);
            diffuseB = max(value*LightColor[i].z, diffuseB);
        }

        FragmentColor.r = min(1.0f, FragmentColor.r * diffuseR);
        FragmentColor.g = min(1.0f, FragmentColor.g * diffuseG);
        FragmentColor.b = min(1.0f, FragmentColor.b * diffuseB);
    }
    if (FragmentColor.a < 0.01){
        discard;
    }
}
