#include <math.h>
#include <Box2D/Box2D.h>
#include "scene.h"
#include "graphicfuncs.h"

const float M2P=20;
const float P2M=1/M2P;

class GameScene : IMPLEMENTS Scene
{
	b2Vec2 gravity;
	b2World world;

public:
	GameScene()
	:
	gravity(0.0f, 0.0f),
	world(gravity)
	{
		
	}

	virtual ~GameScene()
	{
	}

	virtual void OnInit()
	{
		addRect(400,300,20,20);
		addRect(50,50,40,40);
	}

	b2Body* addRect(int x,int y,int w,int h,bool dyn=true)
	{
		b2BodyDef bodydef;
		bodydef.position.Set(x*P2M,y*P2M);
		if(dyn)
		{
			bodydef.type=b2_dynamicBody;
		}
		b2Body* body=world.CreateBody(&bodydef);
       
		b2PolygonShape shape;
		shape.SetAsBox(P2M*w/2,P2M*h/2);
       
		b2FixtureDef fixturedef;
		fixturedef.shape=&shape;
		fixturedef.density=1.0;
		body->CreateFixture(&fixturedef);
        return body;
	}

	void drawSquare(b2Vec2* points,b2Vec2 center,float angle)
	{
			glColor3f(1,1,1);
			glPushMatrix();
				glTranslatef(center.x*M2P,center.y*M2P,0);
				glRotatef(angle*180.0/3.14159,0,0,1);
				glBegin(GL_QUADS);
				for(int i=0;i<4;i++)
				{
					glVertex2f(points[i].x*M2P,points[i].y*M2P);
				}
				glEnd();
			glPopMatrix();
	}

	virtual void OnRender()
	{
		glEnable2D();

		// foreach body in the world
		b2Body* currBody = world.GetBodyList();
		b2Vec2 points[4];
		while (currBody)
		{
			for(int i=0;i<4;i++)
			{
                points[i]=((b2PolygonShape*)currBody->GetFixtureList()->GetShape())->GetVertex(i);
			}
			drawSquare(points,currBody->GetWorldCenter(),currBody->GetAngle());
			currBody = currBody->GetNext();
		}

		glDisable2D();
	}

	virtual void OnEvent(SDL_Event* event)
	{
	}

	virtual void OnLoop()
	{
		float32 timeStep = 1.0f / 60.0f;
		int32 velocityIterations = 8;
		int32 positionIterations = 3;
		world.Step(timeStep, velocityIterations, positionIterations);
		

		b2Body* currBody = world.GetBodyList();
		float angle = currBody->GetAngle();

		Uint8 *keystate = SDL_GetKeyState(NULL);

		bool rotate = false;
		bool thrust = false;

		if (keystate[SDLK_w] )
		{
			currBody->ApplyForce( b2Vec2(4*cos(angle),4 * sin(angle)), currBody->GetWorldCenter() );
			thrust = true;
		}
		
		if (keystate[SDLK_s] )
		{
			currBody->ApplyForce( b2Vec2(-4*cos(angle),-4 * sin(angle)), currBody->GetWorldCenter() );
			thrust = true;
		}
		
		if (keystate[SDLK_a] )
		{
			currBody->ApplyTorque( 0.1 );
			rotate = true;
		}

		if (keystate[SDLK_d] )
		{
			currBody->ApplyTorque( -0.1 );
			rotate = true;
		}
		
		
		if (!rotate)
		{
			if (currBody->GetAngularVelocity() < 0)
			{
				currBody->ApplyTorque( 0.1 );
			}
			else if (currBody->GetAngularVelocity() > 0)
			{
				currBody->ApplyTorque( -0.1 );
			}
		}
	}

	virtual bool IsComplete()
	{
		return false;
	}
};