#pragma once
#include "Definitions.h"
#include "LawnApp.h"

#pragma region Enums
enum SeedType
{
	SEED_PEASHOOTER = 0x0,
	SEED_SUNFLOWER = 0x1,
	SEED_CHERRYBOMB = 0x2,
	SEED_WALLNUT = 0x3,
	SEED_POTATOMINE = 0x4,
	SEED_SNOWPEA = 0x5,
	SEED_CHOMPER = 0x6,
	SEED_REPEATER = 0x7,
	SEED_PUFFSHROOM = 0x8,
	SEED_SUNSHROOM = 0x9,
	SEED_FUMESHROOM = 0xA,
	SEED_GRAVEBUSTER = 0xB,
	SEED_HYPNOSHROOM = 0xC,
	SEED_SCAREDYSHROOM = 0xD,
	SEED_ICESHROOM = 0xE,
	SEED_DOOMSHROOM = 0xF,
	SEED_LILYPAD = 0x10,
	SEED_SQUASH = 0x11,
	SEED_THREEPEATER = 0x12,
	SEED_TANGLEKELP = 0x13,
	SEED_JALAPENO = 0x14,
	SEED_SPIKEWEED = 0x15,
	SEED_TORCHWOOD = 0x16,
	SEED_TALLNUT = 0x17,
	SEED_SEASHROOM = 0x18,
	SEED_PLANTERN = 0x19,
	SEED_CACTUS = 0x1A,
	SEED_BLOVER = 0x1B,
	SEED_SPLITPEA = 0x1C,
	SEED_STARFRUIT = 0x1D,
	SEED_PUMPKIN = 0x1E,
	SEED_MAGNETSHROOM = 0x1F,
	SEED_CABBAGEPULT = 0x20,
	SEED_FLOWERPOT = 0x21,
	SEED_KERNELPULT = 0x22,
	SEED_INSTANT_COFFEE = 0x23,
	SEED_GARLIC = 0x24,
	SEED_UMBRELLA = 0x25,
	SEED_MARIGOLD = 0x26,
	SEED_MELONPULT = 0x27,
	SEED_GATLINGPEA = 0x28,
	SEED_TWINSUNFLOWER = 0x29,
	SEED_GLOOMSHROOM = 0x2A,
	SEED_CATTAIL = 0x2B,
	SEED_WINTERMELON = 0x2C,
	SEED_GOLD_MAGNET = 0x2D,
	SEED_SPIKEROCK = 0x2E,
	SEED_COBCANNON = 0x2F,
	SEED_IMITATER = 0x30,
	SEED_EXPLODE_O_NUT = 0x31,
	SEED_GIANT_WALLNUT = 0x32,
	SEED_SPROUT = 0x33,
	SEED_LEFTPEATER = 0x34,
	NUM_SEED_TYPES = 0x35,
	SEED_BEGHOULED_BUTTON_SHUFFLE = 0x36,
	SEED_BEGHOULED_BUTTON_CRATER = 0x37,
	SEED_SLOT_MACHINE_SUN = 0x38,
	SEED_SLOT_MACHINE_DIAMOND = 0x39,
	SEED_ZOMBIQUARIUM_SNORKLE = 0x3A,
	SEED_ZOMBIQUARIUM_TROPHY = 0x3B,
	SEED_ZOMBIE_NORMAL = 0x3C,
	SEED_ZOMBIE_TRAFFIC_CONE = 0x3D,
	SEED_ZOMBIE_POLEVAULTER = 0x3E,
	SEED_ZOMBIE_PAIL = 0x3F,
	SEED_ZOMBIE_LADDER = 0x40,
	SEED_ZOMBIE_DIGGER = 0x41,
	SEED_ZOMBIE_BUNGEE = 0x42,
	SEED_ZOMBIE_FOOTBALL = 0x43,
	SEED_ZOMBIE_BALLOON = 0x44,
	SEED_ZOMBIE_SCREEN_DOOR = 0x45,
	SEED_ZOMBONI = 0x46,
	SEED_ZOMBIE_POGO = 0x47,
	SEED_ZOMBIE_DANCER = 0x48,
	SEED_ZOMBIE_GARGANTUAR = 0x49,
	SEED_ZOMBIE_IMP = 0x4A,
	NUM_SEEDS_IN_CHOOSER = 0x31,
	SEED_NONE = 0xFFFFFFFF
};
enum PlantState
{
	STATE_NOTREADY = 0x0,
	STATE_READY = 0x1,
	STATE_DOINGSPECIAL = 0x2,
	STATE_SQUASH_LOOK = 0x3,
	STATE_SQUASH_PRE_LAUNCH = 0x4,
	STATE_SQUASH_RISING = 0x5,
	STATE_SQUASH_FALLING = 0x6,
	STATE_SQUASH_DONE_FALLING = 0x7,
	STATE_GRAVEBUSTER_LANDING = 0x8,
	STATE_GRAVEBUSTER_EATING = 0x9,
	STATE_CHOMPER_BITING = 0xA,
	STATE_CHOMPER_BITING_GOT_ONE = 0xB,
	STATE_CHOMPER_BITING_MISSED = 0xC,
	STATE_CHOMPER_DIGESTING = 0xD,
	STATE_CHOMPER_SWALLOWING = 0xE,
	STATE_POTATO_RISING = 0xF,
	STATE_POTATO_ARMED = 0x10,
	STATE_POTATO_MASHED = 0x11,
	STATE_SPIKEWEED_ATTACKING = 0x12,
	STATE_SPIKEWEED_ATTACKING_2 = 0x13,
	STATE_SCAREDYSHROOM_LOWERING = 0x14,
	STATE_SCAREDYSHROOM_SCARED = 0x15,
	STATE_SCAREDYSHROOM_RAISING = 0x16,
	STATE_SUNSHROOM_SMALL = 0x17,
	STATE_SUNSHROOM_GROWING = 0x18,
	STATE_SUNSHROOM_BIG = 0x19,
	STATE_MAGNETSHROOM_SUCKING = 0x1A,
	STATE_MAGNETSHROOM_CHARGING = 0x1B,
	STATE_BOWLING_UP = 0x1C,
	STATE_BOWLING_DOWN = 0x1D,
	STATE_CACTUS_LOW = 0x1E,
	STATE_CACTUS_RISING = 0x1F,
	STATE_CACTUS_HIGH = 0x20,
	STATE_CACTUS_LOWERING = 0x21,
	STATE_TANGLEKELP_GRABBING = 0x22,
	STATE_COBCANNON_ARMING = 0x23,
	STATE_COBCANNON_LOADING = 0x24,
	STATE_COBCANNON_READY = 0x25,
	STATE_COBCANNON_FIRING = 0x26,
	STATE_KERNELPULT_BUTTER = 0x27,
	STATE_UMBRELLA_TRIGGERED = 0x28,
	STATE_UMBRELLA_REFLECTING = 0x29,
	STATE_IMITATER_MORPHING = 0x2A,
	STATE_ZEN_GARDEN_WATERED = 0x2B,
	STATE_ZEN_GARDEN_NEEDY = 0x2C,
	STATE_ZEN_GARDEN_HAPPY = 0x2D,
	STATE_MARIGOLD_ENDING = 0x2E,
	STATE_FLOWERPOT_INVULNERABLE = 0x2F,
	STATE_LILYPAD_INVULNERABLE = 0x30
};
enum PlantOnBungeeState
{
	NOT_ON_BUNGEE = 0x0,
	GETTING_GRABBED_BY_BUNGEE = 0x1,
	RISING_WITH_BUNGEE = 0x2
};
enum MagnetItemType
{
	MAGNET_ITEM_NONE = 0x0,
	MAGNET_ITEM_PAIL_1 = 0x1,
	MAGNET_ITEM_PAIL_2 = 0x2,
	MAGNET_ITEM_PAIL_3 = 0x3,
	MAGNET_ITEM_FOOTBALL_HELMET_1 = 0x4,
	MAGNET_ITEM_FOOTBALL_HELMET_2 = 0x5,
	MAGNET_ITEM_FOOTBALL_HELMET_3 = 0x6,
	MAGNET_ITEM_DOOR_1 = 0x7,
	MAGNET_ITEM_DOOR_2 = 0x8,
	MAGNET_ITEM_DOOR_3 = 0x9,
	MAGNET_ITEM_POGO_1 = 0xA,
	MAGNET_ITEM_POGO_2 = 0xB,
	MAGNET_ITEM_POGO_3 = 0xC,
	MAGNET_ITEM_JACK_IN_THE_BOX = 0xD,
	MAGNET_ITEM_LADDER_1 = 0xE,
	MAGNET_ITEM_LADDER_2 = 0xF,
	MAGNET_ITEM_LADDER_3 = 0x10,
	MAGNET_ITEM_LADDER_PLACED = 0x11,
	MAGNET_ITEM_SILVER_COIN = 0x12,
	MAGNET_ITEM_GOLD_COIN = 0x13,
	MAGNET_ITEM_DIAMOND = 0x14,
	MAGNET_ITEM_PICK_AXE = 0x15
};
#pragma endregion

struct MagnetItem
{
	FVector2 Position;
	FVector2 Destination;
	MagnetItemType ItemType;
};

class Plant : public GameObject
{
	typedef void(__stdcall* Die_Type)(Plant*);
	typedef void(__stdcall* Fire_Type)(Plant*, Zombie*, int, bool);
public:
#pragma region Fields
	SeedType Type;
	int Column;
	int AnimTimer;
	int CurrentFrame;
	int TicksPerFrame;
	int FrameCount;
	PlantState State;
	int Health;
	int MaxHealth;
	int Subclass;
	int VanishTimer;
	int SpecialTimer;
	int StateTimer;
	int ActionTimer;
	int ActionRate;
	IRect PlantRect;
	IRect PlantAttackRect;
	IVector2 Target;
	int StartLane;
	ParticleSystemID ParticleID;
	int ShootingTimer;
	ReanimationID BodyReanimID;
	ReanimationID HeadReanimID;
	ReanimationID HeadReanimID2;
	ReanimationID HeadReanimID3;
	ReanimationID BlinkReanimID;
	ReanimationID LightReanimID;
	ReanimationID SleepingReanimID;
	int BlinkTimer;
	int EatenTimer;
	int GlowTimer;
	int HighlightTimer;
	FVector2 ShakeOffset;
	MagnetItem MagnetItems[5];
	ZombieID TargetZombieID;
	int WakingTimer;
	PlantOnBungeeState OnBungeeState;
	SeedType ImitaterType;
	int PottedPlantIndex;
	bool AnimPing;
	bool Dead;
	bool Squished;
	bool Asleep;
	bool OnLawn;
	bool Highlighted;
#pragma endregion

	Plant()
	{
		Visible = true;
		RenderOrder = 400000;
		TicksPerFrame = 10;
		Bounds = IRect(0, 0, 100, 100);
		PlantRect = IRect(0, 0, 100, 100);
		Type = SEED_WALLNUT;
	}

	void __thiscall Initialize(SeedType Type, SeedType Imitater, int Lane, int Column);
	void __thiscall PlantingEffects();
	void Die();
	void Fire(Zombie* Target = NULL, int Lane = -1, bool IsSecondary = false);
};