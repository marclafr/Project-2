#ifndef __j1TEXTURES_H__
#define __j1TEXTURES_H__

#include "j1Module.h"
#include <vector>

struct SDL_Texture;
struct SDL_Surface;

enum TextureID
{
	T_UI,
	T_MAP,
	T_TILESET,
	T_AOE_UI_FONT,
	T_CONSOLE_TEXT,
	T_TURRET,
	T_WALL,
	T_TOWNHALL,
	T_RESOURCE,
	T_ARROW,
	T_ARROW_BOMB,
	T_FIRE,

	//Units:
	T_VILLAGER,
		//INFANTRY
	T_MILITIA,
	T_ENEMY_MILITIA,
	T_MANATARMS,
	T_ENEMY_MANATARMS,
	T_LONGSWORDMAN,
	T_TWOHANDEDSWORDMAN,
	T_CHAMPION,

	T_SPEARMAN,
	T_ENEMY_SPEARMAN,
	T_PIKEMAN,
	T_ENEMY_PIKEMAN,

		//ARCHERS
	T_ARCHER,
	T_ENEMY_ARCHER,
	T_ARBALEST,
	T_ENEMY_ARBALEST,
	T_CAVALRYARCHER,
	T_ENEMY_CAVALRYARCHER,
	T_HEAVYCAVALRYARCHER,
	T_ENEMY_HEAVYCAVALRYARCHER,
	
		//CAVALRY
	T_KNIGHT,
	T_ENEMY_KNIGHT,
	T_CAVALIER,
	T_ENEMY_CAVALIER,
	T_PALADIN,
	T_ENEMY_PALADIN,
		
		//SIEGE
	T_SIEGERAM
	//--
};

struct Texture
{
private:
	SDL_Texture* texture;
	TextureID id;

public:

	Texture(SDL_Texture* tex, TextureID id);
	Texture(const Texture& copy);
	~Texture();

	TextureID GetID() const;
	SDL_Texture* GetTexture() const;

};

class j1Textures : public j1Module
{
public:

	j1Textures();

	// Destructor
	virtual ~j1Textures();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before the first frame
	bool Start();

	// Called before quitting
	bool CleanUp();

	// Load Texture
	SDL_Texture* const	Load(const char* path, TextureID id);
	bool UnLoad(Texture* texture);
	bool UnLoad(TextureID id);
	SDL_Texture* const	LoadSurface(SDL_Surface* surface, TextureID id);

	void GetSize(const SDL_Texture* texture, uint& width, uint& height) const;
	SDL_Texture* GetTexture(const TextureID id) const;

	//Quick Solution for Labels on UIHUDPanelInfo
	bool UnloadLabel(SDL_Texture* texture);
	SDL_Texture* const j1Textures::LoadSurfaceLabel(SDL_Surface* surface);

private:

	std::vector<Texture*> textures;
};


#endif // __j1TEXTURES_H__