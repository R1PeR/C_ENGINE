#include <windows.h>
#include <stdint.h>
#include <stdio.h>
#include "utils.c"
#include "math.c"
#include "platform_common.c"
struct {
    int width, height;
    u32 *pixels;
    BITMAPINFO bitmap_info;
} typedef Render_Buffer;
global Render_Buffer render_buffer;

#include "software_rendering.c"
#include "game.c"

internal LRESULT CALLBACK window_callback(
                        HWND    window,
                        UINT    message,
                        WPARAM  wParam,
                        LPARAM  lParam
                                          )
{
    LRESULT result = 0;
    switch(message){
        case WM_CLOSE:
        case WM_DESTROY:{
        running = false;
        PostQuitMessage(0);
        }break;
        case WM_SIZE:{
            RECT rect;
            GetWindowRect(window, &rect);
            render_buffer.width = rect.right - rect.left;
            render_buffer.height = rect.bottom - rect.top;
            if(render_buffer.pixels){
                VirtualFree(render_buffer.pixels,0,MEM_RELEASE);
            }
            render_buffer.pixels = VirtualAlloc(0, sizeof(u32)*render_buffer.width*render_buffer.height, MEM_COMMIT|MEM_RESERVE,PAGE_READWRITE);
            render_buffer.bitmap_info.bmiHeader.biSize = sizeof(render_buffer.bitmap_info.bmiHeader);
            render_buffer.bitmap_info.bmiHeader.biWidth = render_buffer.width;
            render_buffer.bitmap_info.bmiHeader.biHeight = render_buffer.height;
            render_buffer.bitmap_info.bmiHeader.biPlanes = 1;
            render_buffer.bitmap_info.bmiHeader.biBitCount = 32;
            render_buffer.bitmap_info.bmiHeader.biCompression = BI_RGB;
        }break;
        default:
        result = DefWindowProcA(window, message, wParam, lParam);
        break;
    }
    return result;
}

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd){
    WNDCLASSA window_class = {0};
    window_class.style = CS_HREDRAW | CS_VREDRAW;
    window_class.lpszClassName = "Game_Window_Class";
    window_class.lpfnWndProc = window_callback;
    RegisterClassA(&window_class);
    HWND window = CreateWindowExA(0,window_class.lpszClassName, "Game_window", WS_VISIBLE|WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 1280, 720, 0, 0, 0, 0);
    HDC hdc = GetDC(window);
    
    RAWINPUTDEVICE Rid[1];
    Rid[0].usUsagePage = 0x01;
    Rid[0].usUsage = 0x02;
    Rid[0].dwFlags = 0; //ideally RIDEV_INPUTSINK but that prevents registration
    Rid[0].hwndTarget = window; //ideally this would be Null to be independent of the active window
    if (RegisterRawInputDevices(Rid, 1, sizeof(Rid[0])) == FALSE) {
        //registration failed. Call GetLastError for the cause of the error
        printf("Registration Error");
    }
#if 0
    AllocConsole();
	freopen("CONOUT$", "w", stdout);
	freopen("CONIN$", "r", stdin);
	//sync_with_stdio();
#endif
    u32 objSize = 0;
    Input input = {0};
    Render_object obj[10];
    u32 add_new = false;
    u32 obj_size = 0;
    V2 obj_to_add[4];
    //Polygon3 polyArray[2];
    //polyArray[0] = (Polygon3){0.0,0.0,1.0,1.0,1.0,0.0,0x33ffff};
    //polyArray[1] = (Polygon3){2.0,2.0,3.0,3.0,3.0,0.0,0x33ffff};
    //Point rayPoint = (Point){-1.0,-1.0};
    //Ray ray = (Ray){-1.0,-1.0,1.0,1.0};
    V2 rayPoint = {0.0,0.0};
    
    while(running){
        //input
        MSG message;
        for(int i = 0; i<button_count; i++) input.buttons[i].changed = false;
        //for(int i = 0; i<3; i++) input.mouse_buttons[i].changed = false;
        while(PeekMessageA(&message, window, 0, 0, PM_REMOVE)){
            switch(message.message){
                case WM_SYSKEYUP:
                case WM_SYSKEYDOWN:
                case WM_KEYUP:
                case WM_KEYDOWN:{
                    u32 vk_code = (u32)message.wParam;
                    b32 was_down  = ((message.lParam & (1 << 30)) != 0);
                    b32 is_down = ((message.lParam & (1 << 31)) == 0);
                    //if(vk_code > )
                        input.buttons[vk_code].changed = (is_down != input.buttons[vk_code].is_down);
                    input.buttons[vk_code].is_down = is_down;
                }break;
                case WM_INPUT:{
                    
                    u32 dwsize;
                    //HRAWINPUT hRawInput = (HRAWINPUT)message.lParam;
                    GetRawInputData((HRAWINPUT)message.lParam, RID_INPUT,NULL,&dwsize,sizeof(RAWINPUTHEADER) );
                    LPBYTE * buffer = VirtualAlloc(0, dwsize, MEM_COMMIT|MEM_RESERVE, PAGE_READWRITE);
                    if(GetRawInputData((HRAWINPUT)message.lParam, RID_INPUT, buffer, &dwsize, sizeof(RAWINPUTHEADER) )){
                        POINT p;
                        GetCursorPos(&p);
                        ScreenToClient(window, &p);
                        input.mouse = back_convert_to_aspect2(p.x,render_buffer.height-p.y);
                        RAWINPUT * raw = (RAWINPUT *)buffer;
                        input.raw_mouse_x = raw->data.mouse.lLastX;
                        input.raw_mouse_y = raw->data.mouse.lLastY;
                        switch(raw->data.mouse.usButtonFlags){
                            case RI_MOUSE_LEFT_BUTTON_DOWN:{
                                input.buttons[VK_LBUTTON].is_down = true;
                                input.buttons[VK_LBUTTON].changed = true;
                            }break;
                            case RI_MOUSE_RIGHT_BUTTON_DOWN:{
                                input.buttons[VK_RBUTTON].is_down = true;
                                input.buttons[VK_RBUTTON].changed = true;
                            }break;
                            case RI_MOUSE_MIDDLE_BUTTON_DOWN:{
                                input.buttons[VK_MBUTTON].is_down = true;
                                input.buttons[VK_MBUTTON].changed = true;
                            }break;
                            case RI_MOUSE_LEFT_BUTTON_UP:{
                                input.buttons[VK_LBUTTON].is_down = false;
                                input.buttons[VK_LBUTTON].changed = true;
                            }break;
                            case RI_MOUSE_RIGHT_BUTTON_UP:{
                                input.buttons[VK_RBUTTON].is_down = false;
                                input.buttons[VK_RBUTTON].changed = true;
                            }break;
                            case RI_MOUSE_MIDDLE_BUTTON_UP:{
                                input.buttons[VK_MBUTTON].is_down = false;
                                input.buttons[VK_MBUTTON].changed = true;
                            }break;
                            default:
                            break;
                        }
                        };
                    VirtualFree(buffer,0,MEM_RELEASE);
                }break;
                default:{
            TranslateMessage(&message);
            DispatchMessage(&message);
                }
            }
        }
        
        //game
        clear_screen(0x222222);
        
        if(pressed(0x4E)){
            add_new = true;
        }
        if(add_new){
            if(pressed(0x01)){
                obj_to_add[obj_size++] = input.mouse;
            }
            if(pressed(0x0D)){
                switch (obj_size){
                    case 2:{
                        obj[objSize].type = e_line;
                        obj[objSize].line = (Line){obj_to_add[0],obj_to_add[1],0x33ffff};
                        objSize++;
                        add_new = false;
                        obj_size = 0;
                    }break;
                    case 3:{
                        obj[objSize].type = e_polygon3;
                        obj[objSize].polygon3 = (Polygon3){obj_to_add[0], obj_to_add[1], obj_to_add[2], 0x33ffff};
                        objSize++;
                        add_new = false;
                        obj_size = 0;
                    }break;
                    case 4:{
                        obj[objSize].type = e_polygon4;
                        obj[objSize].polygon4 = (Polygon4){obj_to_add[0], obj_to_add[1], obj_to_add[2], obj_to_add[3], 0x33ffff};
                        objSize++;
                        add_new = false;
                        obj_size = 0;
                    }break;
                    default:
                    break;
                }
            }
        }
            rayPoint.x = input.mouse.x;
            rayPoint.y = input.mouse.y;
        for(int i = 0; i < 6;i++){
            switch(obj[i].type){
                case e_line:{
                    draw_line_obj(obj[i].line);
                }break;
                case e_polygon3:{
                    draw_polygon3(obj[i].polygon3);
                }break;
                case e_polygon4:{
                    draw_polygon4(obj[i].polygon4);
                }
                default:
                break;
            }
        }
        //ray.v1 = normalized_pos(ray.v0,input.mouse);
        //draw_line_vector(ray.v0,vectorAdd(ray.v0,vectorMultiply(ray.v1,0.2)),0x33ffff);
        for(int j = 0; j < 360;j+=1){
            Ray r;
            r.v0 = rayPoint;
             r.v1 = deg_to_pol(j);
            //draw_circle(rayPoint.x,rayPoint.y,0.1,0x33ffff);
            Point point = NULL_RAY;
        for(int i = 0; i < objSize; i ++){
                Point temp = NULL_RAY; 
                switch(obj[i].type){
                    case e_line:{
                        temp = cast_line(r,obj[i].line);
                    }break;
                    case e_polygon3:{
                        temp = cast_polygon3(r,obj[i].polygon3);
                    }break;
                    case e_polygon4:{
                        temp = cast_polygon4(r,obj[i].polygon4);
                    }break;
                    default:
                    break;
                }
            if((temp.valid && (distance(r.v0,point.v0) > distance(r.v0,temp.v0))) || !point.valid){
                point = temp;
            }
        }
        if(point.valid){
                draw_line_vector(rayPoint,point.v0,0xffffff);
            }else{
                draw_line_vector(rayPoint,(V2){rayPoint.x+r.v1.x*100,rayPoint.y+r.v1.y*100},0xffffff);
            }
        }
        //render
        //BITMAPINFO bitmap_info; 
        StretchDIBits(
                                        hdc, 0, 0, render_buffer.width, render_buffer.height, 0, 0, render_buffer.width, render_buffer.height, render_buffer.pixels, &render_buffer.bitmap_info, DIB_RGB_COLORS, SRCCOPY
                          ); 
    }
    return 0;
}