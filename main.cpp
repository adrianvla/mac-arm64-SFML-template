#define GL_SILENCE_DEPRECATION
#include <SFML/Graphics/RenderWindow.hpp>
#include <stdio.h>
#include <iostream>
#include <thread>
#include <utility>
#include <chrono>
#include <fstream>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <math.h>
#define PI 3.1415926535897932384626433832795



extern "C" {
    #include "/opt/homebrew/Cellar/lua/5.4.4_1/include/lua5.4/lauxlib.h"
    #include "/opt/homebrew/Cellar/lua/5.4.4_1/include/lua5.4/lua.h"
    #include "/opt/homebrew/Cellar/lua/5.4.4_1/include/lua5.4/lualib.h"
}

using namespace std;
using namespace sf;


extern "C" int SampleCFunction(lua_State* L)
{
    int a = lua_tointeger(L, 1); // First argument
    int b = lua_tointeger(L, 2); // Second argument

    lua_pushinteger(L, a+b);

    return 1; // Count of returned values
}

void placePixel(uint x, uint y, uint8_t r, uint8_t g, uint8_t b, uint8_t a, uint W, uint H, uint8_t* pixels){
    pixels[(((y*W) + x)*4)] = r;
    pixels[(((y*W) + x)*4)+1] = g;
    pixels[(((y*W) + x)*4)+2] = b;
    pixels[(((y*W) + x)*4)+3] = a;
}


void drawline (int x1, int y1, int x2, int y2, uint8_t r, uint8_t g, uint8_t b, uint8_t a, uint W, uint H, uint8_t* pixels, void (*f)(uint,uint,uint8_t,uint8_t,uint8_t,uint8_t,uint,uint,uint8_t*))
{
    int x,y,dx,dy,dx1,dy1,px,py,xe,ye,i;
    dx=x2-x1;
    dy=y2-y1;
    dx1=abs(dx);
    dy1=abs(dy);
    px=2*dy1-dx1;
    py=2*dx1-dy1;
    if(dy1<=dx1){
        if(dx>=0){
            x=x1;
            y=y1;
            xe=x2;
        }else{
            x=x2;
            y=y2;
            xe=x1;
        }
        (*f)(x,y,r,g,b,a,W,H,pixels);
        for(i=0;x<xe;i++){
            x++;
            if(px<0){
                px=px+2*dy1;
            }else{
                if((dx<0 && dy<0) || (dx>0 && dy>0)){
                    y++;
                }else{
                    y--;
                }
                px=px+2*(dy1-dx1);
            }
            (*f)(x,y,r,g,b,a,W,H,pixels);
        }
    }else{
        if(dy>=0){
            x=x1;
            y=y1;
            ye=y2;
        }else{
            x=x2;
            y=y2;
            ye=y1;
        }
        (*f)(x,y,r,g,b,a,W,H,pixels);

        for(i=0;y<ye;i++){
            y++;
            if(py<=0){
                py=py+2*dx1;
            }else{
                if((dx<0 && dy<0) || (dx>0 && dy>0)){
                        x++;
                    }else{
                        x--;
                    }
                py=py+2*(dx1-dy1);
            }
            (*f)(x,y,r,g,b,a,W,H,pixels);
        }
    }
}



int hyp[1000000];
void drawTriangle(int x0, int y0, int x1, int y1, int x2, int y2, uint8_t r, uint8_t g, uint8_t b, uint8_t a, uint W, uint H, uint8_t* pixels, void (*f)(uint,uint,uint8_t,uint8_t,uint8_t,uint8_t,uint,uint,uint8_t*)){
    // y0<=y1<=y2
    int t0,t1;
    if(y0>y1){
        t0=x1;
        t1=y1;
        x1=x0;
        y1=y0;
        x0=t0;
        y0=t1;
    }
    if(y1>y2){
        t0=x2;
        t1=y2;
        x2=x1;
        y2=y1;
        x1=t0;
        y1=t1;
    }
    if(y0>y2){
        t0=x2;
        t1=y2;
        x2=x0;
        y2=y0;
        x0=t0;
        y0=t1;
    }

    int x,y,dx,dy,dx0,dy0,px,py,xe,ye,i;
    int _x,_y,_dx,_dy,_dx0,_dy0,_px,_py,_xe,_ye,_i;
    int X,Y,DX,DY,DX1,DY1,PX,PY,XE,YE,I;
    uint U = 0;
    dx=x1-x0;
    dy=y1-y0;
    dx0=abs(dx);
    dy0=abs(dy);
    px=2*dy0-dx0;
    py=2*dx0-dy0;
    bool sw;
    if(dy0<=dx0){
        if(dx>=0){
            x=x0;
            y=y0;
            xe=x1;
        }else{
            x=x1;
            y=y1;
            xe=x0;
        }
        sw=false;
    }else{
        if(dy>=0){
            x=x0;
            y=y0;
            ye=y1;
        }else{
            x=x1;
            y=y1;
            ye=y0;
        }
        sw=true;
    }
    // for(I=0;I<DY0;I++) hyp[I]=100;
    // put line into hyp
    
    if(x2>=x0){//&a     \ 
        // hdtf(0,0,x2-x0,y2-y0,hyp);
        DX=x2-x0;
        DY=y2-y0;
        DX1=abs(DX);
        DY1=abs(DY);
        PX=2*DY1-DX1;
        PY=2*DX1-DY1;
        if(DY1<=DX1){
            if(DX>=0){
                X=0;
                Y=0;
                XE=x2-x0;
            }else{
                X=x2-x0;
                Y=y2-y0;
                XE=0;
            }
            // (*f)(x,y,r,g,b,a,W,H,pixels);
            hyp[Y]=abs(X);

            for(I=0;X<XE;I++){
                X++;
                if(PX<0){
                    PX=PX+2*DY1;
                }else{
                    if((DX<0 && DY<0) || (DX>0 && DY>0)){
                        Y++;
                    }else{
                        Y--;
                    }
                    PX=PX+2*(DY1-DX1);
                }
                // hyp[Y]=X;
                hyp[Y]=abs(X);
                
            }
        }else{
            if(DY>=0){
                X=0;
                Y=0;
                YE=y2-y0;
            }else{
                X=x2-x0;
                Y=y2-y0;
                YE=0;
            }
            // hyp[Y]=X;
            hyp[Y]=abs(X);


            for(I=0;Y<YE;I++){
                Y++;
                if(PY<=0){
                    PY=PY+2*DX1;
                }else{
                    if((DX<0 && DY<0) || (DX>0 && DY>0)){
                            X++;
                        }else{
                            X--;
                        }
                    PY=PY+2*(DX1-DY1);
                }
                // hyp[Y]=X;
                hyp[Y]=abs(X);

            }
        }
    }else{//&a          / 
        // hdtf(x0-x2,0,0,y2-y0,hyp);
        DX=-x0+x2;
        DY=y2-y0;
        DX1=abs(DX);
        DY1=abs(DY);
        PX=2*DY1-DX1;
        PY=2*DX1-DY1;
        if(DY1<=DX1){
            if(DX>=0){
                X=x0-x2;
                Y=0;
                XE=0;
            }else{
                X=0;
                Y=y2-y0;
                XE=x0-x2;
            }
            // (*f)(x,y,r,g,b,a,W,H,pixels);
            hyp[Y]=X;

            for(I=0;X<XE;I++){
                X++;
                if(PX<0){
                    PX=PX+2*DY1;
                }else{
                    if((DX<0 && DY<0) || (DX>0 && DY>0)){
                        Y++;
                    }else{
                        Y--;
                    }
                    PX=PX+2*(DY1-DX1);
                }
                hyp[Y]=X;
            }
        }else{
            if(DY>=0){
                X=x0-x2;
                Y=0;
                YE=y2-y0;
            }else{
                X=0;
                Y=y2-y0;
                YE=0;
            }
            hyp[Y]=X;

            for(I=0;Y<YE;I++){
                Y++;
                if(PY<=0){
                    PY=PY+2*DX1;
                }else{
                    if((DX<0 && DY<0) || (DX>0 && DY>0)){
                            X++;
                        }else{
                            X--;
                        }
                    PY=PY+2*(DX1-DY1);
                }
                hyp[Y]=X;
            }
        }
    }




    for(i=0;(sw?y:x)<(sw?ye:xe);i++){ //0 - 1
        sw?y++:x++;
        if((sw?py:px)<0){
            if(sw){
                py=py+2*dx0;
            }else{
                px=px+2*dy0;
            }
        }else{
            if((dx<0 && dy<0) || (dx>0 && dy>0)){
                sw?x++:y++;
            }else{
                sw?x--:y--;
            }
            if(sw){
                py=py+2*(dx0-dy0);
            }else{
                px=px+2*(dy0-dx0);
            }
        }
        for(U = x;U<hyp[y-y0]+(x2>=x0?x0:x2);U++){
            (*f)(U,y,r,g,b,a,W,H,pixels);
        }
    }
    _dx=x2-x1;
    _dy=y2-y1;
    _dx0=abs(_dx);
    _dy0=abs(_dy);
    _px=2*_dy0-_dx0;
    _py=2*_dx0-_dy0;
    bool _sw;
    if(_dy0<=_dx0){
        if(_dx>=0){
            _x=x1;
            _y=y1;
            _xe=x2;
        }else{
            _x=x2;
            _y=y2;
            _xe=x1;
        }
        _sw=false;
    }else{
        if(_dy>=0){
            _x=x1;
            _y=y1;
            _ye=y2;
        }else{
            _x=x2;
            _y=y2;
            _ye=y1;
        }
        _sw=true;
    }

    for(_i=0;(_sw?_y:_x)<(_sw?_ye:_xe);_i++){
        _sw?_y++:_x++;
        if((_sw?_py:_px)<0){
            if(_sw){
                _py=_py+2*_dx0;
            }else{
                _px=_px+2*_dy0;
            }
        }else{
            if((_dx<0 && _dy<0) || (_dx>0 && _dy>0)){
                _sw?_x++:_y++;
            }else{
                _sw?_x--:_y--;
            }
            if(_sw){
                _py=_py+2*(_dx0-_dy0);
            }else{
                _px=_px+2*(_dy0-_dx0);
            }
        }

        for(U = _x;U<hyp[_y-y0]+(x2>=x0?x0:x2);U++){
            (*f)(U,_y,r,g,b,a,W,H,pixels);
        }
    }


}

void render(uint8_t* pixels, sf::Texture* tex, uint W, uint H, sf::Clock* clk){
    tex->create(sf::Vector2u(W, H)); 
    uint s = clk->getElapsedTime().asMilliseconds();
    // cout << "W: " << W << "\nH: " << H << "\n" << s << "\n";
    uint i = 0;
    // int X = 0;
    // int Y = 0;
    // for(int y = 0;y < H;y++){
    //     for(int x = 0;x < W;x++){
    //         i=((y*W) + x)*4;
    //         pixels[i] = (x*255)/W;    //r
    //         pixels[i+1] = (y*255)/H;    //g
    //         pixels[i+2] = ((x+y)*255)/(H+W);    //b
    //         pixels[i+3] = 255;  //a
    //     }
    // }
    for(i;i<H*W*4;i+=4){
        pixels[i]= 0;
        pixels[i+1]= 0;
        pixels[i+2]= 0;
        pixels[i+3]= 255;
    }
    // drawline(100,100,200,200,0,255,0,255,W,H,pixels,&placePixel);
    // drawline(200,100,100,200,0,255,0,255,W,H,pixels,&placePixel);
    // drawline(250,100,350,50,0,255,0,255,W,H,pixels,&placePixel);
    // drawline(350,100,250,50,0,255,0,255,W,H,pixels,&placePixel);
    drawTriangle(200,200,150,300,230,350,255,255,255,255,W,H,pixels,&placePixel);
    
    // drawline(100,100,100+(cos(s/100)*50),100+(sin(s/100)*50),s%270,(s+100)%200,(s+150)%120,255,W,H,pixels);
    tex->update(pixels);
}

int main()
{

    // Request a 24-bits depth buffer when creating the window
    sf::ContextSettings contextSettings;
    contextSettings.depthBits = 24;
    contextSettings.stencilBits = 8;
    contextSettings.antialiasingLevel = 4;

    lua_State *L = luaL_newstate();
    luaL_openlibs(L);
    lua_register(L, "SampleCFunction", SampleCFunction);

    int SampleCValue = 13;

    lua_pushinteger(L, SampleCValue);
    lua_setglobal(L, "SampleCValue");

    luaL_dofile(L, "script.lua");

    // Create the main window
    uint W = 1280;
    uint H = 480;
    sf::RenderWindow window(sf::VideoMode({W, H}), "test", sf::Style::Default, contextSettings);
    window.setVerticalSyncEnabled(true);

    uint8_t* pixels = new uint8_t[W*H*4];
    
    sf::Texture texture;
    texture.create(sf::Vector2u(W, H)); 

    // sf::Sprite sp_px(texture); 

    sf::Clock clock;
    render(pixels, &texture, W, H, &clock);

    texture.update(pixels);
    // Make it the active window for OpenGL calls
    window.setActive();

    
    // sf::Clock deltaClock;
    // std::thread renderer(renderingThread, &window, &clock, &deltaClock); //&r
    // Start the game loop
    while (window.isOpen())
    {
        // Process events
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Close window: exit
            if (event.type == sf::Event::Closed)
                window.close();

            // Escape key: exit
            if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape))
                window.close();

            // Resize event: adjust the viewport
            if (event.type == sf::Event::Resized){
                W = event.size.width;
                H = event.size.height;
                // sf::FloatRect view(0, 0, event.size.width, event.size.height);
                sf::FloatRect view(Vector2<float>(0,0), Vector2<float>(event.size.width, event.size.height));
                window.setView(sf::View(view));
                render(pixels, &texture, W, H, &clock);
            }
                // glViewport(0, 0, event.size.width, event.size.height);
        }
        render(pixels, &texture, W, H, &clock);

        window.clear();
        sf::Sprite sp_px(texture); 

        window.draw(sp_px);
        window.display();
        // break;
        // Clear the color and depth buffers
    }
    free(pixels);
    return EXIT_SUCCESS;
}
