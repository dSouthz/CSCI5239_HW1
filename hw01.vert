//  Basic vertex shader

// vector fed to fragment shader containing RGB mapping
varying vec4 vColor;


void main()
{
   //  Set vertex coordinates
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
   
    // Get pixel location in mapping vector
    vColor = gl_Position;

    // Map NDC (-1 to 1) to RGB (0 to 1):
    // -1 to 1 -> 0 to 2 to be positive like RGB
    // 0 to 2 -> NGB space is 2x as large as RBG -> divide by 2
    // 
    // 4D -> 3D
    // w might not be 1 -> need to divide by w
    vColor = (((vColor/vColor.w) + 1.0) / 2.0); 
}
