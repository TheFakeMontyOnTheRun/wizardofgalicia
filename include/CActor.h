#ifndef WOG_CActor_H
#define WOG_CActor_H

namespace WizardOfGalicia {

  enum class Stance {
    STANDING,
    ATTACKING,
    DEAD
  };
  
  enum class Team {
    HEROES,
      VILLAINS,
      RELICS
      };
  
  enum class Direction {
    N,
    E,
    S,
    W
  };
  
  const int FIREBALL_COST = 5;

  class CMap;

  class CActor : public IMapElement {
    
  public:
    
    Stance stance;
    Direction direction;
    int hp;
    int defence;
    int attack;
    int magicEnergy;
    Team team;
    
    virtual void performAttack( std::shared_ptr<CActor> other);
    CActor();
    virtual void endOfTurn();
    virtual void onMove();
    virtual void onAttack();
    virtual bool canMove();
    virtual bool canAttack();
    virtual void update( std::shared_ptr<CMap> map) = 0;
    virtual void turnLeft();
    virtual void turnRight();
  };
}
#endif
