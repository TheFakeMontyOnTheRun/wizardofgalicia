#ifndef WOG_CActor_H
#define WOG_CActor_H

namespace WizardOfGalicia {

  enum Stance {
    STANDING,
    ATTACKING,
    DEAD
  };
  
  enum Team {
    HEROES,
    VILLAINS
  };
  
  enum Direction {
    N,
    E,
    S,
    W
  };
  

  class CMap;

  class CActor : public IMapElement {
    
  public:
    
    Stance stance;
    Direction direction;
    int hp;
    int defence;
    int attack;
    Team team;
    
    virtual void performAttack( std::shared_ptr<CActor> other);
    CActor();
    virtual void endOfTurn();
    virtual void onMove();
    virtual void onAttack();
    virtual bool canMove();
    virtual bool canAttack();
    virtual void update( std::shared_ptr<CMap> map) = 0;
  };
}
#endif
