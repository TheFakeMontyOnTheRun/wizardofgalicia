#ifndef WOG_CDoorway_H
#define WOG_CDoorway_H

namespace WizardOfGalicia {
  enum DoorwayFunction {
    ENTRY,
    EXIT
  };
  
  
  class CDoorway : public IMapElement {
    
  public:
    CDoorway( const Vec2i& pos, DoorwayFunction doorFunction );
    
    const DoorwayFunction doorFunction;
  };
}
#endif
