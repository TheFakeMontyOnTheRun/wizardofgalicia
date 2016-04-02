#ifndef WOG_CMap_H
#define WOG_CMap_H

namespace WizardOfGalicia {
  class CMap {
  public:
    IMapElement *map[ 20 ][ 20 ];
    bool block[ 20 ][ 20 ];
    std::vector<CActor*> actors;
    CActor *mWizard;

    CMap( const std::string &data );
    void move( Direction d, CActor &a );
    bool attackIfNotFriendly( Direction d, CActor &a, bool mutual );
    void endOfTurn();
    CActor* attack( CActor &a, int x, int y, bool mutual );
  };
}
#endif
