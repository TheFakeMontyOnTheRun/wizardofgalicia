#ifndef WOG_IMapElement_H
#define WOG_IMapElement_H

namespace WizardOfGalicia {
  class IMapElement {
    
  public:
    Vec2i position;
    char view;
    
    IMapElement();
    virtual void endOfTurn();
  };
}
#endif
