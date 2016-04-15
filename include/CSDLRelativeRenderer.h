#ifndef WOG_CSDLRelativeRenderer_H
#define WOG_CSDLRelativeRenderer_H

namespace WizardOfGalicia {
  class CSDLRelativeRenderer : public IRenderer {

    SDL_Surface *video;
    SDL_Surface *showing;
    SDL_Surface *titleScreen;
    SDL_Surface *gameOverScreen;
    SDL_Surface *victoryScreen;
    bool waitingForFire;
    std::map<char, SDL_Surface*> sprites;
  public:
    void init() override;
    void drawMap( CMap &map, std::shared_ptr<CActor> current ) override;
    void shutdown() override;
    char update() override;
    void showTitleScreen() override;
    void showGameOverScreen() override;
    void showVictoryScreen() override;
  };
}
#endif
