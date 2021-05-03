#version 330 core
in vec4 gl_FragCoord;
 
out vec4 fragColor;
out float gl_FragDepth;

uniform vec2 Resolution; 
uniform vec2 center;
uniform float zoom;
 
#define MAX_ITERATIONS 2000
 
int get_iterations()
{
    float real = ((gl_FragCoord.x / Resolution.x - 0.5f) * zoom + center.x) * 4.0f;
    float imag = ((gl_FragCoord.y / Resolution.y - 0.5f) * zoom + center.y) * 4.0f;
 
    int iterations = 0;
    float const_real = real;
    float const_imag = imag;
 
    while (iterations < MAX_ITERATIONS)
    {
        float tmp_real = real;
        real = (real * real - imag * imag) + const_real;
        imag = (2.0 * tmp_real * imag) + const_imag;
         
        float dist = real * real + imag * imag;
 
        if (dist > 4.0)
            break;
 
        ++iterations;
    }
    return iterations;
}
 
 
vec4 return_color()
{
    int iter = get_iterations();
    if (iter == MAX_ITERATIONS)
    {
        gl_FragDepth = 0.0f;
        return vec4(0.0f, 0.0f, 0.0f, 1.0f);
    }
 
    float iterations = float(iter) / MAX_ITERATIONS;
    gl_FragDepth = iterations;
 
    return vec4((mod(iterations, 2) * 22.0f), 0.0f, (mod(iterations, 4) * 33.0f), 1.0f);
   
}
 
void main()
{
    fragColor = return_color();
}