#ifndef CSDLSHOWIMAGERENDERER_H
#define CSDLSHOWIMAGERENDERER_H

namespace WizardOfGalicia {
  class CSDLShowImageRenderer : public IRenderer {
    SDL_Surface *mTarget;
    SDL_Surface *mImageToShow;
  public:
    virtual void playFireballSound() override;
    virtual void playMeeleeSound() override;
    virtual void playPowerUpSound() override;
    virtual bool waitingForKey() override;
    virtual void init() override;
    virtual void drawMap( CMap &map, std::shared_ptr<CActor> current ) override;
    virtual char update() override;
    virtual void shutdown() override;
    void showImage( SDL_Surface *target, SDL_Surface *image );
  };
}
#endif
