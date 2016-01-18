//  Basic vertex shader

// uniform float time;
// varying vec4 location;
varying vec4 vColor;


void main()
{
   //  Use color unchanged
   // foo = gl_Color;
   //  Set vertex coordinates
   // vec4 pos = gl_Vertex + vec4(0.5*sin(4.0*time),0.5*cos(4.0*time),0.5*sin(time),0.0);
   // gl_Position = gl_ModelViewProjectionMatrix * pos
   
     vColor = gl_ModelViewProjectionMatrix * gl_Vertex;
   gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}
