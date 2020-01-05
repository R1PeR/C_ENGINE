#include <math.h>

global const f64 PI          = 3.141592653589793;
global const f64 HALF_PI     = 1.570796326794897;
global const f64 DOUBLE_PI   = 6.283185307179586;

struct {
            f32 x;
            f32 y;
} typedef V2;

internal int clamp(int min, int val, int max){
    if(val > max){
         return max;
    }
    if(val < min){
        return min;
    }
    return val;
};
/*internal f32 g_sqrt(const float m)
{
    f32 i=0;
    f32 x1,x2;
    while( (i*i) <= m )
        i+=0.1f;
    x1=i;
    for(int j=0;j<10;j++)
    {
        x2=m;
        x2/=x1;
        x2+=x1;
        x2/=2;
        x1=x2;
    }
    return x2;
};*/
internal int abs(int m){
    if(m > 0){
    return m;
    }else{
        return -m;
    }
}
internal f32 absf(f32 m){
    if(m > 0){
        return m;
    }else{
        return -m;
    }
}
internal int signf(f32 m){
    if(m>0.0){
        return 1;
    }
    if(m<0.0){
        return -1;
    }
    return 0;
}
internal void swap(int * a, int * b){
    int temp = *a;
    *a = *b;
    *b = temp;
}
internal f32 areaTriangle(int x1, int y1, int x2, int y2, int x3, int y3){
    return absf((x1*(y2-y3) + x2*(y3-y1) + x3*(y1-y2))/2.0);
}
internal f32 areaRectangle(int x1, int y1, int x2, int y2, int x3, int y3,int x4, int y4){
    return
        ((x4+x1)*(y4-y1)+ (x1+x2)*(y1-y2)+ (x2+x3)*(y2-y3)+ (x3+x4)*(y3-y4))/2.0;
}
 internal u32 pointInsideTriangle(int x1, int y1, int x2, int y2, int x3, int y3,int x,int y){
    f32 a = areaTriangle(x1,y1,x2,y2,x3,y3);
    f32 a1 = areaTriangle(x,y,x2,y2,x3,y3);
    f32 a2 = areaTriangle(x1,y1,x,y,x3,y3);
    f32 a3 = areaTriangle(x1,y1,x2,y2,x,y);
    f32 err = 1.0;
    //return(a >= (a1+a2+a3)-err && a <= (a1+a2+a3)+err);
    //b->a>b
    return((int)a == (int)(a1+a2+a3));
}
internal u32 pointInsideRectangle(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4,int x,int y){
    f32 a = areaRectangle(x1,y1,x2,y2,x3,y3,x4,y4);
    f32 a1 = areaRectangle(x,y,x2,y2,x3,y3,x4,y4);
    f32 a2 = areaRectangle(x1,y1,x,y,x3,y3,x4,y4);
    f32 a3 = areaRectangle(x1,y1,x2,y2,x,y,x4,y4);
    f32 a4 = areaRectangle(x1,y1,x2,y2,x3,y3,x,y);
    //return(a < (a1+a2+a3)+0.1 && a > (a1+a2+a3)-0.1);
    return(a -(a1+a2+a3+a4));
}
internal u32 pointRect(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4,int x,int y){
    u32 a = pointInsideTriangle(x1,y1,x2,y2,x3,y3,x,y);
    u32 b = pointInsideTriangle(x1,y1,x3,y3,x4,y4,x,y);
    return(!a != !b);
}
internal f32 distance(V2 v1, V2 v2){
    return sqrt(((v1.x-v2.x)*(v1.x-v2.x)) + ((v1.y-v2.y)*(v1.y-v2.y)));
}
internal V2 vectorAdd(V2 v0, V2 v1){
    V2 result;
    result.x = v0.x + v1.x;
    result.y = v0.y + v1.y;
    return result;
}
internal V2 vectorMultiply(V2 v0, f32 m){
    V2 result;
    result.x = v0.x * m;
    result.y = v0.y * m;
    return result;
}
/*internal V2 pos_to_dir(V2 v0, V2 v1){
    f32 angle = atan2(v1.y, v1.x) - atan2(v0.y, v0.x);
    if (angle < 0) { angle += 2 * acos(-1.0); }
    return (V2){sin(angle),cos(angle)};
}*/
internal V2 normalize(V2 v){
    if(v.x != 0.0 && v.y != 0.0){
        f32 w = sqrtf((v.x * v.x) + (v.y * v.y));
        return (V2){v.x/w,v.y/w};
    }else{
        return (V2){0.0,0.0};
    }
}
internal V2 normalized_pos(V2 v0, V2 v1){
    V2 result;
    result.x = v1.x - v0.x;
    result.y = v1.y - v0.y;
    result = normalize(result);
    return result;
}
internal V2 deg_to_pol(f32 a){
    V2 result;
     f32 radians = a * PI / 180.0;
    result.x = cos(radians);
    result.y = sin(-radians);
    return result;
}