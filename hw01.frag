//  Basic fragment shader

// vector written by vertex shader containing RGB mapping
varying vec4 vColor;

void main()
{
    gl_FragColor = vColor; 
}

