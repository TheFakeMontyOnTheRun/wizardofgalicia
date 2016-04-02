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
  

  class CActor : public IMapElement {
    
  public:
    
    Stance stance;
    Direction direction;
    int hp;
    int defence;
    int attack;
    Team team;
    
    void performAttack( CActor &other);
    CActor();
    void endOfTurn();
    void onMove();
    void onAttack();
    bool canMove();
    bool canAttack();
  };
}
#endif
