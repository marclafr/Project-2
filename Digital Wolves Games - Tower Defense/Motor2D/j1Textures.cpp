#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1FileSystem.h"
#include "j1Textures.h"
#include "Entity.h"

#include "SDL_image/include/SDL_image.h"
#pragma comment( lib, "SDL_image/libx86/SDL2_image.lib" )

j1Textures::j1Textures() : j1Module()
{
	name.assign("textures");
}

// Destructor
j1Textures::~j1Textures()
{}

// Called before render is available
bool j1Textures::Awake(pugi::xml_node& config)
{
	LOG("Init Image library");
	bool ret = true;
	// load support for the PNG image format
	int flags = IMG_INIT_PNG;
	int init = IMG_Init(flags);

	if ((init & flags) != flags)
	{
		LOG("Could not initialize Image lib. IMG_Init: %s", IMG_GetError());
		ret = false;
	}

	return ret;
}

// Called before the first frame
bool j1Textures::Start()
{
	LOG("start textures");

	//ADD UNIT: IF ANY UNIT IS ADDED ADD CODE HERE:
	//Units
		//INFANTRY
	App->tex->Load("animations/Units/Militia.png", T_MILITIA);
	App->tex->Load("animations/Units/MilitiaRed.png", T_ENEMY_MILITIA);
	App->tex->Load("animations/Units/Manatarms.png", T_MANATARMS);
	App->tex->Load("animations/Units/ManatarmsRed.png", T_ENEMY_MANATARMS);
	App->tex->Load("animations/Units/Longswordman.png", T_LONGSWORDMAN);
	App->tex->Load("animations/Units/Twohandedswordman.png", T_TWOHANDEDSWORDMAN);
	App->tex->Load("animations/Units/Champion.png", T_CHAMPION);
	
	App->tex->Load("animations/Units/Spearman.png", T_SPEARMAN);
	App->tex->Load("animations/Units/SpearmanRed.png", T_ENEMY_SPEARMAN);
	App->tex->Load("animations/Units/Pikeman.png", T_PIKEMAN);
	App->tex->Load("animations/Units/PikemanRed.png", T_ENEMY_PIKEMAN);
	
		//ARCHERS
	App->tex->Load("animations/Units/Archer.png", T_ARCHER);
	App->tex->Load("animations/Units/ArcherRed.png", T_ENEMY_ARCHER);
	App->tex->Load("animations/Units/Arbalest.png", T_ARBALEST);
	App->tex->Load("animations/Units/ArbalestRed.png", T_ENEMY_ARBALEST);
	App->tex->Load("animations/Units/Cavalryarcher.png", T_CAVALRYARCHER);
	App->tex->Load("animations/Units/CavalryarcherRed.png", T_ENEMY_CAVALRYARCHER);
	App->tex->Load("animations/Units/Heavycavalryarcher.png", T_HEAVYCAVALRYARCHER);
	App->tex->Load("animations/Units/HeavycavalryarcherRed.png", T_ENEMY_HEAVYCAVALRYARCHER);

		//CAVALRY
	App->tex->Load("animations/Units/Knight.png", T_KNIGHT);
	App->tex->Load("animations/Units/KnightRed.png", T_ENEMY_KNIGHT);
	App->tex->Load("animations/Units/Cavalier.png", T_CAVALIER);
	App->tex->Load("animations/Units/CavalierRed.png", T_ENEMY_CAVALIER);
	App->tex->Load("animations/Units/Paladin.png", T_PALADIN);
	App->tex->Load("animations/Units/PaladinRed.png", T_ENEMY_PALADIN);

		//SIEGE
	App->tex->Load("animations/Units/SiegeRam.png", T_SIEGERAM);
	//--
	App->tex->Load("textures/Towers.png", T_TURRET);
	App->tex->Load("animations/StoneWall.png", T_WALL);
	App->tex->Load("textures/Extras.png", T_TOWNHALL);
	App->tex->Load("textures/Resouce.png", T_RESOURCE);
	App->tex->Load("animations/HorizontalArrowAngles.png", T_ARROW);
	App->tex->Load("textures/ArrowsBombs.png", T_ARROW_BOMB);
	App->tex->Load("animations/fireTest.png", T_FIRE);

	bool ret = true;
	return ret;
}

// Called before quitting
bool j1Textures::CleanUp()
{
	LOG("Freeing textures and Image library");
	std::vector<Texture*>::iterator item;

	for (item = textures.begin(); item != textures.end(); ++item)
	{
		delete (*item);
	}

	textures.clear();
	IMG_Quit();
	return true;
}

// Load new texture from file path
SDL_Texture* const j1Textures::Load(const char* path, TextureID id)
{
	SDL_Texture* texture = NULL;
	SDL_Surface* surface = IMG_Load_RW(App->fs->Load(path), 1);

	if (surface == NULL)
	{
		LOG("Could not load surface with path: %s. IMG_Load: %s", path, IMG_GetError());
	}
	else
	{
		//ignores pink from images
		SDL_SetColorKey(surface, 1, SDL_MapRGB(surface->format, 0xFF, 0, 0xFF));

		texture = LoadSurface(surface, id);
		SDL_FreeSurface(surface);
	}

	return texture;
}

// Unload texture
bool j1Textures::UnLoad(Texture* texture)
{
	for (int i = 0; i < textures.size(); i++)
	{
		if (texture == textures[i])
		{
			delete (textures[i]);
			std::vector<Texture*>::iterator item = textures.begin() + i;
			textures.erase(item);
			return true;
		}
	}

	return false;
}

bool j1Textures::UnLoad(TextureID id)
{
	std::vector<Texture*>::iterator item;

	for (int i = 0; i < textures.size(); i++)
	{
		if (id == textures[i]->GetID())
		{
			delete (textures[i]);
			std::vector<Texture*>::iterator item = textures.begin() + i;
			textures.erase(item);
			return true;
		}
	}

	return false;
}

// Translate a surface into a texture
SDL_Texture* const j1Textures::LoadSurface(SDL_Surface* surface, TextureID id)
{
	SDL_Texture* texture = SDL_CreateTextureFromSurface(App->render->renderer, surface);

	if (texture == NULL)
	{
		LOG("Unable to create texture from surface! SDL Error: %s\n", SDL_GetError());
	}
	else
	{
		Texture* ptr = new Texture(texture, id);
		textures.push_back(ptr);
	}

	return texture;
}

// Retrieve size of a texture
void j1Textures::GetSize(const SDL_Texture* texture, uint& width, uint& height) const
{
	SDL_QueryTexture((SDL_Texture*)texture, NULL, NULL, (int*)&width, (int*)&height);
}

//-------------------------------------------------------------------------

Texture::Texture(SDL_Texture * tex, TextureID id) : texture(tex), id(id)
{}

Texture::Texture(const Texture & copy): id(copy.id)
{}

Texture::~Texture()
{
	//SDL_DestroyTexture(texture);
}

TextureID Texture::GetID() const
{
	return id;
}

SDL_Texture * Texture::GetTexture() const
{
	return texture;
}

SDL_Texture * j1Textures::GetTexture(const TextureID id) const
{
	for (int i = 0; i < textures.size(); i++)
		if (textures[i]->GetID() == id)
			return textures[i]->GetTexture();
	return nullptr;
}

//Quick fix for labels on HUDUIPanelInfo
bool j1Textures::UnloadLabel(SDL_Texture * texture)
{
	SDL_DestroyTexture(texture);
	texture = nullptr;
	return true;
}

SDL_Texture * const j1Textures::LoadSurfaceLabel(SDL_Surface * surface)
{
	SDL_Texture* texture = SDL_CreateTextureFromSurface(App->render->renderer, surface);

	if (texture == NULL)
	{
		LOG("Unable to create texture from surface! SDL Error: %s\n", SDL_GetError());
	}

	return texture;
}
