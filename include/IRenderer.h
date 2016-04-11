#ifndef IRENDERER_H
#define IRENDERER_H

namespace WizardOfGalicia {
  class IRenderer {
  public:
    virtual void init() = 0;
    virtual void drawMap( CMap &map, std::shared_ptr<CActor> current ) = 0;
    virtual char update() = 0;
    virtual void shutdown() = 0;
  };
}
#endif
