#ifndef __j1PATHFINDING_H__
#define __j1PATHFINDING_H__

#include <vector>
#include <list>

#include "j1Module.h"
#include "p2Point.h"
#include "j1PerfTimer.h"


#define DEFAULT_PATH_LENGTH 0
#define INVALID_WALK_CODE 255

//JPS
#define DIAGONAL_COST 1.414214f // sqrt 2
#define STRAIGHT_COST 1.0f
#define FIND_RADIUS 10
#define NUM_TILES 75 * 75

// --------------------------------------------------
// Recommended reading:
// Intro: http://www.raywenderlich.com/4946/introduction-to-a-pathfinding
// Details: http://theory.stanford.edu/~amitp/GameProgramming/
// --------------------------------------------------

struct PathNode;
struct ForcedNeighbour;
class Entity;
class Elipse;
class Unit;

enum X_DIRECTION
{
	X_NO_DIR,
	X_RIGHT,
	X_LEFT
};

enum Y_DIRECTION
{
	Y_NO_DIR,
	Y_UP,
	Y_DOWN
};

class j1PathFinding : public j1Module
{
public:

	j1PathFinding();

	// Destructor
	~j1PathFinding();

	//Awake
	bool Start();

	// Called before quitting
	bool CleanUp();
	void CleanUpJPS();

	// Sets up the walkability map
	void SetMap(uint width, uint height, uchar* data);
	void SetConstructibleMaps(uint width, uint height, uchar* data, uchar* data2);

	//float CreatePath(const iPoint & origin, const iPoint & destination);
	// To request all tiles involved in the last generated path
	const std::vector<iPoint>& GetLastPath() const;

	// Utility: return true if pos is inside the map boundaries
	bool CheckBoundaries(const iPoint& tile) const;

	// Utility: returns true is the tile is walkable
	bool IsWalkable(const iPoint& tile) const;
	bool IsConstructible_ally(const iPoint& tile) const;
	bool IsConstructible_neutral(const iPoint& tile) const;
	void MakeNoConstruible_ally(const iPoint& tile);
	void MakeNoConstruible_neutral(const iPoint& tile);
	void MakeConstruible_ally(const iPoint& tile);
	void MakeConstruible_neutral(const iPoint& tile);
	void MakeNoWalkable(const iPoint& tile);
	void MakeWalkable(const iPoint& tile);
	iPoint FindEmptyTile(iPoint from, Elipse collision) const;
	iPoint FindClosestEmptyAttackTile(iPoint objective_pos, int tile_range, Entity* attacker);

	// Utility: return the walkability value of a tile
	uchar GetTileAt(const iPoint& pos) const;
	uchar GetTileAtConstructible_ally(const iPoint& pos) const;
	uchar GetTileAtConstructible_neutral(const iPoint& pos) const;

	PathNode* GetPathNode(int x, int y);

	void Debug();

	void AddPath(std::vector<iPoint>* path);

	bool IsEmpty(const iPoint tile, const Entity* exeption = nullptr) const;

	iPoint FindNearestEmpty(Unit* unit) const;

	bool IsInPath(const iPoint& tile, const std::vector<iPoint>& path) const;

	const iPoint& FindNearestWalkableToDestination(const Unit* unit) const;

	//JPS
	//Main function of JPS Algorithm
	bool CalculatePath(iPoint origin,const iPoint& destination, std::vector<iPoint>& vec_to_fill);

private:

	//Look for forced neighbours from one point and in one direction (open a node)
	bool CheckForTiles(const PathNode* start, X_DIRECTION dx, Y_DIRECTION dy);

	//Open 4 diagonals from the origin
	void OpenOrigin();

	//Check Directions
	bool CheckX(const PathNode* start, X_DIRECTION dir);
	bool CheckY(const PathNode* start, Y_DIRECTION dir);
	void MoveDiagonal(PathNode* diagonal, X_DIRECTION dx, Y_DIRECTION dy, bool& diagonal_end);

	//Called When ForcedNeighbour is found
	bool FoundForcedNeighbour(const PathNode* before, iPoint after_pos,const PathNode* path_to);
	bool FoundForcedNeighbour(const PathNode* before, iPoint after_pos);

	//Called when destination is reached through a path
	void DestinationReached(const PathNode* destination, const PathNode* path_to);
	void DestinationReached(const PathNode* destination);

	//check for neighbours before moving diagonal
	bool CheckForForcedNeighbours(const PathNode* node, X_DIRECTION dx, Y_DIRECTION dy);

	//Push new node to list or change existing node with same pos so that it has the better path
	const PathNode* PushToVisited(const PathNode* node);

	//Push new ForcedNeighbour to list or change existing ForcedNeighbour so that it has the better path
	void PushToFrontier(ForcedNeighbour& fn);

	//Get Lowest Priority Forced neighbour aka most probable best path 
	bool GetLowestFN(ForcedNeighbour& fn);

	//Fill given vector after finding Path 
	void FillPathVec(std::vector<iPoint>& vec);

	//Update cost_so_far of all Nodes opened from a given tile after changin this ones cost
	void ChangeCosts(std::vector<PathNode*>::iterator from, float new_cost);

	//Getters
	const bool FrontierFinished() const;
	const PathNode* GetNodeFromVisited(const iPoint& pos);
	
	//Chech if pointers are in the list & delete them if not	
	void DeleteIfNotPushed(PathNode*& ptr);
	//JPS end

private:

	// size of the map
	uint width;
	uint height;

	// all map walkability values [0..255]
	uchar* map = nullptr;
	PathNode* node_map = nullptr;
	uchar* constructible_map_ally = nullptr;
	uchar* constructible_map_neutral = nullptr;

	// we store the created path here
	std::vector<iPoint> last_path;

	std::vector<std::vector<iPoint>*> allied_paths;

	//JPS
	bool destination_reached = false;
	PathNode* origin = nullptr;
	PathNode* destination = nullptr;
	std::list<ForcedNeighbour> frontier;
	std::vector<PathNode*> visited;

	SDL_Texture* debug_tex;
};

// ---------------------------------------------------------------------
// Pathnode: Helper struct to represent a node in the path creation
// ---------------------------------------------------------------------

//JPS
struct PathNode
{
	// -----------
	float cost_so_far;
	float distance;
	float priority;
	iPoint pos;
	iPoint parent; // needed to reconstruct the path in the end

	// Convenient constructors
	PathNode();
	PathNode(iPoint pos);
	PathNode(float cost, float dist, const iPoint& pos,iPoint parent);
	PathNode(const PathNode& node);

	~PathNode();
	
	//Usefull
	float CalculateDistance(iPoint destination);
	float CalculatePriority(const iPoint& destination);

	//Operators
	const PathNode& operator = (const PathNode& node);
	inline bool operator > (const PathNode & rhs) const;
	inline bool operator < (const PathNode & rhs) const;
	inline bool operator == (const PathNode & rhs) const;	
};

struct ForcedNeighbour
{
	iPoint before;
	iPoint after;
	float priority;
	bool opened;

	//Constructors
	ForcedNeighbour(const iPoint& before,const iPoint& after, float priority);
	ForcedNeighbour();

	//Operators
	const ForcedNeighbour& operator = (const ForcedNeighbour& rhs);
};



#endif // __j1PATHFINDING_H__