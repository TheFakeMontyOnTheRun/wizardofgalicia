#ifndef WOG_CGame_H
#define WOG_CGame_H

namespace WizardOfGalicia {

  enum class GameResult {
    UndefinedBehaviour,
      PlayerHasDied,
      PlayerHasFinishedLevel,
      PlayerHasReturnedALevel
      };
  
  class CGame {

    IRenderer *renderer;
  public:
    CGame();
    int turn;
    std::shared_ptr<CMap> map;
    std::shared_ptr<CWizard> mPlayer;
    void endOfTurn();
    std::string readMap( int level );
    GameResult runGame( IRenderer *renderer, int level );
    void update();
    bool updatePendingProjectiles();
    void putAt( const Vec2i& position, std::shared_ptr<CActor> actor );
    GameResult tick();
    bool playerIsDead( std::shared_ptr<CActor> avatar );
    bool playerHasFinishedLevel( std::shared_ptr<CActor> avatar, std::shared_ptr<CMap> map );
    bool hasPlayerReturnedToPreviousLevel( std::shared_ptr<CActor> avatar, std::shared_ptr<CMap> map );
  };
}
#endif
