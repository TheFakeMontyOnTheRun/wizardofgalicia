#ifndef WOG_CConsoleRenderer_H
#define WOG_CConsoleRenderer_H

namespace WizardOfGalicia {
  class CConsoleRenderer : public IRenderer {
  public:
    void drawMap( CMap &map, std::shared_ptr<CActor> current ) override;
  };
}
#endif
