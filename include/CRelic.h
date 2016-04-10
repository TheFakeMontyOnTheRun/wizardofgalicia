#ifndef WOG_CRelic_H
#define WOG_CRelic_H

namespace WizardOfGalicia {
  class CRelic : public CActor {
    
  public:
    CRelic();
    virtual void update( std::shared_ptr<CMap> map) override;
    virtual void performAttack( std::shared_ptr<CActor> other) override;
  };
}
#endif
