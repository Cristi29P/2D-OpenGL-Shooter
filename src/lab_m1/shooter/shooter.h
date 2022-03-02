#pragma once

#include "components/simple_scene.h"
#include "lab_m1/shooter/player.h"
#include "lab_m1/shooter/bullet.h"
#include "lab_m1/shooter/obstacle.h"
#include "lab_m1/shooter/enemy.h"

typedef std::tuple<bool, int, glm::vec2> Collision;

namespace m1
{
	class shooter : public gfxc::SimpleScene
	{
	public:
		struct ViewportSpace
		{
			ViewportSpace() : x(0), y(0), width(1), height(1) {}
			ViewportSpace(int x, int y, int width, int height)
				: x(x), y(y), width(width), height(height) {}
			int x;
			int y;
			int width;
			int height;
		};

		struct LogicSpace
		{
			LogicSpace() : x(0), y(0), width(1), height(1) {}
			LogicSpace(float x, float y, float width, float height)
				: x(x), y(y), width(width), height(height) {}
			float x;
			float y;
			float width;
			float height;
		};

	public:
		shooter();
		~shooter();

		void Init() override;

	private:
		void FrameStart() override;
		void Update(float deltaTimeSeconds) override;
		void FrameEnd() override;

		bool mapCollisionProjectile(glm::vec3 position);
		bool obstacleCollisionProjectile(glm::vec3 position, std::vector<obstacle*> obstacles);

		void addObstacles();
		void mapCollisionPlayer();

		int findDirection(glm::vec2 target);
		Collision checkCollisionPlayerObstacle(player* player, obstacle* obstacol);
		bool playerEnemyCollision(player* jucatorul, std::list<enemy>::iterator one_enemy);
		bool checkEnemyProjectile(std::list<enemy>::iterator one_enemy, std::list<bullet>::iterator one_bullet);
		void objectCollisionPlayer();

		void bulletCollisionEnemy();
		void spawnEnemies();

		void DrawScene(glm::mat3 visMatrix);
		void DrawMap(glm::mat3 visMatrix);
		void DrawUI(glm::mat3 visMatrix);
		void DrawBullets(glm::mat3 visMatrix);
		void DrawPlayer(glm::mat3 visMatrix);
		void DrawEnemies(glm::mat3 visMatrix);

		void OnInputUpdate(float deltaTime, int mods) override;
		void OnKeyPress(int key, int mods) override;
		void OnKeyRelease(int key, int mods) override;
		void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
		void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
		void OnWindowResize(int width, int height) override;

		// Sets the logic space and view space
		// logicSpace: { x, y, width, height }
		// viewSpace: { x, y, width, height }
		glm::mat3 VisualizationTransf2DUnif(const LogicSpace& logicSpace, const ViewportSpace& viewSpace);

		void SetViewportArea(const ViewportSpace& viewSpace, glm::vec3 colorColor = glm::vec3(0), bool clear = true);

	protected:
		// Preluat din vis2D
		ViewportSpace viewSpace;
		LogicSpace logicSpace;
		glm::mat3 modelMatrix = glm::mat3(1);
		glm::mat3 visMatrix = glm::mat3(1);

		// Player
		player* jucator = nullptr;
		float timeUntilNextShot = 0.f;

		// Vector de gloante
		std::list<bullet> bullets;

		// Vector de gloante
		std::vector<obstacle*> obstacles;

		// Vector de inamici
		std::list<enemy> enemies;

		std::list<enemy>::iterator one_enemy;
		std::list<bullet>::iterator one_bullet;
		std::list<bullet>::iterator fired_bullet;
		std::list<enemy>::iterator moving_enemy;

		float timeUntilNextWave = 5.f;
		float spawnrate = 10.f;
	};
}
