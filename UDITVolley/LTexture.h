#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include <string>

#define SCREEN_WIDTH                        640
#define SCREEN_HEIGHT                       480


class LTexture
{
public:
	//Initializes variables
	LTexture();

	//Deallocates memory
	~LTexture();

	//Loads image at specified path
	bool load_from_file(std::string path, SDL_Renderer* gRenderer);

    //Creates image from font string
    bool load_from_renderer_text(std::string textureText, SDL_Color textColor, TTF_Font* gFont, SDL_Renderer* gRenderer);

	//Deallocates texture
	void free();

	//Set color modulation
	void set_color(Uint8 red, Uint8 green, Uint8 blue);

	//Set blending
	void set_blend_mode(SDL_BlendMode blending);

	//Set alpha modulation
	void set_alpha(Uint8 alpha);

	//Renders texture at given point
	void render(SDL_Renderer* gRenderer, int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);

	//Gets image dimensions
	int get_width();
	int get_height();

private:
	//The actual hardware texture
	SDL_Texture* mTexture;

	//Image dimensions
	int mWidth;
	int mHeight;

};

