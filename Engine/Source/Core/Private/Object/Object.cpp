#include "Core/Object/Object.h"
#include "Core/Object/ClassManager.h"

namespace Spark
{
	const Class* Object::GetClass()
	{
		return GClassManager->GetBase();
	}

	const Class* Object::GetStaticClass()
	{
		return GClassManager->GetBase();
	}
}