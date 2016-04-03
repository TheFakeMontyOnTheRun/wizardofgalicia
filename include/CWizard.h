#ifndef WOG_CWizard_H
#define WOG_CWizard_H

namespace WizardOfGalicia {
  class CWizard : public CActor {
  public:
    CWizard();
    void update( std::shared_ptr<CMap> map ) override;
  };
}
#endif
