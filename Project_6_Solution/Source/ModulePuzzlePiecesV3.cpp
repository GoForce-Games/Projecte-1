#include "ModulePuzzlePiecesV3.h"

#include "Application.h"
#include "ModuleCollisions.h"
#include "ModuleTextures.h"

ModulePuzzlePiecesV3::ModulePuzzlePiecesV3(bool startEnabled) : Module(startEnabled)
{
	dropDelay = MAX_DROP_DELAY;
	moveDelay = MAX_MOVE_DELAY;
	moveSpeed = MOVE_SPEED;
	gravity = GRAVITY;
	wallCount = 0;
}

ModulePuzzlePiecesV3::~ModulePuzzlePiecesV3()
{
}

bool ModulePuzzlePiecesV3::Init()
{
	for (uint i = 0; i < MAX_PIECES; i++)
	{
		pieces[i] = nullptr;
	}

	for (uint i = 0; i < MAX_WALLS; i++)
	{
		walls[i] = nullptr;
	}
	return true;
}

bool ModulePuzzlePiecesV3::Start()
{
	EnableDependencies();
	LoadTextures();
	InitAnims();
	InitTemplates();
	playArea.Init();
	InitWalls();
	InitPlayers();
	InitMisc();

	return true;
}

Update_Status ModulePuzzlePiecesV3::Update()
{
	return Update_Status::UPDATE_CONTINUE;
}

Update_Status ModulePuzzlePiecesV3::PostUpdate()
{
	return Update_Status::UPDATE_CONTINUE;
}

void ModulePuzzlePiecesV3::OnCollision(Collider* c1, Collider* c2)
{
}

bool ModulePuzzlePiecesV3::CleanUp()
{
	return true
		;
}

std::stack<PuzzlePiece*>& ModulePuzzlePiecesV3::GeneratePuzzlePieces(std::stack<PuzzlePiece*>& stack, uint amount)
{
	return stack;
}

PuzzlePiece* ModulePuzzlePiecesV3::AddPuzzlePiece(const PuzzlePiece& newPiece, Collider::Type type)
{
	return nullptr;
}

void ModulePuzzlePiecesV3::RemovePuzzlePiece(PuzzlePiece* piece)
{
}

bool ModulePuzzlePiecesV3::WillCollide(PlayerCollisionCheck direction)
{
	// TODO usar el modulo de colisiones existente
	return false;
}

void ModulePuzzlePiecesV3::PlacePieces()
{
}

bool ModulePuzzlePiecesV3::PieceCanDrop(PuzzlePiece* piece)
{
	return false;
}

void ModulePuzzlePiecesV3::EnableDependencies()
{
	App->collisions->Enable();
}

void ModulePuzzlePiecesV3::LoadTextures()
{
	// TODO textura para probar, hay que recortar el spritesheet
	textureBomberman = App->textures->Load("Assets/testerman.png");
}

void ModulePuzzlePiecesV3::InitAnims()
{
	// Animacion temporal, sacado de la demo de R-type que hicimos en clase
	animDefault.PushBack({ 0, 0, 16, 16 });
	animDefault.PushBack({ 16, 0, 16, 16 });
	animDefault.PushBack({ 32, 0, 16, 16 });
	animDefault.PushBack({ 48, 0, 16, 16 });
	animDefault.speed = 0.08f;

	animNone.PushBack({ 0,0,0,0 });
	animNone.loop = false;
	animNone.speed = 1.0f;
}

void ModulePuzzlePiecesV3::InitTemplates()
{
	templateMan.collider = App->collisions->AddCollider({ -64,-64,16,16 }, Collider::Type::NONE);
	templateMan.position.create(-64, -64);
	templateMan.texture = textureBomberman;
	templateMan.SetAnimation(&animDefault);
	templateMan.moving = false;
	templateMan.type = PieceType::WHITE;


	emptyPiece = AddPuzzlePiece(templateMan, Collider::Type::NONE);
	emptyPiece->currentAnimation = &animNone;
	emptyPiece->isEmpty = true;
	emptyPiece->type = PieceType::NONE;
}

void ModulePuzzlePiecesV3::InitWalls()
{
	PuzzlePiece* templateWall = new PuzzlePiece(*emptyPiece);
	templateWall->moving = false;
	templateWall->type = PieceType::WALL;

	// Columna izquierda
	iPoint offset = playArea.position;
	//offset.y += PIECE_SIZE;
	for (size_t i = 0; i < PLAY_AREA_Y; i++)
	{
		templateWall->position = offset;
		PuzzlePiece* piece = playArea.table[0][i] = AddPuzzlePiece(*templateWall, Collider::Type::WALL);
		offset.y += PIECE_SIZE;
	}

	// Columna derecha
	offset = playArea.position;
	//offset.y += PIECE_SIZE;
	offset.x += PIECE_SIZE * (PLAY_AREA_X - 1);
	for (size_t i = 0; i < PLAY_AREA_Y; i++)
	{
		templateWall->position = offset;
		PuzzlePiece* piece = playArea.table[PLAY_AREA_X - 1][i] = AddPuzzlePiece(*templateWall, Collider::Type::WALL);
		offset.y += PIECE_SIZE;

	}


	// Fondo
	offset = playArea.position;
	offset.y += PIECE_SIZE * (PLAY_AREA_Y);
	for (size_t i = 0; i < PLAY_AREA_X; i++)
	{
		templateWall->position = offset;
		PuzzlePiece* piece = playArea.table[i][PLAY_AREA_Y - 1] = AddPuzzlePiece(*templateWall, Collider::Type::WALL);
		offset.x += PIECE_SIZE;
	}

	//Techo (por ciertas razones relacionadas con la deteccion de grupos)
	// Fondo
	offset = playArea.position;
	offset.y += 0;
	for (size_t i = 0; i < PLAY_AREA_X; i++)
	{
		templateWall->position = offset;
		PuzzlePiece* piece = playArea.table[i][0] = AddPuzzlePiece(*templateWall, Collider::Type::WALL);
		offset.x += PIECE_SIZE;
	}

	delete templateWall;
}

void ModulePuzzlePiecesV3::InitPlayers()
{
	player.position.create(64, 16);

	PuzzlePiece* newPieces[4];
	newPieces[0] = AddPuzzlePiece(templateMan);
	newPieces[1] = AddPuzzlePiece(templateMan);
	newPieces[2] = AddPuzzlePiece(templateMan);
	newPieces[3] = AddPuzzlePiece(*emptyPiece);
	player.setPieces(newPieces);
}

void ModulePuzzlePiecesV3::InitMisc()
{
	collisionTester = App->collisions->AddCollider({ -64,-64,16,16 }, Collider::Type::PLAYER);

	playArea.collisionTester = collisionTester;
}

void ModulePuzzlePiecesV3::ProcessInput()
{
}

void ModulePuzzlePiecesV3::ApplyPhysics()
{
}

void ModulePuzzlePiecesV3::ApplyLogic()
{
}

iPoint ModulePuzzlePiecesV3::ScreenToLocal(PlayArea& localArea, iPoint sCoordinates)
{
	return iPoint();
}

iPoint ModulePuzzlePiecesV3::AreaToScreen(PlayArea& localArea, iPoint lCoordinates)
{
	return iPoint();
}
