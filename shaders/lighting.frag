#version 330 core

in vec4 position; // raw position in the model coord
in vec3 normal;   // raw normal in the model coord

uniform mat4 modelview; // from model coord to eye coord
uniform mat4 view;      // from world coord to eye coord

// Material parameters
uniform vec4 ambient;
uniform vec4 diffuse;
uniform vec4 specular;
uniform vec4 emision;
uniform float shininess;

// Light source parameters
const int maximal_allowed_lights = 10;
uniform bool enablelighting;
uniform int nlights;
uniform vec4 lightpositions[ maximal_allowed_lights ];
uniform vec4 lightcolors[ maximal_allowed_lights ];

// Output the frag color
out vec4 fragColor;


void main (void){
    if (!enablelighting){
        // Default normal coloring (you don't need to modify anything here)
        vec3 N = normalize(normal);
        fragColor = vec4(0.5f*N + 0.5f , 1.0f);
    } else{     // In Terms of Camera
        vec4 R =  vec4(0,0,0,0);

        vec4 P = modelview * position;
        vec3 p = vec3(P.xyz/P.w);

        vec3 v = normalize(-p);

        vec4 N = inverse(transpose(modelview))*vec4(normal,0);
        vec3 n = normalize(N.xyz);
        
        for(int j = 0; j < maximal_allowed_lights; j++){
            
            vec4 L = view*lightpositions[j];
            vec3 l = normalize(P.w*L.xyz- L.w*P.xyz);
            vec3 h = normalize(l+v);
            
            R += (ambient + diffuse*max(dot(n,l),0) + specular*pow(max(dot(n,h),0),shininess))*lightcolors[j];
       }
        
        fragColor = emision + R;
    }
}
