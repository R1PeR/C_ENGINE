#define NULL_RAY ((Point){0.0,0.0,false})
global f32 fov = 10.0;
V2 camera_position = {0.0,0.0};
//TODO: Raytracing
struct{
    V2 v0;
    V2 v1;
    u32 color;
} typedef Line;
struct{
    V2 v0;
    V2 v1;
    V2 v2;
    u32 color;
}typedef Polygon3;
struct{
    V2 v0;
    V2 v1;
    V2 v2;
    V2 v3;
    u32 color;
}typedef Polygon4;
struct {
    V2 v0;
    V2 v1;
}typedef Ray;
struct{
    V2 v0;
    u32 valid;
}typedef Point;

struct {
    char type;
    union {
        Line line;
        Polygon3 polygon3;
        Polygon4 polygon4;
    };
}typedef  Render_object;
enum {
    e_line,
    e_polygon3,
    e_polygon4
};

inline void convert_to_aspect4(f32 * x0,f32 * y0,f32 * x1,f32 * y1)
{
    f32 aspect = (f32)render_buffer.height; 
if((f32)render_buffer.width / render_buffer.height < 1.77f){ 
aspect = (f32)render_buffer.width * 1.77f; 
}
f32 fovAspect = aspect / fov; 
*x0 *= fovAspect; 
*y0 *= fovAspect; 
*x1 *= fovAspect; 
*y1 *= fovAspect; 
*x0 += (f32)render_buffer.width/2.0f + camera_position.x*fovAspect; 
*x1 += (f32)render_buffer.width/2.0f + camera_position.x*fovAspect; 
*y0 += (f32)render_buffer.height/2.0f + camera_position.y*fovAspect; 
    *y1 += (f32)render_buffer.height/2.0f + camera_position.y*fovAspect; 
}
inline void convert_to_aspect2(f32 * x0,f32 * y0)
{
    f32 aspect = (f32)render_buffer.height; 
    if((f32)render_buffer.width / render_buffer.height < 1.77f){ 
        aspect = (f32)render_buffer.width * 1.77f; 
    }
    f32 fovAspect = aspect / fov; 
    *x0 *= fovAspect; 
    *y0 *= fovAspect;
    *x0 += (f32)render_buffer.width/2.0f + camera_position.x*fovAspect; 
    *y0 += (f32)render_buffer.height/2.0f + camera_position.y*fovAspect; 
}
inline V2 back_convert_to_aspect2(int x0, int y0){
    V2 result;
    f32 aspect = (f32)render_buffer.height; 
    if((f32)render_buffer.width / render_buffer.height < 1.77f){ 
        aspect = (f32)render_buffer.width * 1.77f; 
    }
    f32 fovAspect = aspect / fov; 
    result.x = x0/fovAspect;
    result.y = y0/fovAspect;
     result.x -= (f32)render_buffer.width/2.0f/fovAspect - camera_position.x*fovAspect; 
    result.y -= (f32)render_buffer.height/2.0f/fovAspect - camera_position.y*fovAspect;
    return result;
}
inline void convert_to_aspect6(f32 * x0,f32 * y0,f32 * x1, f32 * y1, f32 * x2, f32 * y2)
{
    f32 aspect = (f32)render_buffer.height; 
    if((f32)render_buffer.width / render_buffer.height < 1.77f){ 
        aspect = (f32)render_buffer.width * 1.77f; 
    }
    f32 fovAspect = aspect / fov; 
    *x0 *= fovAspect; 
    *y0 *= fovAspect;
    *x1 *= fovAspect;
    *y1 *= fovAspect;
    *x2 *= fovAspect;
    *y2 *= fovAspect;
    *x0 += (f32)render_buffer.width/2.0f + camera_position.x*fovAspect; 
    *y0 += (f32)render_buffer.height/2.0f + camera_position.y*fovAspect;
    *x1 += (f32)render_buffer.width/2.0f + camera_position.x*fovAspect; 
    *y1 += (f32)render_buffer.height/2.0f + camera_position.y*fovAspect;
    *x2 += (f32)render_buffer.width/2.0f + camera_position.x*fovAspect;
    *y2 += (f32)render_buffer.height/2.0f + camera_position.y*fovAspect;
    
}
inline void convert_to_aspect8(f32 * x0,f32 * y0,f32 * x1, f32 * y1, f32 * x2, f32 * y2, f32 * x3, f32 * y3)
{
    f32 aspect = (f32)render_buffer.height; 
    if((f32)render_buffer.width / render_buffer.height < 1.77f){ 
        aspect = (f32)render_buffer.width * 1.77f; 
    }
    f32 fovAspect = aspect / fov; 
    *x0 *= fovAspect; 
    *y0 *= fovAspect;
    *x1 *= fovAspect;
    *y1 *= fovAspect;
    *x2 *= fovAspect;
    *y2 *= fovAspect;
    *x3 *= fovAspect;
    *y3 *= fovAspect;
    *x0 += (f32)render_buffer.width/2.0f + camera_position.x*fovAspect; 
    *y0 += (f32)render_buffer.height/2.0f + camera_position.y*fovAspect;
    *x1 += (f32)render_buffer.width/2.0f + camera_position.x*fovAspect; 
    *y1 += (f32)render_buffer.height/2.0f + camera_position.y*fovAspect;
    *x2 += (f32)render_buffer.width/2.0f + camera_position.x*fovAspect;
    *y2 += (f32)render_buffer.height/2.0f + camera_position.y*fovAspect;
    *x3 += (f32)render_buffer.width/2.0f + camera_position.x*fovAspect;
    *y3 += (f32)render_buffer.height/2.0f + camera_position.y*fovAspect;
    
}
inline void convert_to_aspect3_outline(f32 * x0, f32 * y0, f32 * radius, f32 * outline){
    f32 aspect = (f32)render_buffer.height; 
    if((f32)render_buffer.width / render_buffer.height < 1.77f){ 
        aspect = (f32)render_buffer.width * 1.77f; 
    }
    f32 fovAspect = aspect / fov; 
    *x0 *= fovAspect; 
    *y0 *= fovAspect;
    *radius *= fovAspect;
    *outline *= fovAspect;
    *x0 += (f32)render_buffer.width/2.0f + camera_position.x*fovAspect; 
    *y0 += (f32)render_buffer.height/2.0f + camera_position.y*fovAspect; 
}
inline void convert_to_aspect3(f32 * x0, f32 * y0, f32 * radius){
    f32 aspect = (f32)render_buffer.height; 
    if((f32)render_buffer.width / render_buffer.height < 1.77f){ 
        aspect = (f32)render_buffer.width * 1.77f; 
    }
    f32 fovAspect = aspect / fov; 
    *x0 *= fovAspect; 
    *y0 *= fovAspect;
    *radius *= fovAspect;
    *x0 += (f32)render_buffer.width/2.0f + camera_position.x*fovAspect; 
    *y0 += (f32)render_buffer.height/2.0f + camera_position.y*fovAspect; 
}
internal void clear_screen(u32 color){
    u32 *pixel = render_buffer.pixels;
    for(int y = 0; y<render_buffer.height; y++){
        for(int x = 0; x<render_buffer.width; x++){
            *pixel++ = color;
        }
    }
}
internal void draw_pixel(int x,int y, u32 color){
    if(x>=0 && x<render_buffer.width && y>=0 && y<render_buffer.height){
        u32 * pixel = render_buffer.pixels+(x+render_buffer.width*y);
        * pixel = color;
}
}
internal void draw_rect_pixels(int x0, int y0, int x1, int y1, u32 color){
    for(int y = y0; y<y1; y++){
        for(int x = x0; x<x1; x++){
            draw_pixel(x,y,color);
        }
    }
}
internal void draw_line_pixels(int x0, int y0, int x1, int y1, u32 color){
    f32 dx,dy,delta;
    u32 c = color;
    int inc = 1;
    if(abs(y1-y0) < abs(x1-x0)){
        if(x0 > x1){
            swap(&x0,&x1);
            swap(&y0,&y1);
        }
         dx = x1-x0;
        dy = y1-y0;
        if(dy < 0){
            inc = -1;
            dy = -dy;
        }
         delta = 2*dy - dx;
        int y = y0;
        for(int x = x0; x < x1;x++){
            draw_pixel(x,y,color);
            if(delta > 0){
                y += inc;
                delta -= 2*dx;
            }
            delta += 2*dy;
        }
    }else{
        if(y0 > y1){
            swap(&x0,&x1);
            swap(&y0,&y1);
        }
        dx = x1-x0;
        dy = y1-y0;
        if(dx < 0){
            inc = -1;
            dx = -dx;
        }
        delta = 2*dx - dy;
         int x = x0;
        for(int y = y0; y < y1;y++){
            draw_pixel(x,y,color);
            if(delta > 0){
                 x+= inc;
                delta -= 2*dy;
            }
            delta += 2*dx;
        }
    }
}
internal void draw_rect(f32 x0, f32 y0, f32 x1, f32 y1,u32 color){
    convert_to_aspect4(&x0,&y0,&x1,&y1);
    draw_rect_pixels((int)(x0), (int)(y0), (int)(x1), (int)(y1), color);
}
internal void draw_line(f32 x0, f32 y0, f32 x1, f32 y1, u32 color){
    convert_to_aspect4(&x0,&y0,&x1,&y1);
    draw_line_pixels(x0,y0,x1,y1,color);
}
internal void draw_line_obj(Line l){
    convert_to_aspect4(&l.v0.x,&l.v0.y,&l.v1.x,&l.v1.y);
    draw_line_pixels(l.v0.x,l.v0.y,l.v1.x,l.v1.y,l.color);
}
internal void draw_line_vector(V2 v0, V2 v1, u32 color){
    convert_to_aspect4(&v0.x,&v0.y,&v1.x,&v1.y);
    draw_line_pixels(v0.x,v0.y,v1.x,v1.y,color);
}
internal void draw_3d_square(f32 x0,f32 y0, u32 color){
    draw_rect(x0-1.0, y0-1.0, x0+0.0, y0+0.0, color);
    draw_line(x0-0.0, y0-0.0, x0+0.5, y0+0.5, color);
    draw_line(x0-1.0, y0-1.0, x0-1.5, y0-1.5, color);
    draw_line(x0-0.0, y0-1.0, x0+1.5, y0-1.5, color);
    draw_line(x0-1.0, y0+0.0, x0-1.5, y0+1.5, color);
    draw_line(x0-0.5, y0+0.5, x0+0.5, y0+0.5, color);
    draw_line(x0+0.5, y0-0.5, x0+0.5, y0+0.5, color);
}
internal void draw_circle_pixels_outline(int x, int y, int radius, int outline, u32 color){
    f32 r2 = radius * radius;
    for(int j = -(radius+outline); j < (radius+outline); j++){
        for(int i = -(radius+outline); i < (radius+outline); i++){
            int sum =  i*i+j*j;
            if(sum > r2-outline && sum < r2+outline ){
                draw_pixel(x+i,y+j,color);
            }
    }
    }
}
internal void draw_circle_pixels(int x, int y, int radius, u32 color){
    f32 r2 = radius * radius;
    for(int j = -(radius); j < (radius); j++){
        for(int i = -(radius); i < (radius); i++){
            int sum =  i*i+j*j;
            if(sum < r2){
                draw_pixel(x+i,y+j,color);
            }
        }
    }
}
internal void draw_circle_outline(f32 x0, f32 y0, f32 radius,f32 outline, u32 color){
    convert_to_aspect3_outline(&x0, &y0, &radius, &outline);
    draw_circle_pixels_outline(x0,y0,radius,outline,color);
}
internal void draw_circle(f32 x0, f32 y0, f32 radius, u32 color){
    convert_to_aspect3(&x0,&y0,&radius);
    draw_circle_pixels(x0,y0,radius,color);
        
}
internal void draw_triangle_outline(f32 x0, f32 y0,f32 x1, f32 y1,f32 x2, f32 y2, u32 color ){
    draw_line(x0, y0, x1, y1, color);
    draw_line(x1, y1, x2, y2, color);
    draw_line(x2, y2, x0, y0, color);
}
internal void draw_triangle_outline_pixels(int x0, int y0,int x1, int y1,int x2, int y2, u32 color ){
    draw_line_pixels(x0, y0, x1, y1, color);
    draw_line_pixels(x1, y1, x2, y2, color);
    draw_line_pixels(x2, y2, x0, y0, color);
}
internal void draw_filled_4point_pixels(int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3, u32 color){
    
    int startY, endY, startX, endX;
    startX = x0;
    if(startX > x1) startX = x1;
    if(startX > x2) startX = x2;
    if(startX > x3) startX = x3;
    startY = y0;
    if(startY > y1) startY = y1;
    if(startY > y2) startY = y2;
    if(startY > y3) startY = y3;
    endX = x0;
    if(endX < x1) endX = x1;
    if(endX < x2) endX = x2;
    if(endX < x3) endX = x3;
    endY = y0;
    if(endY < y1) endY = y1;
    if(endY < y2) endY = y2;
    if(endY < y3) endY = y3;
    draw_triangle_outline_pixels(x0,y0,x1,y1,x2,y2,color);
    draw_triangle_outline_pixels(x0,y0,x2,y2,x3,y3,color);
    for(int y = startY; y < endY; y++){
        for(int x = startX; x < endX; x++){
            if(pointRect(x0,y0,x1,y1,x2,y2,x3,y3,x,y))
               {
                draw_pixel(x,y,color);
            }
        }
    }
}
internal void draw_filled_3point_pixels(int x0, int y0, int x1, int y1, int x2, int y2, u32 color){
    int startY, endY, startX, endX;
    startX = x0;
    if(startX > x1) startX = x1;
    if(startX > x2) startX = x2;
    startY = y0;
    if(startY > y1) startY = y1;
    if(startY > y2) startY = y2;
    endX = x0;
    if(endX < x1) endX = x1;
    if(endX < x2) endX = x2;
    endY = y0;
    if(endY < y1) endY = y1;
    if(endY < y2) endY = y2;
    draw_triangle_outline_pixels(x0,y0,x1,y1,x2,y2,color);
    for(int y = startY; y < endY; y++){
        for(int x = startX; x < endX; x++){
            if(pointInsideTriangle(x0,y0,x1,y1,x2,y2,x,y)){
                draw_pixel(x,y,color);
            }
        }
    }
}
internal void draw_filled_triangle(f32 x0, f32 y0,f32 x1, f32 y1, f32 x2, f32 y2, u32 color){
    convert_to_aspect6(&x0,&y0,&x1,&y1,&x2,&y2);
    draw_filled_3point_pixels(x0,y0,x1,y1,x2,y2,color);
}
internal void draw_polygon3(Polygon3 poly){
    draw_filled_triangle(poly.v0.x, poly.v0.y, poly.v1.x, poly.v1.y, poly.v2.x, poly.v2.y, poly.color);
}
internal void draw_polygon4(Polygon4 poly){
    convert_to_aspect8(&poly.v0.x, &poly.v0.y, &poly.v1.x, &poly.v1.y, &poly.v2.x, &poly.v2.y, &poly.v3.x, &poly.v3.y);
    draw_filled_4point_pixels(poly.v0.x, poly.v0.y, poly.v1.x, poly.v1.y, poly.v2.x, poly.v2.y, poly.v3.x, poly.v3.y, poly.color);
}
internal Point cast_line(Ray ray, Line line){
     f32 x1 = line.v0.x;
     f32 y1 = line.v0.y;
     f32 x2 = line.v1.x;
     f32 y2 = line.v1.y;
    
     f32 x3 = ray.v0.x;
     f32 y3 = ray.v0.y;
     f32 x4 = ray.v0.x + ray.v1.x;
     f32 y4 = ray.v0.y + ray.v1.y;
    
    f32 den = (x1-x2)*(y3-y4)-(y1-y2)*(x3-x4);
    if(den == 0){
        return NULL_RAY;
    }
     f32 t = ((x1-x3)*(y3-y4)-(y1-y3)*(x3-x4))/den;
     f32 u = -((x1-x2)*(y1-y3)-(y1-y2)*(x1-x3))/den;
    if(t > 0 && t < 1 && u > 0){
         Point p;
        p.v0.x = x1 + t * (x2-x1);
        p.v0.y = y1 + t * (y2-y1);
        p.valid = true;
        return p;
    }
    return NULL_RAY;
}
internal Point cast_polygon3(Ray ray, Polygon3 poly){
     Point p1 = cast_line(ray,(Line){poly.v0,poly.v1});
     Point p2 = cast_line(ray,(Line){poly.v1,poly.v2});
    Point p3 = cast_line(ray,(Line){poly.v2,poly.v0});
    Point result = p1;
    if((p2.valid &&  distance(p2.v0,ray.v0) < distance(result.v0,ray.v0)) || !result.valid){
        result = p2;
    }
    if((p3.valid &&  distance(p3.v0,ray.v0) < distance(result.v0,ray.v0)) || !result.valid){
        result = p3;
    }
    if(!result.valid){
        return NULL_RAY;
    }else{
        return result; 
    }
    return NULL_RAY;
}
internal Point cast_polygon4(Ray ray, Polygon4 poly){
    Point p1 = cast_line(ray,(Line){poly.v0,poly.v1});
    Point p2 = cast_line(ray,(Line){poly.v1,poly.v2});
    Point p3 = cast_line(ray,(Line){poly.v2,poly.v3});
    Point p4 = cast_line(ray,(Line){poly.v3,poly.v0});
    Point result = p1;
    if((p2.valid &&  distance(p2.v0,ray.v0) < distance(result.v0,ray.v0)) || !result.valid){
        result = p2;
    }
    if((p3.valid &&  distance(p3.v0,ray.v0) < distance(result.v0,ray.v0)) || !result.valid){
        result = p3;
    }
    if((p4.valid &&  distance(p4.v0,ray.v0) < distance(result.v0,ray.v0)) || !result.valid){
        result = p4;
    }
    if(!result.valid){
        return NULL_RAY;
    }else{
        return result; 
    }
    return NULL_RAY;
}