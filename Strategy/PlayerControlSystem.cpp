#include "PlayerControlSystem.h"
#include "ei/SDL/SDL.h"
#include "VelocityComponent.h"

const float mapScale = 8192.0;
const float Yscale = 1250.0;

float getpixel(const SDL_Surface *surface, int x, int y)
{
	int bpp = surface->format->BytesPerPixel;
	unsigned char *p = (unsigned char *)surface->pixels + y * surface->pitch + x * bpp;

	return (p[0] / 255.0f) * Yscale;
}

float PlayerControlSystem::sampleHeight(glm::vec3 pos)
{
	pos /= mapScale;
	pos = glm::fract(pos);
	pos *= glm::vec3(hmap.get().getWidth(), 0.0f, hmap.get().getHeight());

	glm::vec3 p00 = glm::vec3(glm::floor(pos.x), 0.0f, glm::floor(pos.z));
	glm::vec3 p10 = glm::vec3(glm::ceil(pos.x), 0.0f, glm::floor(pos.z));
	glm::vec3 p01 = glm::vec3(glm::floor(pos.x), 0.0f, glm::ceil(pos.z));
	glm::vec3 p11 = glm::vec3(glm::ceil(pos.x), 0.0f, glm::ceil(pos.z));

	if (glm::ceil(pos.x) > hmap.get().getWidth() - 1.0f)
	{
		p10.x = p11.x = 0.0f;
	}

	if (glm::ceil(pos.z) > hmap.get().getHeight() - 1.0f)
	{
		p01.z = p11.z = 0.0f;
	}

	pos = glm::fract(pos);

	p00.y = getpixel(hmap.get().getImage(), p00.x, p00.z);
	p10.y = getpixel(hmap.get().getImage(), p10.x, p10.z);
	p01.y = getpixel(hmap.get().getImage(), p01.x, p01.z);
	p11.y = getpixel(hmap.get().getImage(), p11.x, p11.z);

	float lerpX0 = pos.x * p10.y + (1.0f - pos.x) * p00.y;
	float lerpX1 = pos.x * p11.y + (1.0f - pos.x) * p01.y;

	return pos.z * lerpX1 + (1.0f - pos.z) * lerpX0;
};

void PlayerControlSystem::Tick(EntityID player, double deltaT)
{
	int mx, my;
	SDL_GetMouseState(&mx, &my);

	mx -= 512;
	my -= 384;

	Camera& activeCam = data.getEntityManager().entityGetComponent<CameraComponent>(player);
		
	activeCam.AddYawPitch(0.1f * mx, 0.1f * my);

	float speed = 5.0f * deltaT;

	if (data.getKeyboardState(SDLK_LSHIFT))
	{
		speed *= 3.0f;
	}

	SDL_WarpMouse(512, 384);

	if (data.getKeyboardState(SDLK_q))
	{
		activeCam.setFlying(true);
	}

	if (data.getKeyboardState(SDLK_e))
	{
		activeCam.setFlying(false);
	}

	if (data.getKeyboardState(SDLK_w))
	{
		activeCam.Move(speed);
	}

	if (data.getKeyboardState(SDLK_a))
	{
		activeCam.Strafe(-speed);
	}

	if (data.getKeyboardState(SDLK_s))
	{
		activeCam.Move(-speed);
	}

	if (data.getKeyboardState(SDLK_d))
	{
		activeCam.Strafe(speed);
	}

	VelocityComponent& vel = data.getEntityManager().entityGetComponent<VelocityComponent>(player);

	float h = sampleHeight(activeCam.getPos());

	if (data.getKeyboardState(SDLK_SPACE))
	{
		if (activeCam.getPos().y < (h + 1.8001f))
		{
			vel.setVelocity(glm::vec3(0.0f, 4.4294f, 0.0f));
		}
	}

	glm::vec3 newPos = activeCam.getPos() + vel.getVelocity() * (float)deltaT;

	if (newPos.y < h + 1.8f)
	{
		newPos.y = h + 1.8f;
		vel.setY(0.0f);
	}

	activeCam.setPos(newPos);

	vel += glm::vec3(0.0f, -9.81f  * deltaT, 0.0f);
}