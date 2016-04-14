#ifndef WOG_IMapElement_H
#define WOG_IMapElement_H

namespace WizardOfGalicia {
  class IMapElement {
    
  public:
    Vec2i position;
    char view;
    int emission;
    IMapElement();
    virtual void endOfTurn();
  };
}
#endif
