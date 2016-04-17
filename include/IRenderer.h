#ifndef IRENDERER_H
#define IRENDERER_H

namespace WizardOfGalicia {
  class IRenderer {
  public:
    virtual void playFireballSound() = 0;
    virtual void playMeeleeSound() = 0;
    virtual void playPowerUpSound() = 0;
    virtual bool waitingForKey() = 0;
    virtual void init() = 0;
    virtual void drawMap( CMap &map, std::shared_ptr<CActor> current ) = 0;
    virtual char update() = 0;
    virtual void shutdown() = 0;
    virtual void showTitleScreen() = 0;
    virtual void showGameOverScreen() = 0;
    virtual void showVictoryScreen() = 0;
    virtual void showInstructionsScreen() = 0;
  };
}
#endif
