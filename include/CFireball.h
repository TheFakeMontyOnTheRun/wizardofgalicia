#ifndef WOG_CFireball_H
#define WOG_CFireball_H

namespace WizardOfGalicia {
  class CFireball : public CActor, public std::enable_shared_from_this<CFireball> {
  public:
    CFireball( Direction d, int energy, const Vec2i& p );
    void update( std::shared_ptr<CMap> map ) override;
  };
}


#endif
