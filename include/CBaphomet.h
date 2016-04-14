#ifndef WOG_CBaphomet_H
#define WOG_CBaphomet_H

namespace WizardOfGalicia {
  class CBaphomet : public CActor, public std::enable_shared_from_this<CBaphomet> {
  public:
    CBaphomet();
    bool dealWith( std::shared_ptr<CMap> map, int x, int y );
    void update( std::shared_ptr<CMap> map ) override;
    bool actOn( int x, int y,  std::shared_ptr<CMap> map );
  };
}
#endif
