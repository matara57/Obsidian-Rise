// Header
//Use USTRUCT(BlueprintType) if you would like to include your Struct in Blueprints
#include "FPlayerStats.generated.h"

USTRUCT(BlueprintType)
struct FPlayerStats
{
    GENERATED_BODY()

    //~ The following member variable will be accessible by Blueprint Graphs:
    // This is the tooltip for our test variable.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test Variables")
    int32 MyIntegerMemberVariable;

    //~ The following member variable will be not accessible by Blueprint Graphs:
    int32 NativeOnlyMemberVariable;

    /**~
    * This UObject pointer is not accessible to Blueprint Graphs, but
    * is visible to UE4's reflection, smart pointer, and garbage collection
    * systems.
    */
    UPROPERTY()
    UObject* SafeObjectPointer;

};