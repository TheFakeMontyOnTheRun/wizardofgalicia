#ifndef WOG_CSDLRelativeRenderer_H
#define WOG_CSDLRelativeRenderer_H

namespace WizardOfGalicia {
  class CSDLRelativeRenderer : public IRenderer {

    SDL_Surface *video;
  public:
    void init() override;
    void drawMap( CMap &map, std::shared_ptr<CActor> current ) override;
    void shutdown() override;
    char update() override;
  };
}
#endif
