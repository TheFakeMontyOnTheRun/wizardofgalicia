#ifndef IRENDERER_H
#define IRENDERER_H

namespace WizardOfGalicia {
  class IRenderer {
  public:
    virtual void drawMap( CMap &map, std::shared_ptr<CActor> current ) = 0;
  };
}
#endif
