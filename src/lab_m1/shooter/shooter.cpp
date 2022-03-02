#include "lab_m1/shooter/shooter.h"

#include "lab_m1/shooter/transform2D.h"
#include "lab_m1/shooter/object2D.h"

#include <iostream>

using namespace std;
using namespace m1;

shooter::shooter()
{
}

shooter::~shooter()
{
}

void shooter::Init()
{
	auto camera = GetSceneCamera();
	camera->SetPosition(glm::vec3(0, 0, 50));
	camera->SetRotation(glm::vec3(0, 0, 0));
	camera->Update();
	GetCameraInput()->SetActive(false);

	// Coordonate fereastra logica - initiale
	logicSpace.x = 0;       // logic x
	logicSpace.y = 0;       // logic y
	logicSpace.width = 8;   // logic width
	logicSpace.height = 4.5;  // logic height

	// Coltul ferestrei logice - fata de care se face crearea meshelor
	glm::vec3 corner = glm::vec3(0.f, 0.f, 0.f);

	// Initializare player si parametri generici
	jucator = new player(glm::vec3(0.f, 0.f, 1.f));
	jucator->setHealth(100.f);
	jucator->setAngle(0.f);
	jucator->setFirerate(0.25f);
	jucator->setProjectileRange(10.f);
	jucator->setBulletSpeed(4.f);

	// Incarcare meshe
	Mesh* map = object2D::CreateSquare("mapFrame", corner, 1.f, glm::vec3(0.298, 0.737, 0.839), false);
	AddMeshToList(map);

	Mesh* mapBG = object2D::CreateSquare("mapBG", corner, 1.f, glm::vec3(0.776, 0.831, 0.843), true);
	AddMeshToList(mapBG);

	Mesh* obstacle = object2D::CreateSquare("obstacle", corner, 1.f, glm::vec3(0.5f, 1, 0), true);
	AddMeshToList(obstacle);

	Mesh* wireHealth = object2D::CreateSquare("wireHealth", corner, 1.f, glm::vec3(1, 0, 0), false);
	AddMeshToList(wireHealth);

	Mesh* currentHealth = object2D::CreateSquare("currentHealth", corner, 1.f, glm::vec3(1, 0, 0), true);
	AddMeshToList(currentHealth);

	Mesh* playerBody = object2D::CreateCircle("body", corner, 0.3f, glm::vec3(1, 1, 0), true);
	AddMeshToList(playerBody);

	Mesh* playerEye = object2D::CreateCircle("eye", corner, 0.1f, glm::vec3(0.713f, 0.607f, 0.086f), true);
	AddMeshToList(playerEye);

	Mesh* projectile = object2D::CreateSquare("projectile", corner, 0.2f, glm::vec3(0, 0.5f, 0.5f), true);
	AddMeshToList(projectile);

	Mesh* enemyBody = object2D::CreateCircle("enemyBody", corner, 0.3f, glm::vec3(1, 0, 0), true);
	AddMeshToList(enemyBody);

	Mesh* enemyEye = object2D::CreateSquare("enemyEye", corner, 0.2f, glm::vec3(0.772f, 0.403f, 0.419f), true);
	AddMeshToList(enemyEye);

	glLineWidth(8);

	// Se adauga o singura data in init obstacolele
	addObstacles();
}

void shooter::addObstacles() {
	obstacles.push_back(new obstacle(1.f, 1.f, 5.5f, glm::vec3(8.f, 6.75f, 1.f)));
	obstacles.push_back(new obstacle(1.f, 1.f, 5.5f, glm::vec3(16.f, 6.75f, 1.f)));
	obstacles.push_back(new obstacle(1.f, 2.f, 2.f, glm::vec3(12.f, 6.75f, 1.f)));
	obstacles.push_back(new obstacle(1.f, 1.f, 4.5f, glm::vec3(4.f, 9.f, 1.f)));
	obstacles.push_back(new obstacle(1.f, 1.f, 4.5f, glm::vec3(20.f, 4.5f, 1.f)));
	obstacles.push_back(new obstacle(1.f, 4.5f, 1.f, glm::vec3(12.f, 2.25f, 1.f)));
	obstacles.push_back(new obstacle(1.f, 4.5f, 1.f, glm::vec3(16.f, 11.25f, 1.f)));
}

// Fereastra => poarta din Lab3_Vis2D
glm::mat3 shooter::VisualizationTransf2DUnif(const LogicSpace& logicSpace, const ViewportSpace& viewSpace)
{
	float sx, sy, tx, ty, smin;
	sx = viewSpace.width / logicSpace.width;
	sy = viewSpace.height / logicSpace.height;
	if (sx < sy) {
		smin = sx;
	}
	else {
		smin = sy;
	}

	tx = viewSpace.x - smin * logicSpace.x + (viewSpace.width - smin * logicSpace.width) / 2;
	ty = viewSpace.y - smin * logicSpace.y + (viewSpace.height - smin * logicSpace.height) / 2;

	return glm::transpose(glm::mat3(
		smin, 0.0f, tx,
		0.0f, smin, ty,
		0.0f, 0.0f, 1.0f));
}

void shooter::SetViewportArea(const ViewportSpace& viewSpace, glm::vec3 colorColor, bool clear)
{
	glViewport(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);

	glEnable(GL_SCISSOR_TEST);
	glScissor(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);

	// Clears the color buffer (using the previously set color) and depth buffer
	glClearColor(colorColor.r, colorColor.g, colorColor.b, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_SCISSOR_TEST);

	GetSceneCamera()->SetOrthographic((float)viewSpace.x, (float)(viewSpace.x + viewSpace.width), (float)viewSpace.y, (float)(viewSpace.y + viewSpace.height), 0.1f, 400);
	GetSceneCamera()->Update();
}

void shooter::FrameStart()
{
	// Clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

int shooter::findDirection(glm::vec2 target)
{
	glm::vec2 general_directions[] = {
		glm::vec2(0.0f, 1.0f),
		glm::vec2(1.0f, 0.0f),
		glm::vec2(0.0f, -1.0f),
		glm::vec2(-1.0f, 0.0f)
	};
	float max = 0.0f;
	int direction = -1;
	for (int i = 0; i < 4; i++)
	{
		float dot_product = glm::dot(glm::normalize(target), general_directions[i]);
		if (dot_product > max) {
			max = dot_product;
			direction = i;
		}
	}
	return direction;
}

Collision shooter::checkCollisionPlayerObstacle(player* jucatorul, obstacle* obstacol) {
	glm::vec2 player_pos = glm::vec2(jucatorul->getPosition().x, jucator->getPosition().y);

	glm::vec2 center(player_pos);

	glm::vec2 aabb_half_extents((obstacol->m_sX * obstacol->m_sideLength) / 2.0f, (obstacol->m_sY * obstacol->m_sideLength) / 2.0f);
	glm::vec2 aabb_center(
		obstacol->m_position.x,
		obstacol->m_position.y
	);

	glm::vec2 difference = center - aabb_center;
	glm::vec2 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);

	glm::vec2 closest = aabb_center + clamped;

	difference = closest - center;

	if (glm::length(difference) <= 0.3f)
		return std::make_tuple(true, findDirection(difference), difference);
	else
		return std::make_tuple(false, 0, glm::vec2(0.0f, 0.0f));
}

bool shooter::checkEnemyProjectile(std::list<enemy>::iterator one_enemy, std::list<bullet>::iterator one_bullet) {
	glm::vec2 enemy_pos = glm::vec2(one_enemy->getPosition().x, one_enemy->getPosition().y);

	glm::vec2 center(enemy_pos);

	glm::vec2 aabb_half_extents(0.1f, 0.1f);
	glm::vec2 aabb_center(
		one_bullet->getPosition().x,
		one_bullet->getPosition().y
	);

	glm::vec2 difference = center - aabb_center;
	glm::vec2 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);

	glm::vec2 closest = aabb_center + clamped;

	difference = closest - center;

	return glm::length(difference) < 0.3f;
}

bool shooter::playerEnemyCollision(player* jucatorul, std::list<enemy>::iterator one_enemy) {
	return 0.6f * 0.6f >= (jucatorul->getPosition().x - one_enemy->getPosition().x) *
		(jucatorul->getPosition().x - one_enemy->getPosition().x) +
		(jucatorul->getPosition().y - one_enemy->getPosition().y) *
		(jucatorul->getPosition().y - one_enemy->getPosition().y);
}

bool shooter::obstacleCollisionProjectile(glm::vec3 position, vector<obstacle*> obstacles) {
	bool collisionX = false, collisionY = false;
	for (auto& obstacol : obstacles) {
		collisionX = position.x + 0.1f >= obstacol->m_position.x - (obstacol->m_sideLength * obstacol->m_sX / 2.0f) &&
			obstacol->m_position.x + (obstacol->m_sideLength * obstacol->m_sX / 2.0f) >= position.x - 0.1f;
		collisionY = position.y + 0.1f >= obstacol->m_position.y - (obstacol->m_sideLength * obstacol->m_sY / 2.0f) &&
			obstacol->m_position.y + (obstacol->m_sideLength * obstacol->m_sY / 2.0f) >= position.y - 0.1f;

		if (collisionX && collisionY) {
			return true;
		}
	}

	return false;
}

bool shooter::mapCollisionProjectile(glm::vec3 position) {
	if (position.x - 0.1f < 0) {
		return true;
	}

	if (position.y - 0.1f < 0) {
		return true;
	}

	if (position.x + 0.1f > 24.f) {
		return true;
	}

	if (position.y + 0.1f > 13.5f) {
		return true;
	}

	return false;
}

void shooter::mapCollisionPlayer() {
	if (jucator->getPosition().x - 0.3f < 0) {
		logicSpace.x = -3.7f;
	}

	if (jucator->getPosition().y - 0.3f < 0) {
		logicSpace.y = -1.95f;
	}

	if (jucator->getPosition().x + 0.3f > 24.f) {
		logicSpace.x = 19.7f;
	}

	if (jucator->getPosition().y + 0.3f > 13.5f) {
		logicSpace.y = 10.95f;
	}
}

void shooter::objectCollisionPlayer() {
	for (auto& obstacol : obstacles) {
		Collision collision = checkCollisionPlayerObstacle(jucator, obstacol);
		if (get<0>(collision)) {
			int dir = get<1>(collision);
			glm::vec2 diff_vector = get<2>(collision);
			if (dir == 3 || dir == 1) {
				float penetration = 0.3f - abs(diff_vector.x);
				if (dir == 3) {
					logicSpace.x += penetration;
				}
				else {
					logicSpace.x -= penetration;
				}
			}
			else {
				float penetration = 0.3f - abs(diff_vector.y);
				if (dir == 0) {
					logicSpace.y -= penetration;
				}
				else {
					logicSpace.y += penetration;
				}
			}
		}
	}
}

void shooter::bulletCollisionEnemy() {
	bool flag = false;
	fired_bullet = bullets.begin();
	moving_enemy = enemies.begin();
	if (bullets.size() != 0 && enemies.size() != 0) {
		while (fired_bullet != bullets.end()) {
			moving_enemy = enemies.begin();
			flag = false;
			while (moving_enemy != enemies.end()) {
				if (checkEnemyProjectile(moving_enemy, fired_bullet)) {
					bullets.erase(fired_bullet++);
					enemies.erase(moving_enemy++);
					jucator->increasePoints(1);
					cout << "Puncte: " << jucator->getPoints() << endl;
					flag = true;
					break;
				}
				else {
					++moving_enemy;
				}
			}
			if (flag) {
				continue;
			}
			++fired_bullet;
		}
	}
}

void shooter::spawnEnemies() {
	if (timeUntilNextWave < Engine::GetElapsedTime()) {
		timeUntilNextWave = (float)Engine::GetElapsedTime() + spawnrate;

		for (int i = 0; i < 5; i++) {
			float random_speed;
			glm::vec3 random_position;
			random_speed = 1.f + (float)(std::rand() % 2);
			random_position.z = 1.f;
			random_position.x = 0.5f + (float)(std::rand() % 23);
			random_position.y = 0.5f + (float)(std::rand() % 13);

			enemy new_enemy(random_position, random_speed);
			enemies.push_back(new_enemy);
		}
	}
}

void shooter::Update(float deltaTimeSeconds)
{
	glm::ivec2 resolution = window->GetResolution();
	glm::vec3 cursorPos;

	cursorPos.x = (float)window->GetCursorPosition().x;
	cursorPos.y = (float)(resolution.y - window->GetCursorPosition().y);
	cursorPos.z = 1.f;
	cursorPos.x -= resolution.x / 2;
	cursorPos.y -= resolution.y / 2;

	cursorPos = glm::normalize(cursorPos);
	jucator->setAngle(atan2(cursorPos.y, cursorPos.x));

	viewSpace = ViewportSpace(0, 0, resolution.x, resolution.y);
	SetViewportArea(viewSpace, glm::vec3(0), true);

	visMatrix = glm::mat3(1);
	visMatrix *= VisualizationTransf2DUnif(logicSpace, viewSpace);

	// Spawn enemies
	spawnEnemies();
	/// Entities updates ///
	jucator->updatePlayer(logicSpace.x, logicSpace.y);

	// Enemy updates //
	for (auto& enemy : enemies) {
		glm::vec2 aux;
		aux.x = jucator->getPosition().x - enemy.getPosition().x;
		aux.y = jucator->getPosition().y - enemy.getPosition().y;

		aux = glm::normalize(aux);

		enemy.update(deltaTimeSeconds, atan2(aux.y, aux.x), aux);
	}

	one_bullet = bullets.begin();
	if (bullets.size() != 0) {
		while (one_bullet != bullets.end()) {
			one_bullet->update(deltaTimeSeconds);
			if (one_bullet->getDistance() >= jucator->getProjectileRange() ||
				mapCollisionProjectile(one_bullet->getPosition()) ||
				obstacleCollisionProjectile(one_bullet->getPosition(), obstacles)) { // harta - proiectil
				bullets.erase(one_bullet++);
				continue;
			}
			++one_bullet;
		}
	}

	one_enemy = enemies.begin();
	if (enemies.size() != 0) {
		while (one_enemy != enemies.end()) {
			if (playerEnemyCollision(jucator, one_enemy)) { // jucator - inamic
				jucator->setHealth(jucator->getHealth() - 10.f);
				enemies.erase(one_enemy++);
				if (jucator->getHealth() == 0) {
					cout << "Jocul s-a terminat cu punctajul final: " << jucator->getPoints() << endl;
					exit(1);
				}
				continue;
			}
			++one_enemy;
		}
	}

	// Testare coliziune bullet - enemy
	bulletCollisionEnemy();

	// Testare coliziune harta - player
	mapCollisionPlayer();

	// Testare coliziune obstacole - player
	objectCollisionPlayer();

	DrawScene(visMatrix);
}

void shooter::FrameEnd()
{
}

void shooter::DrawMap(glm::mat3 visMatrix) {
	// Randam obstacolele
	for (auto& obstacol : obstacles) {
		RenderMesh2D(meshes["obstacle"], shaders["VertexColor"], visMatrix * obstacol->m_modelMatrix);
	}

	/// Randam border-ul hartii ///
	modelMatrix =
		visMatrix *
		transform2D::Translate(0, 0) *
		transform2D::Scale(24, 13.5);
	RenderMesh2D(meshes["mapFrame"], shaders["VertexColor"], modelMatrix);

	/// Randam background-ul hartii ///
	modelMatrix =
		visMatrix *
		transform2D::Translate(0, 0) *
		transform2D::Scale(24, 13.5);
	RenderMesh2D(meshes["mapBG"], shaders["VertexColor"], modelMatrix);
}

void shooter::DrawUI(glm::mat3 visMatrix) {
	modelMatrix = visMatrix * transform2D::Translate(logicSpace.x + 5.75f, logicSpace.y + 4.f) * transform2D::Scale(2, 0.25);
	RenderMesh2D(meshes["wireHealth"], shaders["VertexColor"], modelMatrix);

	float scaleHp = jucator->getHealth() * 2.f / 100;

	float trDiff = 2 - scaleHp;

	modelMatrix = visMatrix * transform2D::Translate(logicSpace.x + 5.75f + trDiff, logicSpace.y + 4.f)
		* transform2D::Scale(scaleHp, 0.25);
	RenderMesh2D(meshes["currentHealth"], shaders["VertexColor"], modelMatrix);
}

void shooter::DrawBullets(glm::mat3 visMatrix) {
	for (auto& bullet : bullets) {
		RenderMesh2D(meshes["projectile"], shaders["VertexColor"], visMatrix * bullet.getModelMatrix());
	}
}

void shooter::DrawPlayer(glm::mat3 visMatrix) {
	modelMatrix = visMatrix *
		jucator->getModelMatrix();
	RenderMesh2D(meshes["body"], shaders["VertexColor"], modelMatrix);

	modelMatrix = visMatrix *
		jucator->getModelMatrix() *
		transform2D::Translate(0.28f, 0.28f);

	RenderMesh2D(meshes["eye"], shaders["VertexColor"], modelMatrix);

	modelMatrix = visMatrix *
		jucator->getModelMatrix() *
		transform2D::Translate(0.28f, -0.28f);

	RenderMesh2D(meshes["eye"], shaders["VertexColor"], modelMatrix);
}

void shooter::DrawEnemies(glm::mat3 visMatrix) {
	for (auto& enemy : enemies) {
		modelMatrix = visMatrix *
			enemy.getModelMatrix();

		RenderMesh2D(meshes["enemyBody"], shaders["VertexColor"], modelMatrix);

		modelMatrix = visMatrix *
			enemy.getModelMatrix() *
			transform2D::Translate(0.15f, 0.1f);

		RenderMesh2D(meshes["enemyEye"], shaders["VertexColor"], modelMatrix);

		modelMatrix = visMatrix *
			enemy.getModelMatrix() *
			transform2D::Translate(0.15f, -0.3f);

		RenderMesh2D(meshes["enemyEye"], shaders["VertexColor"], modelMatrix);
	}
}

void shooter::DrawScene(glm::mat3 visMatrix)
{
	DrawUI(visMatrix);

	DrawPlayer(visMatrix);

	DrawEnemies(visMatrix);

	DrawBullets(visMatrix);

	DrawMap(visMatrix);
}

void shooter::OnInputUpdate(float deltaTime, int mods)
{
	if (window->KeyHold(GLFW_KEY_W))
		logicSpace.y += deltaTime * 2.f;

	if (window->KeyHold(GLFW_KEY_S))
		logicSpace.y -= deltaTime * 2.f;

	if (window->KeyHold(GLFW_KEY_A))
		logicSpace.x -= deltaTime * 2.f;

	if (window->KeyHold(GLFW_KEY_D))
		logicSpace.x += deltaTime * 2.f;
}

void shooter::OnKeyPress(int key, int mods)
{
}

void shooter::OnKeyRelease(int key, int mods)
{
}

void shooter::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
}

void shooter::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_2 && timeUntilNextShot < Engine::GetElapsedTime()) {
		timeUntilNextShot = (float)Engine::GetElapsedTime() + jucator->getFirerate();
		bullet bullet_new;
		bullet_new.setInitialPosition(jucator->getPosition());
		bullet_new.setAngle(jucator->getAngle());
		bullet_new.setBulletSpeed(jucator->getBulletSpeed());

		glm::ivec2 resolution = window->GetResolution();
		glm::vec3 direction;
		direction.x = (float)window->GetCursorPosition().x - (resolution.x / 2);
		direction.y = (float)(resolution.y - window->GetCursorPosition().y) - (resolution.y / 2);
		direction.z = 1.f;
		direction = glm::normalize(direction);
		bullet_new.setDirection(direction);

		bullets.push_back(bullet_new);
	}
}

void shooter::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
}

void shooter::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void shooter::OnWindowResize(int width, int height)
{
}