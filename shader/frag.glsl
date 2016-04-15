uniform int outputID;
uniform sampler2D envmap;
uniform vec3 eyePos;
varying vec3 normal;
varying vec3 vertex;

const float mandel_x = -2.0;
const float mandel_y = -2.0;
const float mandel_width = 4.0;
const float mandel_height = 4.0;
const float mandel_x2 = -2.0+1.29;
const float mandel_y2 = -2.0+2.27;
const float mandel_width2 = 0.04;
const float mandel_height2 = 0.04;

const int mandel_iterations = 128;

uniform float time;
uniform int itime;
varying vec2 texCoord;

#define PI 3.1415926

vec3 shadeDiffuseFromEye();

vec3 shadePhong(vec3 lightPos);

vec3 shadeEnvmapReflection();

vec4 mod289(vec4 x)
{
    return x - floor(x * (1.0 / 289.0)) * 289.0;
}

vec4 permute(vec4 x)
{
    return mod289(((x*34.0)+1.0)*x);
}

vec4 taylorInvSqrt(vec4 r)
{
    return 1.79284291400159 - 0.85373472095314 * r;
}

vec2 fade(vec2 t) {
    return t*t*t*(t*(t*6.0-15.0)+10.0);
}

// Classic Perlin noise
float cnoise(vec2 P)
{
    vec4 Pi = floor(P.xyxy) + vec4(0.0, 0.0, 1.0, 1.0);
    vec4 Pf = fract(P.xyxy) - vec4(0.0, 0.0, 1.0, 1.0);
    Pi = mod289(Pi); // To avoid truncation effects in permutation
    vec4 ix = Pi.xzxz;
    vec4 iy = Pi.yyww;
    vec4 fx = Pf.xzxz;
    vec4 fy = Pf.yyww;

    vec4 i = permute(permute(ix) + iy);

    vec4 gx = fract(i * (1.0 / 41.0)) * 2.0 - 1.0 ;
    vec4 gy = abs(gx) - 0.5 ;
    vec4 tx = floor(gx + 0.5);
    gx = gx - tx;

    vec2 g00 = vec2(gx.x,gy.x);
    vec2 g10 = vec2(gx.y,gy.y);
    vec2 g01 = vec2(gx.z,gy.z);
    vec2 g11 = vec2(gx.w,gy.w);

    vec4 norm = taylorInvSqrt(vec4(dot(g00, g00), dot(g01, g01), dot(g10, g10), dot(g11, g11)));
    g00 *= norm.x;
    g01 *= norm.y;
    g10 *= norm.z;
    g11 *= norm.w;

    float n00 = dot(g00, vec2(fx.x, fy.x));
    float n10 = dot(g10, vec2(fx.y, fy.y));
    float n01 = dot(g01, vec2(fx.z, fy.z));
    float n11 = dot(g11, vec2(fx.w, fy.w));

    vec2 fade_xy = fade(Pf.xy);
    vec2 n_x = mix(vec2(n00, n01), vec2(n10, n11), fade_xy.x);
    float n_xy = mix(n_x.x, n_x.y, fade_xy.y);
    return 2.3 * n_xy;
}

// Classic Perlin noise, periodic variant
float pnoise(vec2 P, vec2 rep)
{
    vec4 Pi = floor(P.xyxy) + vec4(0.0, 0.0, 1.0, 1.0);
    vec4 Pf = fract(P.xyxy) - vec4(0.0, 0.0, 1.0, 1.0);
    Pi = mod(Pi, rep.xyxy); // To create noise with explicit period
    Pi = mod289(Pi);        // To avoid truncation effects in permutation
    vec4 ix = Pi.xzxz;
    vec4 iy = Pi.yyww;
    vec4 fx = Pf.xzxz;
    vec4 fy = Pf.yyww;

    vec4 i = permute(permute(ix) + iy);

    vec4 gx = fract(i * (1.0 / 41.0)) * 2.0 - 1.0 ;
    vec4 gy = abs(gx) - 0.5 ;
    vec4 tx = floor(gx + 0.5);
    gx = gx - tx;

    vec2 g00 = vec2(gx.x,gy.x);
    vec2 g10 = vec2(gx.y,gy.y);
    vec2 g01 = vec2(gx.z,gy.z);
    vec2 g11 = vec2(gx.w,gy.w);

    vec4 norm = taylorInvSqrt(vec4(dot(g00, g00), dot(g01, g01), dot(g10, g10), dot(g11, g11)));
    g00 *= norm.x;
    g01 *= norm.y;
    g10 *= norm.z;
    g11 *= norm.w;

    float n00 = dot(g00, vec2(fx.x, fy.x));
    float n10 = dot(g10, vec2(fx.y, fy.y));
    float n01 = dot(g01, vec2(fx.z, fy.z));
    float n11 = dot(g11, vec2(fx.w, fy.w));

    vec2 fade_xy = fade(Pf.xy);
    vec2 n_x = mix(vec2(n00, n01), vec2(n10, n11), fade_xy.x);
    float n_xy = mix(n_x.x, n_x.y, fade_xy.y);
    return 2.3 * n_xy;
}

float fbm(vec2 P, int octaves, float lacunarity, float gain)
{
    float sum = 0.0;
    float amp = 1.0;
    vec2 pp = P;

    int i;

    for(i = 0; i < octaves; i+=1)
    {
        amp *= gain;
        sum += amp * cnoise(pp);
        pp *= lacunarity;
    }
    return sum;

}


float pattern(in vec2 p) {
    float l = 2.5;
    float g = 0.4;
    int oc = 10;

    vec2 q = vec2( fbm( p + vec2(0.0,0.0),oc,l,g),fbm( p + vec2(5.2,1.3),oc,l,g));
    vec2 r = vec2( fbm( p + 4.0*q + vec2(1.7,9.2),oc,l,g ), fbm( p + 4.0*q + vec2(8.3,2.8) ,oc,l,g));
    return fbm( p + 4.0*r ,oc,l,g);
}

float pattern2( in vec2 p, out vec2 q, out vec2 r , in float time)
{
    float l = 2.3;
    float g = 0.4;
    int oc = 10;

    q.x = fbm( p + vec2(time,time),oc,l,g);
    q.y = fbm( p + vec2(5.2*time,1.3*time) ,oc,l,g);

    r.x = fbm( p + 4.0*q + vec2(1.7,9.2),oc,l,g );
    r.y = fbm( p + 4.0*q + vec2(8.3,2.8) ,oc,l,g);

    return fbm( p + 4.0*r ,oc,l,g);
}

void main()
{
    if(outputID == 0)
    {
        gl_FragColor = vec4(shadeDiffuseFromEye(), 1.0);
        return;
    }
    else if(outputID == 1)
    {
        gl_FragColor = vec4(shadePhong(vec3(cos(float(itime) / 100.0), 10, 10)), 1.0);
        return;
    }
    else if(outputID == 2)
    {
        gl_FragColor = vec4(shadeEnvmapReflection(), 1.0);
        return;
    }
    else if(outputID == 3)
    {
        // Do something interesting here for extra credit.
        vec2 q = gl_FragCoord.xy / vec2(640.0,480.0);
        vec2 p = -1.0 + 2.0 * q;
        vec2 qq;
        vec2 r;
        float color = pattern2(p,qq,r,time);

        vec4 c = vec4(color,color,color,color);
        c *= 3.5;

        gl_FragColor = c;
    }
    // ...
}

vec3 shadeDiffuseFromEye()
{
    vec3 n = normalize(normal);
    vec3 out_vec = normalize(eyePos - vertex);
    float diffuse = abs(dot(out_vec, n));
    return vec3(diffuse);
}

vec3 shadePhong(vec3 lightPos)
{   // TODO Part 6.
    // TODO Compute Phong shading here. You can choose any color you like. But please
    // TODO make sure that you have noticeable specular highlights in the shading.
    // TODO Variables to use: eyePos, lightPos, normal, vertex
    float Ka = 0.2;
    float Kd = 1.0;
    float Ks = 1.0;
    float I = 1.0;
    float Ia = 1.0;
    float p = 70.0;

    vec3 n = normalize(normal);
    vec3 v = normalize(eyePos - vertex);
    vec3 l = normalize(lightPos - vertex);
    vec3 h = normalize(v + l);

    float La = Ka * Ia;
    float Ld = Kd * I * max(0.0, dot(n, l));
    float Ls = Ks * I * pow(max(0.0, dot(n, h)), p);
    return (La + Ld + Ls) * vec3(0.5, 0.5, 0.5);
}

vec3 shadeEnvmapReflection()
{   // TODO Part 6.
    // TODO Compute environment mapping based on reflection direction here.
    // TODO First compute the reflection direction using eyePos, normal and vertex.
    // TODO Then convert the reflection direction to spherical coordinates (theta, phi). 
    // TODO Note that the up vector is along +Z axis.
    // TODO Finally, grab the color from envmap using (theta, phi).
    // TODO You will need to use function "vec4 texture2D(sampler2D tex, vec2 texCoord)" to 
    // TODO get colors from textures.
    vec3 i = normalize(eyePos - vertex);
    vec3 n = normalize(normal);
    vec3 o = 2.0 * dot(i, n) * n - i;
    float theta = atan(o.y, o.x);
    float phi = acos(o.z / length(o));
    
    return vec3(texture2D(envmap, vec2(theta / (2.0 * PI), phi / PI)));
}