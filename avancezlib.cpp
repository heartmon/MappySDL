#include "avancezlib.h"




// Creates the main window. Returns true on success.
//bool AvancezLib::init(int width, int height) {
//	init(width, height, nullptr);
//}
bool AvancezLib::init(int width, int height, Router* router, GameViewport* gameViewport)
{
	SDL_LogSetAllPriority(SDL_LOG_PRIORITY_WARN);

	SDL_Log("Initializing the system...\n");

	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL failed the initialization: %s\n", SDL_GetError());
		return false;
	}

	//Create window
	window = SDL_CreateWindow("aVANCEZ", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
	if (window == NULL)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Window could not be created! SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	//Create renderer for window
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == NULL)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Renderer could not be created! SDL Error: %s\n", SDL_GetError());
		return false;
	}

	TTF_Init();
	font = TTF_OpenFont("data/space_invaders.ttf", 16); //this opens a font style and sets a size
	fontLarge = TTF_OpenFont("data/space_invaders.ttf", 24); //this opens a font style and sets a size
	if (font == NULL)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "font cannot be created! SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	// initialize the keys
	key.fire = false;	key.left = false;	key.right = false;	key.enter = false;

	//Initialize renderer color
	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);

	//Clear screen
	SDL_RenderClear(renderer);

	SDL_Log("System up and running...\n");

	this->router = router;
	this->gameViewport = gameViewport;

	return true;
}


// Destroys the avancez library instance
void AvancezLib::destroy()
{
	SDL_Log("Shutting down the system\n");

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	TTF_CloseFont(font);
	TTF_CloseFont(fontLarge);

	TTF_Quit();
	SDL_Quit();
}


bool AvancezLib::update()
{
	bool go_on = true;
	SDL_Event event;


	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
			go_on = false;

		if (event.type == SDL_KEYDOWN)
		{
			switch (event.key.keysym.sym)
			{
			case SDLK_ESCAPE:
			case SDLK_q:
				go_on = false;
				break;
			case SDLK_SPACE:
				key.fire = true;
				break;
			case SDLK_LEFT:
				key.left = true;
				break;
			case SDLK_RIGHT:
				key.right = true;
				break;
			case SDLK_RETURN:
				key.enter = true;
				break;
			}
		}

		if (event.type == SDL_KEYUP)
		{
			switch (event.key.keysym.sym)
			{
			case SDLK_SPACE:
				key.fire = false;
				break;
			case SDLK_LEFT:
				key.left = false;
				break;
			case SDLK_RIGHT:
				key.right = false;
				break;
			case SDLK_RETURN:
				key.enter = false;
				break;
			}
		}

	}

	//Update screen
	SDL_RenderPresent(renderer);

	//Clear screen
	SDL_RenderClear(renderer);

	return go_on;
}


Sprite * AvancezLib::createSprite(const char * path)
{
	SDL_Surface* surf = SDL_LoadBMP(path);
	if (surf == NULL)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to load image %s! SDL_image Error: %s\n", path, SDL_GetError());
		return NULL;
	}

	//Create texture from surface pixels
	SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, surf);
	if (texture == NULL)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to create texture from %s! SDL Error: %s\n", path, SDL_GetError());
		return NULL;
	}

	//Get rid of old loaded surface
	SDL_FreeSurface(surf);

	Sprite * sprite = new Sprite(renderer, texture);

	return sprite;
}

SpriteSheet * AvancezLib::createSpriteSheet(const char * path) {
	//Get rid of preexisting texture
	//free();

	int width = 0;
	int height = 0;

	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path);
	if (loadedSurface == NULL)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to load image %s! SDL_image Error: %s\n", path, IMG_GetError());
	}
	else
	{
		//SDL_Log("LOAD FILE SUCCESSFULLY");
		//Color key image
		SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));

		//Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
		if (newTexture == NULL)
		{
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to create texture from %s! SDL Error: %s\n", path, SDL_GetError());
		}
		else
		{
			//Get image dimensions
			width = loadedSurface->w;
			height = loadedSurface->h;

			//SDL_Log("LOADED IMAGE DIMENSION: W - %d", mWidth);
			//SDL_Log("LOADED IMAGE DIMENSION: W - %d", mHeight);
		}


		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	SpriteSheet * sprite = new SpriteSheet(renderer, newTexture, width, height);

	return sprite;

	//Return success
	/*mTexture = newTexture;
	return mTexture != NULL;


	SpriteSheet spriteSheetTexture;

	spriteSheetTexture.setRenderer(renderer);
	spriteSheetTexture.loadFromFile(path);

	SDL_Log("%d", spriteSheetTexture.getWidth());

	return &spriteSheetTexture;*/
}



Sprite * AvancezLib::createSpriteNonBmp(const char * path) {
	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path);
	if (loadedSurface == NULL)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to load image %s! SDL_image Error: %s\n", path, IMG_GetError());
		return NULL;
	}
	//Create texture from surface pixels
	SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, loadedSurface);

	if (texture == NULL)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to create texture from %s! SDL Error: %s\n", path, SDL_GetError());
		return NULL;
	}

	//Get rid of old loaded surface
	SDL_FreeSurface(loadedSurface);

	Sprite * sprite = new Sprite(renderer, texture);

	return sprite;
}
void AvancezLib::drawText(int x, int y, const char * msg) {
	SDL_Color white = { 255, 255, 255 };
	drawText(x, y, msg, white, false);
}
void AvancezLib::drawText(int x, int y, const char * msg, SDL_Color color, bool isLargeFont)
{
	//SDL_Color black = { 255, 255, 255 };  // this is the color in rgb format, maxing out all would give you the color white, and it will be your text's color
	TTF_Font* toBeRenderedFont;
	if (isLargeFont) {
		toBeRenderedFont = fontLarge;
	}
	else {
		toBeRenderedFont = font;
	}
	SDL_Surface* surf = TTF_RenderText_Solid(toBeRenderedFont, msg, color); // as TTF_RenderText_Solid could only be used on SDL_Surface then you have to create the surface first

	SDL_Texture* msg_texture = SDL_CreateTextureFromSurface(renderer, surf); //now you can convert it into a texture

	int w = 0;
	int h = 0;
	SDL_QueryTexture(msg_texture, NULL, NULL, &w, &h);
	SDL_Rect dst_rect = { x, y, w, h };

	SDL_RenderCopy(renderer, msg_texture, NULL, &dst_rect);

	SDL_DestroyTexture(msg_texture);
	SDL_FreeSurface(surf);
}

float AvancezLib::getElapsedTime()
{
	return SDL_GetTicks() / 1000.f;
}

void AvancezLib::getKeyStatus(KeyStatus & keys)
{
	keys.fire = key.fire;
	keys.left = key.left;
	keys.right = key.right;
	keys.enter = key.enter;
}


Sprite::Sprite(SDL_Renderer * renderer, SDL_Texture * texture)
{
	this->renderer = renderer;
	this->texture = texture;
}


void Sprite::draw(int x, int y)
{
	SDL_Rect rect;

	rect.x = x;
	rect.y = y;

	SDL_QueryTexture(texture, NULL, NULL, &(rect.w), &(rect.h));

	//Render texture to screen
	SDL_RenderCopy(renderer, texture, NULL, &rect);
}

void Sprite::draw(int x, int y, int w, int h) {
	SDL_Rect rect;

	rect.x = x;
	rect.y = y;
	rect.w = w;
	rect.h = h;

	SDL_QueryTexture(texture, NULL, NULL, &(rect.w), &(rect.h));

	//Render texture to screen
	SDL_RenderCopy(renderer, texture, NULL, &rect);
}

void Sprite::draw(SDL_Rect* rect) {
	SDL_QueryTexture(texture, NULL, NULL, &(rect->w), &(rect->h));

	//Render texture to screen
	SDL_RenderCopy(renderer, texture, NULL, rect);
}

void Sprite::destroy()
{
	SDL_DestroyTexture(texture);
}

//SpriteSheet


SpriteSheet::SpriteSheet(SDL_Renderer * renderer, SDL_Texture * texture, int width, int height)
{
	//Initialize
	this->renderer = renderer;
	this->mTexture = texture;
	//mTexture = NULL;
	mWidth = width;
	mHeight = height;
}

SpriteSheet::~SpriteSheet()
{
	//Deallocate
	//free();
}


void SpriteSheet::SetTintColor(unsigned int  r, unsigned int g, unsigned int b) {
	SDL_SetTextureColorMod(mTexture, r, g, b);
}

//
//bool SpriteSheet::loadFromFile(const char * path)
//{
//	//Get rid of preexisting texture
//	free();
//
//	//The final texture
//	SDL_Texture* newTexture = NULL;
//
//	//Load image at specified path
//	SDL_Surface* loadedSurface = IMG_Load(path);
//	if (loadedSurface == NULL)
//	{
//		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to load image %s! SDL_image Error: %s\n", path, IMG_GetError());
//	}
//	else
//	{
//		SDL_Log("LOAD FILE SUCCESSFULLY");
//		//Color key image
//		SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));
//
//		//Create texture from surface pixels
//		newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
//		if (newTexture == NULL)
//		{
//			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to create texture from %s! SDL Error: %s\n", path, SDL_GetError());
//		}
//		else
//		{
//			//Get image dimensions
//			mWidth = loadedSurface->w;
//			mHeight = loadedSurface->h;
//
//			SDL_Log("LOADED IMAGE DIMENSION: W - %d", mWidth);
//			SDL_Log("LOADED IMAGE DIMENSION: W - %d", mHeight);
//		}
//
//
//		//Get rid of old loaded surface
//		SDL_FreeSurface(loadedSurface);
//	}
//
//	//Return success
//	mTexture = newTexture;
//	return mTexture != NULL;
//}

void SpriteSheet::free()
{
	//Free texture if it exists
	if (mTexture != NULL)
	{
		SDL_Log("Destroy texture?");
		SDL_DestroyTexture(mTexture);
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}

void SpriteSheet::Destroy()
{
	//if (mTexture != nullptr) {
	//	SDL_DestroyTexture(mTexture);
	//}
}


void SpriteSheet::render(int x, int y, SDL_Rect* clip, bool flip)
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };

	//Set clip rendering dimensions
	if (clip != NULL)
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	//Render to screen
	if (flip) {
		SDL_RenderCopyEx(renderer, mTexture, clip, &renderQuad, 0, 0, SDL_FLIP_HORIZONTAL);
	}
	else {
		SDL_RenderCopy(renderer, mTexture, clip, &renderQuad);
	}
}

int SpriteSheet::getWidth()
{
	return mWidth;
}

int SpriteSheet::getHeight()
{
	return mHeight;
}

void SpriteSheet::setRenderer(SDL_Renderer* renderer) {
	this->renderer = renderer;
}