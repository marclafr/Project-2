#ifndef _ENTITY
#define _ENTITY

#include "p2Point.h"

enum ENTITY_TYPE
{
	NO_ENTITY = 0,
	BUILDING,
	UNIT
};
enum ENTITY_STATUS
{
	SELECTEDD = 0
};
class Entity
{
private:
	bool to_delete;
	iPoint position;
	int hp;
	int armor;
	enum ENTITY_TYPE entity_type;
	enum ENTITY_STATUS entity_status = SELECTEDD;
public:
	Entity(ENTITY_TYPE entity_type, iPoint pos);
	~Entity();

	virtual void Update() = 0;
	virtual void AI() = 0;
	virtual void Draw() = 0;


	void Die();//to_delete = true



	//Geters
	bool ToDelete() const;
	ENTITY_TYPE GetEntityType() const;
	ENTITY_STATUS GetEntityStatus();
	const int GetX() const;
	const int GetY() const;

protected:
	//Seters
	void SetArmor(int new_armor);
	void SetHp(int new_hp);
};
#endif
