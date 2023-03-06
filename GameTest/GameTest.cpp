//------------------------------------------------------------------------
// GameTest.cpp
//------------------------------------------------------------------------
#include "stdafx.h"
//------------------------------------------------------------------------
#include <windows.h> 
#include <math.h>  
//------------------------------------------------------------------------
#include "app\app.h"
#include "src\Mesh.h"
#include "src\Vec.h"
#include "src\Camera.h"
#include "src\Brick.h"
#include "src\Bomb.h"
//------------------------------------------------------------------------

Mesh* monkey;
Mesh* cubes;
Mesh* cube;
Mesh* arena;
std::shared_ptr<Mesh> brick;
std::shared_ptr<Mesh> sphere;
Camera* camera;

std::vector<Brick> bricks;
std::vector<Bomb> bombs;

Vec3F player_pos {0.0f};

Mat4F player_model;

float counter = -5.0f;

//------------------------------------------------------------------------

//------------------------------------------------------------------------
// Called before first update. Do any initial setup here.
//------------------------------------------------------------------------
void Init()
{
	monkey = new Mesh(".\\res\\mesh\\monkey.obj");
	cube = new Mesh(".\\res\\mesh\\cube.obj");
	arena = new Mesh(".\\res\\mesh\\arena2.obj");
	sphere = std::make_shared<Mesh>(".\\res\\mesh\\sphere.obj");
	brick = std::make_shared<Mesh>(".\\res\\mesh\\other_bricks.obj");
	camera = new Camera({0, 20, -2}, {0, 0, 0});
	cubes = new Mesh();

	// initialize bricks
	for (int i = 0; i < 7; i++)
		bricks.emplace_back(brick, Vec3F{6.0f - 2.0f * i, 0.0f, 7.0f});
	for (int i = 0; i < 7; i++) {
		for (int j = 0; j < 8; j++) {
			bricks.emplace_back(brick, Vec3F{7.0f - 2.0f * j, 0.0f, 6.0f - 2.0f * i});
		}
		for (int j = 0; j < 7; j++) 
			bricks.emplace_back(brick, Vec3F{6.0f - 2.0f * j, 0.0f, 5.0f - 2.0f * i});
	}
	bricks[48].active = false;
	bricks[55].active = false;
	bricks[56].active = false;
	bricks[63].active = false;

	player_model = AffineRotation(3.1415f);
}

//------------------------------------------------------------------------
// Update your simulation here. deltaTime is the elapsed time since the last update in ms.
// This will be called at no greater frequency than the value of APP_MAX_FRAME_RATE
//------------------------------------------------------------------------
void Update(float deltaTime)
{
	Vec3F motion {0.0f};
	if (App::GetController().GetLeftThumbStickX() > 0.5f) {
		motion[0] -= 1;
	} else if (App::GetController().GetLeftThumbStickX() < -0.5f) {
		motion[0] += 1;
	}
	if (App::GetController().GetLeftThumbStickY() > 0.5f) {
		motion[2] -= 1;
	} else if (App::GetController().GetLeftThumbStickY() < -0.5f) {
		motion[2] += 1;
	}

	if (App::GetController().CheckButton(XINPUT_GAMEPAD_A, true)
	|| counter < 0) {
		if (bombs.size() == 0) {
			bombs.emplace_back(sphere, player_pos);

		}
		counter = 10.0f;
	}
	counter -= deltaTime;

	if (motion[0] != 0.0f || motion[2] != 0.0f) {
		float phi = atanf(- motion[0] / motion[2]);
		motion.normalize();
		player_pos += motion * deltaTime / 100;
		
		player_model = AffineTranslation(player_pos) * AffineRotation(phi);

	}

	for (Bomb& b : bombs) {
		b.update(deltaTime);
	}
}

//------------------------------------------------------------------------
// Add your display calls here (DrawLine,Print, DrawSprite.) 
// See App.h 
//------------------------------------------------------------------------
void Render()
{	
	Mat4F ident {
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f,
	};
	camera->lookat(player_pos + Vec3F{0.0f, 6.0f, -1.0f}, player_pos);
	camera->render(*arena, {1.0f, 1.0f, 1.0f}, {0.6f, 0.6f, 0.6f}, ident);

	for (Brick& b : bricks) {
		b.render(*camera);
	}

	for (Bomb& b : bombs) {
		b.render(*camera);
	}

	camera->render(*monkey, {0.5f, 0.8f, 0.6f}, {0.3f, 0.7f, 0.4f}, player_model);
}
//------------------------------------------------------------------------
// Add your shutdown code here. Called when the APP_QUIT_KEY is pressed.
// Just before the app exits.
//------------------------------------------------------------------------
void Shutdown()
{	
	delete camera;
	delete monkey;
	delete cube;
	delete cubes;
	delete arena;
}