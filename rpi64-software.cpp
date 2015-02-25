#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <string>
#include <iostream>
#include <map>


//==============================================================================
class Element {
public:
	Element(std::string filename, bool show = true);
	~Element();
	void apply_surface(int x, int y, SDL_Surface* destination);
	bool show;
	std::string path;

private:
	SDL_Surface* surface;
};

Element::Element(std::string filename, bool show){
	this->show = show;
	this->path = filename;

	SDL_Surface* loadedImage = NULL;
	SDL_Surface* optimizedImage = NULL;

	loadedImage = IMG_Load (filename.c_str() );

	if( loadedImage != NULL){
		surface = SDL_DisplayFormatAlpha( loadedImage );

		SDL_FreeSurface( loadedImage );
	}
}

Element::~Element(){
	SDL_FreeSurface( surface );
}

void Element::apply_surface(int x, int y, SDL_Surface* destination){
	if( show ){
		SDL_Rect offset;
		offset.x = x;
		offset.y = y;

		SDL_BlitSurface( surface, NULL, destination, &offset);
	}
}	

//==============================================================================
int main(int argc, char** argv){

	const int SCREEN_WITDH  = 800;
	const int SCREEN_HEIGHT = 650;
	const int SCREEN_BPM    = 32;

	SDL_Surface* screen     = NULL;

	if( SDL_Init ( SDL_INIT_EVERYTHING ) == -1)		return -1;
	IMG_Init( IMG_INIT_PNG );

	screen = SDL_SetVideoMode( SCREEN_WITDH, 
		                       SCREEN_HEIGHT, 
		                       SCREEN_BPM, 
		                       SDL_SWSURFACE );

	std::cout << "nr joysticks: " << SDL_NumJoysticks() << std::endl;

	if (screen == NULL) return -1;

	SDL_WM_SetCaption( "RPi64 Software", NULL );

	// prepate base (joystick shape), top (buttons) and all glows
	Element base( "images/base.png" );
	Element top( "images/top.png" );

	std::map<int, Element*> btns;
	btns[SDLK_UP]    = new Element( "images/glow_yellow_top.png", false);
	btns[SDLK_DOWN]  = new Element( "images/glow_yellow_bottom.png", false);
	btns[SDLK_LEFT]  = new Element( "images/glow_yellow_left.png", false);
	btns[SDLK_RIGHT] = new Element( "images/glow_yellow_right.png", false);

	// loop
	bool stop = false;
	SDL_Event event;

	while( !stop ) {
		while ( SDL_PollEvent(&event) ){
			if (event.type == SDL_QUIT ) {
				stop = true;
				std::cout << "exiting" << std::endl;
			} 
			else if( event.type == SDL_KEYDOWN ){
				std::cout << "SDL_KEYDOWN: " << event.key.keysym.sym << std::endl;
				btns[event.key.keysym.sym]->show = true;
			} 
			else if ( event.type == SDL_KEYUP ){
				std::cout << "SDL_KEYUP: " << event.key.keysym.sym << std::endl;
				btns[event.key.keysym.sym]->show = false;
			}
		}

		SDL_FillRect(screen, NULL, 0x000000);
		base.apply_surface( 0, 0, screen);

		for(std::map<int, Element*>::iterator iterator = btns.begin(); 
				iterator != btns.end(); 
				iterator++) {
    		iterator->second->apply_surface( 0, 0, screen);;
		}

		top.apply_surface( 0, 0, screen);

		SDL_Flip(screen);
		SDL_Delay(100);
	}

	// clear elements
	for(std::map<int, Element*>::iterator iterator = btns.begin(); 
			iterator != btns.end(); 
			iterator++) {
		delete iterator->second;
	}

	SDL_Quit();

	return 0;
}
