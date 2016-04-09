#ifndef WOG_CMap_H
#define WOG_CMap_H

namespace WizardOfGalicia {
  class CMap {
  public:
    std::shared_ptr<CActor> map[ 20 ][ 20 ];
    bool block[ 20 ][ 20 ];
    std::vector<std::shared_ptr<CActor>> actors;
    std::vector<std::shared_ptr<CDoorway>> mDoorways;
    std::shared_ptr<CActor> mWizard;

    CMap( const std::string &data );
    void move( Direction d, std::shared_ptr<CActor> a );
    bool attackIfNotFriendly( Direction d, std::shared_ptr<CActor> a, bool mutual );
    bool isBlockAt( int x, int y );
    bool isAtExit( std::shared_ptr<CActor> character );
    bool isAtEntrance( std::shared_ptr<CActor> character );
    bool isValid( int x, int y );
    void endOfTurn();
    void cast( std::shared_ptr<CActor> actor );
    std::shared_ptr<CActor> attack( std::shared_ptr<CActor> actor, int x, int y, bool mutual );
  };
}
#endif
