//  Basic fragment shader

// uniform float time;
varying vec4 vColor;

void main()
{
    vec4 color;
    
   // gl_FragColor = foo+vec4(sin(time),cos(time),0.0,0.0);
   // gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0);
    
    //c.x = location
    gl_FragColor = vColor; 
}

