#ifndef WOG_CGame_H
#define WOG_CGame_H

namespace WizardOfGalicia {

  enum class GameResult {
    UndefinedBehaviour,
      PlayerHasDied,
      PlayerHasFinishedLevel
      };
  
  class CGame {
  public:

    int turn;
    std::shared_ptr<CMap> map;

    void endOfTurn();
    std::string readMap( int level );
    GameResult runGame( IRenderer *renderer, int level );
    void update();

    std::shared_ptr<CActor> getActorAt( const Vec2i& position );
    void putAt( const Vec2i& position, std::shared_ptr<CActor> actor );

    bool playerIsDead( std::shared_ptr<CActor> avatar );
    bool playerHasFinishedLevel( std::shared_ptr<CActor> avatar, std::shared_ptr<CMap> map ) ;
  
  };
}
#endif
