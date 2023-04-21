#include "Module.h"

Module::Module(bool startEnabled)
{
}

bool Module::Init()
{
	return true;
}

bool Module::Start()
{
	return true;
}

Update_Status Module::PreUpdate()
{
	return Update_Status::UPDATE_CONTINUE;
}

Update_Status Module::Update()
{
	return Update_Status::UPDATE_CONTINUE;
}

Update_Status Module::PostUpdate()
{
	return Update_Status::UPDATE_CONTINUE;
}

bool Module::CleanUp()
{
	return true;
}

void Module::OnCollision(Collider* c1, Collider* c2)
{

}

void Module::Enable()
{
	if (!isEnabled)
	{
		isEnabled = true;
		Start();
	}
}

void Module::Disable()
{
	// TODO 0: Call CleanUp() for disabling a module
	if (isEnabled)
	{
		isEnabled = false;
		CleanUp();
	}
}
