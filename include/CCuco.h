#ifndef WOG_CCuco_H
#define WOG_CCuco_H

namespace WizardOfGalicia {
  class CCuco : public CActor, public std::enable_shared_from_this<CCuco> {
  public:
    CCuco();
    bool dealWith( std::shared_ptr<CMap> map, int x, int y );
    void update( std::shared_ptr<CMap> map ) override;
    bool actOn( int x, int y,  std::shared_ptr<CMap> map );
  };
}
#endif
