#include <ARBase/DebugDraw.h>

#if AR_BUILD_DEBUG

#include <ARBase/Macros.h>

#include <Engine/Console.h>

#include <iostream>
#include <map>

namespace
{

struct DebugCategoryData
{
	std::string Name;
	bool Enabled = false;

	// Tracking data.
	std::string FromFile;
	int FromLine;

	// Console command data (has to be stable in memory.
	std::unique_ptr<FAutoConsoleCommand> ConsoleCommand;
};

using DebugCategoryMap = std::map<uint32, DebugCategoryData>;

DebugCategoryMap& GetDebugCategories()
{
	static DebugCategoryMap categories;
	return categories;
}

void PrintRegisteredCategories(const DebugCategoryMap& categories)
{
	std::cout << "Registered categories:" << std::endl;
	for (const auto& [key, dc] : categories)
	{
		std::cout << "- KEY " << key << ": " << dc.Name << " (registered from " << dc.FromFile
				  << ":" << dc.FromLine << ")" << std::endl;
	}
}

DebugCategoryData& FindCategoryData(int32 category)
{
	auto& categories = GetDebugCategories();
	auto it = categories.find(category);
	if (it == categories.end())
	{
		std::cout << "Debug category with value " << category << " not registered!" << std::endl;
		PrintRegisteredCategories(categories);

		// Put a couple so that all compilers will show the break line correctly.
		AR_DEBUGTRAP();
		AR_DEBUGTRAP();
		AR_DEBUGTRAP();
	}

	return it->second;
}

} // namespace

__DebugCategoryRegisterer::__DebugCategoryRegisterer(int32 category, std::string&& name,
													 bool default_enabled,
													 std::string&& description, std::string&& file,
													 int line)
{
	auto& categories = GetDebugCategories();

	// The category should not be already registered.
	// Check the |file| and |line| variables to see where this category is being registered.
	// The console log should show where all categories are defined from, but you can also check
	// the data structures if needed.
	if (categories.find(category) != categories.end())
	{
		std::cout << "Debug category " << name << " already registered!" << std::endl
				  << "Registered from " << file << ":" << line << std::endl;
		PrintRegisteredCategories(categories);

		// Put a couple so that all compilers will show the break line correctly.
		AR_DEBUGTRAP();
		AR_DEBUGTRAP();
		AR_DEBUGTRAP();
	}

	// Create the Unreal console command that will toggle this category.
	FString command_name =
		FString::Printf(TEXT("ar.DebugDraw.ToggleCategory.%s"), *FString(name.c_str()));

	FString command_help =
		FString::Printf(TEXT("Toggle the debug draw for \"%s\". Category registered in %s:%d."),
						*FString(description.c_str()), *FString(file.c_str()), line);

	FConsoleCommandDelegate delegate;
	delegate.BindLambda([category]() { ARDebugDraw::ToggleCategory(category); });
	auto command = std::make_unique<FAutoConsoleCommand>(*command_name, *command_help,
														 std::move(delegate), ECVF_Cheat);

	DebugCategoryData data = {};
	data.Name = std::move(name);
	data.Enabled = default_enabled;
	data.FromFile = std::move(file);
	data.FromLine = line;
	data.ConsoleCommand = std::move(command);
	categories[category] = std::move(data);
}

bool ARDebugDraw::IsCategoryEnabled(int32 category)
{
	const auto& data = FindCategoryData(category);
	return data.Enabled;
}

void ARDebugDraw::EnableCategory(int32 category, bool enabled)
{
	auto& data = FindCategoryData(category);
	data.Enabled = enabled;
}

void ARDebugDraw::ToggleCategory(int32 category)
{
	auto& data = FindCategoryData(category);
	data.Enabled = !data.Enabled;
}
void ARDebugDraw::Text(int32 category, const FString& msg, const FColor& color, float lifetime)
{
	if (!IsCategoryEnabled(category))
	{
		return;
	}

#ifdef AR_BUILD_SERVER
	{
		static const TCHAR* kLocation = TEXT("Server");
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, lifetime, color,
										 FString::Printf(TEXT("%s - %s"), kLocation, *msg));
	}
#endif // AR_BUILD_SERVER

#ifdef AR_BUILD_CLIENT
	{
		static const TCHAR* kLocation = TEXT("Client");
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, lifetime, color,
										 FString::Printf(TEXT("%s - %s"), kLocation, *msg));
	}
#endif // AR_BUILD_CLIENT
}

void ARDebugDraw::Box(int32 category, NotNullPtr<UWorld> world, const FVector& center,
					  const FVector& extents, const FColor& color, float thickness, float lifetime)
{
	if (!IsCategoryEnabled(category))
	{
		return;
	}

	DrawDebugBox(world, center, extents, color, false, lifetime, 0, thickness);
}

void ARDebugDraw::Cylinder(int32 category, NotNullPtr<UWorld> world, const FVector& start,
						   const FVector& end, float radius, float segments, const FColor& color,
						   float thickness, float lifetime)
{
	if (!IsCategoryEnabled(category))
	{
		return;
	}

	DrawDebugCylinder(world, start, end, radius, segments, color, false, lifetime, 0, thickness);
}
void ARDebugDraw::DirectionalArrow(int32 category, NotNullPtr<UWorld> world, const FVector& start,
								   const FVector& end, float arrow_size, const FColor& color,
								   float thickness, float lifetime)
{
	if (!IsCategoryEnabled(category))
	{
		return;
	}

	DrawDebugDirectionalArrow(world, start, end, arrow_size, color, false, lifetime, 0, thickness);
}

void ARDebugDraw::Sphere(int32 category, NotNullPtr<UWorld> world, const FVector& center,
						 float radius, float segments, const FColor& color, float thickness,
						 float lifetime)
{
	if (!IsCategoryEnabled(category))
	{
		return;
	}

	DrawDebugSphere(world, center, radius, segments, color, false, lifetime, 0, thickness);
}


#endif // HVN_BUILD_DEBUG