// Copyright 2020 SparkyPotato

#include "Core/Object/Object.h"

#include "Core/Object/ClassManager.h"

namespace Spark
{
	const Class& Object::GetClass()
	{
		return GClassManager->GetBase();
	}

	bool operator==(const Class& first, const Class& second)
	{
		return first.Name == second.Name;
	}
}
