#include "cEntityBuilder.h"
#include "cComponentFactory.h"

#include "cEulerMovementSystem.h"
#include "cMeshRenderSystem.h"
#include "cDebugRenderSystem.h"
#include "cColliderSystem.h"
#include "cParticleSystem.h"

#include "cRenderMeshComponent.h"
#include "cTransformComponent.h"
#include "cEulerMovementComponent.h"
#include "cDebugRenderComponent.h"
#include "cParticleComponent.h"
#include "cCollider.h"

#include "physicsShapes.h"
#include "cShaderManager.h"
#include "cLightManager.h"
#include "cSceneManager.h"
#include "c3dSoundComponent.h"
#include "cSoundManager.h"

void ballDebugRendering(cEntity* entity);

cEntityBuilder::cEntityBuilder(){
}


cEntityBuilder::~cEntityBuilder(){
	for (cEntity* entity : this->allocatedEntites) {
		delete entity;
	}
}

cEntityBuilder* cEntityBuilder::getInstance() {
	static cEntityBuilder instance;

	return &instance;
}

cEntity* cEntityBuilder::createEntity(int entityType) {
	cEntity* result = nullptr;

	switch (entityType) {
	
		//allowed entity types
		case 0: // sphere
		case 1: // Inverted sphere (Sky box object)
		case 2: // Terrain (Currently Peach's Castle) //
		case 3: // Mario
		case 4: // Luigi
		case 5: // Peach
		case 6: // Toad
		case 7: // Bowser
		case 8: // Boo
		case 9: // Goomba
		case 10: // Yoshi
		case 11: // Torch flame
		case 12: // Green Pipe
		case 13: // Lantern
		case 14: // Pirahna Plant
		case 15: // Star
		case 16: // Torch
		case 17: // Cube 
		case 18: // Star Imposter
		case 19: // Bunny (used for blending)
			result = new cEntity(entityType);
			break;
	}

	if (result != nullptr) {
		// add the components required
		addRequiredComponents(result, entityType);

		this->allocatedEntites.push_back(result);
	}

	return result;
}

cEntity* cEntityBuilder::createEntity(int entityType, glm::vec3 position) {
	cEntity* result = nullptr;

	result = this->createEntity(entityType);

	this->setEntityPosition(result, position);

	return result;
}

void cEntityBuilder::setEntityPosition(cEntity* entity, glm::vec3 position) {

	cTransformComponent* transformComponent = (cTransformComponent*) entity->components[1];
	transformComponent->position = position;
}

cEntity* cEntityBuilder::createEntityRandomPosColor(int entityType) {
	cEntity* result = this->createEntity(entityType);

	// make sure we have the compoents required
	if ((result->componentBitField & 5) != 5) {
		return result;
	}

	cTransformComponent* pTransformComponent =static_cast<cTransformComponent*>(result->components[1]);
	pTransformComponent->position = glm::vec3(this->getRandInRange<float>(-75.0f, 75.0f), this->getRandInRange<float>(-75.0f, 75.0f), this->getRandInRange<float>(-75.0f, 75.0f));

	cRenderMeshComponent* pRenderMeshComponent = static_cast<cRenderMeshComponent*>(result->components[3]);
	pRenderMeshComponent->setDiffuseColour(glm::vec3(this->getRandBetween0and1<float>(), this->getRandBetween0and1<float>(), this->getRandBetween0and1<float>()));
	return result;
}

void cEntityBuilder::addRequiredComponents(cEntity* entity, int entityType) {

	cComponentFactory* pComponentFactory = cComponentFactory::getInstance();

	switch (entityType)
	{
		case 0:
		{
			entity->componentBitField |= 1;
			entity->components[1] = pComponentFactory->createComponent(1);
			
			cTransformComponent* pTransformComponent = static_cast<cTransformComponent*>(entity->components[1]);
			pTransformComponent->position = glm::vec3(0.0f, 2.0f, 0.0f);
			

			entity->componentBitField |= 4;
			entity->components[3] = pComponentFactory->createComponent(3);
			{
				cRenderMeshComponent* meshComp = static_cast<cRenderMeshComponent*>(entity->components[3]);
				meshComp->meshName = "Sphere_320_faces_xyz_n_GARBAGE_uv.ply";
				meshComp->bIsVisible = true;
				meshComp->bIsWireFrame = true;
				meshComp->setDiffuseColour(glm::vec3(1.0f, 0.0f, 0.0f));
				meshComp->bUseVertexColour = false;
				meshComp->bDontLight = true;
				meshComp->shaderId = cShaderManager::getInstance()->getIDFromFriendlyName("BasicUberShader");
			}

			// register the entity with the component systems
			cMeshRenderSystem::getInstance()->registerEntity(entity);
		}
		break;

		case 1:
		{
			entity->componentBitField |= 1;
			entity->components[1] = pComponentFactory->createComponent(1);

			cTransformComponent* pTransformComponent = static_cast<cTransformComponent*>(entity->components[1]);
			pTransformComponent->position = glm::vec3(0.0f, 0.0f, 0.0f);
			pTransformComponent->scale = glm::vec3(5000.0f);


			entity->componentBitField |= 4;
			entity->components[3] = pComponentFactory->createComponent(3);
			{
				cRenderMeshComponent* meshComp = static_cast<cRenderMeshComponent*>(entity->components[3]);
				meshComp->meshName = "Sphere_320_faces_xyz_n_GARBAGE_uv_INVERTED_NORMALS.ply";
				meshComp->friendlyName = "SkyBoxObject";
				meshComp->bIsVisible = true;
				meshComp->setDiffuseColour(glm::vec3(1.0f, 105.0f / 255.0f, 180.0f / 255.0f));
				meshComp->bUseVertexColour = false;
				meshComp->bDontLight = false;
				meshComp->shaderId = cShaderManager::getInstance()->getIDFromFriendlyName("BasicUberShader");
			}

			// register the entity with the component systems
			cMeshRenderSystem::getInstance()->registerEntity(entity);
		}
		break;

		case 2:
		{
			entity->componentBitField |= 1;
			entity->components[1] = pComponentFactory->createComponent(1);
			{
				cTransformComponent* transformComponent = static_cast<cTransformComponent*>(entity->components[1]);
				transformComponent->position.y = 0.0f;
			}

			entity->componentBitField |= 2;
			entity->components[2] = pComponentFactory->createComponent(2);

			entity->componentBitField |= 4;
			entity->components[3] = pComponentFactory->createComponent(3);
			{
				cRenderMeshComponent* meshComp = (cRenderMeshComponent*)entity->components[3];
				meshComp->meshName = "Peach_Castle_xyz_n_uv.ply";
				meshComp->friendlyName = "Castle";
				meshComp->bIsVisible = true;
				meshComp->setDiffuseColour(glm::vec3(0.0f, 0.0f, 0.0f));
				meshComp->bUseVertexColour = false;
				meshComp->bDontLight = false;
				meshComp->shaderId = cShaderManager::getInstance()->getIDFromFriendlyName("BasicUberShader");
				meshComp->setSpecularColour(glm::vec3(1.0f, 1.0f, 1.0f));

				/*{
					sTextureInfo texture;
					texture.name = "castle.bmp";
					texture.strength = 1.0f;

					meshComp->textures.push_back(texture);
				}*/
				
				// register the entity with the component systems
				//	cEulerMovmentSystem::getInstance()->registerEntity(entity);
				cMeshRenderSystem::getInstance()->registerEntity(entity);
			}
		}
		break;

		case 3:
		{
			entity->componentBitField |= 1;
			entity->components[1] = pComponentFactory->createComponent(1);
			{
				cTransformComponent* transformComponent = static_cast<cTransformComponent*>(entity->components[1]);
				transformComponent->position.y = 0.0f;
			}

			entity->componentBitField |= 4;
			entity->components[3] = pComponentFactory->createComponent(3);
			{
				cRenderMeshComponent* meshComp = (cRenderMeshComponent*)entity->components[3];
				meshComp->meshName = "mario_xyz_n_uv.ply";
				meshComp->friendlyName = "Mario";
				meshComp->bIsVisible = true;
				meshComp->setDiffuseColour(glm::vec3(0.0f, 0.0f, 0.0f));
				meshComp->setSpecularColour(glm::vec3(1.0f, 1.0f, 1.0f));
				meshComp->bUseVertexColour = false;
				meshComp->bDontLight = false;
				meshComp->shaderId = cShaderManager::getInstance()->getIDFromFriendlyName("BasicUberShader");


				/*{
					sTextureInfo texture;
					texture.name = "mario.bmp";
					texture.strength = 1.0f;

					meshComp->textures.push_back(texture);
				}*/
	
				// register the entity with the component systems
				cMeshRenderSystem::getInstance()->registerEntity(entity);
			}
		}
		break;

		case 4:
		{
			entity->componentBitField |= 1;
			entity->components[1] = pComponentFactory->createComponent(1);
		
			cTransformComponent* pTransformComponent = static_cast<cTransformComponent*>(entity->components[1]);
			pTransformComponent->position.y = 0.0f;
			

			entity->componentBitField |= 4;
			entity->components[3] = pComponentFactory->createComponent(3);
			{
				cRenderMeshComponent* meshComp = (cRenderMeshComponent*)entity->components[3];
				meshComp->meshName = "luigi_xyz_n_uv.ply";
				meshComp->friendlyName = "Luigi";
				meshComp->bIsVisible = true;
				meshComp->setDiffuseColour(glm::vec3(0.0f, 0.0f, 0.0f));
				meshComp->bUseVertexColour = false;
				meshComp->bDontLight = false;
				meshComp->shaderId = cShaderManager::getInstance()->getIDFromFriendlyName("BasicUberShader");
				meshComp->setSpecularColour(glm::vec3(1.0f, 1.0f, 1.0f));

				/*{
					sTextureInfo texture;
					texture.name = "luigi.bmp";
					texture.strength = 1.0f;

					meshComp->textures.push_back(texture);
				}*/

				// register the entity with the component systems
				cMeshRenderSystem::getInstance()->registerEntity(entity);
			}
		}
		break;

		case 5:
		{
			entity->componentBitField |= 1;
			entity->components[1] = pComponentFactory->createComponent(1);
			{
				cTransformComponent* transformComponent = static_cast<cTransformComponent*>(entity->components[1]);
				transformComponent->scale = glm::vec3(0.1f);

			}

			entity->componentBitField |= 4;
			entity->components[3] = pComponentFactory->createComponent(3);
			{
				cRenderMeshComponent* meshComp = (cRenderMeshComponent*)entity->components[3];
				meshComp->meshName = "Peach_xyz_n_uv.ply";
				meshComp->friendlyName = "Peach";
				meshComp->bIsVisible = true;
				meshComp->setDiffuseColour(glm::vec3(0.0f, 0.0f, 0.0f));
				meshComp->bUseVertexColour = false;
				meshComp->bDontLight = false;
				meshComp->shaderId = cShaderManager::getInstance()->getIDFromFriendlyName("BasicUberShader");
				meshComp->setSpecularColour(glm::vec3(1.0f, 1.0f, 1.0f));

				/*{
					sTextureInfo texture;
					texture.name = "Peach.bmp";
					texture.strength = 1.0f;

					meshComp->textures.push_back(texture);
				}*/

				// register the entity with the component systems
				cMeshRenderSystem::getInstance()->registerEntity(entity);
			}
		}
		break;

		case 6:
		{
			entity->componentBitField |= 1;
			entity->components[1] = pComponentFactory->createComponent(1);
			{
				cTransformComponent* transformComponent = static_cast<cTransformComponent*>(entity->components[1]);
				transformComponent->scale = glm::vec3(0.65f);

			}

			entity->componentBitField |= 4;
			entity->components[3] = pComponentFactory->createComponent(3);
			{
				cRenderMeshComponent* meshComp = (cRenderMeshComponent*)entity->components[3];
				meshComp->meshName = "toad_xyz_n_uv.ply";
				meshComp->friendlyName = "Toad";
				meshComp->bIsVisible = true;
				meshComp->setDiffuseColour(glm::vec3(0.0f, 0.0f, 0.0f));
				meshComp->bUseVertexColour = false;
				meshComp->bDontLight = false;
				meshComp->shaderId = cShaderManager::getInstance()->getIDFromFriendlyName("BasicUberShader");
				meshComp->setSpecularColour(glm::vec3(1.0f, 1.0f, 1.0f));

				/*{
					sTextureInfo texture;
					texture.name = "Toad.bmp";
					texture.strength = 1.0f;

					meshComp->textures.push_back(texture);
				}*/

				// register the entity with the component systems
				cMeshRenderSystem::getInstance()->registerEntity(entity);
			}
		}
		break;

		case 7:
		{
			entity->componentBitField |= 1;
			entity->components[1] = pComponentFactory->createComponent(1);
			{
				cTransformComponent* transformComponent = static_cast<cTransformComponent*>(entity->components[1]);
				transformComponent->scale = glm::vec3(0.01f);

			}

			entity->componentBitField |= 4;
			entity->components[3] = pComponentFactory->createComponent(3);
			{
				cRenderMeshComponent* meshComp = (cRenderMeshComponent*)entity->components[3];
				meshComp->meshName = "bowser_xyz_n_uv.ply";
				meshComp->friendlyName = "Bowser";
				meshComp->bIsVisible = true;
				meshComp->setDiffuseColour(glm::vec3(0.0f, 0.0f, 0.0f));
				meshComp->bUseVertexColour = false;
				meshComp->bDontLight = false;
				meshComp->shaderId = cShaderManager::getInstance()->getIDFromFriendlyName("BasicUberShader");
				meshComp->setSpecularColour(glm::vec3(1.0f, 1.0f, 1.0f));

				/*{
					sTextureInfo texture;
					texture.name = "bowser.bmp";
					texture.strength = 1.0f;

					meshComp->textures.push_back(texture);
				}*/

				// register the entity with the component systems
				cMeshRenderSystem::getInstance()->registerEntity(entity);
			}
			entity->components[7] = pComponentFactory->createComponent(7); 
			{
				cParticleComponent* particleComp = static_cast<cParticleComponent*>(entity->components[7]);

				{
					sTextureInfo texture;
					texture.name = "flames.bmp";
					texture.strength = 1.0f;

					static_cast<cRenderMeshComponent*>(particleComp->particleEntity->components[3])->textures.push_back(texture);
				}

				cParticleComponent::sParticleCreationParams particleParams;
				particleParams.totalNumberOfParticles = 100;
				particleParams.minLifeTime = 5.0f;
				particleParams.maxLifeTime = 10.0f;
				// Will fade to fully transparent in the last 1 second of "life"
				particleParams.deathTransparencyFadeTimeSeconds = 2.0f;

				particleParams.position = glm::vec3(0.0f, 5.0f, 0.0f);
				particleParams.minInitialVelocity = glm::vec3(-0.5, 0.5, -0.5);
				particleParams.maxInitialVelocity = glm::vec3(0.5, 1.0, 0.5);
				particleParams.acceleration = glm::vec3(0.0f, 1.0f, 0.0f);
				particleParams.minNumberNewParticles = 2;
				particleParams.maxNumberNewParticles = 5;
				particleParams.minTimeBetweenParticleGenSeconds = 0.1f;
				particleParams.maxTimeBetweenParticleGenSeconds = 0.5f;
				particleParams.minInitialScale = 0.1f;
				particleParams.maxInitialScale = 0.15f;
				particleParams.minScaleChange = 0.0f;
				particleParams.maxScaleChange = 0.1f;	// This is per frame change

				// Degrees per frame
				particleParams.minOrientationChangeEuler = glm::vec3(-0.25f, -0.25f, -0.25f);
				particleParams.maxOrientationChangeEuler = glm::vec3(+0.25f, +0.25f, +0.25f);

				particleComp->init(particleParams);
				particleComp->reset(false);

				// Register with the system
				cParticleSystem::getInstance()->registerEntity(entity);
			}
		}
		break;

		case 8:
		{
			entity->componentBitField |= 1;
			entity->components[1] = pComponentFactory->createComponent(1);
			{
				cTransformComponent* transformComponent = static_cast<cTransformComponent*>(entity->components[1]);
				transformComponent->scale = glm::vec3(0.1f);

			}

			entity->componentBitField |= 4;
			entity->components[3] = pComponentFactory->createComponent(3);
			{
				cRenderMeshComponent* meshComp = (cRenderMeshComponent*)entity->components[3];
				meshComp->meshName = "boo_xyz_n_uv.ply";
				meshComp->friendlyName = "Boo";
				meshComp->bIsVisible = true;
				meshComp->setDiffuseColour(glm::vec3(0.0f, 0.0f, 0.0f));
				meshComp->bUseVertexColour = false;
				meshComp->bDontLight = false;
				meshComp->shaderId = cShaderManager::getInstance()->getIDFromFriendlyName("BasicUberShader");
				meshComp->setSpecularColour(glm::vec3(1.0f, 1.0f, 1.0f));

				/*{
					sTextureInfo texture;
					texture.name = "boo.bmp";
					texture.strength = 1.0f;

					meshComp->textures.push_back(texture);
				}*/

				// register the entity with the component systems
				cMeshRenderSystem::getInstance()->registerEntity(entity);
			}
		}
		break;

		case 9:
		{
			entity->componentBitField |= 1;
			entity->components[1] = pComponentFactory->createComponent(1);
			{
				cTransformComponent* transformComponent = static_cast<cTransformComponent*>(entity->components[1]);
				transformComponent->scale = glm::vec3(0.5f);

			}

			entity->componentBitField |= 4;
			entity->components[3] = pComponentFactory->createComponent(3);
			{
				cRenderMeshComponent* meshComp = (cRenderMeshComponent*)entity->components[3];
				meshComp->meshName = "goomba_xyz_n_uv.ply";
				meshComp->friendlyName = "Goomba";
				meshComp->bIsVisible = true;
				meshComp->setDiffuseColour(glm::vec3(0.0f, 0.0f, 0.0f));
				meshComp->bUseVertexColour = false;
				meshComp->bDontLight = false;
				meshComp->shaderId = cShaderManager::getInstance()->getIDFromFriendlyName("BasicUberShader");
				meshComp->setSpecularColour(glm::vec3(1.0f, 1.0f, 1.0f));

				/*{
					sTextureInfo texture;
					texture.name = "goomba.bmp";
					texture.strength = 1.0f;

					meshComp->textures.push_back(texture);
				}*/

				// register the entity with the component systems
				cMeshRenderSystem::getInstance()->registerEntity(entity);
			}
		}
		break;

		case 10:
		{
			entity->componentBitField |= 1;
			entity->components[1] = pComponentFactory->createComponent(1);

			cTransformComponent* transformComponent = static_cast<cTransformComponent*>(entity->components[1]);
			transformComponent->scale = glm::vec3(0.25f);

			entity->componentBitField |= 2;
			entity->components[2] = pComponentFactory->createComponent(2);

			entity->componentBitField |= 4;
			entity->components[3] = pComponentFactory->createComponent(3);
			{
				cRenderMeshComponent* meshComp = (cRenderMeshComponent*)entity->components[3];
				meshComp->meshName = "yoshi_xyz_n_uv.ply";
				meshComp->friendlyName = "Yoshi";
				meshComp->bIsVisible = true;
				meshComp->setDiffuseColour(glm::vec3(0.0f, 0.0f, 0.0f));
				meshComp->bUseVertexColour = false;
				meshComp->bDontLight = false;
				meshComp->shaderId = cShaderManager::getInstance()->getIDFromFriendlyName("BasicUberShader");
				meshComp->setSpecularColour(glm::vec3(1.0f, 1.0f, 1.0f));

				/*{
					sTextureInfo texture;
					texture.name = "yoshi.bmp";
					texture.strength = 0.75f;

					meshComp->textures.push_back(texture);

					sTextureInfo texture2;
					texture2.name = "toad.bmp";
					texture2.strength = 0.25f;

					meshComp->textures.push_back(texture2);
				}*/

				// register the entity with the component systems
				cMeshRenderSystem::getInstance()->registerEntity(entity);
				cEulerMovementSystem::getInstance()->registerEntity(entity);
			}


		}
		break;

		case 11:
		{
			entity->componentBitField |= 1;
			entity->components[1] = pComponentFactory->createComponent(1);

			cTransformComponent* pTransformComponent = static_cast<cTransformComponent*>(entity->components[1]);
			pTransformComponent->position = glm::vec3(0.0f, 0.0f, 0.0f);


			entity->componentBitField |= 4;
			entity->components[3] = pComponentFactory->createComponent(3);
			{
				cRenderMeshComponent* meshComp = static_cast<cRenderMeshComponent*>(entity->components[3]);
				meshComp->meshName = "fire_xyz_n_uv.ply";
				meshComp->friendlyName = "Fire";
				meshComp->bIsVisible = true;
				meshComp->bIsWireFrame = false;
				meshComp->setDiffuseColour(glm::vec3(0.0f, 0.0f, 0.0f));
				meshComp->bUseVertexColour = false;
				meshComp->bDontLight = false;
				meshComp->shaderId = cShaderManager::getInstance()->getIDFromFriendlyName("BasicUberShader");

				/*{
					sTextureInfo texture;
					texture.name = "fire.bmp";
					texture.strength = 1.0f;

					meshComp->textures.push_back(texture);
				}*/
			}

			
			// register the entity with the component systems
			cMeshRenderSystem::getInstance()->registerEntity(entity);
		}
		break;

		case 12:
		{
			entity->componentBitField |= 1;
			entity->components[1] = pComponentFactory->createComponent(1);

			cTransformComponent* pTransformComponent = static_cast<cTransformComponent*>(entity->components[1]);
			pTransformComponent->position = glm::vec3(0.0f, 0.0f, 0.0f);

			entity->componentBitField |= 4;
			entity->components[3] = pComponentFactory->createComponent(3);
			{
				cRenderMeshComponent* meshComp = static_cast<cRenderMeshComponent*>(entity->components[3]);
				meshComp->meshName = "GreenPipe_xyz_n_uv.ply";
				meshComp->friendlyName = "Pipe";
				meshComp->bIsVisible = true;
				meshComp->bIsWireFrame = false;
				meshComp->setDiffuseColour(glm::vec3(0.0f, 0.0f, 0.0f));
				meshComp->bUseVertexColour = false;
				meshComp->bDontLight = false;
				meshComp->shaderId = cShaderManager::getInstance()->getIDFromFriendlyName("BasicUberShader");

				/*{
					sTextureInfo texture;
					texture.name = "green_pipe.bmp";
					texture.strength = 1.0f;

					meshComp->textures.push_back(texture);
				}*/
			}

			// register the entity with the component systems
			cMeshRenderSystem::getInstance()->registerEntity(entity);
		}
		break;

		case 13:
		{
			entity->componentBitField |= 1;
			entity->components[1] = pComponentFactory->createComponent(1);

			cTransformComponent* pTransformComponent = static_cast<cTransformComponent*>(entity->components[1]);
			pTransformComponent->position.y = 0.0f;


			entity->componentBitField |= 4;
			entity->components[3] = pComponentFactory->createComponent(3);
			{
				cRenderMeshComponent* meshComp = (cRenderMeshComponent*)entity->components[3];
				meshComp->meshName = "lantern_xyz_n_uv.ply";
				meshComp->friendlyName = "Lantern";
				meshComp->bIsVisible = true;
				meshComp->setDiffuseColour(glm::vec3(0.0f, 0.0f, 0.0f));
				meshComp->bUseVertexColour = false;
				meshComp->bDontLight = false;
				meshComp->shaderId = cShaderManager::getInstance()->getIDFromFriendlyName("BasicUberShader");
				meshComp->setSpecularColour(glm::vec3(1.0f, 1.0f, 1.0f));

				/*{
					sTextureInfo texture;
					texture.name = "old_lantern.bmp";
					texture.strength = 1.0f;

					meshComp->textures.push_back(texture);
				}*/


				cMeshRenderSystem::getInstance()->registerEntity(entity);
			}
		}
		break;
				
		case 14:
		{
			entity->componentBitField |= 1;
			entity->components[1] = pComponentFactory->createComponent(1);

			cTransformComponent* pTransformComponent = static_cast<cTransformComponent*>(entity->components[1]);
			pTransformComponent->position.y = 0.0f;


			entity->componentBitField |= 4;
			entity->components[3] = pComponentFactory->createComponent(3);
			{
				cRenderMeshComponent* meshComp = (cRenderMeshComponent*)entity->components[3];
				meshComp->meshName = "pirahna_plant_xyz_n_uv.ply";
				meshComp->friendlyName = "PirahnaPlant";
				meshComp->bIsVisible = true;
				meshComp->setDiffuseColour(glm::vec3(0.0f, 0.0f, 0.0f));
				meshComp->bUseVertexColour = false;
				meshComp->bDontLight = false;
				meshComp->shaderId = cShaderManager::getInstance()->getIDFromFriendlyName("BasicUberShader");
				meshComp->setSpecularColour(glm::vec3(1.0f, 1.0f, 1.0f));

				/*{
					sTextureInfo texture;
					texture.name = "pirahna_plant.bmp";
					texture.strength = 1.0f;

					meshComp->textures.push_back(texture);
				}*/

				// register the entity with the component systems
				cMeshRenderSystem::getInstance()->registerEntity(entity);
			}
		}
		break;

		case 15:
		{
			entity->componentBitField |= 1;
			entity->components[1] = pComponentFactory->createComponent(1);

			cTransformComponent* transformComponent = static_cast<cTransformComponent*>(entity->components[1]);

			entity->componentBitField |= 2;
			entity->components[2] = pComponentFactory->createComponent(2);

			entity->componentBitField |= 4;
			entity->components[3] = pComponentFactory->createComponent(3);
			{
				cRenderMeshComponent* meshComp = (cRenderMeshComponent*)entity->components[3];
				meshComp->meshName = "star_xyz_n_uv.ply";
				meshComp->friendlyName = "Star";
				meshComp->bIsVisible = true;
				meshComp->setDiffuseColour(glm::vec3(0.0f, 0.0f, 0.0f));
				meshComp->bUseVertexColour = false;
				meshComp->bDontLight = false;
				meshComp->shaderId = cShaderManager::getInstance()->getIDFromFriendlyName("BasicUberShader");
				meshComp->setSpecularColour(glm::vec3(1.0f, 1.0f, 1.0f));

				/*{
					sTextureInfo texture;
					texture.name = "star.bmp";
					texture.strength = 1.0f;

					meshComp->textures.push_back(texture);
				}*/

				// register the entity with the component systems
				cMeshRenderSystem::getInstance()->registerEntity(entity);
				cEulerMovementSystem::getInstance()->registerEntity(entity);
			}


		}
		break;

		case 16:
		{
			entity->componentBitField |= 1;
			entity->components[1] = pComponentFactory->createComponent(1);

			cTransformComponent* transformComponent = static_cast<cTransformComponent*>(entity->components[1]);

			entity->componentBitField |= 2;
			entity->components[2] = pComponentFactory->createComponent(2);

			entity->componentBitField |= 4;
			entity->components[3] = pComponentFactory->createComponent(3);
			{
				cRenderMeshComponent* meshComp = (cRenderMeshComponent*)entity->components[3];
				meshComp->meshName = "torch_xyz_n_uv.ply";
				meshComp->friendlyName = "Torch";
				meshComp->bIsVisible = true;
				meshComp->setDiffuseColour(glm::vec3(0.0f, 0.0f, 0.0f));
				meshComp->bUseVertexColour = false;
				meshComp->bDontLight = false;
				meshComp->shaderId = cShaderManager::getInstance()->getIDFromFriendlyName("BasicUberShader");
				meshComp->setSpecularColour(glm::vec3(1.0f, 1.0f, 1.0f));

				/*{
					sTextureInfo texture;
					texture.name = "torch.bmp";
					texture.strength = 1.0f;

					meshComp->textures.push_back(texture);
				}*/

				/*{
					sTextureInfo texture;
					texture.name = "metal.bmp";
					texture.strength = 0.75f;

					meshComp->textures.push_back(texture);

					sTextureInfo texture2;
					texture2.name = "rust.bmp";
					texture2.strength = 0.25f;

					meshComp->textures.push_back(texture2);
				}*/

				/*{
					sTextureInfo texture;
					texture.name = "wood.bmp";
					texture.strength = 0.45f;

					meshComp->textures.push_back(texture);

					sTextureInfo texture2;
					texture2.name = "aged_wood.bmp";
					texture2.strength = 0.65f;

					meshComp->textures.push_back(texture2);
				}*/

				/*{
					sTextureInfo texture;
					texture.name = "old_metal.bmp";
					texture.strength = 1.0f;

					meshComp->textures.push_back(texture);
				}*/
				
				// register the entity with the component systems
				cMeshRenderSystem::getInstance()->registerEntity(entity);
				cEulerMovementSystem::getInstance()->registerEntity(entity);
			}
			entity->components[7] = pComponentFactory->createComponent(7);
			{
				cParticleComponent* particleComp = static_cast<cParticleComponent*>(entity->components[7]);

				{
					sTextureInfo texture;
					texture.name = "flames.bmp";
					texture.strength = 1.0f;

					static_cast<cRenderMeshComponent*>(particleComp->particleEntity->components[3])->textures.push_back(texture);
				}

				cParticleComponent::sParticleCreationParams particleParams;
				particleParams.totalNumberOfParticles = 100;
				particleParams.minLifeTime = 5.0f;
				particleParams.maxLifeTime = 10.0f;
				// Will fade to fully transparent in the last 1 second of "life"
				particleParams.deathTransparencyFadeTimeSeconds = 2.0f;

				// Grab torches position and offset the y
				particleParams.position = transformComponent->position;
				particleParams.position.y += 5.0f;
				particleParams.minInitialVelocity = glm::vec3(-0.5, 0.5, -0.5);
				particleParams.maxInitialVelocity = glm::vec3(0.5, 1.0, 0.5);
				particleParams.acceleration = glm::vec3(0.0f, 1.0f, 0.0f);
				particleParams.minNumberNewParticles = 2;
				particleParams.maxNumberNewParticles = 5;
				particleParams.minTimeBetweenParticleGenSeconds = 0.1f;
				particleParams.maxTimeBetweenParticleGenSeconds = 0.5f;
				particleParams.minInitialScale = 0.1f;
				particleParams.maxInitialScale = 0.15f;
				particleParams.minScaleChange = 0.0f;
				particleParams.maxScaleChange = 0.1f;	// This is per frame change

				// Degrees per frame
				particleParams.minOrientationChangeEuler = glm::vec3(-0.25f, -0.25f, -0.25f);
				particleParams.maxOrientationChangeEuler = glm::vec3(+0.25f, +0.25f, +0.25f);

				particleComp->init(particleParams);
				particleComp->reset(false);

				// Register with the system
				cParticleSystem::getInstance()->registerEntity(entity);
			}
		}
		break;

		case 17:
		{
			entity->componentBitField |= 1;
			entity->components[1] = pComponentFactory->createComponent(1);

			cTransformComponent* pTransformComponent = static_cast<cTransformComponent*>(entity->components[1]);

			entity->componentBitField |= 4;
			entity->components[3] = pComponentFactory->createComponent(3);
			{
				cRenderMeshComponent* meshComp = static_cast<cRenderMeshComponent*>(entity->components[3]);
				meshComp->meshName = "cube_flat_shaded_xyz_n.ply";
				meshComp->bIsVisible = true;
				meshComp->bIsWireFrame = true;
				meshComp->setDiffuseColour(glm::vec3(1.0f, 0.0f, 0.0f));
				meshComp->bUseVertexColour = false;
				meshComp->bDontLight = true;
				meshComp->shaderId = cShaderManager::getInstance()->getIDFromFriendlyName("BasicUberShader");
			}

			// register the entity with the component systems
			cMeshRenderSystem::getInstance()->registerEntity(entity);
		}
		break;

		case 18:
		{
			entity->componentBitField |= 1;
			entity->components[1] = pComponentFactory->createComponent(1);

			cTransformComponent* pTransformComponent = static_cast<cTransformComponent*>(entity->components[1]);
			pTransformComponent->setUniformScale(0.25f);

			entity->componentBitField |= 4;
			entity->components[3] = pComponentFactory->createComponent(3);
			{
				cRenderMeshComponent* meshComp = static_cast<cRenderMeshComponent*>(entity->components[3]);
				meshComp->meshName = "1x1_Star_1_Quad_2_Sided_xyz_n_uv.ply";
				meshComp->bIsVisible = false;
				meshComp->bIsWireFrame = false;
				meshComp->setDiffuseColour(glm::vec3(1.0f, 1.0f, 1.0f));
				meshComp->bUseVertexColour = false;
				meshComp->bDontLight = false;
				meshComp->shaderId = cShaderManager::getInstance()->getIDFromFriendlyName("BasicUberShader");
			}
		}
		break;

		case 19:
		{
			entity->componentBitField |= 1;
			entity->components[1] = pComponentFactory->createComponent(1);

			cTransformComponent* pTransformComponent = static_cast<cTransformComponent*>(entity->components[1]);
			pTransformComponent->setUniformScale(3.0f);

			entity->componentBitField |= 4;
			entity->components[3] = pComponentFactory->createComponent(3);
			{
				cRenderMeshComponent* meshComp = static_cast<cRenderMeshComponent*>(entity->components[3]);
				meshComp->meshName = "bun_res3_xyz_n_uv.ply";
				meshComp->bIsVisible = true;
				meshComp->bIsWireFrame = false;
				meshComp->setDiffuseColour(glm::vec3(0.0f, 0.0f, 0.0f));
				meshComp->bUseVertexColour = false;
				meshComp->bDontLight = false;
				meshComp->friendlyName = "Bunny";
				meshComp->shaderId = cShaderManager::getInstance()->getIDFromFriendlyName("BasicUberShader");

				/*{
					sTextureInfo texture;
					texture.name = "sprial.bmp";
					texture.strength = 0.45f;

					meshComp->textures.push_back(texture);

					sTextureInfo texture2;
					texture2.name = "wood.bmp";
					texture2.strength = 0.65f;

					meshComp->textures.push_back(texture2);
				}*/
				
				// register the entity with the component systems
				cMeshRenderSystem::getInstance()->registerEntity(entity);
			}
		}
		break;

	}
}

template<typename T>
T cEntityBuilder::getRandBetween0and1(void)
{
	return (T)((double)rand() / (RAND_MAX));
}

template <class T>
T cEntityBuilder::getRandInRange(T min, T max)
{
	double value =
		min + static_cast <double> (rand())
		/ (static_cast <double> (RAND_MAX / (static_cast<double>(max - min))));
	return static_cast<T>(value);
}