#ifndef WOG_CGame_H
#define WOG_CGame_H

namespace WizardOfGalicia {
  
  class CGame {
  public:

    int turn;
    std::shared_ptr<CMap> map;

    void endOfTurn();
    std::string readMap( std::string filename );
    void runGame( IRenderer *renderer );
    void update();
  };
}
#endif
