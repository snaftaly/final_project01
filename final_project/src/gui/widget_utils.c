#include "widget_utils.h"

Widget * createGeneralWidget(Sint16 x, Sint16 y, Uint16 width, Uint16 height, widget_type type){
	Widget * new_widget = malloc(sizeof(Widget));
	if(new_widget == NULL){
		perrorPrint("malloc");
		return NULL;
	}
	new_widget->type = type;
	SDL_Rect location_rect = {x, y, width, height};
	new_widget->location_rect = location_rect;
	new_widget->parentWidget = NULL;
	return new_widget;
}

Widget * create_window(int width, int height, int red, int green, int blue){
	/*SDL_WM_SetCaption("sharon and yishay", "sharon and yishay");*/
	Widget * new_window = createGeneralWidget(0,0,width,height,WINDOW);
	if(new_window == NULL){
		return NULL;
	}
	new_window->surface  = SDL_SetVideoMode(width, height, 0, SDL_HWSURFACE | SDL_DOUBLEBUF);
	if (new_window->surface == NULL){
		sdlErrorPrint("failed to set video mode");
		return NULL;
	}
	if (SDL_FillRect(new_window->surface,0,SDL_MapRGB(new_window->surface->format, red, green, blue)) != 0) {
		sdlErrorPrint("failed to fill rect with color");
		return NULL;
	}
	return new_window;
}

Widget * create_panel(int x, int y, int width, int height, int red, int green, int blue){
	Widget * new_panel = createGeneralWidget(x, y, width, height, PANEL);
	if(new_panel == NULL){
		return NULL;
	}
	new_panel->surface  = SDL_CreateRGBSurface(0, width, height, 32,0, 0, 0, 0);
	if (new_panel->surface == NULL){
		sdlErrorPrint("failed to create RGB surface");
	}
	if (SDL_FillRect(new_panel->surface,0,SDL_MapRGB(new_panel->surface->format, red, green, blue)) != 0) {
		sdlErrorPrint("failed to fill rect with color");
		return NULL;
	}
	return new_panel;
}

Widget * create_image(int x, int y, int width, int height, SDL_Surface * img,
		int imgx, int imgy){ //, int setTransparent
	Widget * new_image = createGeneralWidget(x, y, width, height, IMAGE);
	if(new_image == NULL){
		return NULL;
	}
	new_image->surface = img;
	SDL_Rect img_rect = {imgx, imgy, width ,height};
	new_image->img_rect= img_rect;
	//if (setTransparent)
		//setImageTransparent(new_image, 255, 255, 255);
	return new_image;
}

Widget * create_button(Sint16 x, Sint16 y, Uint16 width, Uint16 height, SDL_Surface * img,
		Sint16 imgSx, Sint16 imgSy, Sint16 imgNSx, Sint16 imgNSy, int isSelected){
	Widget * new_button = createGeneralWidget(x, y, width, height, BUTTON);
	if(new_button == NULL){
		return NULL;
	}
	new_button->surface = img;
	SDL_Rect button_selected_rect = {imgSx, imgSy, width ,height};
	SDL_Rect button_non_selected_rect = {imgNSx, imgNSy, width ,height};
	new_button->button_selected_rect = button_selected_rect;
	new_button->button_non_selected_rect = button_non_selected_rect;
	new_button->isButtonSelected = isSelected;
	if (isSelected)
		new_button->img_rect = button_selected_rect;
	else
		new_button->img_rect = button_non_selected_rect;
	return new_button;
}

void setButtonSelected(Widget * button){
	button->isButtonSelected = 1;
	button->img_rect = button->button_selected_rect;
}

void setButtonNotSelected(Widget * button){
	button->isButtonSelected = 0;
	button->img_rect = button->button_non_selected_rect;
}

void setButtonDisabled(Widget * button){
	setButtonSelected(button);
}

void setButtonEnabled(Widget * button){
	setButtonNotSelected(button);
}

void setImageTransparent(Widget *image, int red, int green, int blue){
	if (SDL_SetColorKey(image->surface, SDL_SRCCOLORKEY, SDL_MapRGB(image->surface->format, red, green, blue)) !=0){
		sdlErrorPrint("failed setting image transparent");
		isError = 1;
	}
}

void clearPanel(void * panel){
	Widget * toClear = panel;
	/* get panel colors */
	Uint32 temp, pixel;
	Uint8 red, green, blue;
	SDL_PixelFormat * fmt=toClear->surface->format;
	pixel=*((Uint32*)toClear->surface->pixels);
	temp=pixel&fmt->Rmask; /* Isolate red component */
	temp=temp>>fmt->Rshift;/* Shift it down to 8-bit */
	temp=temp<<fmt->Rloss; /* Expand to a full 8-bit number */
	red=(Uint8)temp;
	temp=pixel&fmt->Gmask; /* Isolate green component */
	temp=temp>>fmt->Gshift;/* Shift it down to 8-bit */
	temp=temp<<fmt->Gloss; /* Expand to a full 8-bit number */
	green=(Uint8)temp;
	/* Get Blue component */
	temp=pixel&fmt->Bmask; /* Isolate blue component */
	temp=temp>>fmt->Bshift;/* Shift it down to 8-bit */
	temp=temp<<fmt->Bloss; /* Expand to a full 8-bit number */
	blue=(Uint8)temp;
	/* recolor the panel */
	if (SDL_FillRect(toClear->surface,0,SDL_MapRGB(toClear->surface->format, red, green, blue)) != 0) {
		sdlErrorPrint("failed to fill rect with color");
		return;
	}
}


void freeWidget(void * widgetPtr){
	Widget * widget = widgetPtr;
	if (widget != NULL){
		if (widget->surface != NULL && (widget->type == PANEL || widget->type == WINDOW)){ //WINDOW??????
			SDL_FreeSurface(widget->surface); //free() invalid pointer error!!!
		}
		free(widget);
	}
}
