#ifndef WOG_CDoorway_H
#define WOG_CDoorway_H

namespace WizardOfGalicia {
  enum DoorwayFunction {
    ENTRY,
    EXIT
  };
  
  
  class CDoorway : public IMapElement {
    
  public:
    CDoorway( DoorwayFunction doorFunction );
    
    const DoorwayFunction doorFunction;
  };
}
#endif
