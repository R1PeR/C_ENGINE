struct{
    b32 is_down;
    b32 changed;
}typedef Button;
global int button_count = 256;
struct{
    V2 mouse;
    int raw_mouse_x;
    int raw_mouse_y;
    //Button mouse_buttons[3];
    Button buttons[256];
}typedef Input;

#define pressed(b) (input.buttons[b].is_down && input.buttons[b].changed)
#define released(b) (!input.buttons[b].is_down && input.buttons[b].changed)
#define down(b) (input.buttons[b].is_down)