#include "Module.h"
#include "Globals.h"
#include "Animation.h"
#include "Path.h"

struct SDL_Texture;

class WinLose : public Module
{
public:
    //Constructor
    WinLose(bool startEnabled);

    //Destructor
    ~WinLose();


    // Called when the module is activated
    // Loads the necessary textures for the map background
    bool Start() override;
    bool CleanUp() override;



    // Called at the middle of the application loop
    // Updates the scene's background animations
    Update_Status Update() override;

    // Called at the end of the application loop.
    // Performs the render call of all the parts of the scene's background
    Update_Status PostUpdate() override;

public:

    bool gameFinish = false;
    
    // The scene sprite sheet loaded into an SDL_Texture

    SDL_Texture* WinTexture = nullptr;
    SDL_Texture* LoseTexture = nullptr;
    SDL_Texture* ActiveTexture = nullptr;


    Animation idleAnimation;
    Animation WinAnimation;
    Animation LoseAnimation;

    Path AAnimationPath;
    Path WAnimationPath;
    Path LAnimationPath;

    uint LoseFX;

};
